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

  sc_bit.h -- Bit class.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

// $Log: sc_bit.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.5  2006/04/12 20:17:52  acg
//  Andy Goodrich: enabled deprecation message for sc_bit.
//
// Revision 1.4  2006/01/24 20:50:55  acg
// Andy Goodrich: added warnings indicating that sc_bit is deprecated and that
// the C bool data type should be used in its place.
//
// Revision 1.3  2006/01/13 18:53:53  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_BIT_H
#define SC_BIT_H


#include "sysc/datatypes/int/sc_nbdefs.h"
#include "sysc/utils/sc_iostream.h"


namespace sc_dt
{

// classes defined in this module
class sc_bit;

// forward class declarations
class sc_logic;

extern void sc_deprecated_sc_bit();

// friend operator declarations

    // relational operators and functions

    // MANDATORY

    inline bool operator == ( const sc_bit& a, const sc_bit& b );
    inline bool operator == ( const sc_bit& a, int b );
    inline bool operator == ( const sc_bit& a, bool b );
    inline bool operator == ( const sc_bit& a, char b );
    inline bool operator == ( int a, const sc_bit& b );
    inline bool operator == ( bool a, const sc_bit& b );
    inline bool operator == ( char a, const sc_bit& b );

    // OPTIONAL

    inline bool equal( const sc_bit& a, const sc_bit& b );
    inline bool equal( const sc_bit& a, int b );
    inline bool equal( const sc_bit& a, bool b );
    inline bool equal( const sc_bit& a, char b );
    inline bool equal( int a, const sc_bit& b );
    inline bool equal( bool a, const sc_bit& b );
    inline bool equal( char a, const sc_bit& b );

    // MANDATORY

    inline bool operator != ( const sc_bit& a, const sc_bit& b );
    inline bool operator != ( const sc_bit& a, int b );
    inline bool operator != ( const sc_bit& a, bool b );
    inline bool operator != ( const sc_bit& a, char b );
    inline bool operator != ( int a, const sc_bit& b );
    inline bool operator != ( bool a, const sc_bit& b );
    inline bool operator != ( char a, const sc_bit& b );

    // OPTIONAL

    inline bool not_equal( const sc_bit& a, const sc_bit& b );
    inline bool not_equal( const sc_bit& a, int b );
    inline bool not_equal( const sc_bit& a, bool b );
    inline bool not_equal( const sc_bit& a, char b );
    inline bool not_equal( int a, const sc_bit& b );
    inline bool not_equal( bool a, const sc_bit& b );
    inline bool not_equal( char a, const sc_bit& b );


    // bitwise operators and functions

    // bitwise complement

    // MANDATORY

    inline const sc_bit operator ~ ( const sc_bit& a );

    // OPTIONAL

    inline const sc_bit b_not( const sc_bit& a );

    // RECOMMENDED

    inline void b_not( sc_bit& r, const sc_bit& a );


    // bitwise or

    // MANDATORY

    inline const sc_bit operator | ( const sc_bit& a, const sc_bit& b );
    inline const sc_bit operator | ( const sc_bit& a, int b );
    inline const sc_bit operator | ( const sc_bit& a, bool b );
    inline const sc_bit operator | ( const sc_bit& a, char b );
    inline const sc_bit operator | ( int a, const sc_bit& b );
    inline const sc_bit operator | ( bool a, const sc_bit& b );
    inline const sc_bit operator | ( char a, const sc_bit& b );

    // OPTIONAL

    inline const sc_bit b_or( const sc_bit& a, const sc_bit& b );
    inline const sc_bit b_or( const sc_bit& a, int b );
    inline const sc_bit b_or( const sc_bit& a, bool b );
    inline const sc_bit b_or( const sc_bit& a, char b );
    inline const sc_bit b_or( int a, const sc_bit& b );
    inline const sc_bit b_or( bool a, const sc_bit& b );
    inline const sc_bit b_or( char a, const sc_bit& b );

    // RECOMMENDED

    inline void b_or( sc_bit& r, const sc_bit& a, const sc_bit& b );
    inline void b_or( sc_bit& r, const sc_bit& a, int b );
    inline void b_or( sc_bit& r, const sc_bit& a, bool b );
    inline void b_or( sc_bit& r, const sc_bit& a, char b );
    inline void b_or( sc_bit& r, int a, const sc_bit& b );
    inline void b_or( sc_bit& r, bool a, const sc_bit& b );
    inline void b_or( sc_bit& r, char a, const sc_bit& b );


