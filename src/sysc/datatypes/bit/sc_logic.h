/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
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

  sc_logic.h -- C++ implementation of logic type. Behaves
                pretty much the same way as HDLs except with 4 values.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_logic.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:53:53  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_LOGIC_H
#define SC_LOGIC_H


#include <cstdio>

#include "sysc/utils/sc_iostream.h"
#include "sysc/kernel/sc_macros.h"
#include "sysc/utils/sc_mempool.h"
#include "sysc/datatypes/bit/sc_bit.h"


namespace sc_dt
{

// classes defined in this module
class sc_logic;


// ----------------------------------------------------------------------------
//  ENUM : sc_logic_value_t
//
//  Enumeration of values for sc_logic.
// ----------------------------------------------------------------------------

enum sc_logic_value_t
{
    Log_0 = 0,
    Log_1,
    Log_Z,
    Log_X
};


// friend operator declarations
    // bitwise and

    inline const sc_logic operator & ( const sc_logic& a, const sc_logic& b );

    inline const sc_logic operator & ( const sc_logic& a, sc_logic_value_t b );

    inline const sc_logic operator & ( const sc_logic& a, bool b );

    inline const sc_logic operator & ( const sc_logic& a, char b );

    inline const sc_logic operator & ( const sc_logic& a, int b );

    inline const sc_logic operator & ( sc_logic_value_t a, const sc_logic& b );

    inline const sc_logic operator & ( bool a, const sc_logic& b );

    inline const sc_logic operator & ( char a, const sc_logic& b );

    inline const sc_logic operator & ( int a, const sc_logic& b );


    // bitwise or

    inline const sc_logic operator | ( const sc_logic& a, const sc_logic& b );

    inline const sc_logic operator | ( const sc_logic& a, sc_logic_value_t b );

    inline const sc_logic operator | ( const sc_logic& a, bool b );

    inline const sc_logic operator | ( const sc_logic& a, char b );

    inline const sc_logic operator | ( const sc_logic& a, int b );

    inline const sc_logic operator | ( sc_logic_value_t a, const sc_logic& b );

    inline const sc_logic operator | ( bool a, const sc_logic& b );

    inline const sc_logic operator | ( char a, const sc_logic& b );

    inline const sc_logic operator | ( int a, const sc_logic& b );


    // bitwise xor

    inline const sc_logic operator ^ ( const sc_logic& a, const sc_logic& b );

    inline const sc_logic operator ^ ( const sc_logic& a, sc_logic_value_t b );

    inline const sc_logic operator ^ ( const sc_logic& a, bool b );

    inline const sc_logic operator ^ ( const sc_logic& a, char b );

    inline const sc_logic operator ^ ( const sc_logic& a, int b );

    inline const sc_logic operator ^ ( sc_logic_value_t a, const sc_logic& b );

    inline const sc_logic operator ^ ( bool a, const sc_logic& b );

    inline const sc_logic operator ^ ( char a, const sc_logic& b );

    inline const sc_logic operator ^ ( int a, const sc_logic& b );


    // relational operators and functions

    inline bool operator == ( const sc_logic& a, const sc_logic& b );

    inline bool operator == ( const sc_logic& a, sc_logic_value_t b );

    inline bool operator == ( const sc_logic& a, bool b );

    inline bool operator == ( const sc_logic& a, char b );

    inline bool operator == ( const sc_logic& a, int b );

    inline bool operator == ( sc_logic_value_t a, const sc_logic& b );

    inline bool operator == ( bool a, const sc_logic& b );

    inline bool operator == ( char a, const sc_logic& b );

    inline bool operator == ( int a, const sc_logic& b );


    inline bool operator != ( const sc_logic& a, const sc_logic& b );

    inline bool operator != ( const sc_logic& a, sc_logic_value_t b );

    inline bool operator != ( const sc_logic& a, bool b );

    inline bool operator != ( const sc_logic& a, char b );

    inline bool operator != ( const sc_logic& a, int b );

    inline bool operator != ( sc_logic_value_t a, const sc_logic& b );

    inline bool operator != ( bool a, const sc_logic& b );

    inline bool operator != ( char a, const sc_logic& b );

