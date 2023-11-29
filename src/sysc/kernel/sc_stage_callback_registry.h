/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_stage_callback_registry.h -- Definition of the simulation stage callbacks

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-15

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_STAGE_CALLBACK_REGISTRY_H_INCLUDED_
#define SC_STAGE_CALLBACK_REGISTRY_H_INCLUDED_

#include "sysc/kernel/sc_simcontext.h"
#include "sysc/kernel/sc_object_int.h"
#include "sysc/kernel/sc_status.h"

#include <vector>

namespace sc_core {

class sc_stage_callback_registry
{
public:
    typedef sc_stage_callback_registry this_type;
    typedef sc_stage_callback_if       cb_type;
    typedef cb_type::stage_cb_mask     mask_type;

    struct entry
    {
        cb_type*  target;
        mask_type mask;
    };

    friend class sc_simcontext;
    friend class sc_object;
    friend SC_API void sc_register_stage_callback(sc_stage_callback_if & cb,
                                                  sc_stage_callback_if::stage_cb_mask mask);
    friend SC_API void sc_unregister_stage_callback(sc_stage_callback_if & cb,
                                                    sc_stage_callback_if::stage_cb_mask mask);


private: // interface completely internal

    explicit
    sc_stage_callback_registry( sc_simcontext& simc );

    ~sc_stage_callback_registry();

    // --- callback forwarders

    void construction_done()   const;
    void elaboration_done()    const;
    void start_simulation()    const;

    void update_done()         const;
    void before_timestep()     const;

    void pre_suspend()         const;
    void post_suspend()        const;

    void simulation_paused()   const;
    void simulation_stopped()  const;
    void simulation_done()     const;


    // --- callback registration and implementation

    void register_callback( cb_type&, mask_type mask );
    void unregister_callback( cb_type&, mask_type mask );

    // generic caller
    void do_callback( sc_stage ) const;

private:
    typedef std::vector<entry>    storage_type;
    typedef std::vector<cb_type*> single_storage_type;

    // set and restore simulation stage
    struct scoped_stage
    {
        scoped_stage( sc_stage& ref, sc_stage s )
          : ref_(ref), prev_(ref) 
	{ 
	      sc_scoped_lock lock(sc_get_curr_simcontext()->m_simulation_status_mutex); ref_ = s;
        }
        ~scoped_stage() { 
	    sc_scoped_lock lock(sc_get_curr_simcontext()->m_simulation_status_mutex); ref_ = prev_; 
	}
    private:
        sc_stage& ref_;
        sc_stage  prev_;
    }; // scoped_stage

    mask_type validate_mask( cb_type&, mask_type, bool warn );

private:

    sc_simcontext*        m_simc;
    storage_type          m_cb_vec;          // all callbacks
    single_storage_type   m_cb_update_vec;   //  - update cb shortcut
    single_storage_type   m_cb_timestep_vec; //  - timestep cb shortcut

private:
    // disabled
    sc_stage_callback_registry( const this_type& );
    this_type& operator=(const this_type&);

}; // sc_stage_callback_registry


// -------------------- callback implementations --------------------
//                   (empty, if feature is disabled)

inline void
sc_stage_callback_registry::construction_done() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_BEFORE_END_OF_ELABORATION );
    do_callback( SC_POST_BEFORE_END_OF_ELABORATION );
}

inline void
sc_stage_callback_registry::elaboration_done() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_END_OF_ELABORATION );
    do_callback( SC_POST_END_OF_ELABORATION );
}

inline void
sc_stage_callback_registry::start_simulation() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_START_OF_SIMULATION );
    do_callback( SC_POST_START_OF_SIMULATION );
}

inline void
sc_stage_callback_registry::pre_suspend() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_PRE_SUSPEND );
    do_callback( SC_PRE_SUSPEND );
}

inline void
sc_stage_callback_registry::post_suspend() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_SUSPEND );
    do_callback( SC_POST_SUSPEND );
}

inline void
sc_stage_callback_registry::simulation_paused() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_PRE_PAUSE );
    do_callback( SC_PRE_PAUSE );
}

inline void
sc_stage_callback_registry::simulation_stopped() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_PRE_STOP );
    do_callback( SC_PRE_STOP );
}

inline void
sc_stage_callback_registry::simulation_done() const
{
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_END_OF_SIMULATION );
    do_callback( SC_POST_END_OF_SIMULATION );
}

// -------------- specialized callback implementations --------------

inline void
sc_stage_callback_registry::update_done() const
{

    if( SC_LIKELY_(!m_cb_update_vec.size()) ) return;
    scoped_stage scope( m_simc->m_stage
                       , SC_POST_UPDATE );

    typedef single_storage_type::const_iterator it_type;
    single_storage_type const & vec = m_cb_update_vec;
 
    for(it_type it = vec.begin(), end = vec.end(); it != end; ++it)
        (*it)->stage_callback(SC_POST_UPDATE);
}

inline void
sc_stage_callback_registry::before_timestep() const
{
    if( SC_LIKELY_(!m_cb_timestep_vec.size()) ) return;

    scoped_stage scope( m_simc->m_stage
                       , SC_PRE_TIMESTEP );
    typedef single_storage_type::const_iterator it_type;
    single_storage_type const & vec = m_cb_timestep_vec;

    for(it_type it = vec.begin(), end = vec.end(); it != end; ++it)
        (*it)->stage_callback(SC_PRE_TIMESTEP);
}

// ------------------------------------------------------------------

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#endif /* SC_STAGE_CALLBACK_REGISTRY_H_INCLUDED_ */

// Taf!

