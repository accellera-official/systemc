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

  sc_fxnum_observer.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_fxnum_observer.h,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:58  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_FXNUM_OBSERVER_H
#define SC_FXNUM_OBSERVER_H


#include "sysc/datatypes/fx/sc_fxdefs.h"


namespace sc_dt
{

// classes defined in this module
class sc_fxnum_observer;
class sc_fxnum_fast_observer;

// forward class declarations
class sc_fxnum;
class sc_fxnum_fast;


#ifdef SC_ENABLE_OBSERVERS

#define SC_FXNUM_OBSERVER_CONSTRUCT_(object)                                  \
    SC_OBSERVER_(object,sc_fxnum_observer*,construct)
#define SC_FXNUM_OBSERVER_DESTRUCT_(object)                                   \
    SC_OBSERVER_(object,sc_fxnum_observer*,destruct)
#define SC_FXNUM_OBSERVER_READ_(object)                                       \
    SC_OBSERVER_(object,sc_fxnum_observer*,read)
#define SC_FXNUM_OBSERVER_WRITE_(object)                                      \
    SC_OBSERVER_(object,sc_fxnum_observer*,write)
#define SC_FXNUM_OBSERVER_DEFAULT_                                            \
    SC_OBSERVER_DEFAULT_(sc_fxnum_observer)

#define SC_FXNUM_FAST_OBSERVER_CONSTRUCT_(object)                             \
    SC_OBSERVER_(object,sc_fxnum_fast_observer*,construct)
#define SC_FXNUM_FAST_OBSERVER_DESTRUCT_(object)                              \
    SC_OBSERVER_(object,sc_fxnum_fast_observer*,destruct)
#define SC_FXNUM_FAST_OBSERVER_READ_(object)                                  \
    SC_OBSERVER_(object,sc_fxnum_fast_observer*,read)
#define SC_FXNUM_FAST_OBSERVER_WRITE_(object)                                 \
    SC_OBSERVER_(object,sc_fxnum_fast_observer*,write)
#define SC_FXNUM_FAST_OBSERVER_DEFAULT_                                       \
    SC_OBSERVER_DEFAULT_(sc_fxnum_fast_observer)

#else

#define SC_FXNUM_OBSERVER_CONSTRUCT_(object)
#define SC_FXNUM_OBSERVER_DESTRUCT_(object)
#define SC_FXNUM_OBSERVER_READ_(object)
#define SC_FXNUM_OBSERVER_WRITE_(object)
#define SC_FXNUM_OBSERVER_DEFAULT_

#define SC_FXNUM_FAST_OBSERVER_CONSTRUCT_(object)
#define SC_FXNUM_FAST_OBSERVER_DESTRUCT_(object)
#define SC_FXNUM_FAST_OBSERVER_READ_(object)
#define SC_FXNUM_FAST_OBSERVER_WRITE_(object)
#define SC_FXNUM_FAST_OBSERVER_DEFAULT_

#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_observer
//
//  Abstract base class for fixed-point types observers; arbitrary precision.
// ----------------------------------------------------------------------------

class sc_fxnum_observer
{

protected:
    
    sc_fxnum_observer() {}
    virtual ~sc_fxnum_observer() {}

public:

    virtual void construct( const sc_fxnum& );
    virtual void  destruct( const sc_fxnum& );
    virtual void      read( const sc_fxnum& );
    virtual void     write( const sc_fxnum& );

    static sc_fxnum_observer* (*default_observer) ();

};


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_observer
//
//  Abstract base class for fixed-point types observers; limited precision.
// ----------------------------------------------------------------------------

class sc_fxnum_fast_observer
{

protected:
    
    sc_fxnum_fast_observer() {}
    virtual ~sc_fxnum_fast_observer() {}

public:

    virtual void construct( const sc_fxnum_fast& );
    virtual void  destruct( const sc_fxnum_fast& );
    virtual void      read( const sc_fxnum_fast& );
    virtual void     write( const sc_fxnum_fast& );

    static sc_fxnum_fast_observer* (*default_observer) ();

};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_observer
//
//  Abstract base class for fixed-point types observers; arbitrary precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxnum_observer::construct( const sc_fxnum& )
{}

inline
void
sc_fxnum_observer::destruct( const sc_fxnum& )
{}

inline
void
sc_fxnum_observer::read( const sc_fxnum& )
{}

inline
void
sc_fxnum_observer::write( const sc_fxnum& )
{}


// ----------------------------------------------------------------------------
//  CLASS : sc_fxnum_fast_observer
//
//  Abstract base class for fixed-point types observers; limited precision.
// ----------------------------------------------------------------------------

inline
void
sc_fxnum_fast_observer::construct( const sc_fxnum_fast& )
{}

inline
void
sc_fxnum_fast_observer::destruct( const sc_fxnum_fast& )
{}

inline
void
sc_fxnum_fast_observer::read( const sc_fxnum_fast& )
{}

inline
void
sc_fxnum_fast_observer::write( const sc_fxnum_fast& )
{}

} // namespace sc_dt


#endif

// Taf!
