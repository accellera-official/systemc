/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __SIMPLE_TARGET_SOCKET_H__
#define __SIMPLE_TARGET_SOCKET_H__

#include "tlm.h"
#include "peq_fifo.h"

template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class simple_target_socket :
  public tlm::tlm_target_socket<BUSWIDTH,
                               tlm::tlm_fw_transport_if<TYPES>,
                               tlm::tlm_bw_transport_if<TYPES> >
{
  friend class FwProcess;
  friend class BwProcess;
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_transport_if<TYPES>               fw_interface_type;
  typedef tlm::tlm_bw_transport_if<TYPES>               bw_interface_type;
  typedef tlm::tlm_target_socket<BUSWIDTH,
                                 fw_interface_type,
                                 bw_interface_type>     base_type;

public:
  explicit simple_target_socket(const char* n = "simple_target_socket") :
    base_type(sc_core::sc_gen_unique_name(n)),
    mFwProcess(this),
    mBwProcess(this)
  {
    bind(mFwProcess);
  }

  // bw transport must come thru us.
  tlm::tlm_bw_transport_if<TYPES> * operator ->() {return &mBwProcess;}

  // REGISTER_XXX
  void register_nb_transport_fw(MODULE* mod,
                                sync_enum_type (MODULE::*cb)(transaction_type&,
                                                             phase_type&,
                                                             sc_core::sc_time&))
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setNBTransportPtr(mod, cb);
  }

  void register_b_transport(MODULE* mod,
                            void (MODULE::*cb)(transaction_type&,
                                               sc_core::sc_time&))
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setBTransportPtr(mod, cb);
  }

  void register_transport_dbg(MODULE* mod,
                              unsigned int (MODULE::*cb)(transaction_type&))
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setTransportDebugPtr(mod, cb);
  }

  void register_get_direct_mem_ptr(MODULE* mod,
                                   bool (MODULE::*cb)(transaction_type&,
                                                      tlm::tlm_dmi&))
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setGetDMIPtr(mod, cb);
  }

