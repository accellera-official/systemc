/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2008 by all Contributors.
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

#ifndef __TLM_GLOBAL_QUANTUM_H__
#define __TLM_GLOBAL_QUANTUM_H__

#include <systemc>

namespace tlm {

  namespace tlm_qk_internal {

    //
    // tlm_global_quantum class
    //
    // The global quantum is the maximum time an initiator can run ahead of
    // systemC time. All initiators should synchronize on timingpoints that
    // are multiples of the global quantum value.
    //
    // sc_set_time_resolution can only be called before the first
    // sc_time object is created. This means that after setting the
    // global quantum it will not be possible to call sc_set_time_resolution.
    // If sc_set_time_resolution must be called this must be done before
    // the global quantum is set.
    //
    // NOTE:
    // The templated class tlm::tlm_qk_internal::tlm_global_quantum<Dummy> is
    // an artifact of the implementation and not part of the core TLM standard.
    // It should never be used directly, instead the class
    // tlm::tlm_global_quantum should be used.
    //
    // The dummy template parameter is introduced to make it possible to
    // initialize the static member (m_instance) in the headerfile. This
    // is only possible for template classes, otherwise the static member must
    // be initialized in a source file.
    //
    template <typename Dummy>
    class tlm_global_quantum
    {
    public:
      //
      // Returns a reference to the tlm_global_quantum singleton
      //
      static tlm_global_quantum& instance()
      {
        if (!m_instance) {
          m_instance = new tlm_global_quantum;
        }
        return *m_instance;
      }

    public:
      virtual ~tlm_global_quantum()
      {
        m_instance = 0;
      }
    
      //
      // Setter/getter for the global quantum
      //
      void set(const sc_core::sc_time& t)
      {
        m_global_quantum = t;
      }
    
      const sc_core::sc_time& get() const
      {
        return m_global_quantum;
      }

      //
      // This function will calculate the maximum value for the next local
      // quantum for an initiator. All initiators should synchronize on
      // integer multiples of the global quantum value. The value for the
      // local quantum of an initiator can be smaller, but should never be
      // greater than the value returned by this method.
      //
      sc_core::sc_time compute_local_quantum()
      {
        if (m_global_quantum != sc_core::SC_ZERO_TIME) {
          const sc_dt::uint64 current = sc_core::sc_time_stamp().value();
          const sc_dt::uint64 g_quant = m_global_quantum.value();
          const sc_dt::uint64 tmp = (current/g_quant+sc_dt::uint64(1)) * g_quant;
          const sc_core::sc_time remainder = sc_core::sc_time(tmp - current,
                                                              false);
          return remainder;

        } else {
          return sc_core::SC_ZERO_TIME;
        }
      }

    protected:
      tlm_global_quantum() : m_global_quantum(sc_core::SC_ZERO_TIME)
      {
      }
  
    protected:
      static tlm_global_quantum* m_instance;
      sc_core::sc_time m_global_quantum;
    };
    
    template <typename Dummy>
    tlm_global_quantum<Dummy>* tlm_global_quantum<Dummy>::m_instance = 0;
  
    struct tlm_dummy {};

  } // namespace tlm_qk_internal
  
  typedef tlm_qk_internal::tlm_global_quantum<tlm_qk_internal::tlm_dummy> tlm_global_quantum;

} // namespace tlm

#endif
