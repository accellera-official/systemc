/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2007 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

#ifndef __TLM_QUANTUMKEEPER_H__
#define __TLM_QUANTUMKEEPER_H__

namespace tlm_qk_dummy {

  // FIMXE: Dummy template argument to be able to initialize
  //        mGlobalQuantum in the headerfile
  template <typename Dummy>
  class tlm_quantumkeeper
  {
  public:
    static void setGlobalQuantum(const sc_core::sc_time& t)
    {
      mGlobalQuantum = t;
    }
  
    static const sc_core::sc_time& getGlobalQuantum() 
    {
      return mGlobalQuantum;
    }
  
  public:
    tlm_quantumkeeper() :
      mLocalTime(sc_core::SC_ZERO_TIME)
    {
      computeLocalQuantum();
    }
  
    void inc(const sc_core::sc_time& t)
    {
      mLocalTime += t;
    }
  
    bool needSync() const
    {
      return mLocalTime >= mLocalQuantum;
    }
  
    void reset()
    {
      mLocalTime = sc_core::SC_ZERO_TIME;
      computeLocalQuantum();
    }
  
    sc_core::sc_time getCurrentTime() const
    {
      return sc_core::sc_time_stamp() + mLocalTime;
    }
  
    const sc_core::sc_time& getLocalTime() const
    {
      return mLocalTime;
    }
  
    sc_core::sc_time& getLocalTime()
    {
      return mLocalTime;
    }
  
    void sync()
    {
      sc_core::wait(mLocalTime);
      reset();
    }
  
  protected:
    void computeLocalQuantum()
    {
      mLocalQuantum = mGlobalQuantum;
    }
  
  private:
    static sc_core::sc_time mGlobalQuantum;
  
  private:
    sc_core::sc_time mLocalQuantum;
    sc_core::sc_time mLocalTime;
  };
  
  template <typename Dummy>
  sc_core::sc_time tlm_quantumkeeper<Dummy>::mGlobalQuantum = sc_core::SC_ZERO_TIME;

  struct tlm_dummy {};
} // namespace tlm_qk_dummy

namespace tlm {
typedef ::tlm_qk_dummy::tlm_quantumkeeper< ::tlm_qk_dummy::tlm_dummy >
  tlm_quantumkeeper;

} // namespace tlm

#endif