private:
  //make call on bw path.
  sync_enum_type bw_nb_transport(transaction_type &trans, phase_type &phase, sc_core::sc_time &t)
  {
    return base_type::operator ->()->nb_transport_bw(trans, phase, t);
  }

  void bw_invalidate_direct_mem_ptr(sc_dt::uint64 s,sc_dt::uint64 e)
  {
    base_type::operator ->()->invalidate_direct_mem_ptr(s, e);
  }

  //Helper class to handle bw path calls
  // Needed to detect transaction end when called from b_transport.
  class BwProcess : public tlm::tlm_bw_transport_if<TYPES>
  {
  public:
    BwProcess(simple_target_socket *pOwn) : mOwner(pOwn)
    {
    }

    sync_enum_type nb_transport_bw(transaction_type &trans, phase_type &phase, sc_core::sc_time &t)
    {
      typename std::map<transaction_type*, sc_core::sc_event *>::iterator it;
      
      it = mOwner->mPendingTrans.find(&trans);
      if(it == mOwner->mPendingTrans.end()) {
        // Not a blocking call, forward.
        return mOwner->bw_nb_transport(trans, phase, t);

      } else {
        if (phase == tlm::END_REQ) {
          mOwner->mEndRequest.notify(sc_core::SC_ZERO_TIME);
          return tlm::TLM_ACCEPTED;
        
        } else if (phase == tlm::BEGIN_RESP) {
          if (mOwner->mCurrentTransaction == &trans) {
            mOwner->mEndRequest.notify(sc_core::SC_ZERO_TIME);
          }
          //TODO: add response-accept delay?
          it->second->notify(t);
          mOwner->mPendingTrans.erase(it);
          return tlm::TLM_COMPLETED;

        } else {
          assert(0); exit(1);
        }

//        return tlm::TLM_COMPLETED;  //Should not reach here
      }
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 s,sc_dt::uint64 e)
    {
      return mOwner->bw_invalidate_direct_mem_ptr(s, e);
    }

  private:
    simple_target_socket *mOwner;
  };

  class FwProcess : public tlm::tlm_fw_transport_if<TYPES>,
                    public tlm::mm_interface
  {
  public:
    typedef sync_enum_type (MODULE::*NBTransportPtr)(transaction_type&,
                                                     tlm::tlm_phase&,
                                                     sc_core::sc_time&);
    typedef void (MODULE::*BTransportPtr)(transaction_type&,
                                          sc_core::sc_time&);
    typedef unsigned int (MODULE::*TransportDebugPtr)(transaction_type&);
    typedef bool (MODULE::*GetDMIPtr)(transaction_type&,
                                      tlm::tlm_dmi&);
      
    FwProcess(simple_target_socket *pOwn) :
      mName(pOwn->name()),
      mOwner(pOwn),
      mMod(0),
      mNBTransportPtr(0),
      mBTransportPtr(0),
      mTransportDebugPtr(0),
      mGetDMIPtr(0),
      mPEQ(sc_core::sc_gen_unique_name("mPEQ")),
      mResponseInProgress(false)
    {
      sc_core::sc_spawn_options opts;
      opts.set_sensitivity(&mPEQ.get_event());
      sc_core::sc_spawn(sc_bind(&FwProcess::b2nb_thread, this), 
                        sc_core::sc_gen_unique_name("b2nb_thread"), &opts);
    }
  
    void setNBTransportPtr(MODULE* mod, NBTransportPtr p)
    {
      if (mNBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mNBTransportPtr = p;
      }
    }

    void setBTransportPtr(MODULE* mod, BTransportPtr p)
    {
      if (mBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mBTransportPtr = p;
      }
    }

    void setTransportDebugPtr(MODULE* mod, TransportDebugPtr p)
    {
      if (mTransportDebugPtr) {
        std::cerr << mName << ": debug callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportDebugPtr = p;
      }
    }

    void setGetDMIPtr(MODULE* mod, GetDMIPtr p)
    {
      if (mGetDMIPtr) {
        std::cerr << mName << ": get DMI pointer callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mGetDMIPtr = p;
      }
    }
// Interface implementation
    sync_enum_type nb_transport_fw(transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
    {
      if (mNBTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mNBTransportPtr)(trans, phase, t);

      } else if (mBTransportPtr) {
        if (phase == tlm::BEGIN_REQ) {
          // create thread to do blocking call
          sc_core::sc_spawn_options opts;
          opts.dont_initialize();
          sc_core::sc_event *e = new sc_core::sc_event;
          opts.set_sensitivity(e);
          sc_core::sc_spawn(sc_bind(&FwProcess::nb2b_thread, this, sc_ref(trans), e), 
                            sc_core::sc_gen_unique_name("nb2b_thread"), &opts);
          e->notify(t);
          return tlm::TLM_ACCEPTED;

        } else if (phase == tlm::END_RESP) {
          mResponseInProgress = false;
          mEndResponse.notify(t);
          return tlm::TLM_COMPLETED;

        } else {
          assert(0); exit(1);
//          return tlm::TLM_COMPLETED;   ///< unreachable code
        }

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    void b_transport(transaction_type& trans, sc_core::sc_time& t)
    {
      if (mBTransportPtr) {
        // forward call
        assert(mMod);
        (mMod->*mBTransportPtr)(trans, t);
        return;
      
      } else if (mNBTransportPtr) {
        mPEQ.notify(trans, t);
        t = sc_core::SC_ZERO_TIME;

        mm_end_event_ext mm_ext;
        const bool mmAdded = !mMMProxy.has_mm(&trans);

        if (mmAdded) {
          mMMProxy.set_mm(&trans, this);
          trans.set_nb_extension(&mm_ext);
          trans.acquire();
        }

        // wait until transaction is finished
        sc_core::sc_event endEvent;
        mOwner->mPendingTrans[&trans] = &endEvent;
        sc_core::wait(endEvent);

        if (mmAdded) {
          // release will not delete the transaction, it will notify mm_ext.done
          trans.release();
          if (trans.get_ref_count()) {
            sc_core::wait(mm_ext.done);
          }
          mMMProxy.set_mm(&trans, 0);
        }

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    unsigned int transport_dbg(transaction_type& trans)
    {
      if (mTransportDebugPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mTransportDebugPtr)(trans);

      } else {
        // No debug support
        return 0;
      }
    }

    bool get_direct_mem_ptr(transaction_type& trans,
                            tlm::tlm_dmi&  dmi_data)
    {
      if (mGetDMIPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mGetDMIPtr)(trans, dmi_data);

      } else {
        // No DMI support
        dmi_data.allow_read_write();
        dmi_data.set_start_address(0x0);
        dmi_data.set_end_address((sc_dt::uint64)-1);
        return false;
      }
    }

  private:
    void nb2b_thread(transaction_type& trans, sc_core::sc_event* e)
    {
      sc_core::sc_time t = sc_core::SC_ZERO_TIME;

      // forward call
      assert(mMod);
      (mMod->*mBTransportPtr)(trans, t);

      sc_core::wait(t);

      // return path
      while (mResponseInProgress) {
        sc_core::wait(mEndResponse);
      }
      t = sc_core::SC_ZERO_TIME;
      phase_type phase = tlm::BEGIN_RESP;
      if (mOwner->bw_nb_transport(trans, phase, t) != tlm::TLM_COMPLETED) {
        mResponseInProgress = true;
      }

      // cleanup
      delete e;
    }

    void b2nb_thread()
    {
      while (true) {
        sc_core::wait(mPEQ.get_event());

        transaction_type* trans;
        while ((trans = mPEQ.get_next_transaction())!=0) {
          assert(mMod);
          assert(mNBTransportPtr);
          phase_type phase = tlm::BEGIN_REQ;
          sc_core::sc_time t = sc_core::SC_ZERO_TIME;

          switch ((mMod->*mNBTransportPtr)(*trans, phase, t)) {
          case tlm::TLM_COMPLETED:
          {
            // notify transaction is finished
            typename std::map<transaction_type*, sc_core::sc_event *>::iterator it =
              mOwner->mPendingTrans.find(trans);
            assert(it != mOwner->mPendingTrans.end());
            it->second->notify(t);
            mOwner->mPendingTrans.erase(it);
            break;
          }

          case tlm::TLM_ACCEPTED:
          case tlm::TLM_UPDATED:
            switch (phase) {
            case tlm::BEGIN_REQ:
              mOwner->mCurrentTransaction = trans;
              sc_core::wait(mOwner->mEndRequest);
              mOwner->mCurrentTransaction = 0;
              break;

            case tlm::END_REQ:
              sc_core::wait(t);
              break;

            case tlm::BEGIN_RESP:
            {
              phase = tlm::END_RESP;
              t = sc_core::SC_ZERO_TIME;
              (mMod->*mNBTransportPtr)(*trans, phase, t);
          
              // notify transaction is finished
              typename std::map<transaction_type*, sc_core::sc_event *>::iterator it =
                mOwner->mPendingTrans.find(trans);
              assert(it != mOwner->mPendingTrans.end());
              it->second->notify(t);
              mOwner->mPendingTrans.erase(it);
              break;
            }

            default:
              assert(0); exit(1);
            };
            break;

          default:
            assert(0); exit(1);
          };
        }
      }
    }

    void free(transaction_type* trans)
    {
      mm_end_event_ext* ext = trans->template get_extension<mm_end_event_ext>();
      assert(ext);
      // notif event first before freeing extensions (reset)
      ext->done.notify();
      trans->reset();
    }

  private:
    struct mm_end_event_ext : public tlm::tlm_extension<mm_end_event_ext>
    {
      tlm::tlm_extension_base* clone() const { return NULL; }
      void free() {}
      void copy_from(tlm::tlm_extension_base const &) {}
      sc_core::sc_event done;
    };

  private:
    const std::string mName;
    simple_target_socket *mOwner;
    MODULE* mMod;
    NBTransportPtr mNBTransportPtr;
    BTransportPtr mBTransportPtr;
    TransportDebugPtr mTransportDebugPtr;
    GetDMIPtr mGetDMIPtr;
    peq_fifo<transaction_type> mPEQ;
    bool mResponseInProgress;
    sc_core::sc_event mEndResponse;
    tlm::mm_proxy mMMProxy;
  };

private:
  FwProcess mFwProcess;
  BwProcess mBwProcess;
  std::map<transaction_type*, sc_core::sc_event *> mPendingTrans;
  sc_core::sc_event mEndRequest;
  transaction_type* mCurrentTransaction;
};

//ID Tagged version
template <typename MODULE,
          unsigned int BUSWIDTH = 32,
          typename TYPES = tlm::tlm_generic_payload_types>
class simple_target_socket_tagged :
  public tlm::tlm_target_socket<BUSWIDTH,
                               tlm::tlm_fw_transport_if<TYPES>,
                               tlm::tlm_bw_transport_if<TYPES> >
{
  friend class FwProcess;
  friend class BwProcess;
public:
  typedef typename TYPES::tlm_payload_type              transaction_type;
  typedef typename TYPES::tlm_phase_type                phase_type;
  typedef tlm::tlm_sync_enum                            sync_enum_type;
  typedef tlm::tlm_fw_transport_if<TYPES>               fw_interface_type;
  typedef tlm::tlm_bw_transport_if<TYPES>               bw_interface_type;
  typedef tlm::tlm_target_socket<BUSWIDTH,
                                 fw_interface_type,
                                 bw_interface_type>     base_type;

public:
  explicit simple_target_socket_tagged(const char* n = "simple_target_socket_tagged") :
    base_type(sc_core::sc_gen_unique_name(n)),
    mFwProcess(this),
    mBwProcess(this)
  {
    bind(mFwProcess);
  }

  // bw transport must come thru us.
  tlm::tlm_bw_transport_if<TYPES> * operator ->() {return &mBwProcess;}

  // REGISTER_XXX
  void register_nb_transport_fw(MODULE* mod,
                                sync_enum_type (MODULE::*cb)(int id,
                                                             transaction_type&,
                                                             phase_type&,
                                                             sc_core::sc_time&),
                                int id)
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setNBTransportPtr(mod, cb);
    mFwProcess.setNBTransportUserId(id);
  }

  void register_b_transport(MODULE* mod,
                            void (MODULE::*cb)(int id,
                                               transaction_type&,
                                               sc_core::sc_time&),
                            int id)
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setBTransportPtr(mod, cb);
    mFwProcess.setBTransportUserId(id);
  }

  void register_transport_dbg(MODULE* mod,
                              unsigned int (MODULE::*cb)(int id,
                                                         transaction_type&),
                              int id)
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setTransportDebugPtr(mod, cb);
    mFwProcess.setTransportDebugUserId(id);
  }

  void register_get_direct_mem_ptr(MODULE* mod,
                                   bool (MODULE::*cb)(int id,
                                                      transaction_type&,
                                                      tlm::tlm_dmi&),
                                   int id)
  {
    assert(!sc_core::sc_get_curr_simcontext()->elaboration_done());
    mFwProcess.setGetDMIPtr(mod, cb);
    mFwProcess.setGetDMIUserId(id);
  }

