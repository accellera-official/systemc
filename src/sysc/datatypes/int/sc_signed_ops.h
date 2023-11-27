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

  sc_signed_ops.h -- operator implementations for sc_signed and sc_unsigned 
                     results
  
  This file contains inline implementations that require the other "int" 
  headers to have been included before its contents can be resolved.

  Original Author: Andy Goodrich, Cadence Design Systems

 *****************************************************************************/

#ifndef SC_SIGNED_OPS_H
#define SC_SIGNED_OPS_H

namespace sc_dt {

#define OPS_MIN(A,B) ( (A) <= (B) ? A : B )
#define OPS_MAX(A,B) ( (A) >= (B) ? A : B )

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned addition operators"
// | 
// | These are the operators for addition that involves at least one instance
// | of sc_signed or sc_unsigned. The implementation is done using three macros:
// |   (a) ADD_BIG_BIG
// |   (b) ADD_BIG_NATIVE
// |   (c) ADD_NATIVE_BIG
// +----------------------------------------------------------------------------

#define ADD_BIG_BIG(RESULT_TYPE,LEFT_TYPE,RIGHT_TYPE) \
    inline \
    const RESULT_TYPE \
    operator+(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	const int left_n  = left.get_actual_length(); \
	const int right_n = right.get_actual_length(); \
     \
	if ( left_n >= right_n ) { \
	    RESULT_TYPE result(left_n+1, false); \
	    vector_add( left.get_hod(), \
			left.get_digits(), \
			right.get_hod(), \
			right.get_digits(), \
			result.get_hod(), \
			result.get_digits() ); \
	    return result; \
	} \
	else { \
	    RESULT_TYPE result(right_n+1, false); \
	    vector_add( right.get_hod(), \
			right.get_digits(), \
			left.get_hod(), \
			left.get_digits(), \
			result.get_hod(), \
			result.get_digits() ); \
	    return result; \
	} \
    } 

#define ADD_BIG_NATIVE(RESULT_TYPE,BIG_TYPE,NATIVE_TYPE) \
    inline \
    const RESULT_TYPE \
    operator+(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
        const int                   left_n = left.get_actual_length(); \
        const int                   right_n = right.get_actual_length(); \
     \
        if ( left_n > right_n ) { \
	    RESULT_TYPE result( left_n+1, false );  \
	    vector_add( left.get_hod(),  \
	                left.get_digits(),  \
	                right.get_hod(),  \
		        right.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
        else { \
	    RESULT_TYPE result( right_n+1, false );  \
	    vector_add( right.get_hod(),  \
	                right.get_digits(),  \
	                left.get_hod(),  \
		        left.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
    } 

#define ADD_NATIVE_BIG(RESULT_TYPE,NATIVE_TYPE,BIG_TYPE) \
    inline \
    const RESULT_TYPE \
    operator+(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
        const int                   left_n = left.get_actual_length(); \
        const int                   right_n = right.get_actual_length(); \
     \
        if ( left_n > right_n ) { \
	    RESULT_TYPE result( left_n+1, false );  \
	    vector_add( left.get_hod(),  \
		        left.get_digits(),  \
	                right.get_hod(),  \
	                right.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
        else { \
	    RESULT_TYPE result( right_n+1, false );  \
	    vector_add(  \
	                right.get_hod(),  \
		        right.get_digits(),  \
	                left.get_hod(),  \
	                left.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
    }

// big type + big type:

ADD_BIG_BIG(sc_signed,sc_signed,sc_signed)
ADD_BIG_BIG(sc_signed,sc_unsigned,sc_signed)
ADD_BIG_BIG(sc_signed,sc_signed,sc_unsigned)
ADD_BIG_BIG(sc_unsigned,sc_unsigned,sc_unsigned)

// sc_signed + C++ native:

ADD_BIG_NATIVE(sc_signed,sc_signed,int64)
ADD_BIG_NATIVE(sc_signed,sc_signed,uint64)
ADD_BIG_NATIVE(sc_signed,sc_signed,long)
ADD_BIG_NATIVE(sc_signed,sc_signed,unsigned long)
ADD_BIG_NATIVE(sc_signed,sc_signed,int)
ADD_BIG_NATIVE(sc_signed,sc_signed,unsigned int)

// C++ native + sc_signed:

ADD_NATIVE_BIG(sc_signed,int64,sc_signed)
ADD_NATIVE_BIG(sc_signed,uint64,sc_signed)
ADD_NATIVE_BIG(sc_signed,long,sc_signed)
ADD_NATIVE_BIG(sc_signed,unsigned long,sc_signed)
ADD_NATIVE_BIG(sc_signed,int,sc_signed)
ADD_NATIVE_BIG(sc_signed,unsigned int,sc_signed)

// sc_unsigned + signed C++ native:

ADD_BIG_NATIVE(sc_signed,sc_unsigned,int64)
ADD_BIG_NATIVE(sc_signed,sc_unsigned,long)
ADD_BIG_NATIVE(sc_signed,sc_unsigned,int)

// signed C++ native + sc_unsigned:

ADD_NATIVE_BIG(sc_signed,int64,sc_unsigned)
ADD_NATIVE_BIG(sc_signed,long,sc_unsigned)
ADD_NATIVE_BIG(sc_signed,int,sc_unsigned)

// sc_unsigned + unsigned C++ native:

ADD_BIG_NATIVE(sc_unsigned,sc_unsigned,uint64)
ADD_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned long)
ADD_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned int)

// unsigned C++ native + sc_unsigned:

ADD_NATIVE_BIG(sc_unsigned,uint64,sc_unsigned)
ADD_NATIVE_BIG(sc_unsigned,unsigned long,sc_unsigned)
ADD_NATIVE_BIG(sc_unsigned,unsigned int,sc_unsigned)

// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator+( const sc_signed& left, const sc_int_base& right )
    { return operator+(left, (int64)right); }

inline
const sc_signed operator+( const sc_signed& left, const sc_uint_base& right )
    { return operator+(left, (uint64)right); }

inline
const sc_signed operator+( const sc_int_base& left, const sc_signed& right )
    { return operator+((int64)left, right); }

inline
const sc_signed operator+( const sc_uint_base& left, const sc_signed& right )
    { return operator+((uint64)left, right); }

inline
const sc_signed operator+( const sc_unsigned& left, const sc_int_base& right )
    { return operator+(left, (int64)right); }

inline
const sc_signed operator+( const sc_int_base& left, const sc_unsigned& right )
    { return operator+((int64)left, right); }

inline
const sc_unsigned operator+(const sc_unsigned& left, const sc_uint_base& right )
    { return operator+(left, (uint64)right); }

inline
const sc_unsigned operator+(const sc_uint_base& left, const sc_unsigned& right )
    { return operator+((uint64)left, right); }

#undef ADD_BIG_BIG
#undef ADD_BIG_NATIVE
#undef ADD_NATIVE_BIG

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned division operators"
// | 
// | These are the operators for division that involves at least one instance
// | of sc_signed or sc_unsigned. The implementation is done using three macros:
// |   (a) DIVIDE_BIG_BIG
// |   (b) DIVIDE_BIG_NATIVE
// |   (c) DIVIDE_NATIVE_BIG
// +----------------------------------------------------------------------------

#define DIVIDE_BIG_BIG(RESULT_TYPE,LEFT_TYPE,RIGHT_TYPE) \
inline \
const RESULT_TYPE \
operator/(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	int left_n  = left.get_actual_length(); \
     \
	RESULT_TYPE quotient(left_n+right.SIGNED); \
	bool ok = vector_divide<LEFT_TYPE::SIGNED,RIGHT_TYPE::SIGNED>( \
		                                      left.get_digits_n(), \
		                                      left.get_digits(), \
		                                      right.get_digits_n(), \
		                                      right.get_digits(), \
		                                      quotient.get_digits_n(), \
		                                      quotient.get_digits(), \
		                                      0, \
		                                      NULL ); \
	if ( !ok ) { \
	    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, \
			     "division by zero detected" ); \
        } \
	return quotient; \
    } 

#define DIVIDE_BIG_NATIVE(RESULT_TYPE,BIG_TYPE,NATIVE_TYPE) \
    inline \
    const RESULT_TYPE \
    operator/(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
        const int                   left_n = left.get_actual_length(); \
     \
	RESULT_TYPE quotient(left_n+right.SIGNED, false); \
	bool ok = \
	  vector_divide<BIG_TYPE::SIGNED,ScNativeDigits<NATIVE_TYPE>::SIGNED>( \
	                left.get_digits_n(),  \
	                left.get_digits(),  \
	                right.get_digits_n(),  \
		        right.get_digits(),  \
	                quotient.get_digits_n(),  \
		        quotient.get_digits(), \
			0, \
			NULL ); \
	if ( !ok ) { \
	    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, \
			     "division by zero detected" ); \
        } \
	return quotient; \
    } 

#define DIVIDE_NATIVE_BIG(RESULT_TYPE,NATIVE_TYPE,BIG_TYPE) \
    inline \
    const RESULT_TYPE \
    operator/(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
        const int            left_n = left.get_actual_length(); \
     \
	RESULT_TYPE quotient(left_n+right.SIGNED, false); \
	bool ok = \
	  vector_divide<ScNativeDigits<NATIVE_TYPE>::SIGNED,BIG_TYPE::SIGNED>( \
	                left.get_digits_n(),  \
		        left.get_digits(),  \
	                right.get_digits_n(),  \
	                right.get_digits(),  \
	                quotient.get_digits_n(),  \
		        quotient.get_digits(), \
			0, \
			NULL ); \
	if ( !ok ) { \
	    SC_REPORT_ERROR( sc_core::SC_ID_OPERATION_FAILED_, \
			     "division by zero detected" ); \
        } \
	return quotient; \
    }

// big type + big type:

DIVIDE_BIG_BIG(sc_signed,sc_signed,sc_signed)
DIVIDE_BIG_BIG(sc_signed,sc_unsigned,sc_signed)
DIVIDE_BIG_BIG(sc_signed,sc_signed,sc_unsigned)
DIVIDE_BIG_BIG(sc_unsigned,sc_unsigned,sc_unsigned)

// sc_signed + C++ native:

DIVIDE_BIG_NATIVE(sc_signed,sc_signed,int64)
DIVIDE_BIG_NATIVE(sc_signed,sc_signed,uint64)
DIVIDE_BIG_NATIVE(sc_signed,sc_signed,long)
DIVIDE_BIG_NATIVE(sc_signed,sc_signed,unsigned long)
DIVIDE_BIG_NATIVE(sc_signed,sc_signed,int)
DIVIDE_BIG_NATIVE(sc_signed,sc_signed,unsigned int)

// C++ native + sc_signed:

DIVIDE_NATIVE_BIG(sc_signed,int64,sc_signed)
DIVIDE_NATIVE_BIG(sc_signed,uint64,sc_signed)
DIVIDE_NATIVE_BIG(sc_signed,long,sc_signed)
DIVIDE_NATIVE_BIG(sc_signed,unsigned long,sc_signed)
DIVIDE_NATIVE_BIG(sc_signed,int,sc_signed)
DIVIDE_NATIVE_BIG(sc_signed,unsigned int,sc_signed)

// sc_unsigned + signed C++ native:

DIVIDE_BIG_NATIVE(sc_signed,sc_unsigned,int64)
DIVIDE_BIG_NATIVE(sc_signed,sc_unsigned,long)
DIVIDE_BIG_NATIVE(sc_signed,sc_unsigned,int)

// signed C++ native + sc_unsigned:

DIVIDE_NATIVE_BIG(sc_signed,int64,sc_unsigned)
DIVIDE_NATIVE_BIG(sc_signed,long,sc_unsigned)
DIVIDE_NATIVE_BIG(sc_signed,int,sc_unsigned)

// sc_unsigned + unsigned C++ native:

DIVIDE_BIG_NATIVE(sc_unsigned,sc_unsigned,uint64)
DIVIDE_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned long)
DIVIDE_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned int)

// unsigned C++ native + sc_unsigned:

DIVIDE_NATIVE_BIG(sc_unsigned,uint64,sc_unsigned)
DIVIDE_NATIVE_BIG(sc_unsigned,unsigned long,sc_unsigned)
DIVIDE_NATIVE_BIG(sc_unsigned,unsigned int,sc_unsigned)

// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator/( const sc_signed& left, const sc_int_base& right )
    { return operator/(left, (int64)right); }

inline
const sc_signed operator/( const sc_signed& left, const sc_uint_base& right )
    { return operator/(left, (uint64)right); }

inline
const sc_signed operator/( const sc_int_base& left, const sc_signed& right )
    { return operator/((int64)left, right); }

inline
const sc_signed operator/( const sc_uint_base& left, const sc_signed& right )
    { return operator/((uint64)left, right); }

inline
const sc_signed operator/( const sc_unsigned& left, const sc_int_base& right )
    { return operator/(left, (int64)right); }

inline
const sc_signed operator/( const sc_int_base& left, const sc_unsigned& right )
    { return operator/((int64)left, right); }

inline
const sc_unsigned operator/(const sc_unsigned& left, const sc_uint_base& right )
    { return operator/(left, (uint64)right); }

inline
const sc_unsigned operator/(const sc_uint_base& left, const sc_unsigned& right )
    { return operator/((uint64)left, right); }

#undef DIVIDE_BIG_BIG
#undef DIVIDE_BIG_NATIVE
#undef DIVIDE_NATIVE_BIG

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned modulus operators"
// | 
// | These are the operators for modulo that involves at least one instance
// | of sc_signed or sc_unsigned. The implementation is done using three macros:
// |   (a) MODULO_BIG_BIG
// |   (b) MODULO_BIG_NATIVE
// |   (c) MODULO_NATIVE_BIG
// +----------------------------------------------------------------------------

#define MODULO_BIG_BIG(RESULT_TYPE,LEFT_TYPE,RIGHT_TYPE) \
inline \
const RESULT_TYPE \
operator%(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	const int left_n  = left.get_actual_length(); \
	const int right_n = right.get_actual_length(); \
	const int width_n = OPS_MIN( left_n,right_n+right.SIGNED ); \
     \
	RESULT_TYPE result(width_n); \
	    vector_divide<LEFT_TYPE::SIGNED,RIGHT_TYPE::SIGNED>( \
						    left.get_digits_n(), \
						    left.get_digits(), \
						    right.get_digits_n(), \
						    right.get_digits(), \
						    0, \
						    NULL, \
						    result.get_digits_n(), \
						    result.get_digits() ); \
	return result; \
    } 

#define MODULO_BIG_NATIVE(RESULT_TYPE,BIG_TYPE,NATIVE_TYPE) \
    inline \
    const RESULT_TYPE \
    operator%(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
        const int            left_n = left.get_actual_length(); \
        const int            right_n = right.get_actual_length(); \
	const int            width_n = OPS_MIN( left_n,right_n+right.SIGNED ); \
     \
	RESULT_TYPE result(width_n); \
	vector_divide<BIG_TYPE::SIGNED,ScNativeDigits<NATIVE_TYPE>::SIGNED>(\
						    left.get_digits_n(),  \
						    left.get_digits(),  \
						    right.get_digits_n(),  \
						    right.get_digits(),  \
						    0, \
						    NULL, \
						    result.get_digits_n(),  \
						    result.get_digits() ); \
	return result; \
    } 

#define MODULO_NATIVE_BIG(RESULT_TYPE,NATIVE_TYPE,BIG_TYPE) \
    inline \
    const RESULT_TYPE \
    operator%(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
        const int  left_n = left.get_actual_length(); \
        const int  right_n = right.get_actual_length(); \
	const int  width_n = OPS_MIN( left_n,right_n+right.SIGNED ); \
     \
	RESULT_TYPE result(width_n); \
	vector_divide<ScNativeDigits<NATIVE_TYPE>::SIGNED,BIG_TYPE::SIGNED>(\
						    left.get_digits_n(),  \
						    left.get_digits(),  \
						    right.get_digits_n(),  \
						    right.get_digits(),  \
						    0, \
						    NULL, \
						    result.get_digits_n(),  \
						    result.get_digits() ); \
	return result; \
    }

// big type + big type:

MODULO_BIG_BIG(sc_signed,sc_signed,sc_signed)
MODULO_BIG_BIG(sc_signed,sc_unsigned,sc_signed)
MODULO_BIG_BIG(sc_signed,sc_signed,sc_unsigned)
MODULO_BIG_BIG(sc_unsigned,sc_unsigned,sc_unsigned)

// sc_signed + C++ native:

MODULO_BIG_NATIVE(sc_signed,sc_signed,int64)
MODULO_BIG_NATIVE(sc_signed,sc_signed,uint64)
MODULO_BIG_NATIVE(sc_signed,sc_signed,long)
MODULO_BIG_NATIVE(sc_signed,sc_signed,unsigned long)
MODULO_BIG_NATIVE(sc_signed,sc_signed,int)
MODULO_BIG_NATIVE(sc_signed,sc_signed,unsigned int)

// C++ native + sc_signed:

MODULO_NATIVE_BIG(sc_signed,int64,sc_signed)
MODULO_NATIVE_BIG(sc_signed,uint64,sc_signed)
MODULO_NATIVE_BIG(sc_signed,long,sc_signed)
MODULO_NATIVE_BIG(sc_signed,unsigned long,sc_signed)
MODULO_NATIVE_BIG(sc_signed,int,sc_signed)
MODULO_NATIVE_BIG(sc_signed,unsigned int,sc_signed)

// sc_unsigned + signed C++ native:

MODULO_BIG_NATIVE(sc_signed,sc_unsigned,int64)
MODULO_BIG_NATIVE(sc_signed,sc_unsigned,long)
MODULO_BIG_NATIVE(sc_signed,sc_unsigned,int)

// signed C++ native + sc_unsigned:

MODULO_NATIVE_BIG(sc_signed,int64,sc_unsigned)
MODULO_NATIVE_BIG(sc_signed,long,sc_unsigned)
MODULO_NATIVE_BIG(sc_signed,int,sc_unsigned)

// sc_unsigned + unsigned C++ native:

MODULO_BIG_NATIVE(sc_unsigned,sc_unsigned,uint64)
MODULO_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned long)
MODULO_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned int)

// unsigned C++ native + sc_unsigned:

MODULO_NATIVE_BIG(sc_unsigned,uint64,sc_unsigned)
MODULO_NATIVE_BIG(sc_unsigned,unsigned long,sc_unsigned)
MODULO_NATIVE_BIG(sc_unsigned,unsigned int,sc_unsigned)

// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator%( const sc_signed& left, const sc_int_base& right )
    { return operator%(left, (int64)right); }

inline
const sc_signed operator%( const sc_signed& left, const sc_uint_base& right )
    { return operator%(left, (uint64)right); }

inline
const sc_signed operator%( const sc_int_base& left, const sc_signed& right )
    { return operator%((int64)left, right); }

inline
const sc_signed operator%( const sc_uint_base& left, const sc_signed& right )
    { return operator%((uint64)left, right); }

inline
const sc_signed operator%( const sc_unsigned& left, const sc_int_base& right )
    { return operator%(left, (int64)right); }

inline
const sc_signed operator%( const sc_int_base& left, const sc_unsigned& right )
    { return operator%((int64)left, right); }

inline
const sc_unsigned operator%(const sc_unsigned& left, const sc_uint_base& right )
    { return operator%(left, (uint64)right); }

inline
const sc_unsigned operator%(const sc_uint_base& left, const sc_unsigned& right )
    { return operator%((uint64)left, right); }

#undef MODULO_BIG_BIG
#undef MODULO_BIG_NATIVE
#undef MODULO_NATIVE_BIG

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned multiplication operators"
// | 
// | These are the operators for multiplication that involves at least one 
// | instance of sc_signed or sc_unsigned. The implementation is done using 
// | three macros:
// |   (a) MULTIPLY_BIG_BIG
// |   (b) MULTIPLY_BIG_NATIVE
// |   (c) MULTIPLY_NATIVE_BIG
// +----------------------------------------------------------------------------

#define MULTIPLY_BIG_BIG(RESULT_TYPE,LEFT_TYPE,RIGHT_TYPE) \
inline \
const RESULT_TYPE \
operator*(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	int left_n  = left.get_actual_length(); \
	int right_n = right.get_actual_length(); \
     \
	RESULT_TYPE result(left_n+right_n, false); \
	vector_multiply( left.get_hod(), \
			 left.get_digits(), \
			 right.get_hod(), \
			 right.get_digits(), \
			 result.get_hod(), \
			 result.get_digits() ); \
	return result; \
    } 

#define MULTIPLY_BIG_NATIVE(RESULT_TYPE,BIG_TYPE,NATIVE_TYPE) \
    inline \
    const RESULT_TYPE \
    operator*(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
        const int                   left_n = left.get_actual_length(); \
        const int                   right_n = right.get_actual_length(); \
     \
	RESULT_TYPE result(left_n+right_n, false); \
	vector_multiply( left.get_hod(),  \
			 left.get_digits(),  \
			 right.get_hod(),  \
			 right.get_digits(),  \
			 result.get_hod(),  \
			 result.get_digits() ); \
	return result; \
    } 

#define MULTIPLY_NATIVE_BIG(RESULT_TYPE,NATIVE_TYPE,BIG_TYPE) \
    inline \
    const RESULT_TYPE \
    operator*(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
        const int            left_n = left.get_actual_length(); \
        const int            right_n = right.get_actual_length(); \
     \
	RESULT_TYPE result(left_n+right_n, false); \
	vector_multiply( left.get_hod(),  \
			 left.get_digits(),  \
			 right.get_hod(),  \
			 right.get_digits(),  \
			 result.get_hod(),  \
			 result.get_digits() ); \
	return result; \
    }

// big type + big type:

MULTIPLY_BIG_BIG(sc_signed,sc_signed,sc_signed)
MULTIPLY_BIG_BIG(sc_signed,sc_unsigned,sc_signed)
MULTIPLY_BIG_BIG(sc_signed,sc_signed,sc_unsigned)
MULTIPLY_BIG_BIG(sc_unsigned,sc_unsigned,sc_unsigned)

// sc_signed + C++ native:

MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,int64)
MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,uint64)
MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,long)
MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,unsigned long)
MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,int)
MULTIPLY_BIG_NATIVE(sc_signed,sc_signed,unsigned int)