    inline bool operator != ( int a, const sc_logic& b );




// ----------------------------------------------------------------------------
//  CLASS : sc_logic
//
//  Four-valued logic type.
// ----------------------------------------------------------------------------

class sc_logic
{
    friend class sc_logic_resolve;

private:

    // support methods

    static void invalid_value( sc_logic_value_t );
    static void invalid_value( char );
    static void invalid_value( int );

    static sc_logic_value_t to_value( sc_logic_value_t v )
	{
	    if( v < Log_0 || v > Log_X ) {
		invalid_value( v );
	    }
	    return v;
	}

    static sc_logic_value_t to_value( bool b )
	{ return ( b ? Log_1 : Log_0 ); }

    static sc_logic_value_t to_value( char c )
	{
	    sc_logic_value_t v;
	    unsigned int index = (int)c;
	    if ( index > 127 )
	    {
	        invalid_value(c);
		v = Log_X;
	    }
	    else
	    {
		v = char_to_logic[index];
		if( v < Log_0 || v > Log_X ) {
		    invalid_value( c );
		}
	    }
	    return v;
	}

    static sc_logic_value_t to_value( int i )
	{
	    if( i < 0 || i > 3 ) {
		invalid_value( i );
	    }
	    return sc_logic_value_t( i );
	}


    void invalid_01() const;

public:

    // conversion tables

    static const sc_logic_value_t char_to_logic[128];
    static const char logic_to_char[4];
    static const sc_logic_value_t and_table[4][4];
    static const sc_logic_value_t or_table[4][4];
    static const sc_logic_value_t xor_table[4][4];
    static const sc_logic_value_t not_table[4];


    // constructors

    sc_logic()
	: m_val( Log_X )
	{}

    sc_logic( const sc_logic& a )
	: m_val( a.m_val )
	{}

    sc_logic( sc_logic_value_t v )
	: m_val( to_value( v ) )
	{}

    explicit sc_logic( bool a )
	: m_val( to_value( a ) )
	{}

    explicit sc_logic( char a )
	: m_val( to_value( a ) )
	{}

    explicit sc_logic( int a )
	: m_val( to_value( a ) )
	{}

    explicit sc_logic( const sc_bit& a )
	: m_val( to_value( a.to_bool() ) )
	{}


    // destructor

    ~sc_logic()
	{}


    // assignment operators

    sc_logic& operator = ( const sc_logic& a )
	{ m_val = a.m_val; return *this; }

    sc_logic& operator = ( sc_logic_value_t v )
	{ *this = sc_logic( v ); return *this; }

    sc_logic& operator = ( bool a )
	{ *this = sc_logic( a ); return *this; }

    sc_logic& operator = ( char a )
	{ *this = sc_logic( a ); return *this; }

    sc_logic& operator = ( int a )
	{ *this = sc_logic( a ); return *this; }

    sc_logic& operator = ( const sc_bit& a )
	{ *this = sc_logic( a ); return *this; }


    // bitwise assignment operators

    sc_logic& operator &= ( const sc_logic& b )
	{ m_val = and_table[m_val][b.m_val]; return *this; }

    sc_logic& operator &= ( sc_logic_value_t v )
	{ *this &= sc_logic( v ); return *this; }

    sc_logic& operator &= ( bool b )
	{ *this &= sc_logic( b ); return *this; }

    sc_logic& operator &= ( char b )
	{ *this &= sc_logic( b ); return *this; }

    sc_logic& operator &= ( int b )
	{ *this &= sc_logic( b ); return *this; }


    sc_logic& operator |= ( const sc_logic& b )
        { m_val = or_table[m_val][b.m_val]; return *this; }

    sc_logic& operator |= ( sc_logic_value_t v )
	{ *this |= sc_logic( v ); return *this; }

    sc_logic& operator |= ( bool b )
	{ *this |= sc_logic( b ); return *this; }

    sc_logic& operator |= ( char b )
	{ *this |= sc_logic( b ); return *this; }

    sc_logic& operator |= ( int b )
	{ *this |= sc_logic( b ); return *this; }


    sc_logic& operator ^= ( const sc_logic& b )
        { m_val = xor_table[m_val][b.m_val]; return *this; }