    // bitwise and

    // MANDATORY

    inline const sc_bit operator & ( const sc_bit& a, const sc_bit& b );
    inline const sc_bit operator & ( const sc_bit& a, int b );
    inline const sc_bit operator & ( const sc_bit& a, bool b );
    inline const sc_bit operator & ( const sc_bit& a, char b );
    inline const sc_bit operator & ( int a, const sc_bit& b );
    inline const sc_bit operator & ( bool a, const sc_bit& b );
    inline const sc_bit operator & ( char a, const sc_bit& b );
    // OPTIONAL

    inline const sc_bit b_and( const sc_bit& a, const sc_bit& b );
    inline const sc_bit b_and( const sc_bit& a, int b );
    inline const sc_bit b_and( const sc_bit& a, bool b );
    inline const sc_bit b_and( const sc_bit& a, char b );
    inline const sc_bit b_and( int a, const sc_bit& b );
    inline const sc_bit b_and( bool a, const sc_bit& b );
    inline const sc_bit b_and( char a, const sc_bit& b );

    // RECOMMENDED

    inline void b_and( sc_bit& r, const sc_bit& a, const sc_bit& b );
    inline void b_and( sc_bit& r, const sc_bit& a, int b );
    inline void b_and( sc_bit& r, const sc_bit& a, bool b );
    inline void b_and( sc_bit& r, const sc_bit& a, char b );
    inline void b_and( sc_bit& r, int a, const sc_bit& b );
    inline void b_and( sc_bit& r, bool a, const sc_bit& b );
    inline void b_and( sc_bit& r, char a, const sc_bit& b );


    // bitwise exor

    // MANDATORY

    inline const sc_bit operator ^ ( const sc_bit& a, const sc_bit& b );
    inline const sc_bit operator ^ ( const sc_bit& a, int b );
    inline const sc_bit operator ^ ( const sc_bit& a, bool b );
    inline const sc_bit operator ^ ( const sc_bit& a, char b );
    inline const sc_bit operator ^ ( int a, const sc_bit& b );
    inline const sc_bit operator ^ ( bool a, const sc_bit& b );
    inline const sc_bit operator ^ ( char a, const sc_bit& b );

    // OPTIONAL

    inline const sc_bit b_xor( const sc_bit& a, const sc_bit& b );
    inline const sc_bit b_xor( const sc_bit& a, int b );
    inline const sc_bit b_xor( const sc_bit& a, bool b );
    inline const sc_bit b_xor( const sc_bit& a, char b );
    inline const sc_bit b_xor( int a, const sc_bit& b );
    inline const sc_bit b_xor( bool a, const sc_bit& b );
    inline const sc_bit b_xor( char a, const sc_bit& b );

    // RECOMMENDED

    inline void b_xor( sc_bit& r, const sc_bit& a, const sc_bit& b );
    inline void b_xor( sc_bit& r, const sc_bit& a, int b );
    inline void b_xor( sc_bit& r, const sc_bit& a, bool b );
    inline void b_xor( sc_bit& r, const sc_bit& a, char b );
    inline void b_xor( sc_bit& r, int a, const sc_bit& b );
    inline void b_xor( sc_bit& r, bool a, const sc_bit& b );
    inline void b_xor( sc_bit& r, char a, const sc_bit& b );


// ----------------------------------------------------------------------------
//  CLASS : sc_bit
//
//  Bit class.
//  Note: VSIA compatibility indicated.
// ----------------------------------------------------------------------------

class sc_bit
{
    // support methods

    static void invalid_value( char );
    static void invalid_value( int );

    static bool to_value( char c )
	{
	    if( c != '0' && c != '1' ) {
		invalid_value( c );
	    }
	    return ( c == '0' ? false : true );
	}

    static bool to_value( int i )
	{
	    if( i != 0 && i != 1 ) {
		invalid_value( i );
	    }
	    return ( i == 0 ? false : true );
	}

public:

    // constructors
    // MANDATORY

    sc_bit()
	: m_val( false )
	{
	    sc_deprecated_sc_bit();
	}

    explicit sc_bit( uint64 a )
	: m_val( a )
	{
	    sc_deprecated_sc_bit();
	}

    explicit sc_bit( bool a )
	: m_val( a )
	{
	    sc_deprecated_sc_bit();
	}