// C++ native + sc_signed:

MULTIPLY_NATIVE_BIG(sc_signed,int64,sc_signed)
MULTIPLY_NATIVE_BIG(sc_signed,uint64,sc_signed)
MULTIPLY_NATIVE_BIG(sc_signed,long,sc_signed)
MULTIPLY_NATIVE_BIG(sc_signed,unsigned long,sc_signed)
MULTIPLY_NATIVE_BIG(sc_signed,int,sc_signed)
MULTIPLY_NATIVE_BIG(sc_signed,unsigned int,sc_signed)

// sc_unsigned + signed C++ native:

MULTIPLY_BIG_NATIVE(sc_signed,sc_unsigned,int64)
MULTIPLY_BIG_NATIVE(sc_signed,sc_unsigned,long)
MULTIPLY_BIG_NATIVE(sc_signed,sc_unsigned,int)

// signed C++ native + sc_unsigned:

MULTIPLY_NATIVE_BIG(sc_signed,int64,sc_unsigned)
MULTIPLY_NATIVE_BIG(sc_signed,long,sc_unsigned)
MULTIPLY_NATIVE_BIG(sc_signed,int,sc_unsigned)

// sc_unsigned + unsigned C++ native:

MULTIPLY_BIG_NATIVE(sc_unsigned,sc_unsigned,uint64)
MULTIPLY_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned long)
MULTIPLY_BIG_NATIVE(sc_unsigned,sc_unsigned,unsigned int)

