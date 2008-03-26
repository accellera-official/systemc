/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2005 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.3 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_signed_subref.h -- Proxy class that is declared in sc_signed.h.  

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref_r
//
//  Proxy class for sc_signed part selection (r-value only).
// ----------------------------------------------------------------------------

// concatenation support

uint64 sc_signed_subref_r::concat_get_uint64() const 
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_uint64();
}


bool sc_signed_subref_r::concat_get_ctrl(sc_digit* dst_p, int low_i ) const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.concat_get_ctrl( dst_p, low_i );
}


bool sc_signed_subref_r::concat_get_data(sc_digit* dst_p, int low_i ) const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.concat_get_data( dst_p, low_i );
}


// implicit conversion to sc_signed

sc_signed_subref_r::operator sc_unsigned () const
{
    return sc_unsigned( m_obj_p, m_left, m_right );
}


// explicit conversions

int
sc_signed_subref_r::to_int() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_int();
}

unsigned int
sc_signed_subref_r::to_uint() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_uint();
}

long
sc_signed_subref_r::to_long() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_long();
}

unsigned long
sc_signed_subref_r::to_ulong() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_ulong();
}

int64
sc_signed_subref_r::to_int64() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_int64();
}

uint64
sc_signed_subref_r::to_uint64() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_uint64();
}

double
sc_signed_subref_r::to_double() const
{
    sc_unsigned a( m_obj_p, m_left, m_right );
    return a.to_double();
}


// explicit conversion to character string

const std::string
sc_signed_subref_r::to_string( sc_numrep numrep ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep );
}