    sc_logic& operator ^= ( sc_logic_value_t v )
	{ *this ^= sc_logic( v ); return *this; }

    sc_logic& operator ^= ( bool b )
	{ *this ^= sc_logic( b ); return *this; }

    sc_logic& operator ^= ( char b )
	{ *this ^= sc_logic( b ); return *this; }

    sc_logic& operator ^= ( int b )
	{ *this ^= sc_logic( b ); return *this; }


    // bitwise operators and functions

    // bitwise complement

    const sc_logic operator ~ () const
	{ return sc_logic( not_table[m_val] ); }

    sc_logic& b_not()
	{ m_val = not_table[m_val]; return *this; }


    // bitwise and

    friend const sc_logic operator & ( const sc_logic& a, const sc_logic& b )
	{ return sc_logic( sc_logic::and_table[a.m_val][b.m_val] ); }

    friend const sc_logic operator & ( const sc_logic& a, sc_logic_value_t b )
	{ return ( a & sc_logic( b ) ); }

    friend const sc_logic operator & ( const sc_logic& a, bool b )
	{ return ( a & sc_logic( b ) ); }

    friend const sc_logic operator & ( const sc_logic& a, char b )
	{ return ( a & sc_logic( b ) ); }

    friend const sc_logic operator & ( const sc_logic& a, int b )
	{ return ( a & sc_logic( b ) ); }

    friend const sc_logic operator & ( sc_logic_value_t a, const sc_logic& b )
	{ return ( sc_logic( a ) & b ); }

    friend const sc_logic operator & ( bool a, const sc_logic& b )
	{ return ( sc_logic( a ) & b ); }

    friend const sc_logic operator & ( char a, const sc_logic& b )
	{ return ( sc_logic( a ) & b ); }

    friend const sc_logic operator & ( int a, const sc_logic& b )
	{ return ( sc_logic( a ) & b ); }


    // bitwise or

    friend const sc_logic operator | ( const sc_logic& a, const sc_logic& b )
	{ return sc_logic( sc_logic::or_table[a.m_val][b.m_val] ); }

    friend const sc_logic operator | ( const sc_logic& a, sc_logic_value_t b )
	{ return ( a | sc_logic( b ) ); }

    friend const sc_logic operator | ( const sc_logic& a, bool b )
	{ return ( a | sc_logic( b ) ); }

    friend const sc_logic operator | ( const sc_logic& a, char b )
	{ return ( a | sc_logic( b ) ); }

    friend const sc_logic operator | ( const sc_logic& a, int b )
	{ return ( a | sc_logic( b ) ); }

    friend const sc_logic operator | ( sc_logic_value_t a, const sc_logic& b )
	{ return ( sc_logic( a ) | b ); }

    friend const sc_logic operator | ( bool a, const sc_logic& b )
	{ return ( sc_logic( a ) | b ); }

    friend const sc_logic operator | ( char a, const sc_logic& b )
	{ return ( sc_logic( a ) | b ); }

    friend const sc_logic operator | ( int a, const sc_logic& b )
	{ return ( sc_logic( a ) | b ); }


    // bitwise xor

    friend const sc_logic operator ^ ( const sc_logic& a, const sc_logic& b )
	{ return sc_logic( sc_logic::xor_table[a.m_val][b.m_val] ); }

    friend const sc_logic operator ^ ( const sc_logic& a, sc_logic_value_t b )
	{ return ( a ^ sc_logic( b ) ); }

    friend const sc_logic operator ^ ( const sc_logic& a, bool b )
	{ return ( a ^ sc_logic( b ) ); }

    friend const sc_logic operator ^ ( const sc_logic& a, char b )
	{ return ( a ^ sc_logic( b ) ); }

    friend const sc_logic operator ^ ( const sc_logic& a, int b )
	{ return ( a ^ sc_logic( b ) ); }

    friend const sc_logic operator ^ ( sc_logic_value_t a, const sc_logic& b )
	{ return ( sc_logic( a ) ^ b ); }

    friend const sc_logic operator ^ ( bool a, const sc_logic& b )
	{ return ( sc_logic( a ) ^ b ); }

    friend const sc_logic operator ^ ( char a, const sc_logic& b )
	{ return ( sc_logic( a ) ^ b ); }

