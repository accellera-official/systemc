/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
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

  sc_proxy.h -- Proxy base class for vector data types.

                This class is created for several purposes:
                1) hiding operators from the global namespace that would be
		   otherwise found by Koenig lookup
                2) avoiding repeating the same operations in every class
		   including proxies that could also be achieved by common
		   base class, but this method allows
                3) improve performance by using non-virtual functions

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_PROXY_H
#define SC_PROXY_H


#include "sysc/kernel/sc_cmnhdr.h"
#include "sysc/utils/sc_iostream.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/utils/sc_string.h"
#include "sysc/datatypes/bit/sc_bit.h"
#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/datatypes/bit/sc_logic.h"
#include "sysc/kernel/sc_macros.h"


namespace sc_dt
{

// classes defined in this module
template <class X> class sc_proxy;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
template <class X> class sc_bitref_r;
template <class X> class sc_bitref;
template <class X> class sc_subref_r;
template <class X> class sc_subref;
template <class X, class Y> class sc_concref_r;
template <class X, class Y> class sc_concref;


const int UL_SIZE = BITS_PER_BYTE * sizeof( unsigned long );

const unsigned long UL_ZERO = 0ul;
const unsigned long UL_ONE  = 1ul;
const unsigned long UL_TWO  = 2ul;


// assignment functions; forward declarations

template <class X, class Y>
inline
void
assign_p_( sc_proxy<X>& px, const sc_proxy<Y>& py );

// Vector types that are not derived from sc_proxy must have a length()
// function and an operator []. The vector argument type must support
// accessing bits that are beyond the msb. The vector argument type
// decides what to do there (e.g. sign extension or zero padding).

template <class X, class T>
inline
void
assign_v_( sc_proxy<X>& px, const T& a );


// other functions; forward declarations

const std::string convert_to_bin( const char* s );
const std::string convert_to_fmt( const std::string& s, sc_numrep numrep, bool );


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_proxy
//
//  Base class template for bit/logic vector classes.
//  (Barton/Nackmann implementation)
// ----------------------------------------------------------------------------

template <class X>
class sc_proxy // #### : public sc_value_base
{
public:

    // virtual destructor

    virtual ~sc_proxy() {}


    // casts

    X& back_cast()
	{ return SCAST<X&>( *this ); }

    const X& back_cast() const
	{ return SCAST<const X&>( *this ); }


    // assignment operators

    template <class Y>
    X& assign_( const sc_proxy<Y>& a )
	{ assign_p_( *this, a ); return back_cast(); }

    X& assign_( const char* a );
    X& assign_( const bool* a );
    X& assign_( const sc_logic* a );

    X& assign_( const sc_unsigned& a )
	{ assign_v_( *this, a ); return back_cast(); }

    X& assign_( const sc_signed& a )
	{ assign_v_( *this, a ); return back_cast(); }

    X& assign_( const sc_uint_base& a )
	{ return assign_( (uint64) a ); }

    X& assign_( const sc_int_base& a )
	{ return assign_( (int64) a ); }

    X& assign_( unsigned long a );
    X& assign_( long a );

    X& assign_( unsigned int a )
	{ return assign_( (unsigned long) a ); }

    X& assign_( int a )
	{ return assign_( (long) a ); }

    X& assign_( uint64 a );
    X& assign_( int64 a );


    // bitwise operators and functions

    // bitwise complement

    X& b_not();

    const sc_lv_base operator ~ () const;


    // bitwise and

    X& operator &= ( const char* b );
    X& operator &= ( const bool* b );
    X& operator &= ( const sc_logic* b );
    X& operator &= ( const sc_unsigned& b );
    X& operator &= ( const sc_signed& b );

    X& operator &= ( const sc_uint_base& b )
	{ return operator &= ( (uint64) b ); }

    X& operator &= ( const sc_int_base& b )
	{ return operator &= ( (int64) b ); }

    X& operator &= ( unsigned long b );
    X& operator &= ( long b );

    X& operator &= ( unsigned int b )
	{ return operator &= ( (unsigned long) b ); }

    X& operator &= ( int b )
	{ return operator &= ( (long) b ); }

    X& operator &= ( uint64 b );
    X& operator &= ( int64 b );