const std::string
sc_signed_subref_r::to_string( sc_numrep numrep, bool w_prefix ) const
{
    sc_unsigned a( length() );
    a = *this;
    return a.to_string( numrep, w_prefix );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signed_subref
//
//  Proxy class for sc_signed part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_signed_subref_r& a )
{
    return operator = ( (sc_unsigned)( a ) );
}

const sc_signed_subref& 
sc_signed_subref::operator = ( const sc_signed_subref& v )
{
    if( this == &v ) {
	return *this;
    }
    return operator = ( (sc_unsigned)( v ) ); 
}

const sc_signed_subref& 
sc_signed_subref::operator = ( const sc_signed& v )
{
    int l = sc_min( m_left, v.nbits - 1 + m_right );
    for( int i = m_right; i <= l; ++ i ) {
	m_obj_p->set( i, v.test( i - m_right ) );
    }
    return *this;
} // #### BUG : WHAT ABOUT SIGN EXTENSION?

const sc_signed_subref& 
sc_signed_subref::operator = ( const sc_unsigned_subref_r& v )
{
    return operator = ( (sc_unsigned)( v ) );
}

const sc_signed_subref& 
sc_signed_subref::operator = ( const sc_unsigned& v )
{
    int l = sc_min( m_left, v.nbits - 1 + m_right );
    for( int i = m_right; i <= l; ++ i ) {
	m_obj_p->set( i, v.test( i - m_right ) );
    }
    return *this;
} // #### BUG: WHAT ABOUT SIGN EXTENSION AND FACT nbits IS ONE TOO HIGH?

const sc_signed_subref& 
sc_signed_subref::operator = ( unsigned long v )
{
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj_p->set( i, static_cast<bool>( v & 1 ) );
	v >>= 1;
    }
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( long v ) 
{
    unsigned long v2 = (unsigned long) v;
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj_p->set( i, static_cast<bool>( v2 & 1 ) );
	v2 >>= 1;
    }
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( uint64 v )
{
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj_p->set( i, static_cast<bool>( v & 1 ) );
	v >>= 1;
    }
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( int64 v ) 
{
    uint64 v2 = (uint64) v;
    for( int i = m_right; i <= m_left; ++ i ) {
	m_obj_p->set( i, static_cast<bool>( v2 & 1 ) );
	v2 >>= 1;
    }
    return *this;
}

const sc_signed_subref& 
sc_signed_subref::operator = ( double v )
{
    is_bad_double(v);

    int nb = m_left - m_right + 1;
    int nd = DIV_CEIL(nb);

#ifdef SC_MAX_NBITS
    sc_digit d[MAX_NDIGITS];
#else
    sc_digit *d = new sc_digit[nd];
#endif

    if (v < 0)
	v = -v;

    register int i = 0;

    while (floor(v) && (i < nd)) {
#ifndef WIN32
	d[i++] = (sc_digit) floor(remainder(v, DIGIT_RADIX));
#else
	d[i++] = (sc_digit) floor(fmod(v, DIGIT_RADIX));
#endif
	v /= DIGIT_RADIX;
    }

    vec_zero(i, nd, d);

    sc_digit val = 1;  // Bit value.
    int j = 0;   // Current digit in d.

    i = 0;  // Current bit in d.

    while (i < nb) {

	m_obj_p->set(i + m_right, (bool) (d[j] & val));

	++i;

	if (i % BITS_PER_DIGIT == 0) {
	    val = 1;
	    ++j;
	}
	else
	    val <<= 1;
    }

#ifndef SC_MAX_NBITS
    delete [] d;
#endif

    return *this;
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_int_base& a )
{
    return operator = ( (int64) a );
}

const sc_signed_subref&
sc_signed_subref::operator = ( const sc_uint_base& a )
{
    return operator = ( (uint64) a );
}

// concatenation methods


void sc_signed_subref::concat_set( int64 src, int low_i )
{
    int  i;
    int  l;
    bool sign = src < 0;

    if ( low_i < 64 )    
    {    
	src = src >> low_i;
	l = sc_min( m_left, (63-low_i) + m_right );    
	for( i = m_right; i <= l; ++ i ) {    
		m_obj_p->set( i, src & 1 );    
		src = src >> 1;    
	}    
	for ( ; i <= m_left; i++ ) m_obj_p->set(i, sign);     
    }    
    else    
    {    
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(i, sign);     
    }    
}

void sc_signed_subref::concat_set( const sc_signed& src, int low_i )
{
    int i;
    int l;
    int src_i;
    bool sign = src.test(src.nbits-1);
    l = src.nbits - (low_i+1);
    if ( l >= 0 )
    {
	l = sc_min( m_left, l + m_right );
	src_i = low_i;
	for( i = m_right; i <= l; ++ i, src_i++ ) {
	    m_obj_p->set( i, src.test( src_i ) );
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(i, sign);
    }
    else
    {    
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(i, sign);     
    }    
}

void sc_signed_subref::concat_set( const sc_unsigned& src, int low_i )
{
    int i;
    int l;
    int src_i;
    l = src.nbits - (low_i+2);
    if ( l >= 0 )
    {
	l = sc_min( m_left, l + m_right );
	src_i = low_i;
	for( i = m_right; i <= l; ++ i, src_i++ ) {
	    m_obj_p->set( i, src.test( src_i ) );
	}
	for ( ; i <= m_left; i++ ) m_obj_p->set(false);
    }
    else
    {    
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(false);     
    }    
}

void sc_signed_subref::concat_set( uint64 src, int low_i )
{
    int  i;
    int  l;

    if ( low_i < 64 )  
    {    
	src = src >> low_i;
	l = sc_min( m_left, (63-low_i) + m_right );    
	for( i = m_right; i <= l; ++ i ) {    
		m_obj_p->set( i, src & 1 );    
		src = src >> 1;    
	}    
	for ( ; i <= m_left; i++ ) m_obj_p->set(false);     
    }    
    else    
    {    
	for( i = m_right; i <= m_left; ++ i ) m_obj_p->set(false);     
    }    
}
// other methods

void
sc_signed_subref::scan( ::std::istream& is )
{
    std::string s;
    is >> s;
    *this = s.c_str();
}


// End of file
