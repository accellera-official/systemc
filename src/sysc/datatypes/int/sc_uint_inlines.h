// +----------------------------------------------------------------------------
// | The following code is derived, directly or indirectly, from the SystemC
// | source code Copyright (c) 1996-2017 by all Contributors.
// | Portions related to performance improvements of sc_biguint and sc_bigint 
// | are Copyright © 2014-2017 Cadence Design Systems
// |  
// | Licensed under the Apache License, Version 2.0 (the "License");
// | you may not use this file except in compliance with the License.
// | You may obtain a copy of the License at
// | 
// |     http://www.apache.org/licenses/LICENSE-2.0
// |
// | Unless required by applicable law or agreed to in writing, software
// | distributed under the License is distributed on an "AS IS" BASIS,
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// | See the License for the specific language governing permissions and
// | limitations under the License.
// +----------------------------------------------------------------------------

/*****************************************************************************

  sc_uint_inlines.h -- This file contains inline implementations that require
                       the other "int" headers to have been included before its
                       contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_UINT_INLINES_H
#define SC_UINT_INLINES_H

namespace sc_dt {

template<int W>
template<int W1>
inline
const sc_uint<W>& sc_uint<W>::operator = ( const sc_bigint<W1>& source ) 
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
const sc_uint<W>& sc_uint<W>::operator = ( const sc_biguint<W1>& source ) 
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
sc_uint_base& 
sc_uint_base::operator = ( const sc_signed_subref_r& a )
{ 
    m_val = a.to_uint64(); 
    extend_sign(); 
    return *this; 
}

inline 
sc_uint_base& 
sc_uint_base::operator = ( const sc_unsigned_subref_r& a )
{
    m_val = a.to_uint64();
    extend_sign();
    return *this;
}

} // namespace sc_dt 

#endif // SC_UINT_INLINES_H