    friend const sc_logic operator ^ ( int a, const sc_logic& b )
	{ return ( sc_logic( a ) ^ b ); }


    // relational operators and functions

    friend bool operator == ( const sc_logic& a, const sc_logic& b )
	{ return ( (int) a.m_val == b.m_val ); }

    friend bool operator == ( const sc_logic& a, sc_logic_value_t b )
	{ return ( a == sc_logic( b ) ); }

    friend bool operator == ( const sc_logic& a, bool b )
	{ return ( a == sc_logic( b ) ); }

    friend bool operator == ( const sc_logic& a, char b )
	{ return ( a == sc_logic( b ) ); }

    friend bool operator == ( const sc_logic& a, int b )
	{ return ( a == sc_logic( b ) ); }

    friend bool operator == ( sc_logic_value_t a, const sc_logic& b )
	{ return ( sc_logic( a ) == b ); }

    friend bool operator == ( bool a, const sc_logic& b )
	{ return ( sc_logic( a ) == b ); }

    friend bool operator == ( char a, const sc_logic& b )
	{ return ( sc_logic( a ) == b ); }

    friend bool operator == ( int a, const sc_logic& b )
	{ return ( sc_logic( a ) == b ); }


    friend bool operator != ( const sc_logic& a, const sc_logic& b )
	{ return ( (int) a.m_val != b.m_val ); }

    friend bool operator != ( const sc_logic& a, sc_logic_value_t b )
	{ return ( a != sc_logic( b ) ); }

    friend bool operator != ( const sc_logic& a, bool b )
	{ return ( a != sc_logic( b ) ); }

    friend bool operator != ( const sc_logic& a, char b )
	{ return ( a != sc_logic( b ) ); }

    friend bool operator != ( const sc_logic& a, int b )
	{ return ( a != sc_logic( b ) ); }

    friend bool operator != ( sc_logic_value_t a, const sc_logic& b )
	{ return ( sc_logic( a ) != b ); }

    friend bool operator != ( bool a, const sc_logic& b )
	{ return ( sc_logic( a ) != b ); }

    friend bool operator != ( char a, const sc_logic& b )
	{ return ( sc_logic( a ) != b ); }

    friend bool operator != ( int a, const sc_logic& b )
	{ return ( sc_logic( a ) != b ); }


    // explicit conversions

    sc_logic_value_t value() const
	{ return m_val; }


    bool is_01() const
	{ return ( (int) m_val == Log_0 || (int) m_val == Log_1 ); }

    bool to_bool() const
	{ if( ! is_01() ) { invalid_01(); } return ( (int) m_val != Log_0 ); }

    char to_char() const
	{ return logic_to_char[m_val]; }


    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
	{ os << to_char(); }

    void scan( ::std::istream& is = ::std::cin );


    // memory (de)allocation

    static void* operator new( std::size_t, void* p ) // placement new
	{ return p; }

    static void* operator new( std::size_t sz )
	{ return sc_core::sc_mempool::allocate( sz ); }

    static void operator delete( void* p, std::size_t sz )
	{ sc_core::sc_mempool::release( p, sz ); }

    static void* operator new [] ( std::size_t sz )
	{ return sc_core::sc_mempool::allocate( sz ); }

    static void operator delete [] ( void* p, std::size_t sz )
	{ sc_core::sc_mempool::release( p, sz ); }

private:

    sc_logic_value_t m_val;

private:

    // disabled
    explicit sc_logic( const char* );
    sc_logic& operator = ( const char* );
};


// ----------------------------------------------------------------------------

inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_logic& a )
{
    a.print( os );
    return os;
}

inline
::std::istream&
operator >> ( ::std::istream& is, sc_logic& a )
{
    a.scan( is );
    return is;
}


extern const sc_logic SC_LOGIC_0;
extern const sc_logic SC_LOGIC_1;
extern const sc_logic SC_LOGIC_Z;
extern const sc_logic SC_LOGIC_X;

// #ifdef SC_DT_DEPRECATED
extern const sc_logic sc_logic_0;
extern const sc_logic sc_logic_1;
extern const sc_logic sc_logic_Z;
extern const sc_logic sc_logic_X;
// #endif

} // namespace sc_dt


#endif
