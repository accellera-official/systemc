/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2014 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.accellera.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_object_int.h -- For inline definitions of some utility functions.
                     DO NOT EXPORT THIS INCLUDE FILE.

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-10

 *****************************************************************************/

#ifndef SC_OBJECT_INT_H_INCLUDED_
#define SC_OBJECT_INT_H_INCLUDED_

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_module.h"
#include "sysc/kernel/sc_simcontext_int.h"
#include "sysc/kernel/sc_phase_callback_registry.h"

namespace sc_core {

class sc_object::hierarchy_scope
{
public:
    explicit hierarchy_scope(sc_object* obj);
    explicit hierarchy_scope(sc_module* mod);
    ~hierarchy_scope();

private:
    sc_module * scope_;

private:
    hierarchy_scope( hierarchy_scope const & other ) /* = delete */;
    hierarchy_scope& operator=(hierarchy_scope const&) /* = delete */;
};


inline
sc_object::hierarchy_scope::hierarchy_scope( sc_object* obj )
  : scope_(0)
{
    if( !obj ) return;

    scope_ = dynamic_cast<sc_module*>(obj);
    if( !scope_ )
        scope_ = dynamic_cast<sc_module*>(obj->get_parent_object());
    if( scope_ )
        scope_->simcontext()->hierarchy_push(scope_);
}


inline
sc_object::hierarchy_scope::hierarchy_scope( sc_module* mod )
  : scope_(mod)
{
    if( scope_ )
        scope_->simcontext()->hierarchy_push(scope_);
}


inline
sc_object::hierarchy_scope::~hierarchy_scope()
{
    if( scope_ )
        scope_->simcontext()->hierarchy_pop();
}


// -----------------------------------------------------------------------

inline void
sc_object::do_simulation_phase_callback()
{
    simulation_phase_callback();
}

// -----------------------------------------------------------------------

} // namespace sc_core

#endif // SC_OBJECT_INT_H_INCLUDED_
// Taf!