// unsigned C++ native + sc_unsigned:

MULTIPLY_NATIVE_BIG(sc_unsigned,uint64,sc_unsigned)
MULTIPLY_NATIVE_BIG(sc_unsigned,unsigned long,sc_unsigned)
MULTIPLY_NATIVE_BIG(sc_unsigned,unsigned int,sc_unsigned)

// sc_int_base and sc_uint base arguments:

inline
const sc_signed operator*( const sc_signed& left, const sc_int_base& right )
    { return operator*(left, (int64)right); }

inline
const sc_signed operator*( const sc_signed& left, const sc_uint_base& right )
    { return operator*(left, (uint64)right); }

inline
const sc_signed operator*( const sc_int_base& left, const sc_signed& right )
    { return operator*((int64)left, right); }

inline
const sc_signed operator*( const sc_uint_base& left, const sc_signed& right )
    { return operator*((uint64)left, right); }

inline
const sc_signed operator*( const sc_unsigned& left, const sc_int_base& right )
    { return operator*(left, (int64)right); }

inline
const sc_signed operator*( const sc_int_base& left, const sc_unsigned& right )
    { return operator*((int64)left, right); }

inline
const sc_unsigned operator*(const sc_unsigned& left, const sc_uint_base& right )
    { return operator*(left, (uint64)right); }

