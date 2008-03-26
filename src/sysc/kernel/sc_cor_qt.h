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

  sc_cor_qt.h -- Coroutine implementation with QuickThreads.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-12-18

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_cor_qt.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:37  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#ifndef SC_COR_QT_H
#define SC_COR_QT_H


#if !defined(WIN32) && !defined(SC_USE_PTHREADS)

#include "sysc/kernel/sc_cor.h"
#include "sysc/qt/qt.h"

namespace sc_core {

class sc_cor_pkg_qt;


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_qt
//
//  Coroutine class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_qt
: public sc_cor
{
public:

    // constructor
    sc_cor_qt()
	: m_stack_size( 0 ), m_stack( 0 ), m_sp( 0 ), m_pkg( 0 )
	{}

    // destructor
    virtual ~sc_cor_qt()
	{ if( m_stack != 0 ) { delete[] (char*) m_stack; } }

    // switch stack protection on/off
    virtual void stack_protect( bool enable );

public:

    std::size_t    m_stack_size;  // stack size
    void*          m_stack;       // stack
    qt_t*          m_sp;          // stack pointer

    sc_cor_pkg_qt* m_pkg;         // the creating coroutine package

private:

    // disabled
    sc_cor_qt( const sc_cor_qt& );
    sc_cor_qt& operator = ( const sc_cor_qt& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg_qt
//
//  Coroutine package class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_pkg_qt
: public sc_cor_pkg
{
public:

    // constructor
    sc_cor_pkg_qt( sc_simcontext* simc );

    // destructor
    virtual ~sc_cor_pkg_qt();

    // create a new coroutine
    virtual sc_cor* create( std::size_t stack_size, sc_cor_fn* fn, void* arg );

    // yield to the next coroutine
    virtual void yield( sc_cor* next_cor );

    // abort the current coroutine (and resume the next coroutine)
    virtual void abort( sc_cor* next_cor );

    // get the main coroutine
    virtual sc_cor* get_main();

private:

    static int instance_count;

private:

    // disabled
    sc_cor_pkg_qt();
    sc_cor_pkg_qt( const sc_cor_pkg_qt& );
    sc_cor_pkg_qt& operator = ( const sc_cor_pkg_qt& );
};

} // namespace sc_core

#endif


#endif

// Taf!
