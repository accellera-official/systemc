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

  sc_constants.h -- Default constants whose values may need to be
                    changed depending on the application.

  Original Author: Ali Dasdan, Synopsys, Inc.

  CHANGE LOG AT THE END OF THE FILE
 *****************************************************************************/

#ifndef SC_CONSTANTS_H
#define SC_CONSTANTS_H

namespace sc_core {

// Maximum number of bits for arbitrary precision arithmetic. If
// defined, the arithmetic becomes faster. If not defined, the
// arithmetic becomes slower and the precision becomes infinite.  It
// is a good idea to define this constant as a multiple of
// BITS_PER_DIGIT, which is defined in numeric_bit/sc_nbdefs.h.
//#define SC_MAX_NBITS    510    // 17 * BITS_PER_DIGIT


// deprecated in 1666-2005 and later, but kept for backwards compatibility
//  - can be set by defining SC_OVERRIDE_DEFAULT_STACK_SIZE
//  - defaults defined in sc_thread_process.cpp
extern const int SC_DEFAULT_STACK_SIZE;


#ifdef DEBUG_SYSTEMC
const int SC_MAX_NUM_DELTA_CYCLES = 10000;
#endif

} // namespace sc_core

// $Log: sc_constants.h,v $
// Revision 1.7  2011/08/26 20:46:09  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.6  2011/02/18 20:33:26  acg
//  Philipp A. Hartmann: added default stack size for CYGWIN32.
//
// Revision 1.5  2011/02/18 20:27:14  acg
//  Andy Goodrich: Updated Copyrights.
//
// Revision 1.4  2011/02/13 21:47:37  acg
//  Andy Goodrich: update copyright notice.
//
// Revision 1.3  2010/03/15 18:29:25  acg
//  Andy Goodrich: Changed the default stack size to 128K from 64K.
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
