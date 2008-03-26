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

  sc_bv_base.h -- Arbitrary size bit vector class.

  Original Author: Gene Bushuyev, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_BV_BASE_H
#define SC_BV_BASE_H


#include "sysc/datatypes/bit/sc_bit_ids.h"
#include "sysc/datatypes/bit/sc_bit_proxies.h"
#include "sysc/datatypes/bit/sc_proxy.h"
#include "sysc/datatypes/int/sc_length_param.h"


namespace sc_dt
{

// classes defined in this module
class sc_bv_base;


// ----------------------------------------------------------------------------
//  CLASS : sc_bv_base
//
//  Arbitrary size bit vector base class.
// ----------------------------------------------------------------------------

class sc_bv_base
    : public sc_proxy<sc_bv_base>
{
    friend class sc_lv_base;


    void init( int length_, bool init_value = false );

    void assign_from_string( const std::string& );
  
public:

    // typedefs

    typedef sc_proxy<sc_bv_base> base_type;


    // constructors

    explicit sc_bv_base( int length_ = sc_length_param().len() )
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( length_ ); }

    explicit sc_bv_base( bool a,
			 int length_ = sc_length_param().len() )
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( length_, a ); }

    sc_bv_base( const char* a );

    sc_bv_base( const char* a, int length_ );

    template <class X>
    sc_bv_base( const sc_proxy<X>& a )
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( a.back_cast().length() ); base_type::assign_( a ); }

    sc_bv_base( const sc_bv_base& a );

#ifdef SC_DT_DEPRECATED

    explicit sc_bv_base( const sc_unsigned& a )
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( a.length() ); base_type::assign_( a ); }

    explicit sc_bv_base( const sc_signed& a )
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( a.length() ); base_type::assign_( a ); }

    explicit sc_bv_base( const sc_uint_base& a)
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( a.length() ); base_type::assign_( a ); }

    explicit sc_bv_base( const sc_int_base& a)
	: m_len( 0 ), m_size( 0 ), m_data( 0 )
	{ init( a.length() ); base_type::assign_( a ); }

#endif


    // destructor

    virtual ~sc_bv_base()
	{ if( m_data != 0 ) delete [] m_data; }


    // assignment operators

    template <class X>
    sc_bv_base& operator = ( const sc_proxy<X>& a )
	{ assign_p_( *this, a ); return *this; }

    sc_bv_base& operator = ( const sc_bv_base& a )
	{ assign_p_( *this, a ); return *this; }

    sc_bv_base& operator = ( const char* a );

    sc_bv_base& operator = ( const bool* a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( const sc_logic* a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( const sc_unsigned& a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( const sc_signed& a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( const sc_uint_base& a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( const sc_int_base& a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( unsigned long a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( long a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( unsigned int a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( int a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( uint64 a )
	{ base_type::assign_( a ); return *this; }

    sc_bv_base& operator = ( int64 a )
	{ base_type::assign_( a ); return *this; }


#if 0

    // bitwise complement

    sc_bv_base& b_not();

    const sc_bv_base operator ~ () const
	{ sc_bv_base a( *this ); return a.b_not(); }


    // bitwise left shift

    sc_bv_base& operator <<= ( int n );

    const sc_bv_base operator << ( int n ) const
	{ sc_bv_base a( *this ); return ( a <<= n ); }


    // bitwise right shift

    sc_bv_base& operator >>= ( int n );

    const sc_bv_base operator >> ( int n ) const
	{ sc_bv_base a( *this ); return ( a >>= n ); }


    // bitwise left rotate

    sc_bv_base& lrotate( int n );


    // bitwise right rotate

    sc_bv_base& rrotate( int n );

#endif


    // common methods

    int length() const
	{ return m_len; }

    int size() const
	{ return m_size; }

    sc_logic_value_t get_bit( int i ) const;
    void set_bit( int i, sc_logic_value_t value );

    unsigned long get_word( int i ) const
	{ return m_data[i]; }

    void set_word( int i, unsigned long w )
	{ m_data[i] = w; }

    unsigned long get_cword( int i ) const
	{ return UL_ZERO; }

    void set_cword( int i, unsigned long w );

    void clean_tail();


    // other methods

    bool is_01() const
	{ return true; }

protected:

    int            m_len;  // length in bits
    int            m_size; // size of data array
    unsigned long* m_data; // data array
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

#if 0

// bitwise left rotate

inline
const sc_bv_base
lrotate( const sc_bv_base& x, int n )
{
    sc_bv_base a( x );
    return a.lrotate( n );
}


// bitwise right rotate

inline
const sc_bv_base
rrotate( const sc_bv_base& x, int n )
{
    sc_bv_base a( x );
    return a.rrotate( n );
}

#endif


// common methods

inline
sc_logic_value_t
sc_bv_base::get_bit( int i ) const
{
    int wi = i / UL_SIZE;
    int bi = i % UL_SIZE;
    return sc_logic_value_t( m_data[wi] >> bi & UL_ONE );
}

inline
void
sc_bv_base::set_bit( int i, sc_logic_value_t value )
{
    int wi = i / UL_SIZE;
    int bi = i % UL_SIZE;
    unsigned long mask = UL_ONE << bi;
    m_data[wi] |= mask; // set bit to 1
    m_data[wi] &= value << bi | ~mask;
}


inline
void
sc_bv_base::set_cword( int i, unsigned long w )
{
    if( w ) {
	SC_REPORT_WARNING( sc_core::SC_ID_SC_BV_CANNOT_CONTAIN_X_AND_Z_, 0 );
    }
}


inline
void
sc_bv_base::clean_tail()
{
    int wi = m_size - 1;
    int bi = m_len % UL_SIZE;
	if ( bi != 0 ) m_data[wi] &= ~UL_ZERO >> (UL_SIZE - bi);
}

} // namespace sc_dt


#endif
