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

  sc_unsigned_ops.h -- operator implementations with sc_unsigned results
  
  This file contains inline implementations that require the other "int" 
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_UNSIGNED_OPS_H
#define SC_UNSIGNED_OPS_H

// NOTICE!!!! NOTICE!!!!
//
// Operators on sc_unsigned data types are contained in the sc_signed_ops.h, since we use a
// set of macros go generate the definitions for both sc_signed and sc_unsigned datatypes.

namespace sc_dt {

inline
sc_unsigned&
sc_unsigned::operator++() // prefix
{
    *this = *this + 1;
    return *this;   
}


inline
const sc_unsigned
sc_unsigned::operator++(int) // postfix
{
  // Copy digit into d.

  sc_unsigned result(*this);

  *this = *this + 1;

  return result;
}

inline
sc_unsigned&
sc_unsigned::operator--() // prefix
{
    *this = *this - 1;
    return *this;   
}


inline
const sc_unsigned
sc_unsigned::operator--(int) // postfix
{
  // Copy digit into d.

  sc_unsigned result(*this);

  *this = *this - 1;

  return result;
}

} // namespace sc_dt


#endif // SC_UNSIGNED_OPS_H
