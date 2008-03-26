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

  sc_unsigned_bitref.h -- Proxy class that is declared in sc_unsigned.h.  

  Original Author: Ali Dasdan, Synopsys, Inc.
 
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref_r
//
//  Proxy class for sc_unsigned bit selection (r-value only).
// ----------------------------------------------------------------------------

// implicit conversion to uint64

sc_unsigned_bitref_r::operator uint64 () const
{
    return m_obj_p->test( m_index );
}

bool
sc_unsigned_bitref_r::operator ! () const
{
    return ( ! m_obj_p->test( m_index ) );
}

bool
sc_unsigned_bitref_r::operator ~ () const
{
    return ( ! m_obj_p->test( m_index ) );
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref
//
//  Proxy class for sc_unsigned bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

const sc_unsigned_bitref&
sc_unsigned_bitref::operator = ( const sc_unsigned_bitref_r& b )
{
    m_obj_p->set( m_index, (bool) b );
    return *this;
}

const sc_unsigned_bitref&
sc_unsigned_bitref::operator = ( const sc_unsigned_bitref& b )
{
    m_obj_p->set( m_index, (bool) b );
    return *this;
}

const sc_unsigned_bitref&
sc_unsigned_bitref::operator = ( bool b )
{
    m_obj_p->set( m_index, b );
    return *this;
}


const sc_unsigned_bitref&
sc_unsigned_bitref::operator &= ( bool b )
{
    if( ! b ) {
	m_obj_p->clear( m_index );
    }
    return *this;
}

const sc_unsigned_bitref&
sc_unsigned_bitref::operator |= ( bool b )
{
    if( b ) {
	m_obj_p->set( m_index );
    }
    return *this;
}

const sc_unsigned_bitref&
sc_unsigned_bitref::operator ^= ( bool b )
{
    if( b ) {
	m_obj_p->invert( m_index );
    }
    return *this;
}

// #### OPTIMIZE
void sc_unsigned_bitref::concat_set(int64 src, int low_i)
{
	bool value = 1 & ((low_i < 64) ? (src >> low_i) : (src >> 63));
    m_obj_p->set(low_i, value);
}

void sc_unsigned_bitref::concat_set(const sc_signed& src, int low_i)
{
    if ( low_i < src.length() )
        m_obj_p->set(low_i, src.test(low_i));
    else
        m_obj_p->set(low_i, src<0);
}

void sc_unsigned_bitref::concat_set(const sc_unsigned& src, int low_i)
{       
    if ( low_i < src.nbits )
        m_obj_p->set(low_i, src.test(low_i));
    else
        m_obj_p->set(low_i, 0);
}

void sc_unsigned_bitref::concat_set(uint64 src, int low_i)
{
	bool value = ((low_i < 64) ? (src >> low_i)&1 : 0);
    m_obj_p->set(low_i, value);
}

// other methods

void
sc_unsigned_bitref::scan( ::std::istream& is )
{
    bool b;
    is >> b;
    *this = b;
}


// End of file