    const sc_lv_base operator & ( const char* b ) const;
    const sc_lv_base operator & ( const bool* b ) const;
    const sc_lv_base operator & ( const sc_logic* b ) const;
    const sc_lv_base operator & ( const sc_unsigned& b ) const;
    const sc_lv_base operator & ( const sc_signed& b ) const;
    const sc_lv_base operator & ( const sc_uint_base& b ) const;
    const sc_lv_base operator & ( const sc_int_base& b ) const;
    const sc_lv_base operator & ( unsigned long b ) const;
    const sc_lv_base operator & ( long b ) const;
    const sc_lv_base operator & ( unsigned int b ) const;
    const sc_lv_base operator & ( int b ) const;
    const sc_lv_base operator & ( uint64 b ) const;
    const sc_lv_base operator & ( int64 b ) const;


    // bitwise or

    X& operator |= ( const char* b );
    X& operator |= ( const bool* b );
    X& operator |= ( const sc_logic* b );
    X& operator |= ( const sc_unsigned& b );
    X& operator |= ( const sc_signed& b );

    X& operator |= ( const sc_uint_base& b )
	{ return operator |= ( (uint64) b ); }

    X& operator |= ( const sc_int_base& b )
	{ return operator |= ( (int64) b ); }

    X& operator |= ( unsigned long b );
    X& operator |= ( long b );

    X& operator |= ( unsigned int b )
	{ return operator |= ( (unsigned long) b ); }

    X& operator |= ( int b )
	{ return operator |= ( (long) b ); }

    X& operator |= ( uint64 b );
    X& operator |= ( int64 b );


    const sc_lv_base operator | ( const char* b ) const;
    const sc_lv_base operator | ( const bool* b ) const;
    const sc_lv_base operator | ( const sc_logic* b ) const;
    const sc_lv_base operator | ( const sc_unsigned& b ) const;
    const sc_lv_base operator | ( const sc_signed& b ) const;
    const sc_lv_base operator | ( const sc_uint_base& b ) const;
    const sc_lv_base operator | ( const sc_int_base& b ) const;
    const sc_lv_base operator | ( unsigned long b ) const;
    const sc_lv_base operator | ( long b ) const;
    const sc_lv_base operator | ( unsigned int b ) const;
    const sc_lv_base operator | ( int b ) const;
    const sc_lv_base operator | ( uint64 b ) const;
    const sc_lv_base operator | ( int64 b ) const;


    // bitwise xor

    X& operator ^= ( const char* b );
    X& operator ^= ( const bool* b );
    X& operator ^= ( const sc_logic* b );
    X& operator ^= ( const sc_unsigned& b );
    X& operator ^= ( const sc_signed& b );

    X& operator ^= ( const sc_uint_base& b )
	{ return operator ^= ( (uint64) b ); }

    X& operator ^= ( const sc_int_base& b )
	{ return operator ^= ( (int64) b ); }

    X& operator ^= ( unsigned long b );
    X& operator ^= ( long b );

    X& operator ^= ( unsigned int b )
	{ return operator ^= ( (unsigned long) b ); }

    X& operator ^= ( int b )
	{ return operator ^= ( (long) b ); }

    X& operator ^= ( uint64 b );
    X& operator ^= ( int64 b );


    const sc_lv_base operator ^ ( const char* b ) const;
    const sc_lv_base operator ^ ( const bool* b ) const;
    const sc_lv_base operator ^ ( const sc_logic* b ) const;
    const sc_lv_base operator ^ ( const sc_unsigned& b ) const;
    const sc_lv_base operator ^ ( const sc_signed& b ) const;
    const sc_lv_base operator ^ ( const sc_uint_base& b ) const;
    const sc_lv_base operator ^ ( const sc_int_base& b ) const;
    const sc_lv_base operator ^ ( unsigned long b ) const;
    const sc_lv_base operator ^ ( long b ) const;
    const sc_lv_base operator ^ ( unsigned int b ) const;
    const sc_lv_base operator ^ ( int b ) const;
    const sc_lv_base operator ^ ( uint64 b ) const;
    const sc_lv_base operator ^ ( int64 b ) const;


    // bitwise left shift

    X& operator <<= ( int n );

    const sc_lv_base operator << ( int n ) const;


    // bitwise right shift

    X& operator >>= ( int n );

    const sc_lv_base operator >> ( int n ) const;


    // bitwise left rotate

    X& lrotate( int n );


    // bitwise right rotate

    X& rrotate( int n );


    // bitwise reverse

    X& reverse();


    // bit selection

    sc_bitref<X> operator [] ( int i )
	{ return sc_bitref<X>( back_cast(), i ); }

    sc_bitref_r<X> operator [] ( int i ) const
	{ return sc_bitref_r<X>( back_cast(), i ); }

    sc_bitref<X> bit( int i )
	{ return sc_bitref<X>( back_cast(), i ); }

    sc_bitref_r<X> bit( int i ) const
	{ return sc_bitref_r<X>( back_cast(), i ); }


