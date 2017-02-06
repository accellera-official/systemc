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
