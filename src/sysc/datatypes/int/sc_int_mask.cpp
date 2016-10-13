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

  sc_int_mask.cpp -- Fills the mask_int lookup table to enable efficient
                     part-selection on sc_ints and sc_uints.

                     (In fact, this file is just dispatching to the real
                      lookup tables in sc_int(32|64)_mask.inc)

  Original Author: Philipp A. Hartmann, OFFIS

 *****************************************************************************/

#ifdef _32BIT_
// non-standard 32-bit sc_(u)int implementation
#  include "sysc/datatypes/int/sc_int32_mask.inc"
#else
// default IEEE 1666 implementation
#  include "sysc/datatypes/int/sc_int64_mask.inc"
#endif
