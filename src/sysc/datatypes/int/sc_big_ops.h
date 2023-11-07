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

  sc_big_ops.h -- operator implementations for sc_bigint<W> and 
                  sc_biguint<W> using twos complement notation. 
  
  This file contains inline implementations that require the other "int" 
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_BIG_OPS_H
#define SC_BIG_OPS_H

#define SC_BIG_MAX(LEFT,RIGHT) ( (LEFT) > (RIGHT) ? (LEFT) : (RIGHT) )
#define SC_BIG_MIN(LEFT,RIGHT) ( (LEFT) < (RIGHT) ? (LEFT) : (RIGHT) )

namespace sc_dt {

// +----------------------------------------------------------------------------
// |"vector_dump"
// | 
// | This inline function dumps the supplied vector.
// |
// | Arguments:
// |     source_hod = index of the high order 'source_p' digit.
// |     source_p   = vector of digits to be dumped.
// +----------------------------------------------------------------------------
inline
void
vector_dump( int       source_hod,
             sc_digit* source_p )
{
    for ( int digit_i = source_hod; digit_i >= 0; --digit_i ) {
        std::cout << std::hex << source_p[digit_i] << " ";
    }
    std::cout << std::dec << std::endl;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Addition"
// | 
// | These inline operators implement addition of sc_bigint and sc_biguint
// | data types.
// |
// | Arguments:
// |     result = where to put the result of the addition.
// |     left   = first operand of the addition.
// |     right  = second operand of the addition.
// | Result:
// |     Is addition of left and right.
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_add( RESULT& result, const LEFT&  left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() + *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value + right_value;
	*result_p = (sc_digit)result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    else if ( (int)LEFT::HOD >= (int)RIGHT::HOD ) {
	vector_add( LEFT::HOD,   left.get_digits(), 
		    RIGHT::HOD,  right.get_digits(), 
		    RESULT::HOD, result.get_digits() );
    }
    else {
	vector_add( RIGHT::HOD,  right.get_digits(), 
		    LEFT::HOD,   left.get_digits(), 
		    RESULT::HOD, result.get_digits() );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::add_result 
operator + ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::add_result result(0, 0);
    // sc_big_add<WL,true,WR,true>(result, left, right);
    sc_big_add(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::add_result 
operator + ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::add_result result(0, 0);
    sc_big_add(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::add_result 
operator + ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,false>::add_result result(0, 0);
    // sc_big_add<WL,false,WR,true>(result, left, right);
    sc_big_add(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::add_result 
operator + ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::add_result result(0, 0);
    // sc_big_add<WL,false,WR,false>(result, left, right);
    sc_big_add(result, left, right);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Bitwise Logical And"
// | 
// | These inline operators implement bit-wise logical ands of sc_bigint and 
// | sc_biguint data types.
// |
// | Arguments:
// |     result = where to put result of the bit-wise logical and.
// |     left   = first operand of the bit-wise logical and.
// |     right  = second operand of the bit-wise logical and.
// | Result:
// |     Is bit-wise logical and of left and right.
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_and( RESULT& result, const LEFT& left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() & *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value & right_value;
	*result_p = result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    else if ( (const int)LEFT::HOD >= (const int)RIGHT::HOD ) {
        vector_and<LEFT::SIGNED,RIGHT::SIGNED>( LEFT::HOD,
                                                left.get_digits(),
                                                RIGHT::HOD,
                                                right.get_digits(),
                                                result.get_digits() );
    }
    else {
        vector_and<RIGHT::SIGNED,LEFT::SIGNED>( RIGHT::HOD,
                                                right.get_digits(),
                                                LEFT::HOD,
                                                left.get_digits(),
                                                result.get_digits() );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::bit_result 
operator & ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::bit_result result(0, 0);
    sc_big_and(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::bit_result 
operator & ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::bit_result result(0, 0);
    sc_big_and(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::bit_result 
operator & ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,false>::bit_result result(0, 0);
    sc_big_and(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::bit_result 
operator & ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::bit_result result(0, 0);
    sc_big_and(result, left, right);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Bitwise Logical Or"
// | 
// | These inline operators implement bit-wise logical ors of sc_bigint and 
// | sc_biguint data types.
// |
// | Arguments:
// |     result = where to put result of the bit-wise logical or.
// |     left   = first operand of the bit-wise logical or.
// |     right  = second operand of the bit-wise logical or.
// | Result:
// |     Is the bit-wise bit-wise logical or of left and right.
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline
void 
sc_big_or( RESULT& result, const LEFT& left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() | *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value | right_value;
	*result_p = result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    else if ( (const int)LEFT::HOD >= (const int)RIGHT::HOD ) {
	vector_or<LEFT::SIGNED,RIGHT::SIGNED>( LEFT::HOD,
		                               left.get_digits(), 
		                               RIGHT::HOD,
		                               right.get_digits(), 
		                               result.get_digits() );
    }
    else {
	vector_or<RIGHT::SIGNED,LEFT::SIGNED>( RIGHT::HOD,
		                               right.get_digits(), 
	                                       LEFT::HOD,
		                               left.get_digits(), 
		                               result.get_digits() );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::bit_result 
operator | ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::bit_result result(0, 0);
    sc_big_or(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::bit_result 
operator | ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::bit_result result(0, 0);
    sc_big_or(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::bit_result 
operator | ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,false>::bit_result result(0, 0);
    sc_big_or(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::bit_result 
operator | ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::bit_result result(0, 0);
    sc_big_or(result, left, right);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Bitwise Logical Xor"
// | 
// | These inline operators implement bit-wise logical xors of sc_bigint and 
// | sc_biguint data types.
// |
// | Arguments:
// |     result = where to put result of the bit-wise logical xor.
// |     left   = first operand of the bit-wise logical xor.
// |     right  = second operand of the bit-wise logical xor.
// | Result:
// |     Is bit-wise logical xor of left and right.
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_xor( RESULT& result, const LEFT& left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() ^ *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value ^ right_value;
	*result_p = result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    else if ( (const int)LEFT::HOD >= (const int)RIGHT::HOD ) {
        vector_xor<LEFT::SIGNED,RIGHT::SIGNED>( LEFT::HOD,
					        left.get_digits(),
                                                RIGHT::HOD,
                                                right.get_digits(),
                                                result.get_digits() );
    }
    else {
        vector_xor<RIGHT::SIGNED,LEFT::SIGNED>( RIGHT::HOD,
                                                right.get_digits(),
                                                LEFT::HOD,
                                                left.get_digits(),
                                                result.get_digits() );
    }

}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::bit_result 
operator ^ ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::bit_result result(0, 0);
    sc_big_xor(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::bit_result 
operator ^ ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::bit_result result(0, 0);
    sc_big_xor(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::bit_result 
operator ^ ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::bit_result result(0, 0);
    sc_big_xor(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::bit_result 
operator ^ ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::bit_result result(0, 0);
    sc_big_xor(result, left, right);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Division"
// | 
// | These inline operators implement division of sc_bigint and sc_biguint
// | data types.
// |
// | Arguments:
// |     result = where to put the result of the division.
// |     numer  = numerator of the division.
// |     denom  = operand of the division.
// | Result:
// |     Is division of numer by denom
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_divide( RESULT& result, const LEFT&  left, const RIGHT& right )
{
    bool ok;

    if ( 0 == RESULT::HOD ) {
        typename LEFT::HOD_TYPE  left_value = *left.get_digits();
        typename RIGHT::HOD_TYPE right_value = *right.get_digits();
	if ( 0 != right_value ) {
	    *result.get_digits() = left_value / right_value;
	    ok = true;
	}
	else {
	    ok = false;
	}
    }
    else if ( 1 == RESULT::HOD && 2 > LEFT::HOD && 2 > RIGHT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else {
	    left_value = (typename LEFT::HOD_TYPE)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else {
	    right_value = (typename RIGHT::HOD_TYPE)*right_p;
	}
	if ( right_value != 0 ) {
	    result_value = left_value / right_value;
	    *result_p = (sc_digit)result_value;
	    result_p[1] = (result_value >> BITS_PER_DIGIT);
	    ok = true;
	}
	else {
	    ok = false;
	}
    }
    else {
	ok = vector_divide<LEFT::SIGNED,RIGHT::SIGNED>( LEFT::DIGITS_N,
			                                left.get_digits(), 
			                                RIGHT::DIGITS_N,
			                                right.get_digits(), 
			                                RESULT::DIGITS_N,
			                                result.get_digits(),
			                                0,
			                                NULL );
    }
    if ( !ok ) {
	SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_,
			 "division by zero detected" );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::div_result
operator / ( const sc_bigint<WL>& numer, const sc_bigint<WR>& denom )
{
    typename sc_big_op_info<WL,true,WR,true>::div_result result(0, 0);
    sc_big_divide(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::div_result
operator / ( const sc_biguint<WL>& numer, const sc_bigint<WR>& denom )
{
    typename sc_big_op_info<WL,false,WR,true>::div_result result(0, 0);
    sc_big_divide(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::div_result
operator / ( const sc_bigint<WL>& numer, const sc_biguint<WR>& denom )
{
    typename sc_big_op_info<WL,true,WR,false>::div_result result(0, 0);
    sc_big_divide(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::div_result
operator / ( const sc_biguint<WL>& numer, const sc_biguint<WR>& denom )
{
    typename sc_big_op_info<WL,false,WR,false>::div_result result(0, 0);
    sc_big_divide(result, numer, denom);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Modulo"
// | 
// | These inline operators implement modulo of sc_bigint and sc_biguint
// | data types.
// |
// | Arguments:
// |     result = where to place the result of the modulo.
// |     numer  = numerator of the modulo.
// |     denom  = denominator of the modulo.
// | Result:
// |     Is modulo of numer by denom
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_modulo( RESULT& result, const LEFT& left, const RIGHT& right )
{
    bool ok; // true if operation was okay, false if divide by zero.

    if ( 0 == RESULT::HOD ) {
        typename LEFT::HOD_TYPE  left_value = *left.get_digits();
        typename RIGHT::HOD_TYPE right_value = *right.get_digits();
	if ( right_value != 0 ) {
	    *result.get_digits() = left_value % right_value;
	    ok = true;
	}
	else {
	    ok = false;
	}
    } 
    else if ( 1 == RESULT::HOD && 2 > LEFT::HOD && 2 > RIGHT::HOD ) {
        const sc_digit* left_p = left.get_digits();
        int64           left_value;
        sc_digit*       result_p = result.get_digits();
        int64           result_value; 
        const sc_digit* right_p = right.get_digits();
        int64           right_value;
        if ( 1 == LEFT::HOD ) {
            left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
        }    
        else {
            left_value = (typename LEFT::HOD_TYPE)*left_p;
        }    
        if ( 1 == RIGHT::HOD ) {
            right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
        }    
        else {
            right_value = (typename RIGHT::HOD_TYPE)*right_p;
        }
	if ( 0 != right_value ) {
	    result_value = left_value % right_value;
	    *result_p = (sc_digit)result_value;
	    result_p[1] = (result_value >> BITS_PER_DIGIT);
	    ok = true;
	}
	else {
	    ok = false;
	}
    }
    else {

        ok = vector_divide<LEFT::SIGNED,RIGHT::SIGNED>( LEFT::DIGITS_N,
                                                        left.get_digits(),
                                                        RIGHT::DIGITS_N,
                                                        right.get_digits(),
                                                        0,
                                                        NULL,
                                                        RESULT::DIGITS_N,
                                                        result.get_digits() );
    }
    if ( !ok ) {
	SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_,
			 "division by zero detected" );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::mod_result
operator % ( const sc_bigint<WL>& numer, const sc_bigint<WR>& denom )
{
    typename sc_big_op_info<WL,true,WR,true>::mod_result result(0, 0);
    sc_big_modulo(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::mod_result
operator % ( const sc_biguint<WL>& numer, const sc_bigint<WR>& denom )
{
    typename sc_big_op_info<WL,false,WR,true>::mod_result result(0, 0);
    sc_big_modulo(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::mod_result
operator % ( const sc_bigint<WL>& numer, const sc_biguint<WR>& denom )
{
    typename sc_big_op_info<WL,true,WR,false>::mod_result result(0, 0);
    sc_big_modulo(result, numer, denom);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::mod_result
operator % ( const sc_biguint<WL>& numer, const sc_biguint<WR>& denom )
{
    typename sc_big_op_info<WL,false,WR,false>::mod_result result(0, 0);
    sc_big_modulo(result, numer, denom);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Multiply"
// | 
// | These inline operators implement multiplication of sc_bigint and sc_biguint
// | data types.
// |
// | Arguments:
// |     result = where to put the result of the multiply.
// |     left   = first operand of the multiplication.
// |     right  = second operand of the multiplication.
// | Result:
// |     Is multiplication of left and right.
// +----------------------------------------------------------------------------

template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_multiply( RESULT& result, const LEFT& left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() * *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value * right_value;
	*result_p = (sc_digit)result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    vector_multiply( LEFT::HOD,   left.get_digits(), 
		     RIGHT::HOD,  right.get_digits(), 
		     RESULT::HOD, result.get_digits() );
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::mul_result
operator * ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::mul_result result(0, 0);
    sc_big_multiply(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::mul_result
operator * ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::mul_result result(0, 0);
    sc_big_multiply(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::mul_result
operator * ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,false>::mul_result result(0, 0);
    sc_big_multiply(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::mul_result 
operator * ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::mul_result result(0, 0);
    sc_big_multiply(result, left, right);
    return result;
}

// +----------------------------------------------------------------------------
// |"sc_big*int Subtract"
// | 
// | These inline operators implement subtraction of sc_bigint and sc_biguint
// | data types.
// |
// | Arguments:
// |     result = where to put the result of the subtraction.
// |     left   = first operand of the subtraction.
// |     right  = second operand of the subtraction.
// | Result:
// |     Is subtraction of left and right.
// +----------------------------------------------------------------------------
template<typename RESULT, typename LEFT, typename RIGHT>
inline 
void 
sc_big_subtract( RESULT& result, const LEFT&  left, const RIGHT& right )
{
    if ( 0 == RESULT::HOD ) {
	*result.get_digits() = *left.get_digits() - *right.get_digits(); 
    }
    else if ( 1 == RESULT::HOD ) {
	const sc_digit* left_p = left.get_digits();
        int64           left_value;
	sc_digit*       result_p = result.get_digits();
	int64           result_value;
	const sc_digit* right_p = right.get_digits();
        int64           right_value;
	if ( 1 == LEFT::HOD ) {
	    left_value = ( (int64)left_p[1] << BITS_PER_DIGIT ) | *left_p;
	}
	else if ( LEFT::SIGNED ) {
	    left_value = (int64)(int)*left_p;
	}
	else {
	    left_value = (int64)*left_p;
	}
	if ( 1 == RIGHT::HOD ) {
	    right_value = ( (int64)right_p[1] << BITS_PER_DIGIT ) | *right_p;
	}
	else if ( RIGHT::SIGNED ) {
	    right_value = (int64)(int)*right_p;
	}
	else {
	    right_value = (int64)*right_p;
	}
	result_value = left_value - right_value;
	*result_p = (sc_digit)result_value;
	result_p[1] = (result_value >> BITS_PER_DIGIT);
    }
    else if ( (const int)LEFT::HOD > (const int)RIGHT::HOD ) {
	vector_subtract_shorter( LEFT::HOD,
		                 left.get_digits(), 
		                 RIGHT::HOD,
		                 right.get_digits(), 
		                 RESULT::HOD,
		                 result.get_digits() );
    }
    else {
	vector_subtract_longer( RIGHT::HOD,
		                right.get_digits(), 
		                LEFT::HOD,
		                left.get_digits(), 
		                RESULT::HOD,
		                result.get_digits() );
    }
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,true>::sub_result
operator - ( const sc_bigint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,true>::sub_result result(0, 0);
    sc_big_subtract(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,true>::sub_result
operator - ( const sc_biguint<WL>& left, const sc_bigint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,true>::sub_result result(0, 0);
    sc_big_subtract(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,true,WR,false>::sub_result
operator - ( const sc_bigint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,true,WR,false>::sub_result result(0, 0);
    sc_big_subtract(result, left, right);
    return result;
}

template<int WL, int WR>
inline
const typename sc_big_op_info<WL,false,WR,false>::sub_result
operator - ( const sc_biguint<WL>& left, const sc_biguint<WR>& right )
{
    typename sc_big_op_info<WL,false,WR,false>::sub_result result(0, 0);
    sc_big_subtract(result, left, right);
    return result;
}

} // namespace sc_dt

#undef SC_BIG_MAX
#undef SC_BIG_MIN

#endif // SC_BIG_OPS_H