inline
const sc_unsigned operator*(const sc_uint_base& left, const sc_unsigned& right )
    { return operator*((uint64)left, right); }

#undef MULTIPLY_BIG_BIG
#undef MULTIPLY_BIG_NATIVE
#undef MULTIPLY_NATIVE_BIG

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned subtraction operators"
// | 
// | These are the operators for subtraction that involves at least one instance
// | of sc_signed or sc_unsigned. The implementation is done using three macros:
// |   (a) SUBTRACT_BIG_BIG
// |   (b) SUBTRACT_BIG_NATIVE
// |   (c) SUBTRACT_NATIVE_BIG
// +----------------------------------------------------------------------------

#define SUBTRACT_BIG_BIG(LEFT_TYPE,RIGHT_TYPE) \
inline \
const sc_signed \
operator-(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	int left_n  = left.get_actual_length(); \
	int right_n = right.get_actual_length(); \
     \
	if ( left_n >= right_n ) { \
	    sc_signed result(left_n+1, false); \
	    vector_subtract_shorter( left.get_hod(), \
			left.get_digits(), \
			right.get_hod(), \
			right.get_digits(), \
			result.get_hod(), \
			result.get_digits() ); \
	    return result; \
	} \
	else { \
	    sc_signed result(right_n+1, false); \
	    vector_subtract_longer( right.get_hod(), \
			right.get_digits(), \
			left.get_hod(), \
			left.get_digits(), \
			result.get_hod(), \
			result.get_digits() ); \
	    return result; \
	} \
    } 

