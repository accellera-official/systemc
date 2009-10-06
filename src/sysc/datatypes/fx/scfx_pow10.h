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

  scfx_pow10.h - 

  Original Author: Robert Graulich, Synopsys, Inc.
                   Martin Janssen,  Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: scfx_pow10.h,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:58  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SCFX_POW10_H
#define SCFX_POW10_H


#include "sysc/datatypes/fx/scfx_rep.h"


namespace sc_dt
{

// classes defined in this module
class scfx_pow10;


// ----------------------------------------------------------------------------
//  CLASS : scfx_pow10
//
//  Class to compute (and cache) powers of 10 in arbitrary precision.
// ----------------------------------------------------------------------------

const int SCFX_POW10_TABLE_SIZE = 32;


class scfx_pow10
{

public:

    scfx_pow10();
    ~scfx_pow10();

    const scfx_rep operator() ( int );

private:

    scfx_rep* pos( int );
    scfx_rep* neg( int );

    scfx_rep m_pos[SCFX_POW10_TABLE_SIZE];
    scfx_rep m_neg[SCFX_POW10_TABLE_SIZE];
};

} // namespace sc_dt


#endif

// Taf!
