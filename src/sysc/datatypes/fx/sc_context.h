/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

/*****************************************************************************

  sc_context.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_context.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.4  2006/03/21 00:00:31  acg
//   Andy Goodrich: changed name of sc_get_current_process_base() to be
//   sc_get_current_process_b() since its returning an sc_process_b instance.
//
// Revision 1.3  2006/01/13 18:53:57  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_CONTEXT_H
#define SC_CONTEXT_H


#include "sysc/datatypes/fx/sc_fx_ids.h"
#include "sysc/kernel/sc_simcontext.h"
#include "sysc/utils/sc_hash.h"

namespace sc_core {
	class sc_process_b;
}

using sc_core::default_ptr_hash_fn;

namespace sc_dt
{

// classes defined in this module
class sc_without_context;
template <class T> class sc_global;
template <class T> class sc_context;


// ----------------------------------------------------------------------------
//  CLASS : sc_without_context
//
//  Empty class that is used for its type only.
// ----------------------------------------------------------------------------

class sc_without_context {};


// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_global
//
//  Template global variable class; singleton; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
class sc_global
{

    sc_global();

    void update();

public:

    static sc_global<T>* instance();

    const T*& value_ptr();

private:

    static sc_global<T>* m_instance;

    sc_core::sc_phash<const sc_core::sc_process_b*,const T*> m_map;
    const sc_core::sc_process_b*                             m_proc;
    const T*                                                    m_value_ptr;

};


// ----------------------------------------------------------------------------
//  ENUM : sc_context_begin
//
//  Enumeration of context begin options.
// ----------------------------------------------------------------------------

enum sc_context_begin
{
    SC_NOW,
    SC_LATER
};


// ----------------------------------------------------------------------------
//  CLASS : sc_context
//
//  Template context class; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
class sc_context
{
    sc_context( const sc_context<T>& );
    void* operator new( std::size_t );

public:

    explicit sc_context( const T&, sc_context_begin = SC_NOW );
    ~sc_context();

    void begin();
    void end();

    static const T& default_value();
    const T& value() const;

private:

    const T   m_value;
    const T*& m_def_value_ptr;
    const T*  m_old_value_ptr;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  TEMPLATE CLASS : sc_global
//
//  Template global variable class; singleton; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
sc_global<T>* sc_global<T>::m_instance = 0;


template <class T>
inline
sc_global<T>::sc_global()
: m_proc( 
	reinterpret_cast<const sc_core::sc_process_b*>( -1 ) ), 
	m_value_ptr( 0 )
{}


template <class T>
inline
void
sc_global<T>::update()
{
    const sc_core::sc_process_b* p = sc_core::sc_get_current_process_b();
    if( p != m_proc )
    {
        const T* vp = m_map[p];
        if( vp == 0 )
        {
            vp = new T( sc_without_context() );
            m_map.insert( p, vp );
        }
        m_proc = p;
        m_value_ptr = vp;
    }
}


template <class T>
inline
sc_global<T>*
sc_global<T>::instance()
{
    if( m_instance == 0 )
    {
        m_instance = new sc_global<T>;
    }
    return m_instance;
}


template <class T>
inline
const T*&
sc_global<T>::value_ptr()
{
    update();
    return m_value_ptr;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_context
//
//  Template context class; co-routine safe.
// ----------------------------------------------------------------------------

template <class T>
inline
sc_context<T>::sc_context( const sc_context<T>& )
: m_value(),
  m_def_value_ptr( sc_global<T>::instance()->value_ptr() ),
  m_old_value_ptr( 0 )
{
    // this constructor should never be called
    SC_REPORT_FATAL( sc_core::SC_ID_INTERNAL_ERROR_, "should never be called" );
}

template <class T>
inline
void*
sc_context<T>::operator new( std::size_t )
{
    // this method should never be called
    SC_REPORT_FATAL( sc_core::SC_ID_INTERNAL_ERROR_, "should never be called" );
    return (void*)0;
}


template <class T>
inline
sc_context<T>::sc_context( const T& value_, sc_context_begin begin )
: m_value( value_ ),
  m_def_value_ptr( sc_global<T>::instance()->value_ptr() ),
  m_old_value_ptr( 0 )
{
    if( begin == SC_NOW )
    {
	m_old_value_ptr = m_def_value_ptr;
	m_def_value_ptr = &m_value;
    }
}

template <class T>
inline
sc_context<T>::~sc_context()
{
    if( m_old_value_ptr != 0 )
    {
        m_def_value_ptr = m_old_value_ptr;
	m_old_value_ptr = 0;
    }
}


template <class T>
inline
void
sc_context<T>::begin()
{
    if( m_old_value_ptr == 0 )
    {
	m_old_value_ptr = m_def_value_ptr;
	m_def_value_ptr = &m_value;
    }
    else
    {
        SC_REPORT_ERROR( sc_core::SC_ID_CONTEXT_BEGIN_FAILED_, 0 );
    }
}

template <class T>
inline
void
sc_context<T>::end()
{
    if( m_old_value_ptr != 0 )
    {
        m_def_value_ptr = m_old_value_ptr;
	m_old_value_ptr = 0;
    }
    else
    {
        SC_REPORT_ERROR( sc_core::SC_ID_CONTEXT_END_FAILED_, 0 );
    }
}


template <class T>
inline
const T&
sc_context<T>::default_value()
{
    return *sc_global<T>::instance()->value_ptr();
}

template <class T>
inline
const T&
sc_context<T>::value() const
{
    return m_value;
}

} // namespace sc_dt


#endif

// Taf!