private:
  //make call on bw path.
  sync_enum_type bw_nb_transport(transaction_type &trans, phase_type &phase, sc_core::sc_time &t)
  {
    return base_type::operator ->()->nb_transport_bw(trans, phase, t);
  }

  void bw_invalidate_direct_mem_ptr(sc_dt::uint64 s,sc_dt::uint64 e)
  {
    base_type::operator ->()->invalidate_direct_mem_ptr(s, e);
  }

  //Helper class to handle bw path calls
  // Needed to detect transaction end when called from b_transport.
  class BwProcess : public tlm::tlm_bw_transport_if<TYPES>
  {
  public:
    BwProcess(simple_target_socket_tagged *pOwn) : mOwner(pOwn)
    {
    }

    sync_enum_type nb_transport_bw(transaction_type &trans, phase_type &phase, sc_core::sc_time &t)
    {
      typename std::map<transaction_type*, sc_core::sc_event *>::iterator it;
      
      it = mOwner->mPendingTrans.find(&trans);
      if(it == mOwner->mPendingTrans.end()) {
        // Not a blocking call, forward.
        return mOwner->bw_nb_transport(trans, phase, t);

      } else {
        if (phase == tlm::END_REQ) {
          mOwner->mEndRequest.notify(sc_core::SC_ZERO_TIME);
          return tlm::TLM_ACCEPTED;
        
        } else if (phase == tlm::BEGIN_RESP) {
          if (mOwner->mCurrentTransaction == &trans) {
            mOwner->mEndRequest.notify(sc_core::SC_ZERO_TIME);
          }
          //TODO: add response-accept delay?
          it->second->notify(t);
          mOwner->mPendingTrans.erase(it);
          return tlm::TLM_COMPLETED;

        } else {
          assert(0); exit(1);
        }

//        return tlm::TLM_COMPLETED;  //Should not reach here
      }
    }

    void invalidate_direct_mem_ptr(sc_dt::uint64 s,sc_dt::uint64 e)
    {
      return mOwner->bw_invalidate_direct_mem_ptr(s, e);
    }

  private:
    simple_target_socket_tagged *mOwner;
  };

  class FwProcess : public tlm::tlm_fw_transport_if<TYPES>,
                    public tlm::mm_interface
  {
  public:
    typedef sync_enum_type (MODULE::*NBTransportPtr)(int id, 
                                                     transaction_type&,
                                                     tlm::tlm_phase&,
                                                     sc_core::sc_time&);
    typedef void (MODULE::*BTransportPtr)(int id, 
                                          transaction_type&,
                                          sc_core::sc_time&);
    typedef unsigned int (MODULE::*TransportDebugPtr)(int id, 
                                                      transaction_type&);
    typedef bool (MODULE::*GetDMIPtr)(int id, 
                                      transaction_type&,
                                      tlm::tlm_dmi&);
      
    FwProcess(simple_target_socket_tagged *pOwn) :
      mName(pOwn->name()),
      mOwner(pOwn),
      mMod(0),
      mNBTransportPtr(0),
      mBTransportPtr(0),
      mTransportDebugPtr(0),
      mGetDMIPtr(0),
      mNBTransportUserId(0),
      mBTransportUserId(0),
      mTransportDebugUserId(0),
      mGetDMIUserId(0),
      mPEQ(sc_core::sc_gen_unique_name("mPEQ")),
      mResponseInProgress(false)
    {
      sc_core::sc_spawn_options opts;
      opts.set_sensitivity(&mPEQ.get_event());
      sc_core::sc_spawn(sc_bind(&FwProcess::b2nb_thread, this), 
                        sc_core::sc_gen_unique_name("b2nb_thread"), &opts);
    }
  
    void setNBTransportUserId(int id) { mNBTransportUserId = id; }
    void setBTransportUserId(int id) { mBTransportUserId = id; }
    void setTransportDebugUserId(int id) { mTransportDebugUserId = id; }
    void setGetDMIUserId(int id) { mGetDMIUserId = id; }

    void setNBTransportPtr(MODULE* mod, NBTransportPtr p)
    {
      if (mNBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mNBTransportPtr = p;
      }
    }

    void setBTransportPtr(MODULE* mod, BTransportPtr p)
    {
      if (mBTransportPtr) {
        std::cerr << mName << ": non-blocking callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mBTransportPtr = p;
      }
    }

    void setTransportDebugPtr(MODULE* mod, TransportDebugPtr p)
    {
      if (mTransportDebugPtr) {
        std::cerr << mName << ": debug callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mTransportDebugPtr = p;
      }
    }

    void setGetDMIPtr(MODULE* mod, GetDMIPtr p)
    {
      if (mGetDMIPtr) {
        std::cerr << mName << ": get DMI pointer callback allready registered" << std::endl;

      } else {
        assert(!mMod || mMod == mod);
        mMod = mod;
        mGetDMIPtr = p;
      }
    }
// Interface implementation
    sync_enum_type nb_transport_fw(transaction_type& trans,
                                   phase_type& phase,
                                   sc_core::sc_time& t)
    {
      if (mNBTransportPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mNBTransportPtr)(mNBTransportUserId, trans, phase, t);

      } else if (mBTransportPtr) {
        if (phase == tlm::BEGIN_REQ) {
          // create thread to do blocking call
          sc_core::sc_spawn_options opts;
          opts.dont_initialize();
          sc_core::sc_event *e = new sc_core::sc_event;
          opts.set_sensitivity(e);
          sc_core::sc_spawn(sc_bind(&FwProcess::nb2b_thread, this, sc_ref(trans), e), 
                            sc_core::sc_gen_unique_name("nb2b_thread"), &opts);
          e->notify(t);
          return tlm::TLM_ACCEPTED;

        } else if (phase == tlm::END_RESP) {
          mResponseInProgress = false;
          mEndResponse.notify(t);
          return tlm::TLM_COMPLETED;

        } else {
          assert(0); exit(1);
//          return tlm::TLM_COMPLETED;   ///< unreachable code
        }

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    void b_transport(transaction_type& trans, sc_core::sc_time& t)
    {
      if (mBTransportPtr) {
        // forward call
        assert(mMod);
        (mMod->*mBTransportPtr)(mBTransportUserId, trans, t);
        return;
      
      } else if (mNBTransportPtr) {
        mPEQ.notify(trans, t);
        t = sc_core::SC_ZERO_TIME;

        mm_end_event_ext mm_ext;
        const bool mmAdded = !mMMProxy.has_mm(&trans);

        if (mmAdded){
          mMMProxy.set_mm(&trans, this);
          trans.set_nb_extension(&mm_ext);
          trans.acquire();
        }

        // wait until transaction is finished
        sc_core::sc_event endEvent;
        mOwner->mPendingTrans[&trans] = &endEvent;
        sc_core::wait(endEvent);

        if (mmAdded) {
          // release will not delete the transaction, it will notify mm_ext.done
          trans.release();
          if (trans.get_ref_count()) {
            sc_core::wait(mm_ext.done);
          }
          mMMProxy.set_mm(&trans, 0);
        }

      } else {
        std::cerr << mName << ": no transport callback registered" << std::endl;
        assert(0); exit(1);
//        return tlm::TLM_COMPLETED;   ///< unreachable code
      }
    }

    unsigned int transport_dbg(transaction_type& trans)
    {
      if (mTransportDebugPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mTransportDebugPtr)(mTransportDebugUserId, trans);

      } else {
        // No debug support
        return 0;
      }
    }

    bool get_direct_mem_ptr(transaction_type& trans,
                            tlm::tlm_dmi&  dmi_data)
    {
      if (mGetDMIPtr) {
        // forward call
        assert(mMod);
        return (mMod->*mGetDMIPtr)(mGetDMIUserId, trans, dmi_data);

      } else {
        // No DMI support
        dmi_data.allow_read_write();
        dmi_data.set_start_address(0x0);
        dmi_data.set_end_address((sc_dt::uint64)-1);
        return false;
      }
    }

  private:
    void nb2b_thread(transaction_type& trans, sc_core::sc_event* e)
    {
      sc_core::sc_time t = sc_core::SC_ZERO_TIME;

      // forward call
      assert(mMod);
      (mMod->*mBTransportPtr)(mBTransportUserId, trans, t);

      sc_core::wait(t);

      // return path
      while (mResponseInProgress) {
        sc_core::wait(mEndResponse);
      }
      t = sc_core::SC_ZERO_TIME;
      phase_type phase = tlm::BEGIN_RESP;
      if (mOwner->bw_nb_transport(trans, phase, t) != tlm::TLM_COMPLETED) {
        mResponseInProgress = true;
      }

      // cleanup
      delete e;
    }

    void b2nb_thread()
    {
      while (true) {
        sc_core::wait(mPEQ.get_event());

        transaction_type* trans;
        while ((trans = mPEQ.get_next_transaction())!=0) {
          assert(mMod);
          assert(mNBTransportPtr);
          phase_type phase = tlm::BEGIN_REQ;
          sc_core::sc_time t = sc_core::SC_ZERO_TIME;

          switch ((mMod->*mNBTransportPtr)(mNBTransportUserId, *trans, phase, t)) {
          case tlm::TLM_COMPLETED:
          {
            // notify transaction is finished
            typename std::map<transaction_type*, sc_core::sc_event *>::iterator it =
              mOwner->mPendingTrans.find(trans);
            assert(it != mOwner->mPendingTrans.end());
            it->second->notify(t);
            mOwner->mPendingTrans.erase(it);
            break;
          }

          case tlm::TLM_ACCEPTED:
          case tlm::TLM_UPDATED:
            switch (phase) {
            case tlm::BEGIN_REQ:
              mOwner->mCurrentTransaction = trans;
              sc_core::wait(mOwner->mEndRequest);
              mOwner->mCurrentTransaction = 0;
              break;

            case tlm::END_REQ:
              sc_core::wait(t);
              break;

            case tlm::BEGIN_RESP:
            {
              phase = tlm::END_RESP;
              t = sc_core::SC_ZERO_TIME;
              (mMod->*mNBTransportPtr)(mNBTransportUserId, *trans, phase, t);
          
              // notify transaction is finished
              typename std::map<transaction_type*, sc_core::sc_event *>::iterator it =
                mOwner->mPendingTrans.find(trans);
              assert(it != mOwner->mPendingTrans.end());
              it->second->notify(t);
              mOwner->mPendingTrans.erase(it);
              break;
            }

            default:
              assert(0); exit(1);
            };
            break;

          default:
            assert(0); exit(1);
          };
        }
      }
    }

    void free(transaction_type* trans)
    {
      mm_end_event_ext* ext = trans->template get_extension<mm_end_event_ext>();
      assert(ext);
      // notif event first before freeing extensions (reset)
      ext->done.notify();
      trans->reset();
    }

  private:
    struct mm_end_event_ext : public tlm::tlm_extension<mm_end_event_ext>
    {
      tlm::tlm_extension_base* clone() const { return NULL; }
      void free() {}
      void copy_from(tlm::tlm_extension_base const &) {}
      sc_core::sc_event done;
    };

  private:
    const std::string mName;
    simple_target_socket_tagged *mOwner;
    MODULE* mMod;
    NBTransportPtr mNBTransportPtr;
    BTransportPtr mBTransportPtr;
    TransportDebugPtr mTransportDebugPtr;
    GetDMIPtr mGetDMIPtr;
    int mNBTransportUserId;
    int mBTransportUserId;
    int mTransportDebugUserId;
    int mGetDMIUserId;
    peq_fifo<transaction_type> mPEQ;
    bool mResponseInProgress;
    sc_core::sc_event mEndResponse;
    tlm::mm_proxy mMMProxy;
  };

private:
  FwProcess mFwProcess;
  BwProcess mBwProcess;
  std::map<transaction_type*, sc_core::sc_event *> mPendingTrans;
  sc_core::sc_event mEndRequest;
  transaction_type* mCurrentTransaction;
};

#endif
