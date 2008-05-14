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

#ifndef __TLM_QUANTUMKEEPER_H__
#define __TLM_QUANTUMKEEPER_H__

namespace tlm {

  namespace tlm_qk_internal {

    //
    // tlm_quantumkeeper class
    //
    // The tlm_quantumkeeper class is used to keep track of the local time in
    // an initiator (how much it has run ahead of the SystemC time), to
    // synchronize with SystemC time etc.
    //
    // NOTE:
    // The templated class tlm::tlm_qk_internal::tlm_quantumkeeper<Dummy> is an
    // artifact of the implementation and not part of the core TLM standard. It
    // should never be used directly, instead the class tlm::tlm_quantumkeeper
    // should be used.
    //
    // The dummy template parameter is introduced to make it possible to
    // initialize the static member (m_global_quantum) in the headerfile. This
    // is only possible for template classes, otherwise the static member must
    // be initialized in a source file.
    //  
    template <typename Dummy>
    class tlm_quantumkeeper
    {
    public:
      //
      // Static setters/getters for the global quantum value.
      //
      // The global quantum is the maximum time an initiator can run ahead of
      // systemC time. All initiators will synchronize on timingpoints that are
      // multiples of the global quantum value.
      //
      static void set_global_quantum(const sc_core::sc_time& t)
      {
        if (!m_global_quantum) {
          m_global_quantum = new sc_core::sc_time(t);

        } else {
          *m_global_quantum = t;
        }
      }
    
      static const sc_core::sc_time& get_global_quantum() 
      {
        if (!m_global_quantum) {
          m_global_quantum = new sc_core::sc_time(sc_core::SC_ZERO_TIME);
        }

        return *m_global_quantum;
      }
    
    public:
      tlm_quantumkeeper() :
        m_next_sync_point(sc_core::SC_ZERO_TIME),
        m_local_time(sc_core::SC_ZERO_TIME)
      {
      }
  
      virtual ~tlm_quantumkeeper() {}
    
      //
      // Increment the local time (the time the initiator is ahead of the
      // systemC time) After incrementing the local time an initiator should
      // check (with the need_sync method) if a sync is required.
      //
      virtual void inc(const sc_core::sc_time& t)
      {
        m_local_time += t;
      }

      //
      // Sets the local time (the time the initiator is ahead of the
      // systemC time) After changing the local time an initiator should
      // check (with the need_sync method) if a sync is required.
      //
      virtual void set(const sc_core::sc_time& t)
      {
        m_local_time = t;
      }
    
      //
      // Checks if a sync to systemC is required for this initiator. This will
      // be the case if the local time becomes greater than the local (current)
      // quantum value for this initiator.
      //
      virtual bool need_sync() const
      {
        return sc_core::sc_time_stamp() + m_local_time >= m_next_sync_point;
      }

      //
      // Synchronize to systemC. This call will do a wait for the time the
      // initiator was running ahead of systemC time and reset the
      // tlm_quantumkeeper.
      //
      virtual void sync()
      {
        sc_core::wait(m_local_time);
        reset();
      }
    
      //
      // Resets the local time to SC_ZERO_TIME and computes the value of the
      // next local quantum. This method should be called by an initiator after
      // a wait because of a synchronization request by a target (TLM_ACCEPTED,
      // or TLM_UPDATED).
      //
      virtual void reset()
      {
        m_local_time = sc_core::SC_ZERO_TIME;
        m_next_sync_point = sc_core::sc_time_stamp() + compute_local_quantum();
      }
    
      //
      // Helper function to get the current systemC time, taken the local time
      // into account. The current systemC time is calculated as the time 
      // returned by sc_time_stamp incremeneted with the time the initiator is
      // running ahead.
      //
      virtual sc_core::sc_time get_current_time() const
      {
        return sc_core::sc_time_stamp() + m_local_time;
      }
    
      //
      // Helper functions to get the time the initiator is running ahead of
      // systenC (local time). This time should be passed to a target in the
      // nb_transport call
      //
      virtual sc_core::sc_time get_local_time() const
      {
        return m_local_time;
      }
    
    protected:
      //
      // This function will calculate the next value of the local quantum for
      // this initiator. All initiators will synchronize on integer multiples
      // of the global quantum value. The maximum value for the local quantum
      // will be equal to the global quantum.
      //
      // The method can be overloaded in a derived object if an initiator wants
      // to use another local quantum. This derived object should also take the
      // global quantum into account. It's local quantum should not be set to a
      // value that is larger than the quantum returned by the
      // compute_local_quantum of it's base class.
      //
      virtual sc_core::sc_time compute_local_quantum()
      {
        if (get_global_quantum() != sc_core::SC_ZERO_TIME) {
          const sc_dt::uint64 current = sc_core::sc_time_stamp().value();
          const sc_dt::uint64 g_quant = get_global_quantum().value();
          const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
          const sc_core::sc_time remainder = sc_core::sc_time(tmp - current,
                                                              false);
          return remainder;
  
        } else {
          return sc_core::SC_ZERO_TIME;
        }
      }
    
    private:
      // sc_set_time_resolution can only be called before the first
      // sc_time object is created. To make it possible to call 
      // sc_set_time_resolution from sc_main, m_global_quantum should be
      // pointer. Otherwise it will be created before main is called,
      // which will cause sc_set_time_resolution to fail.
      static sc_core::sc_time *m_global_quantum;
    
    protected:
      sc_core::sc_time m_next_sync_point;
      sc_core::sc_time m_local_time;
    };
    
    template <typename Dummy>
    sc_core::sc_time* tlm_quantumkeeper<Dummy>::m_global_quantum = 0;
  
    struct tlm_dummy {};

  } // namespace tlm_qk_internal
  
  typedef tlm_qk_internal::tlm_quantumkeeper< tlm_qk_internal::tlm_dummy > tlm_quantumkeeper;

} // namespace tlm

#endif
