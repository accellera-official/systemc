/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2011 by all Contributors.
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

/*****************************************************************************

  sc_cor_fiber.h -- Coroutine implementation with fibers.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-12-18

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_COR_FIBER_H
#define SC_COR_FIBER_H


#ifdef WIN32
#ifdef __GNUC__
#   include <windows.h>
#endif

#include "sysc/kernel/sc_cor.h"
#include "sysc/kernel/sc_cmnhdr.h"

#if defined(__GNUC__) && __USING_SJLJ_EXCEPTIONS__
   // _Unwind_SjLj_Register() & _Unwind_SjLj_Unregister() only need first field.
   struct SjLj_Function_Context {
       struct SjLj_Function_Context *prev;
   };
#endif

namespace sc_core {

class sc_cor_pkg_fiber;

#if( defined(_MSC_VER) && _MSC_VER >= 1300 )
typedef std::size_t size_t;
#endif

// ----------------------------------------------------------------------------
//  CLASS : sc_cor_fiber
//
//  Coroutine class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_fiber
: public sc_cor
{

public:

    // constructor
    sc_cor_fiber()
	: m_stack_size( 0 ), m_fiber( 0 ), m_pkg( 0 )
       {
#         if defined(__GNUC__) && __USING_SJLJ_EXCEPTIONS__
              m_eh.prev = 0;
#         endif
       }

    // destructor
    virtual ~sc_cor_fiber();

public:

    std::size_t       m_stack_size;     // stack size
    PVOID             m_fiber;          // fiber

    sc_cor_pkg_fiber* m_pkg;            // the creating coroutine package
#if defined(__GNUC__) && __USING_SJLJ_EXCEPTIONS__
    struct SjLj_Function_Context m_eh;  // the exception handling context
#endif


private:

    // disabled
    sc_cor_fiber( const sc_cor_fiber& );
    sc_cor_fiber& operator = ( const sc_cor_fiber& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg_fiber
//
//  Coroutine package class implemented with QuickThreads.
// ----------------------------------------------------------------------------

class sc_cor_pkg_fiber
: public sc_cor_pkg
{
  public:

    // constructor
    sc_cor_pkg_fiber( sc_simcontext* simc );

    // destructor
    virtual ~sc_cor_pkg_fiber();

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
    sc_cor_pkg_fiber();
    sc_cor_pkg_fiber( const sc_cor_pkg_fiber& );
    sc_cor_pkg_fiber& operator = ( const sc_cor_pkg_fiber& );
};

} // namespace sc_core

#endif // WIN32

// $Log: sc_cor_fiber.h,v $
// Revision 1.6  2011/08/26 20:46:09  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.5  2011/06/25 17:08:39  acg
//  Andy Goodrich: Jerome Cornet's changes to use libtool to build the
//  library.
//
// Revision 1.4  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.3  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.2  2008/05/22 17:06:25  acg
//  Andy Goodrich: updated copyright notice to include 2008.
//
// Revision 1.1.1.1  2006/12/15 20:20:05  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:44:29  acg
// Added $Log to record CVS changes into the source.
//

#endif

// Taf!
