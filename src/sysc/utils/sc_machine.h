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

  sc_machine.h -- Machine-dependent Environment Settings

  Original Author: Andy Goodrich, Forte Design Systems, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_machine.h,v $
// Revision 1.3  2011/02/18 20:38:44  acg
//  Andy Goodrich: Updated Copyright notice.
//
// Revision 1.2  2010/09/06 16:35:09  acg
//  Andy Goodrich: changed i386 to __i386__ in ifdef.
//
// Revision 1.1.1.1  2006/12/15 20:20:06  acg
// SystemC 2.3
//
// Revision 1.3  2006/01/13 18:53:10  acg
// Andy Goodrich: Added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_MACHINE_H
#define SC_MACHINE_H

// ----------------------------------------------------------------------------
//  Little or big endian machine?
// ----------------------------------------------------------------------------

#if defined( __x86_64__ ) || defined( __i386__ ) || defined(WIN32)
#   define SC_LITTLE_ENDIAN
#elif defined( __ppc__ ) || defined( sparc ) || defined( __hppa )
#   define SC_BIG_ENDIAN
#endif

// ----------------------------------------------------------------------------
//  Are long data types 32-bit or 64-bit?
// ----------------------------------------------------------------------------

#if defined( __x86_64__ ) 
#   define SC_LONG_64
#endif


#endif // !defined(SC_MACHINE_H)