#define SUBTRACT_BIG_NATIVE(BIG_TYPE,NATIVE_TYPE) \
    inline \
    const sc_signed \
    operator-(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
        const int                   left_n = left.get_actual_length(); \
        const int                   right_n = right.get_actual_length(); \
     \
        if ( left_n > right_n ) { \
	    sc_signed result( left_n+1, false );  \
	    vector_subtract_shorter( left.get_hod(),  \
	                             left.get_digits(),  \
	                             right.get_hod(),  \
		                     right.get_digits(),  \
	                             result.get_hod(),  \
		                     result.get_digits() ); \
            return result; \
        } \
        else { \
	    sc_signed result( right_n+1, false );  \
	    vector_subtract_longer( right.get_hod(),  \
	                            right.get_digits(),  \
	                            left.get_hod(),  \
		                    left.get_digits(),  \
	                            result.get_hod(),  \
		                    result.get_digits() ); \
            return result; \
        } \
    } 

#define SUBTRACT_NATIVE_BIG(NATIVE_TYPE,BIG_TYPE) \
    inline \
    const sc_signed \
    operator-(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
        const int            left_n = left.get_actual_length(); \
        const int            right_n = right.get_actual_length(); \
     \
        if ( left_n > right_n ) { \
	    sc_signed result( left_n+1, false );  \
	    vector_subtract_shorter( left.get_hod(),  \
		        left.get_digits(),  \
	                right.get_hod(),  \
	                right.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
        else { \
	    sc_signed result( right_n+1, false );  \
	    vector_subtract_longer(  \
	                right.get_hod(),  \
		        right.get_digits(),  \
	                left.get_hod(),  \
	                left.get_digits(),  \
	                result.get_hod(),  \
		        result.get_digits() ); \
            return result; \
        } \
    }

// big type - big type:

SUBTRACT_BIG_BIG(sc_signed,sc_signed)
SUBTRACT_BIG_BIG(sc_unsigned,sc_signed)
SUBTRACT_BIG_BIG(sc_signed,sc_unsigned)
SUBTRACT_BIG_BIG(sc_unsigned,sc_unsigned)

// sc_signed - C++ native:

SUBTRACT_BIG_NATIVE(sc_signed,int64)
SUBTRACT_BIG_NATIVE(sc_signed,uint64)
SUBTRACT_BIG_NATIVE(sc_signed,long)
SUBTRACT_BIG_NATIVE(sc_signed,unsigned long)
SUBTRACT_BIG_NATIVE(sc_signed,int)
SUBTRACT_BIG_NATIVE(sc_signed,unsigned int)

// C++ native - sc_signed:

SUBTRACT_NATIVE_BIG(int64,sc_signed)
SUBTRACT_NATIVE_BIG(uint64,sc_signed)
SUBTRACT_NATIVE_BIG(long,sc_signed)
SUBTRACT_NATIVE_BIG(unsigned long,sc_signed)
SUBTRACT_NATIVE_BIG(int,sc_signed)
SUBTRACT_NATIVE_BIG(unsigned int,sc_signed)

// sc_unsigned - C++ native:

SUBTRACT_BIG_NATIVE(sc_unsigned,int64)
SUBTRACT_BIG_NATIVE(sc_unsigned,uint64)
SUBTRACT_BIG_NATIVE(sc_unsigned,long)
SUBTRACT_BIG_NATIVE(sc_unsigned,unsigned long)
SUBTRACT_BIG_NATIVE(sc_unsigned,int)
SUBTRACT_BIG_NATIVE(sc_unsigned,unsigned int)

// C++ native - sc_unsigned:

SUBTRACT_NATIVE_BIG(int64,sc_unsigned)
SUBTRACT_NATIVE_BIG(uint64,sc_unsigned)
SUBTRACT_NATIVE_BIG(long,sc_unsigned)
SUBTRACT_NATIVE_BIG(unsigned long,sc_unsigned)
SUBTRACT_NATIVE_BIG(int,sc_unsigned)
SUBTRACT_NATIVE_BIG(unsigned int,sc_unsigned)

// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator-( const sc_signed& left, const sc_int_base& right )
    { return operator-(left, (int64)right); }

inline
const sc_signed operator-( const sc_signed& left, const sc_uint_base& right )
    { return operator-(left, (uint64)right); }

inline
const sc_signed operator-( const sc_int_base& left, const sc_signed& right )
    { return operator-((int64)left, right); }

inline
const sc_signed operator-( const sc_uint_base& left, const sc_signed& right )
    { return operator-((uint64)left, right); }

inline
const sc_signed operator-( const sc_unsigned& left, const sc_int_base& right )
    { return operator-(left, (int64)right); }

inline
const sc_signed operator-( const sc_int_base& left, const sc_unsigned& right )
    { return operator-((int64)left, right); }

inline
const sc_signed operator-(const sc_unsigned& left, const sc_uint_base& right )
    { return operator-(left, (uint64)right); }

inline
const sc_signed operator-(const sc_uint_base& left, const sc_unsigned& right )
    { return operator-((uint64)left, right); }

#undef SUBTRACT_BIG_BIG
#undef SUBTRACT_BIG_NATIVE
#undef SUBTRACT_NATIVE_BIG

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned bit-wise logic operators"
// | 
// | These are the operators for bit-wise logic that involves at least one 
// | instance of sc_signed or sc_unsigned. The implementation is done using 
// | three macros:
// |   (a) BIT_OP_BIG_BIG
// |   (b) BIT_OP_BIG_NATIVE
// |   (c) BIT_OP_NATIVE_BIG
// +----------------------------------------------------------------------------
#define BIT_OP_BIG_BIG(OP,BIT_OP_RTN,RESULT_TYPE,LEFT_TYPE,RIGHT_TYPE) \
inline \
const RESULT_TYPE \
operator OP (const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
	int left_n; \
	int right_n; \
	if ( RESULT_TYPE::SIGNED ) { \
	    left_n  = left.get_actual_length(); \
	    right_n = right.get_actual_length(); \
	} \
	else { \
	    left_n  = left.length(); \
	    right_n = right.length(); \
	} \
     \
	if ( left_n >= right_n ) { \
	    RESULT_TYPE result(left_n, false); \
	    BIT_OP_RTN<LEFT_TYPE::SIGNED,RIGHT_TYPE::SIGNED>(  \
	                left.get_hod(), \
			left.get_digits(), \
			right.get_hod(), \
			right.get_digits(), \
			result.get_digits() ); \
	    result.adjust_hod(); \
	    return result; \
	} \
	else { \
	    RESULT_TYPE result(right_n, false); \
	    BIT_OP_RTN<RIGHT_TYPE::SIGNED,LEFT_TYPE::SIGNED>(  \
	                right.get_hod(), \
			right.get_digits(), \
			left.get_hod(), \
			left.get_digits(), \
			result.get_digits() ); \
	    result.adjust_hod(); \
	    return result; \
	} \
    } 

#define BIT_OP_BIG_NATIVE(OP,BIT_OP_RTN,RESULT_TYPE,BIG_TYPE,NATIVE_TYPE) \
    inline \
    const RESULT_TYPE \
    operator OP (const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
	int                         left_n; \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
	int                         right_n; \
	if ( RESULT_TYPE::SIGNED ) { \
	    left_n  = left.get_actual_length(); \
	    right_n = right.get_actual_length(); \
	} \
	else { \
	    left_n  = left.length(); \
	    right_n = right.length(); \
	} \
     \
        if ( left_n > right_n ) { \
	    RESULT_TYPE result( left_n, false );  \
	    BIT_OP_RTN<BIG_TYPE::SIGNED,ScNativeDigits<NATIVE_TYPE>::SIGNED>(  \
	                result.get_hod() < left.get_hod() ? result.get_hod() : left.get_hod(),  \
	                left.get_digits(),  \
	                result.get_hod() < right.get_hod() ? result.get_hod() : right.get_hod(),  \
		        right.get_digits(),  \
		        result.get_digits() ); \
	    result.adjust_hod(); \
            return result; \
        } \
        else { \
	    RESULT_TYPE result( right_n, false );  \
	    BIT_OP_RTN<ScNativeDigits<NATIVE_TYPE>::SIGNED,BIG_TYPE::SIGNED>( \
	                result.get_hod() < right.get_hod() ? result.get_hod() : right.get_hod(),  \
	                right.get_digits(),  \
	                result.get_hod() < left.get_hod() ? result.get_hod() : left.get_hod(),  \
		        left.get_digits(),  \
		        result.get_digits() ); \
	    result.adjust_hod(); \
            return result; \
        } \
    } 

#define BIT_OP_NATIVE_BIG(OP,BIT_OP_RTN,RESULT_TYPE,NATIVE_TYPE,BIG_TYPE) \
    inline \
    const RESULT_TYPE \
    operator OP (NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
	int                         left_n; \
	int                         right_n; \
	if ( RESULT_TYPE::SIGNED ) { \
	    left_n  = left.get_actual_length(); \
	    right_n = right.get_actual_length(); \
	} \
	else { \
	    left_n  = left.length(); \
	    right_n = right.length(); \
	} \
	\
        if ( left_n > right_n ) { \
	    RESULT_TYPE result( left_n, false );  \
	    BIT_OP_RTN<ScNativeDigits<NATIVE_TYPE>::SIGNED,BIG_TYPE::SIGNED>( \
	                result.get_hod() < left.get_hod() ? result.get_hod() : left.get_hod(),  \
		        left.get_digits(),  \
	                result.get_hod() < right.get_hod() ? result.get_hod() : right.get_hod(),  \
	                right.get_digits(),  \
		        result.get_digits() ); \
	    result.adjust_hod(); \
            return result; \
        } \
        else { \
	    RESULT_TYPE result( right_n, false );  \
	    BIT_OP_RTN<BIG_TYPE::SIGNED,ScNativeDigits<NATIVE_TYPE>::SIGNED>(  \
	                result.get_hod() < right.get_hod() ? result.get_hod() : right.get_hod(),  \
		        right.get_digits(),  \
	                result.get_hod() < left.get_hod() ? result.get_hod() : left.get_hod(),  \
	                left.get_digits(),  \
		        result.get_digits() ); \
	    result.adjust_hod(); \
            return result; \
        } \
    }

#define BIT_OPS_BIG(OPER,BIT_OP_RTN) \
 \
/* big type OPER big type: */ \
 \
BIT_OP_BIG_BIG(OPER,BIT_OP_RTN,sc_signed,sc_signed,sc_signed) \
BIT_OP_BIG_BIG(OPER,BIT_OP_RTN,sc_signed,sc_unsigned,sc_signed) \
BIT_OP_BIG_BIG(OPER,BIT_OP_RTN,sc_signed,sc_signed,sc_unsigned) \
BIT_OP_BIG_BIG(OPER,BIT_OP_RTN,sc_unsigned,sc_unsigned,sc_unsigned) \
 \
/* sc_signed OPER C++ native: */ \
 \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,int64) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,uint64) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,long) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,unsigned long) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,int) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_signed,unsigned int) \
 \
