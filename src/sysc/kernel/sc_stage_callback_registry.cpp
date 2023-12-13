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

  sc_stage_callback_registry.cpp -- Implementation of stage callback registry

  Original Author: Philipp A. Hartmann, OFFIS, 2013-02-15

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_object_int.h"
#include "sysc/kernel/sc_stage_callback_registry.h"
#include "sysc/kernel/sc_kernel_ids.h"
#include "sysc/utils/sc_report.h"

#include <algorithm>
#include <functional>
#include <sstream>

namespace sc_core {

sc_stage_callback_registry::sc_stage_callback_registry( sc_simcontext& simc )
  : m_simc( &simc )
  , m_cb_update_vec()
  , m_cb_timestep_vec()
{}

sc_stage_callback_registry::~sc_stage_callback_registry()
{}

static const sc_stage_callback_registry::mask_type
  SC_STAGE_CALLBACK_MASK = 
    (SC_POST_BEFORE_END_OF_ELABORATION |
     SC_POST_END_OF_ELABORATION |
     SC_POST_START_OF_SIMULATION |
     SC_POST_UPDATE |
     SC_PRE_TIMESTEP |
     SC_PRE_PAUSE |
     SC_PRE_SUSPEND |
     SC_POST_SUSPEND |
     SC_PRE_STOP |
     SC_POST_END_OF_SIMULATION);


namespace /* anonymous */ {

struct entry_match
{
    explicit
    entry_match( sc_stage_callback_registry::cb_type* ref )
      : ref_(ref)
    {}