    explicit sc_bit( int a )
	: m_val( to_value( a ) )
	{
	    sc_deprecated_sc_bit();
	}

    explicit sc_bit( char a )
	: m_val( to_value( a ) )
	{
	    sc_deprecated_sc_bit();
	}

    explicit sc_bit( const sc_logic& a );  // non-VSIA


    // copy constructor
    // MANDATORY

    sc_bit( const sc_bit& a )
	: m_val( a.m_val )
	{}


    // destructor
    // MANDATORY

    ~sc_bit()
	{}


    // assignment operators
    // MANDATORY

    sc_bit& operator = ( const sc_bit& b )
	{ m_val = b.m_val; return *this; }

    sc_bit& operator = ( int64 b )
	{ return ( *this = sc_bit( (int)b ) ); }

    sc_bit& operator = ( uint64 b )
	{ return ( *this = sc_bit( (int)b ) ); }

    sc_bit& operator = ( long b )
	{ return ( *this = sc_bit( (int)b ) ); }

    sc_bit& operator = ( unsigned long b )
	{ return ( *this = sc_bit( (int)b ) ); }

    sc_bit& operator = ( int b )
	{ return ( *this = sc_bit( b ) ); }

    sc_bit& operator = ( bool b )
	{ return ( *this = sc_bit( b ) ); }

    sc_bit& operator = ( char b )
	{ return ( *this = sc_bit( b ) ); }

    sc_bit& operator = ( const sc_logic& b );  // non-VSIA


    // bitwise assignment operators

    sc_bit& operator &= ( const sc_bit& b )
	{ m_val = ( m_val && b.m_val ); return *this; }

    sc_bit& operator &= ( int b )
	{ return ( *this &= sc_bit( b ) ); }

    sc_bit& operator &= ( bool b )
	{ return ( *this &= sc_bit( b ) ); }

    sc_bit& operator &= ( char b )
	{ return ( *this &= sc_bit( b ) ); }


    sc_bit& operator |= ( const sc_bit& b )
	{ m_val = ( m_val || b.m_val ); return *this; }

    sc_bit& operator |= ( int b )
	{ return ( *this |= sc_bit( b ) ); }

    sc_bit& operator |= ( bool b )
	{ return ( *this |= sc_bit( b ) ); }

    sc_bit& operator |= ( char b )
	{ return ( *this |= sc_bit( b ) ); }


    sc_bit& operator ^= ( const sc_bit& b )
	{ m_val = ( m_val != b.m_val ); return *this; }

    sc_bit& operator ^= ( int b )
	{ return ( *this ^= sc_bit( b ) ); }

    sc_bit& operator ^= ( bool b )
	{ return ( *this ^= sc_bit( b ) ); }

    sc_bit& operator ^= ( char b )
	{ return ( *this ^= sc_bit( b ) ); }


    // conversions
    // MANDATORY

    // implicit conversion to bool

    operator bool () const
	{ return m_val; }

    bool operator ! () const  // non-VSIA
	{ return ! m_val; }


    // explicit conversions

    bool to_bool() const  // non-VSIA
	{ return m_val; }

    char to_char() const
	{ return ( m_val ? '1' : '0' ); }


    // relational operators and functions

    // MANDATORY

    friend bool operator == ( const sc_bit& a, const sc_bit& b )
        { return ( a.m_val == b.m_val ); }

    friend bool operator == ( const sc_bit& a, int b )
        { return ( a == sc_bit( b ) ); }

    friend bool operator == ( const sc_bit& a, bool b )
        { return ( a == sc_bit( b ) ); }

    friend bool operator == ( const sc_bit& a, char b )
        { return ( a == sc_bit( b ) ); }

    friend bool operator == ( int a, const sc_bit& b )
	{ return ( sc_bit( a ) == b ); }

    friend bool operator == ( bool a, const sc_bit& b )
	{ return ( sc_bit( a ) == b ); }

    friend bool operator == ( char a, const sc_bit& b )
	{ return ( sc_bit( a ) == b ); }

    // OPTIONAL

    friend bool equal( const sc_bit& a, const sc_bit& b )
        { return ( a == b ); }

    friend bool equal( const sc_bit& a, int b )
        { return ( a == b ); }

    friend bool equal( const sc_bit& a, bool b )
        { return ( a == b ); }