/* C++ native OPER sc_signed: */ \
 \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,int64,sc_signed) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,uint64,sc_signed) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,long,sc_signed) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,unsigned long,sc_signed) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,int,sc_signed) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,unsigned int,sc_signed) \
 \
/* sc_unsigned OPER signed C++ native: */ \
 \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_unsigned,int64) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_unsigned,long) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_signed,sc_unsigned,int) \
 \
/* signed C++ native OPER sc_unsigned: */ \
 \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,int64,sc_unsigned) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,long,sc_unsigned) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_signed,int,sc_unsigned) \
 \
/* sc_unsigned OPER unsigned C++ native: */ \
 \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_unsigned,sc_unsigned,uint64) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_unsigned,sc_unsigned,unsigned long) \
BIT_OP_BIG_NATIVE(OPER,BIT_OP_RTN,sc_unsigned,sc_unsigned,unsigned int) \
 \
/* unsigned C++ native OPER sc_unsigned: */ \
 \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_unsigned,uint64,sc_unsigned) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_unsigned,unsigned long,sc_unsigned) \
BIT_OP_NATIVE_BIG(OPER,BIT_OP_RTN,sc_unsigned,unsigned int,sc_unsigned) 

BIT_OPS_BIG(&,vector_and)

// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator&( const sc_signed& left, const sc_int_base& right )
    { return operator&(left, (int64)right); }

inline
const sc_signed operator&( const sc_signed& left, const sc_uint_base& right )
    { return operator&(left, (uint64)right); }

inline
const sc_signed operator&( const sc_int_base& left, const sc_signed& right )
    { return operator&((int64)left, right); }

inline
const sc_signed operator&( const sc_uint_base& left, const sc_signed& right )
    { return operator&((uint64)left, right); }

inline
const sc_signed operator&( const sc_unsigned& left, const sc_int_base& right )
    { return operator&(left, (int64)right); }

inline
const sc_signed operator&( const sc_int_base& left, const sc_unsigned& right )
    { return operator&((int64)left, right); }

inline
const sc_unsigned operator&(const sc_unsigned& left, const sc_uint_base& right )
    { return operator&(left, (uint64)right); }

