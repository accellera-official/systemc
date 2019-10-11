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

  sc_int_inlines.h -- This file contains inline implementations that require
                      the other "int" headers to have been included before its
                      contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_INT_INLINES_H
#define SC_INT_INLINES_H

namespace sc_dt {

template<int W>
template<int W1>
inline
const sc_int<W>& sc_int<W>::operator = ( const sc_bigint<W1>& source ) 
{
    if ( W1 < 33 ) {
        m_val = (int)*source.digit; 
    }
    else {
        m_val = ( (int64)source.digit[1] << BITS_PER_DIGIT ) | *source.digit;
    }
    return *this;
}

template<int W>
template<int W1>
inline
const sc_int<W>& sc_int<W>::operator = ( const sc_biguint<W1>& source ) 
{
    if ( W1 < 33 ) {
        m_val = *source.digit; 
    }
    else {
        m_val = ( (uint64)source.digit[1] << BITS_PER_DIGIT ) | *source.digit;
    }
    return *this;
}

inline 
sc_int_base& 
sc_int_base::operator = ( const sc_signed_subref_r& a )
{ 
    m_val = a.to_uint64(); 
    extend_sign(); 
    return *this; 
}

inline 
sc_int_base& 
sc_int_base::operator = ( const sc_unsigned_subref_r& a )
{
    m_val = a.to_uint64();
    extend_sign();
    return *this;
}

} // namespace sc_dt 

#endif // SC_INT_INLINES_H