    friend bool equal( const sc_bit& a, char b )
        { return ( a == b ); }

    friend bool equal( int a, const sc_bit& b )
        { return ( a == b ); }

    friend bool equal( bool a, const sc_bit& b )
        { return ( a == b ); }

    friend bool equal( char a, const sc_bit& b )
        { return ( a == b ); }

    // MANDATORY

    friend bool operator != ( const sc_bit& a, const sc_bit& b )
        { return ( a.m_val != b.m_val ); }

    friend bool operator != ( const sc_bit& a, int b )
        { return ( a != sc_bit( b ) ); }

    friend bool operator != ( const sc_bit& a, bool b )
        { return ( a != sc_bit( b ) ); }

    friend bool operator != ( const sc_bit& a, char b )
        { return ( a != sc_bit( b ) ); }

    friend bool operator != ( int a, const sc_bit& b )
        { return ( sc_bit( a ) != b ); }

    friend bool operator != ( bool a, const sc_bit& b )
        { return ( sc_bit( a ) != b ); }

    friend bool operator != ( char a, const sc_bit& b )
        { return ( sc_bit( a ) != b ); }

    // OPTIONAL

    friend bool not_equal( const sc_bit& a, const sc_bit& b )
        { return ( a != b ); }

    friend bool not_equal( const sc_bit& a, int b )
        { return ( a != b ); }

    friend bool not_equal( const sc_bit& a, bool b )
        { return ( a != b ); }

    friend bool not_equal( const sc_bit& a, char b )
        { return ( a != b ); }

    friend bool not_equal( int a, const sc_bit& b )
        { return ( a != b ); }

    friend bool not_equal( bool a, const sc_bit& b )
        { return ( a != b ); }

    friend bool not_equal( char a, const sc_bit& b )
        { return ( a != b ); }


    // bitwise operators and functions

    // bitwise complement

    // MANDATORY

    friend const sc_bit operator ~ ( const sc_bit& a )
        { return sc_bit( ! a.m_val ); }

    // RECOMMENDED

    sc_bit& b_not()
        { m_val = ( ! m_val ); return *this; }

    // OPTIONAL

    friend const sc_bit b_not( const sc_bit& a )
        { return ( ~ a ); }

    // RECOMMENDED

    friend void b_not( sc_bit& r, const sc_bit& a )
        { r = ( ~ a ); }


    // bitwise or

    // MANDATORY

    friend const sc_bit operator | ( const sc_bit& a, const sc_bit& b )
        { return sc_bit( a.m_val || b.m_val ); }

    friend const sc_bit operator | ( const sc_bit& a, int b )
        { return ( a | sc_bit( b ) ); }

    friend const sc_bit operator | ( const sc_bit& a, bool b )
        { return ( a | sc_bit( b ) ); }

    friend const sc_bit operator | ( const sc_bit& a, char b )
        { return ( a | sc_bit( b ) ); }

    friend const sc_bit operator | ( int a, const sc_bit& b )
	{ return ( sc_bit( a ) | b ); }

    friend const sc_bit operator | ( bool a, const sc_bit& b )
	{ return ( sc_bit( a ) | b ); }

    friend const sc_bit operator | ( char a, const sc_bit& b )
	{ return ( sc_bit( a ) | b ); }

    // OPTIONAL

    friend const sc_bit b_or( const sc_bit& a, const sc_bit& b )
        { return ( a | b ); }

    friend const sc_bit b_or( const sc_bit& a, int b )
        { return ( a | b ); }

    friend const sc_bit b_or( const sc_bit& a, bool b )
        { return ( a | b ); }

    friend const sc_bit b_or( const sc_bit& a, char b )
        { return ( a | b ); }

    friend const sc_bit b_or( int a, const sc_bit& b )
        { return ( a | b ); }

    friend const sc_bit b_or( bool a, const sc_bit& b )
        { return ( a | b ); }

    friend const sc_bit b_or( char a, const sc_bit& b )
        { return ( a | b ); }

    // RECOMMENDED

    friend void b_or( sc_bit& r, const sc_bit& a, const sc_bit& b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, const sc_bit& a, int b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, const sc_bit& a, bool b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, const sc_bit& a, char b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, int a, const sc_bit& b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, bool a, const sc_bit& b )
        { r = ( a | b ); }

    friend void b_or( sc_bit& r, char a, const sc_bit& b )
        { r = ( a | b ); }


