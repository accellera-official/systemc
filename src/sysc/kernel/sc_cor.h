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

  sc_cor.h -- Coroutine abstract base classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-12-18

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/


#ifndef SC_COR_H
#define SC_COR_H


#include <cassert>
#include <cstdlib>

namespace sc_core {

class sc_simcontext;


// ----------------------------------------------------------------------------
//  TYPEDEF : sc_cor_fn
//
//  Function type for creating coroutines.
// ----------------------------------------------------------------------------

typedef void (sc_cor_fn)( void* );


// ----------------------------------------------------------------------------
//  CLASS : sc_cor
//
//  Coroutine abstract base class.
// ----------------------------------------------------------------------------

class sc_cor
{
protected:

    // constructor
    sc_cor() {}

public:

    // destructor
    virtual ~sc_cor() {}

    // switch stack protection on/off
    virtual void stack_protect( bool /* enable */ ) {}

private:

    // disabled
    sc_cor( const sc_cor& );
    sc_cor& operator = ( const sc_cor& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_cor_pkg
//
//  Coroutine package abstract base class.
// ----------------------------------------------------------------------------

class sc_cor_pkg
{
public:

    // constructor
    sc_cor_pkg( sc_simcontext* simc )
        : m_simc( simc ) { assert( simc != 0 ); }

    // destructor
    virtual ~sc_cor_pkg() {}

    // create a new coroutine
   virtual sc_cor* create(
	    std::size_t stack_size, sc_cor_fn* fn, void* arg ) = 0;

    // yield to the next coroutine
    virtual void yield( sc_cor* next_cor ) = 0;

    // abort the current coroutine (and resume the next coroutine)
    virtual void abort( sc_cor* next_cor ) = 0;

    // get the main coroutine
    virtual sc_cor* get_main() = 0;

    // get the simulation context
    sc_simcontext* simcontext()
        { return m_simc; }

private:

    sc_simcontext* m_simc;

private:

    // disabled
    sc_cor_pkg();
    sc_cor_pkg( const sc_cor_pkg& );
    sc_cor_pkg& operator = ( const sc_cor_pkg& );
};

} // namespace sc_core

// $Log: sc_cor.h,v $
// Revision 1.7  2011/08/26 20:46:09  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.6  2011/08/15 16:43:24  acg
//  Torsten Maehne: changes to remove unused argument warnings.
//
// Revision 1.5  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.4  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.3  2011/01/19 23:21:49  acg
//  Andy Goodrich: changes for IEEE 1666 2011
//
// Revision 1.2  2008/05/22 17:06:24  acg
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