inline
const sc_unsigned operator&(const sc_uint_base& left, const sc_unsigned& right )
    { return operator&((uint64)left, right); }

BIT_OPS_BIG(|,vector_or)

inline
const sc_signed operator|( const sc_signed& left, const sc_int_base& right )
    { return operator|(left, (int64)right); }

inline
const sc_signed operator|( const sc_signed& left, const sc_uint_base& right )
    { return operator|(left, (uint64)right); }

inline
const sc_signed operator|( const sc_int_base& left, const sc_signed& right )
    { return operator|((int64)left, right); }

inline
const sc_signed operator|( const sc_uint_base& left, const sc_signed& right )
    { return operator|((uint64)left, right); }

inline
const sc_signed operator|( const sc_unsigned& left, const sc_int_base& right )
    { return operator|(left, (int64)right); }

inline
const sc_signed operator|( const sc_int_base& left, const sc_unsigned& right )
    { return operator|((int64)left, right); }

inline
const sc_unsigned operator|(const sc_unsigned& left, const sc_uint_base& right )
    { return operator|(left, (uint64)right); }

inline
const sc_unsigned operator|(const sc_uint_base& left, const sc_unsigned& right )
    { return operator|((uint64)left, right); }

// sc_int_base and sc_uint_base arguments:

BIT_OPS_BIG(^,vector_xor)


// sc_int_base and sc_uint_base arguments:

inline
const sc_signed operator^( const sc_signed& left, const sc_int_base& right )
    { return operator^(left, (int64)right); }

inline
const sc_signed operator^( const sc_signed& left, const sc_uint_base& right )
    { return operator^(left, (uint64)right); }

inline
const sc_signed operator^( const sc_int_base& left, const sc_signed& right )
    { return operator^((int64)left, right); }

inline
const sc_signed operator^( const sc_uint_base& left, const sc_signed& right )
    { return operator^((uint64)left, right); }

inline
const sc_signed operator^( const sc_unsigned& left, const sc_int_base& right )
    { return operator^(left, (int64)right); }

inline
const sc_signed operator^( const sc_int_base& left, const sc_unsigned& right )
    { return operator^((int64)left, right); }

inline
const sc_unsigned operator^(const sc_unsigned& left, const sc_uint_base& right )
    { return operator^(left, (uint64)right); }

inline
const sc_unsigned operator^(const sc_uint_base& left, const sc_unsigned& right )
    { return operator^((uint64)left, right); }


#undef BIT_OP_BIG_BIG 
#undef BIT_OP_BIG_NATIVE
#undef BIT_OP_NATIVE_BIG
#undef BIT_OPS_BIG

// +----------------------------------------------------------------------------
// |"sc_signed auto increment/decrement operators"
// | 
// | These are the auto increment and auto decrement operators for sc_signed.
// +----------------------------------------------------------------------------

inline
sc_signed&
sc_signed::operator++() // prefix
{
    *this = *this + 1;
    return *this;   
}


inline
sc_signed
sc_signed::operator++(int) // postfix
{
  // Make a copy before incrementing and return it.

  sc_signed result(*this);

  *this = *this + 1;

  return result;
}

inline
sc_signed&
sc_signed::operator--() // prefix
{
    *this = *this - 1;
    return *this;   
}


inline
sc_signed
sc_signed::operator--(int) // postfix
{
  // Make a copy before decrementing and return it.

  sc_signed result(*this);

  *this = *this - 1;

  return result;
}

// +----------------------------------------------------------------------------
// |"sc_signed/sc_unsigned comparison operators"
// | 
// | These operators perform comparisons between sc_signed and sc_unsigned
// | variables and another type.
// +----------------------------------------------------------------------------
#define COMPARE_BIG_BIG(OP,LEFT_TYPE,RIGHT_TYPE) \
    inline \
    bool \
    operator OP(const LEFT_TYPE& left, const RIGHT_TYPE& right) \
    { \
     \
	return vector_compare<LEFT_TYPE::SIGNED,RIGHT_TYPE::SIGNED>( \
	                       left.get_hod(), \
			       left.get_digits(), \
			       right.get_hod(), \
			       right.get_digits() ) OP 0; \
    }

#define COMPARE_BIG_NATIVE(OP,BIG_TYPE,NATIVE_TYPE) \
    inline \
    bool \
    operator OP(const BIG_TYPE &left, NATIVE_TYPE native) \
    { \
        ScNativeDigits<NATIVE_TYPE> right( native ); \
     \
	return \
	vector_compare<BIG_TYPE::SIGNED,ScNativeDigits<NATIVE_TYPE>::SIGNED>( \
	                       left.get_hod(), \
			       left.get_digits(), \
			       right.get_hod(), \
			       right.get_digits() ) OP 0; \
    } \

#define COMPARE_NATIVE_BIG(OP,NATIVE_TYPE,BIG_TYPE) \
    inline \
    bool \
    operator OP(NATIVE_TYPE native, const BIG_TYPE &right) \
    { \
     \
        ScNativeDigits<NATIVE_TYPE> left(native); \
     \
	return \
	vector_compare<ScNativeDigits<NATIVE_TYPE>::SIGNED,BIG_TYPE::SIGNED>( \
	                       left.get_hod(), \
			       left.get_digits(), \
			       right.get_hod(), \
			       right.get_digits() ) OP 0; \
    }