    bool operator()( const sc_stage_callback_registry::entry& e ) const
        { return e.target == ref_; }
private:
    sc_stage_callback_registry::cb_type * ref_;

}; // entry_match

template< typename T >
inline void
erase_remove( std::vector<T>* vec, T const& t )
{
    vec->erase( std::remove( vec->begin(), vec->end(), t ) );
}

}  // namespace anonymous


sc_stage_callback_registry::mask_type
sc_stage_callback_registry::validate_mask( cb_type& cb
                                         , mask_type m
                                         , bool warn = false )
{
    if( SC_UNLIKELY_(m & ~SC_STAGE_CALLBACK_MASK) )
    {
        if( warn )
        {
            std::stringstream ss;
            ss << "Invalid stage callback mask: "
               << (sc_stage)m;
            SC_REPORT_WARNING( SC_ID_STAGE_CALLBACK_REGISTER_
                             , ss.str().c_str() );
        }
        m &= SC_STAGE_CALLBACK_MASK;
    }

    mask_type check_mask;

    // elaboration callbacks
    check_mask = ( SC_POST_BEFORE_END_OF_ELABORATION
                 | SC_POST_END_OF_ELABORATION );
    if( SC_UNLIKELY_( (m & check_mask ) && m_simc->elaboration_done() ) )
    {
        if( warn )
        {
            std::stringstream ss;
            ss << "Elaboration done\n\t "
               << (sc_stage)( m & check_mask ) << " callback(s) ignored";
            SC_REPORT_WARNING(SC_ID_STAGE_CALLBACK_REGISTER_
                             , ss.str().c_str() );
        }
        m &= ~check_mask;
    }
    return m;
}


void
sc_stage_callback_registry::register_callback( cb_type& cb, mask_type m )
{
    storage_type::iterator it =
      find_if( m_cb_vec.begin(), m_cb_vec.end(), entry_match(&cb) );

    m = validate_mask(cb, m, /* warn */ true );

    mask_type diff_mask = m;
    mask_type new_mask  = m;

    if( it != m_cb_vec.end() ) // update existing entry
    {
        // update masks
        new_mask   =  (*it).mask | m;
        diff_mask  = ~(*it).mask & m;
        (*it).mask = new_mask;
    }
    else // new entry
    {
        if( !m ) // empty, do nothing
            return;

        entry new_entry = { &cb, new_mask };
        m_cb_vec.push_back( new_entry );
    }

    // add to callback shortcut sets

    if( diff_mask & SC_POST_UPDATE )
        m_cb_update_vec.push_back( &cb );
    if( diff_mask & SC_PRE_TIMESTEP )
        m_cb_timestep_vec.push_back( &cb );
}


void
sc_stage_callback_registry::unregister_callback( cb_type& cb, mask_type m )
{
    storage_type::iterator it =
      find_if( m_cb_vec.begin(), m_cb_vec.end(), entry_match(&cb) );

    m = validate_mask(cb, m);

    mask_type diff_mask = m;
    mask_type new_mask  = m;

    if( it == m_cb_vec.end() ) { // not registered
        return;
    }

    // update masks
    new_mask   = (*it).mask & ~m;
    diff_mask  = (*it).mask & m;
    (*it).mask = new_mask;

    if( !new_mask )
        m_cb_vec.erase(it);

    // drop from callback shortcut sets
    if( diff_mask & SC_POST_UPDATE )
        erase_remove( &m_cb_update_vec, &cb );
    if( diff_mask & SC_PRE_TIMESTEP )
        erase_remove( &m_cb_timestep_vec, &cb );
}


// generic implementation (for non-critical callbacks)
void
sc_stage_callback_registry::do_callback( sc_stage s ) const
{
    typedef storage_type::const_iterator it_type;
    storage_type const & vec = m_cb_vec;

    for(it_type it = vec.begin(), end = vec.end(); it != end; ++it) {
        if( s & it->mask ) {
            it->target->stage_callback(s);
        }
    }
}

// pretty-print a combination of sc_stage bits (i.e. a callback mask)
static std::ostream&
print_stage_expression( std::ostream& os, sc_stage s );

// utility helper to print a simulation stage
SC_API std::ostream& operator << ( std::ostream& os, sc_stage s )
{
    // print primitive values
    switch(s)
    {
#   define PRINT_STAGE( Stage ) \
      case Stage: { os << #Stage; } break

      PRINT_STAGE( SC_POST_BEFORE_END_OF_ELABORATION );
      PRINT_STAGE( SC_POST_END_OF_ELABORATION );
      PRINT_STAGE( SC_POST_START_OF_SIMULATION );
      PRINT_STAGE( SC_POST_UPDATE );
      PRINT_STAGE( SC_PRE_TIMESTEP );
      PRINT_STAGE( SC_PRE_PAUSE );
      PRINT_STAGE( SC_PRE_SUSPEND );
      PRINT_STAGE( SC_POST_SUSPEND );
      PRINT_STAGE( SC_PRE_STOP );
      PRINT_STAGE( SC_POST_END_OF_SIMULATION );

#   undef PRINT_STAGE
    default:

      if( s & SC_STAGE_CALLBACK_MASK ) // combination of stage bits
        print_stage_expression( os, s );
      else                    // invalid number, print hex value
        os << "0x" << std::hex << +s;
    }

    return os;
}

// pretty-print a combination of sc_stage bits (i.e. a callback mask)
static std::ostream&
print_stage_expression( std::ostream& os, sc_stage s )
{
    std::vector<sc_stage> bits;
    unsigned              is_set = 1;
    unsigned              SC_STAGE_LAST = SC_POST_END_OF_SIMULATION;

    // collect bits
    while( is_set <= SC_STAGE_LAST )
    {
        if( s & is_set )
            bits.push_back( (sc_stage)is_set );
        is_set <<= 1;
    }
    if( s & ~SC_STAGE_CALLBACK_MASK ) // remaining bits
        bits.push_back( (sc_stage)( s & ~SC_STAGE_CALLBACK_MASK ) );

    // print expression
    std::vector<sc_status>::size_type i=0, n=bits.size();
    if ( n>1 )
        os << "(";
    for( ; i<n-1; ++i )
        os << bits[i] << "|";
    os << bits[i];
    if ( n>1 )
        os << ")";
    return os;
}

} // namespace sc_core

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// Taf!