    // part selection

    sc_subref<X> operator () ( int hi, int lo )
	{ return sc_subref<X>( back_cast(), hi, lo ); }

    sc_subref_r<X> operator () ( int hi, int lo ) const
	{ return sc_subref_r<X>( back_cast(), hi, lo ); }

    sc_subref<X> range( int hi, int lo )
	{ return sc_subref<X>( back_cast(), hi, lo ); }

    sc_subref_r<X> range( int hi, int lo ) const
	{ return sc_subref_r<X>( back_cast(), hi, lo ); }


    // reduce functions

    sc_logic_value_t and_reduce() const;

    sc_logic_value_t nand_reduce() const
	{ return sc_logic::not_table[and_reduce()]; }

    sc_logic_value_t or_reduce() const;

    sc_logic_value_t nor_reduce() const
	{ return sc_logic::not_table[or_reduce()]; }

    sc_logic_value_t xor_reduce() const;

    sc_logic_value_t xnor_reduce() const
	{ return sc_logic::not_table[xor_reduce()]; }


    // relational operators

    bool operator == ( const char* b ) const;
    bool operator == ( const bool* b ) const;
    bool operator == ( const sc_logic* b ) const;
    bool operator == ( const sc_unsigned& b ) const;
    bool operator == ( const sc_signed& b ) const;
    bool operator == ( const sc_uint_base& b ) const;
    bool operator == ( const sc_int_base& b ) const;
    bool operator == ( unsigned long b ) const;
    bool operator == ( long b ) const;
    bool operator == ( unsigned int b ) const;
    bool operator == ( int b ) const;
    bool operator == ( uint64 b ) const;
    bool operator == ( int64 b ) const;


    // explicit conversions to character string

    const std::string to_string() const;
    const std::string to_string( sc_numrep ) const;
    const std::string to_string( sc_numrep, bool ) const;


    // explicit conversions

    inline int64 to_int64() const
	{ return to_anything_signed(); }
    inline uint64 to_uint64() const;
    int to_int() const
	{ return (int)to_anything_signed(); }

    unsigned int to_uint() const
	{ return (unsigned int)to_anything_unsigned(); }

    long to_long() const
	{ return (long)to_anything_signed(); }

    unsigned long to_ulong() const
	{ return (unsigned long)to_anything_unsigned(); }

#ifdef SC_DT_DEPRECATED

    int to_signed() const
	{ return to_int(); }

    unsigned to_unsigned() const
	{ return to_uint(); }

#endif


    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
	{ 
	    // the test below will force printing in binary if decimal is 
	    // specified.
	    if ( sc_io_base(os, SC_DEC) == SC_DEC )
	        os << to_string();
	    else
	        os << to_string(sc_io_base(os,SC_BIN),sc_io_show_base(os)); 
	}

    void scan( ::std::istream& is = ::std::cin );

protected:

    void check_bounds( int n ) const;  // check if bit n accessible
    void check_wbounds( int n ) const; // check if word n accessible