    // bitwise and

    // MANDATORY

    friend const sc_bit operator & ( const sc_bit& a, const sc_bit& b )
	{ return sc_bit( a.m_val && b.m_val ); }

    friend const sc_bit operator & ( const sc_bit& a, int b )
	{ return ( a & sc_bit( b ) ); }

    friend const sc_bit operator & ( const sc_bit& a, bool b )
	{ return ( a & sc_bit( b ) ); }

    friend const sc_bit operator & ( const sc_bit& a, char b )
	{ return ( a & sc_bit( b ) ); }

    friend const sc_bit operator & ( int a, const sc_bit& b )
	{ return ( sc_bit( a ) & b ); }

    friend const sc_bit operator & ( bool a, const sc_bit& b )
	{ return ( sc_bit( a ) & b ); }

    friend const sc_bit operator & ( char a, const sc_bit& b )
	{ return ( sc_bit( a ) & b ); }

    // OPTIONAL

    friend const sc_bit b_and( const sc_bit& a, const sc_bit& b )
	{ return ( a & b ); }

    friend const sc_bit b_and( const sc_bit& a, int b )
	{ return ( a & b ); }

    friend const sc_bit b_and( const sc_bit& a, bool b )
	{ return ( a & b ); }

    friend const sc_bit b_and( const sc_bit& a, char b )
	{ return ( a & b ); }

    friend const sc_bit b_and( int a, const sc_bit& b )
	{ return ( a & b ); }

    friend const sc_bit b_and( bool a, const sc_bit& b )
	{ return ( a & b ); }

    friend const sc_bit b_and( char a, const sc_bit& b )
	{ return ( a & b ); }

    // RECOMMENDED

    friend void b_and( sc_bit& r, const sc_bit& a, const sc_bit& b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, const sc_bit& a, int b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, const sc_bit& a, bool b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, const sc_bit& a, char b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, int a, const sc_bit& b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, bool a, const sc_bit& b )
	{ r = ( a & b ); }

    friend void b_and( sc_bit& r, char a, const sc_bit& b )
	{ r = ( a & b ); }


    // bitwise exor

    // MANDATORY

    friend const sc_bit operator ^ ( const sc_bit& a, const sc_bit& b )
	{ return sc_bit( a.m_val != b.m_val ); }

    friend const sc_bit operator ^ ( const sc_bit& a, int b )
	{ return ( a ^ sc_bit( b ) ); }

    friend const sc_bit operator ^ ( const sc_bit& a, bool b )
	{ return ( a ^ sc_bit( b ) ); }

    friend const sc_bit operator ^ ( const sc_bit& a, char b )
	{ return ( a ^ sc_bit( b ) ); }

    friend const sc_bit operator ^ ( int a, const sc_bit& b )
	{ return ( sc_bit( a ) ^ b ); }

    friend const sc_bit operator ^ ( bool a, const sc_bit& b )
	{ return ( sc_bit( a ) ^ b ); }

    friend const sc_bit operator ^ ( char a, const sc_bit& b )
	{ return ( sc_bit( a ) ^ b ); }

    // OPTIONAL

    friend const sc_bit b_xor( const sc_bit& a, const sc_bit& b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( const sc_bit& a, int b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( const sc_bit& a, bool b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( const sc_bit& a, char b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( int a, const sc_bit& b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( bool a, const sc_bit& b )
	{ return ( a ^ b ); }

    friend const sc_bit b_xor( char a, const sc_bit& b )
	{ return ( a ^ b ); }

    // RECOMMENDED

    friend void b_xor( sc_bit& r, const sc_bit& a, const sc_bit& b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, const sc_bit& a, int b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, const sc_bit& a, bool b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, const sc_bit& a, char b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, int a, const sc_bit& b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, bool a, const sc_bit& b )
	{ r = ( a ^ b ); }

    friend void b_xor( sc_bit& r, char a, const sc_bit& b )
	{ r = ( a ^ b ); }


    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
	{ os << to_bool(); }

    void scan( ::std::istream& = ::std::cin );

private:

    bool m_val;
};


// ----------------------------------------------------------------------------

inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_bit& a )
{
    a.print( os );
    return os;
}

inline
::std::istream&
operator >> ( ::std::istream& is, sc_bit& a )
{
    a.scan( is );
    return is;
}

} // namespace sc_dt


#endif

// Taf!