#define COMPARE_BIG_OPS(OP) \
     \
    /* big type + big type: */ \
     \
    COMPARE_BIG_BIG(OP,sc_signed,sc_signed) \
    COMPARE_BIG_BIG(OP,sc_unsigned,sc_signed) \
    COMPARE_BIG_BIG(OP,sc_signed,sc_unsigned) \
    COMPARE_BIG_BIG(OP,sc_unsigned,sc_unsigned) \
     \
    /* sc_signed + C++ native: */ \
     \
    COMPARE_BIG_NATIVE(OP,sc_signed,int64) \
    COMPARE_BIG_NATIVE(OP,sc_signed,uint64) \
    COMPARE_BIG_NATIVE(OP,sc_signed,long) \
    COMPARE_BIG_NATIVE(OP,sc_signed,unsigned long) \
    COMPARE_BIG_NATIVE(OP,sc_signed,int) \
    COMPARE_BIG_NATIVE(OP,sc_signed,unsigned int) \
     \
    /* C++ native + sc_signed: */ \
     \
    COMPARE_NATIVE_BIG(OP,int64,sc_signed) \
    COMPARE_NATIVE_BIG(OP,uint64,sc_signed) \
    COMPARE_NATIVE_BIG(OP,long,sc_signed) \
    COMPARE_NATIVE_BIG(OP,unsigned long,sc_signed) \
    COMPARE_NATIVE_BIG(OP,int,sc_signed) \
    COMPARE_NATIVE_BIG(OP,unsigned int,sc_signed) \
     \
    /* sc_unsigned + signed C++ native: */ \
     \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,int64) \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,long) \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,int) \
     \
    /* signed C++ native + sc_unsigned: */ \
     \
    COMPARE_NATIVE_BIG(OP,int64,sc_unsigned) \
    COMPARE_NATIVE_BIG(OP,long,sc_unsigned) \
    COMPARE_NATIVE_BIG(OP,int,sc_unsigned) \
     \
    /* sc_unsigned + unsigned C++ native: */ \
     \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,uint64) \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,unsigned long) \
    COMPARE_BIG_NATIVE(OP,sc_unsigned,unsigned int) \
     \
    /* unsigned C++ native + sc_unsigned: */ \
     \
    COMPARE_NATIVE_BIG(OP,uint64,sc_unsigned) \
    COMPARE_NATIVE_BIG(OP,unsigned long,sc_unsigned) \
    COMPARE_NATIVE_BIG(OP,unsigned int,sc_unsigned) \
     \
    /* sc_int_base and sc_uint_base arguments: */ \
     \
    inline \
    bool operator OP( const sc_signed& left, const sc_int_base& right ) \
	{ return (left OP (int64)right); } \
     \
    inline \
    bool operator OP( const sc_signed& left, const sc_uint_base& right ) \
	{ return (left OP (uint64)right); } \
     \
    inline \
    bool operator OP( const sc_int_base& left, const sc_signed& right ) \
	{ return ((int64)left OP right); } \
     \
    inline \
    bool operator OP( const sc_uint_base& left, const sc_signed& right ) \
	{ return ((uint64)left OP right); } \
     \
    inline \
    bool operator OP( const sc_unsigned& left, const sc_int_base& right ) \
	{ return (left OP (int64)right); } \
     \
    inline \
    bool operator OP( const sc_int_base& left, const sc_unsigned& right ) \
	{ return ((int64)left OP right); } \
     \
    inline \
    bool operator OP(const sc_unsigned& left, const sc_uint_base& right ) \
	{ return (left OP (uint64)right); } \
     \
    inline \
    bool operator OP(const sc_uint_base& left, const sc_unsigned& right ) \
	{ return ((uint64)left OP right); } 

COMPARE_BIG_OPS(==)
COMPARE_BIG_OPS(!=)
COMPARE_BIG_OPS(<)
COMPARE_BIG_OPS(<=)
COMPARE_BIG_OPS(>)
COMPARE_BIG_OPS(>=)

#undef COMPARE_BIG_BIG
#undef COMPARE_BIG_NATIVE
#undef COMPARE_NATIVE_BIG
#undef COMPARE_BIG_OPS

// +----------------------------------------------------------------------------
// |"sc_unsigned auto increment/decrement operators"
// | 
// | These are the auto increment and auto decrement operators for sc_unsigned.
// +----------------------------------------------------------------------------

inline
sc_unsigned&
sc_unsigned::operator++() // prefix
{
    *this = *this + 1;
    return *this;   
}


inline
sc_unsigned
sc_unsigned::operator++(int) // postfix
{
  // Make copy to return before incrementing.

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
sc_unsigned
sc_unsigned::operator--(int) // postfix
{
  // Make copy to return before decrementing.

  sc_unsigned result(*this);

  *this = *this - 1;

  return result;
}

// +----------------------------------------------------------------------------
// |"op equals between sc_signed/sc_unsigned"
// | 
// | This can't be done like ADD_BIT_NATIVE because we end up with 
// | redundant definitions.
// +----------------------------------------------------------------------------

#define BIG_OP_EQUALS(OPEQ,OP,BASE_TYPE,OTHER_TYPE) \
inline  \
BASE_TYPE&  \
BASE_TYPE::operator OPEQ (OTHER_TYPE right)  \
{  \
    *this = *this OP right;  \
    return *this; \
}

#define BIG_OPS_EQUALS(OPEQ,OP) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,const sc_signed&) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,const sc_unsigned&) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,int64) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,long) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,int) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,uint64) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,unsigned long) \
    BIG_OP_EQUALS(OPEQ,OP,sc_signed,unsigned int) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,const sc_signed&) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,const sc_unsigned&) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,int64) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,long) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,int) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,uint64) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,unsigned long) \
    BIG_OP_EQUALS(OPEQ,OP,sc_unsigned,unsigned int) \
    inline sc_signed& sc_signed::operator OPEQ (const sc_int_base& right) \
        { return *this OPEQ  (int64)right; } \
    inline sc_signed& sc_signed::operator OPEQ (const sc_uint_base& right) \
        { return *this OPEQ  (uint64)right; } \
    inline sc_unsigned& sc_unsigned::operator OPEQ (const sc_int_base& right) \
        { return *this OPEQ  (int64)right; } \
    inline sc_unsigned& sc_unsigned::operator OPEQ (const sc_uint_base& right) \
        { return *this OPEQ  (uint64)right; } 

BIG_OPS_EQUALS(+=,+)
BIG_OPS_EQUALS(/=,/)
BIG_OPS_EQUALS(%=,%)
BIG_OPS_EQUALS(*=,*)
BIG_OPS_EQUALS(-=,-)
BIG_OPS_EQUALS(&=,&)
BIG_OPS_EQUALS(|=,|)
BIG_OPS_EQUALS(^=,^)

// +----------------------------------------------------------------------------
// |"operator ~"
// | 
// | These operators return the one's complement of the supplied object
// | instance.
// |
// | Arguments:
// |     u = object instance to return the one's complement of.
// | Result:
// |     one's complement of 'u'.
// +----------------------------------------------------------------------------
inline
sc_signed
operator~(const sc_signed& u)
{
    sc_signed result( u.length() ); 
    sc_digit* result_p = result.get_digits();
    sc_digit* source_p = u.get_digits();
    int       hod      = u.get_hod();  
    for ( int digit_i = 0; digit_i <= hod; ++digit_i ) {
        result_p[digit_i] = ~source_p[digit_i];
    }
    return result;
}

inline
sc_signed
operator~(const sc_unsigned& u)
{
    sc_signed result( u.length()+1 );   
    sc_digit* result_p = result.get_digits();
    sc_digit* source_p = u.get_digits();
    int       hod      = u.get_hod();  
    for ( int digit_i = 0; digit_i <= hod; ++digit_i ) {
        result_p[digit_i] = ~source_p[digit_i];
    }
    if ( result.get_hod() > hod ) {
        result_p[hod] = (sc_digit)-1;
    }
    return result;
}

#undef BIG_OP_EQUALS
#undef BIG_OPS_EQUAL

#undef OPS_MIN
#undef OPS_MAX

} //  namespace sc_dt 

#endif //  SC_SIGNED_OPS_H