    unsigned long to_anything_unsigned() const;
    int64 to_anything_signed() const;
};


// ----------------------------------------------------------------------------

// bitwise operators and functions

// bitwise and

template <class X, class Y>
inline
X&
operator &= ( sc_proxy<X>& px, const sc_proxy<Y>& py );


template <class X, class Y>
inline
const sc_lv_base
operator & ( const sc_proxy<X>& px, const sc_proxy<Y>& py );


#define DECL_BITWISE_AND_OP_T(tp)                                             \
template <class X>                                                            \
inline                                                                        \
const sc_lv_base                                                              \
operator & ( tp b, const sc_proxy<X>& px );

DECL_BITWISE_AND_OP_T(const char*)
DECL_BITWISE_AND_OP_T(const bool*)
DECL_BITWISE_AND_OP_T(const sc_logic*)
DECL_BITWISE_AND_OP_T(const sc_unsigned&)
DECL_BITWISE_AND_OP_T(const sc_signed&)
DECL_BITWISE_AND_OP_T(const sc_uint_base&)
DECL_BITWISE_AND_OP_T(const sc_int_base&)
DECL_BITWISE_AND_OP_T(unsigned long)
DECL_BITWISE_AND_OP_T(long)
DECL_BITWISE_AND_OP_T(unsigned int)
DECL_BITWISE_AND_OP_T(int)
DECL_BITWISE_AND_OP_T(uint64)
DECL_BITWISE_AND_OP_T(int64)

#undef DECL_BITWISE_AND_OP_T


// bitwise or

template <class X, class Y>
inline
X&
operator |= ( sc_proxy<X>& px, const sc_proxy<Y>& py );


template <class X, class Y>
inline
const sc_lv_base
operator | ( const sc_proxy<X>& px, const sc_proxy<Y>& py );


#define DECL_BITWISE_OR_OP_T(tp)                                              \
template <class X>                                                            \
inline                                                                        \
const sc_lv_base                                                              \
operator | ( tp a, const sc_proxy<X>& px );

DECL_BITWISE_OR_OP_T(const char*)
DECL_BITWISE_OR_OP_T(const bool*)
DECL_BITWISE_OR_OP_T(const sc_logic*)
DECL_BITWISE_OR_OP_T(const sc_unsigned&)
DECL_BITWISE_OR_OP_T(const sc_signed&)
DECL_BITWISE_OR_OP_T(const sc_uint_base&)
DECL_BITWISE_OR_OP_T(const sc_int_base&)
DECL_BITWISE_OR_OP_T(unsigned long)
DECL_BITWISE_OR_OP_T(long)
DECL_BITWISE_OR_OP_T(unsigned int)
DECL_BITWISE_OR_OP_T(int)
DECL_BITWISE_OR_OP_T(uint64)
DECL_BITWISE_OR_OP_T(int64)

#undef DECL_BITWISE_OR_OP_T


// bitwise xor

template <class X, class Y>
inline
X&
operator ^= ( sc_proxy<X>& px, const sc_proxy<Y>& py );


template <class X, class Y>
inline
const sc_lv_base
operator ^ ( const sc_proxy<X>& px, const sc_proxy<Y>& py );


#define DECL_BITWISE_XOR_OP_T(tp)                                             \
template <class X>                                                            \
inline                                                                        \
const sc_lv_base                                                              \
operator ^ ( tp a, const sc_proxy<X>& px );

DECL_BITWISE_XOR_OP_T(const char*)
DECL_BITWISE_XOR_OP_T(const bool*)
DECL_BITWISE_XOR_OP_T(const sc_logic*)
DECL_BITWISE_XOR_OP_T(const sc_unsigned&)
DECL_BITWISE_XOR_OP_T(const sc_signed&)
DECL_BITWISE_XOR_OP_T(const sc_uint_base&)
DECL_BITWISE_XOR_OP_T(const sc_int_base&)
DECL_BITWISE_XOR_OP_T(unsigned long)
DECL_BITWISE_XOR_OP_T(long)
DECL_BITWISE_XOR_OP_T(unsigned int)
DECL_BITWISE_XOR_OP_T(int)
DECL_BITWISE_XOR_OP_T(uint64)
DECL_BITWISE_XOR_OP_T(int64)

#undef DECL_BITWISE_XOR_OP_T


// relational operators

template <class X, class Y>
inline
bool
operator == ( const sc_proxy<X>& px, const sc_proxy<Y>& py );

template <class X, class Y>
inline
bool
operator != ( const sc_proxy<X>& px, const sc_proxy<Y>& py );


#define DECL_REL_OP_T(tp)                                                     \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator == ( tp b, const sc_proxy<X>& px );                                  \
                                                                              \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator != ( const sc_proxy<X>& px, tp b );                                  \
                                                                              \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator != ( tp b, const sc_proxy<X>& px );

DECL_REL_OP_T(const char*)
DECL_REL_OP_T(const bool*)
DECL_REL_OP_T(const sc_logic*)
DECL_REL_OP_T(const sc_unsigned&)
DECL_REL_OP_T(const sc_signed&)
DECL_REL_OP_T(const sc_uint_base&)
DECL_REL_OP_T(const sc_int_base&)
DECL_REL_OP_T(unsigned long)
DECL_REL_OP_T(long)
DECL_REL_OP_T(unsigned int)
DECL_REL_OP_T(int)
DECL_REL_OP_T(uint64)
DECL_REL_OP_T(int64)

#undef DECL_REL_OP_T


// l-value concatenation

// Due to the fact that temporary objects cannot be passed to non-const
// references, we have to enumerate, use call by value, and use dynamic
// memory allocation (and deallocation).


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class X>
inline
void
get_words_( const X& x, int wi, unsigned long& x_dw, unsigned long& x_cw )
{
    x_dw = x.get_word( wi );
    x_cw = x.get_cword( wi );
}

template <class X>
inline
void
set_words_( X& x, int wi, unsigned long x_dw, unsigned long x_cw )
{
    x.set_word( wi, x_dw );
    x.set_cword( wi, x_cw );
}

template <class X>
inline
void
extend_sign_w_( X& x, int wi, bool sign )
{
    int sz = x.size();
    unsigned long sgn = (sign ? ~UL_ZERO : UL_ZERO);
    for( int i = wi; i < sz; ++ i ) {
	set_words_( x, i, sgn, UL_ZERO );
    }
}


// assignment functions

template <class X, class Y>
inline
void
assign_p_( sc_proxy<X>& px, const sc_proxy<Y>& py )
{
    if( (void*) &px != (void*) &py ) {
	X& x = px.back_cast();
	const Y& y = py.back_cast();
	int sz = x.size();
	int min_sz = sc_min( sz, y.size() );
	int i = 0;
	for( ; i < min_sz; ++ i ) {
	    set_words_( x, i, y.get_word( i ), y.get_cword( i ) );
	}
	// extend with zeros
	extend_sign_w_( x, i, false );
	x.clean_tail();
    }
}

// Vector types that are not derived from sc_proxy must have a length()
// function and an operator []. The vector argument type must support
// accessing bits that are beyond the msb. The vector argument type
// decides what to do there (e.g. sign extension or zero padding).

template <class X, class T>
inline
void
assign_v_( sc_proxy<X>& px, const T& a )
{
    X& x = px.back_cast();
    int len = x.length();
    for( int i = 0 ; i < len; ++ i ) {
        x.set_bit( i, sc_logic_value_t( (bool) a[i] ) );
    }
}


// assignment operators

template <class X>
inline
X&
sc_proxy<X>::assign_( const char* a )
{
    X& x = back_cast();
    std::string s = convert_to_bin( a );
    int len = x.length();
    int s_len = s.length() - 1;
    int min_len = sc_min( len, s_len );
    int i = 0;
    for( ; i < min_len; ++ i ) {
	char c = s[s_len - i - 1];
	x.set_bit( i, sc_logic::char_to_logic[(unsigned int)c] );
    }
    // if formatted, fill the rest with sign(s), otherwise fill with zeros
    sc_logic_value_t fill = (s[s_len] == 'F' ? sc_logic_value_t( s[0] - '0' )
		                             : sc_logic_value_t( 0 ));
    for( ; i < len; ++ i ) {
	x.set_bit( i, fill );
    }
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( const bool* a )
{
    // the length of 'a' must be larger than or equal to the length of 'this'
    X& x = back_cast();
    int len = x.length();
    for( int i = 0; i < len; ++ i ) {
	x.set_bit( i, sc_logic_value_t( a[i] ) );
    }
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( const sc_logic* a )
{
    // the length of 'a' must be larger than or equal to the length of 'this'
    X& x = back_cast();
    int len = x.length();
    for( int i = 0; i < len; ++ i ) {
	x.set_bit( i, a[i].value() );
    }
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( unsigned long a )
{
    X& x = back_cast();
    set_words_( x, 0, a, UL_ZERO );
    // extend with zeros
    extend_sign_w_( x, 1, false );
    x.clean_tail();
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( long a )
{
    X& x = back_cast();
    set_words_( x, 0, (unsigned long) a, UL_ZERO );
    // extend with sign(a)
    extend_sign_w_( x, 1, (a < 0) );
    x.clean_tail();
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( uint64 a )
{
    X& x = back_cast();
    set_words_( x, 0, ((unsigned long) a & ~UL_ZERO), UL_ZERO );
    if( x.size() > 1 ) {
	set_words_( x, 1,
		    ((unsigned long) (a >> UL_SIZE) & ~UL_ZERO),
		    UL_ZERO );
	// extend with zeros
	extend_sign_w_( x, 2, false );
    }
    x.clean_tail();
    return x;
}

template <class X>
inline
X&
sc_proxy<X>::assign_( int64 a )
{
    X& x = back_cast();
    set_words_( x, 0, ((unsigned long) a & ~UL_ZERO), UL_ZERO );
    if( x.size() > 1 ) {
	set_words_( x, 1,
		    ((unsigned long) ((uint64) a >> UL_SIZE) & ~UL_ZERO),
		    UL_ZERO );
	// extend with sign(a)
	extend_sign_w_( x, 2, (a < 0) );
    }
    x.clean_tail();
    return x;
}


// bitwise operators and functions

// bitwise complement

template <class X>
inline
X&
sc_proxy<X>::b_not()
{
    X& x = back_cast();
    int sz = x.size();
    for( int i = 0; i < sz; ++ i ) {
	unsigned long x_dw, x_cw;
	get_words_( x, i, x_dw, x_cw );
	x.set_word( i, x_cw | ~x_dw );
    }
    x.clean_tail();
    return x;
}


// bitwise and

template <class X, class Y>
inline
X&
b_and_assign_( sc_proxy<X>& px, const sc_proxy<Y>& py )
{
    X& x = px.back_cast();
    const Y& y = py.back_cast();
    assert( x.length() == y.length() );
    int sz = x.size();
    for( int i = 0; i < sz; ++ i ) {
	unsigned long x_dw, x_cw, y_dw, y_cw;
	get_words_( x, i, x_dw, x_cw );
	get_words_( y, i, y_dw, y_cw );
	unsigned long cw = x_dw & y_cw | x_cw & y_dw | x_cw & y_cw;
	unsigned long dw = cw | x_dw & y_dw;
	set_words_( x, i, dw, cw );
    }
    // tail cleaning not needed
    return x;
}


// bitwise or

template <class X, class Y>
inline
X&
b_or_assign_( sc_proxy<X>& px, const sc_proxy<Y>& py )
{
    X& x = px.back_cast();
    const Y& y = py.back_cast();
    assert( x.length() == y.length() );
    int sz = x.size();
    for( int i = 0; i < sz; ++ i ) {
	unsigned long x_dw, x_cw, y_dw, y_cw;
	get_words_( x, i, x_dw, x_cw );
	get_words_( y, i, y_dw, y_cw );
	unsigned long cw = x_cw & y_cw | x_cw & ~y_dw | ~x_dw & y_cw;
	unsigned long dw = cw | x_dw | y_dw;
	set_words_( x, i, dw, cw );
    }
    // tail cleaning not needed
    return x;
}


// bitwise xor

template <class X, class Y>
inline
X&
b_xor_assign_( sc_proxy<X>& a, const sc_proxy<Y>& b )
{
    X& x = a.back_cast();
    const Y& y = b.back_cast();
    assert( x.length() == y.length() );
    int sz = x.size();
    for( int i = 0; i < sz; ++ i ) {
	unsigned long x_dw, x_cw, y_dw, y_cw;
	get_words_( x, i, x_dw, x_cw );
	get_words_( y, i, y_dw, y_cw );
	unsigned long cw = x_cw | y_cw;
	unsigned long dw = cw | x_dw ^ y_dw;
	set_words_( x, i, dw, cw );
    }
    // tail cleaning not needed
    return x;
}


// bitwise left shift

template <class X>
inline
X&
sc_proxy<X>::operator <<= ( int n )
{
    X& x = back_cast();
    if( n < 0 ) {
	char msg[BUFSIZ];
	sprintf( msg,
		 "left shift operation is only allowed with positive "
		 "shift values, shift value = %d", n );
	SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, msg );
    }
    if( n >= x.length() ) {
	extend_sign_w_( x, 0, false );
	// no tail cleaning needed
	return x;
    }
    int sz = x.size();
    int wn = n / UL_SIZE;
    int bn = n % UL_SIZE;
    if( wn != 0 ) {
	// shift words
	int i = sz - 1;
	for( ; i >= wn; -- i ) {
	    set_words_( x, i, x.get_word( i - wn ), x.get_cword( i - wn ) );
	}
	for( ; i >= 0; -- i ) {
	    set_words_( x, i, UL_ZERO, UL_ZERO );
	}
    }
    if( bn != 0 ) {
	// shift bits
	for( int i = sz - 1; i >= 1; -- i ) {
	    unsigned long x_dw, x_cw;
	    get_words_( x, i, x_dw, x_cw );
	    x_dw <<= bn;
	    x_dw |= x.get_word( i - 1 ) >> (UL_SIZE - bn);
	    x_cw <<= bn;
	    x_cw |= x.get_cword( i - 1 ) >> (UL_SIZE - bn);
	    set_words_( x, i, x_dw, x_cw );
	}
	unsigned long x_dw, x_cw;
	get_words_( x, 0, x_dw, x_cw );
	x_dw <<= bn;
	x_cw <<= bn;
	set_words_( x, 0, x_dw, x_cw );
    }
    x.clean_tail();
    return x;
}


// bitwise right shift


template <class X>
inline
X&
sc_proxy<X>::operator >>= ( int n )
{
    X& x = back_cast();
    if( n < 0 ) {
	char msg[BUFSIZ];
	sprintf( msg,
		 "right shift operation is only allowed with positive "
		 "shift values, shift value = %d", n );
	SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, msg );
    }
    if( n >= x.length() ) {
	extend_sign_w_( x, 0, false );
	// no tail cleaning needed
	return x;
    }
    int sz = x.size();
    int wn = n / UL_SIZE;
    int bn = n % UL_SIZE;
    if( wn != 0 ) {
	// shift words
	int i = 0;
	for( ; i < (sz - wn); ++ i ) {
	    set_words_( x, i, x.get_word( i + wn ), x.get_cword( i + wn ) );
	}
	for( ; i < sz; ++ i ) {
	    set_words_( x, i, UL_ZERO, UL_ZERO );
	}
    }
    if( bn != 0 ) {
	// shift bits
	for( int i = 0; i < (sz - 1); ++ i ) {
	    unsigned long x_dw, x_cw;
	    get_words_( x, i, x_dw, x_cw );
	    x_dw >>= bn;
	    x_dw |= x.get_word( i + 1 ) << (UL_SIZE - bn);
	    x_cw >>= bn;
	    x_cw |= x.get_cword( i + 1 ) << (UL_SIZE - bn);
	    set_words_( x, i, x_dw, x_cw );
	}
	unsigned long x_dw, x_cw;
	get_words_( x, sz - 1, x_dw, x_cw );
	x_dw >>= bn;
	x_cw >>= bn;
	set_words_( x, sz - 1, x_dw, x_cw );
    }
    x.clean_tail();
    return x;
}


// bitwise left rotate

template <class X>
inline
const sc_lv_base
lrotate( const sc_proxy<X>& x, int n );


// bitwise right rotate

template <class X>
inline
const sc_lv_base
rrotate( const sc_proxy<X>& x, int n );


// bitwise reverse

template <class X>
inline
X&
sc_proxy<X>::reverse()
{
    X& x = back_cast();
    int len = x.length();
    int half_len = len / 2;
    for( int i = 0, j = len - 1; i < half_len; ++ i, --j ) {
	sc_logic_value_t t = x.get_bit( i );
	x.set_bit( i, x.get_bit( j ) );
	x.set_bit( j, t );
    }
    return x;
}

template <class X>
inline
const sc_lv_base
reverse( const sc_proxy<X>& a );


// reduce functions

template <class X>
inline
sc_logic_value_t
sc_proxy<X>::and_reduce() const
{
    const X& x = back_cast();
    sc_logic_value_t result = sc_logic_value_t( 1 );
    int len = x.length();
    for( int i = 0; i < len; ++ i ) {
	result = sc_logic::and_table[result][x.get_bit( i )];
    }
    return result;
}

template <class X>
inline
sc_logic_value_t
sc_proxy<X>::or_reduce() const
{
    const X& x = back_cast();
    sc_logic_value_t result = sc_logic_value_t( 0 );
    int len = x.length();
    for( int i = 0; i < len; ++ i ) {
	result = sc_logic::or_table[result][x.get_bit( i )];
    }
    return result;
}

template <class X>
inline
sc_logic_value_t
sc_proxy<X>::xor_reduce() const
{
    const X& x = back_cast();
    sc_logic_value_t result = sc_logic_value_t( 0 );
    int len = x.length();
    for( int i = 0; i < len; ++ i ) {
	result = sc_logic::xor_table[result][x.get_bit( i )];
    }
    return result;
}


// relational operators

template <class X, class Y>
inline
bool
operator != ( const sc_proxy<X>& px, const sc_proxy<Y>& py )
{
    return !( px == py );
}


#define DEFN_REL_OP_T(tp)                                                     \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator == ( tp b, const sc_proxy<X>& px )                                   \
{                                                                             \
    return ( px == b );                                                       \
}                                                                             \
                                                                              \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator != ( const sc_proxy<X>& px, tp b )                                   \
{                                                                             \
    return !( px == b );                                                      \
}                                                                             \
                                                                              \
template <class X>                                                            \
inline                                                                        \
bool                                                                          \
operator != ( tp b, const sc_proxy<X>& px )                                   \
{                                                                             \
    return !( px == b );                                                      \
}

DEFN_REL_OP_T(const char*)
DEFN_REL_OP_T(const bool*)
DEFN_REL_OP_T(const sc_logic*)
DEFN_REL_OP_T(const sc_unsigned&)
DEFN_REL_OP_T(const sc_signed&)
DEFN_REL_OP_T(const sc_uint_base&)
DEFN_REL_OP_T(const sc_int_base&)
DEFN_REL_OP_T(unsigned long)
DEFN_REL_OP_T(long)
DEFN_REL_OP_T(unsigned int)
DEFN_REL_OP_T(int)
DEFN_REL_OP_T(uint64)
DEFN_REL_OP_T(int64)

#undef DEFN_REL_OP_T


// explicit conversions to character string

template <class X>
inline
const std::string
sc_proxy<X>::to_string() const
{
    const X& x = back_cast();
    int len = x.length();
    std::string s; // ( len + 1 );
    for( int i = 0; i < len; ++ i ) {
	s += sc_logic::logic_to_char[x.get_bit( len - i - 1 )];
    }
    return s;
}

template <class X>
inline
const std::string
sc_proxy<X>::to_string( sc_numrep numrep ) const
{
    return convert_to_fmt( to_string(), numrep, true );
}

template <class X>
inline
const std::string
sc_proxy<X>::to_string( sc_numrep numrep, bool w_prefix ) const
{
    return convert_to_fmt( to_string(), numrep, w_prefix );
}


// other methods

template <class X>
inline
void
sc_proxy<X>::scan( ::std::istream& is )
{
    std::string s;
    is >> s;
    back_cast() = s.c_str();
}


template <class X>
inline
void
sc_proxy<X>::check_bounds( int n ) const  // check if bit n accessible
{
    if( n < 0 || n >= back_cast().length() ) {
	SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, 0 );
    }
}

template <class X>
inline
void
sc_proxy<X>::check_wbounds( int n ) const  // check if word n accessible
{
    if( n < 0 || n >= back_cast().size() ) {
	SC_REPORT_ERROR( sc_core::SC_ID_OUT_OF_BOUNDS_, 0 );
    }
}


template <class X>
inline
unsigned long
sc_proxy<X>::to_anything_unsigned() const
{
    // only 0 word is returned
    // can't convert logic values other than 0 and 1
    const X& x = back_cast();
    int len = x.length();
    if( x.get_cword( 0 ) != UL_ZERO ) {
	SC_REPORT_WARNING( sc_core::SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, 0 );
    }
    unsigned long w = x.get_word( 0 );
    if( len >= UL_SIZE ) {
	return w;
    }
    return ( w & (~UL_ZERO >> (UL_SIZE - len)) );
}

template <class X>
inline
uint64 
sc_proxy<X>::to_uint64() const
{
    // words 1 and 0 returned.
    // can't convert logic values other than 0 and 1
    const X& x = back_cast();
    int len = x.length();
    if( x.get_cword( 0 ) != UL_ZERO ) {
	SC_REPORT_WARNING( sc_core::SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, 0 );
    }
    uint64 w = x.get_word( 0 );
    if( len > UL_SIZE ) 
    {
	if( x.get_cword( 1 ) != UL_ZERO ) {
	    SC_REPORT_WARNING( sc_core::SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, 0 );
	}
	uint64 w1 = x.get_word( 1 );
        w = w | (w1 << UL_SIZE);	
	return w;
    }
    else if( len == UL_SIZE ) 
    {
	return w;
    }
    else
    {
	return ( w & (~UL_ZERO >> (UL_SIZE - len)) );
    }
}

template <class X>
inline
int64
sc_proxy<X>::to_anything_signed() const
{
    // only 0 word is returned
    // can't convert logic values other than 0 and 1
    const X& x = back_cast();
    int len = x.length();
    if( x.get_cword( 0 ) != UL_ZERO ) {
	SC_REPORT_WARNING( sc_core::SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, 0 );
    }
    int64 w = x.get_word( 0 );
    uint64 zero = 0;
    if( len >= 64 ) {
	return (uint64) w;
    }
    sc_logic_value_t sgn = x.get_bit( len - 1 );
    if( sgn == 0 ) {
	return ( w & (~zero >> (64 - len)) );
    } else {
	return ( w | (~zero << len) );
    }
}


// ----------------------------------------------------------------------------

// functional notation for the reduce methods

template <class X>
inline
sc_logic_value_t
and_reduce( const sc_proxy<X>& a )
{
    return a.and_reduce();
}

template <class X>
inline
sc_logic_value_t
nand_reduce( const sc_proxy<X>& a )
{
    return a.nand_reduce();
}

template <class X>
inline
sc_logic_value_t
or_reduce( const sc_proxy<X>& a )
{
    return a.or_reduce();
}

template <class X>
inline
sc_logic_value_t
nor_reduce( const sc_proxy<X>& a )
{
    return a.nor_reduce();
}

template <class X>
inline
sc_logic_value_t
xor_reduce( const sc_proxy<X>& a )
{
    return a.xor_reduce();
}

template <class X>
inline
sc_logic_value_t
xnor_reduce( const sc_proxy<X>& a )
{
    return a.xnor_reduce();
}


// ----------------------------------------------------------------------------

template <class X>
inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_proxy<X>& a )
{
    a.print( os );
    return os;
}

template <class X>
inline
::std::istream&
operator >> ( ::std::istream& is, sc_proxy<X>& a )
{
    a.scan( is );
    return is;
}

} // namespace sc_dt


#endif
