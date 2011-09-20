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

  scfx_string.h - 

  Original Author: Robert Graulich, Synopsys, Inc.
                   Martin Janssen,  Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
// $Log: scfx_string.h,v $
// Revision 1.1.1.1  2006/12/15 20:20:04  acg
// SystemC 2.3
//
// Revision 1.2  2006/01/03 23:18:34  acg
// Changed copyright to include 2006.
//
// Revision 1.1.1.1  2005/12/19 23:16:43  acg
// First check in of SystemC 2.1 into its own archive.
//
// Revision 1.9  2005/09/15 23:02:03  acg
// Added std:: prefix to appropriate methods and types to get around
// issues with the Edison Front End.
//
// Revision 1.8  2005/06/07 17:27:02  acg
// Fixed bug in scfx_string::operator += where an array reference was used
// rather than the [] operator.  This meant that the buffer may have been
// accessed beyond its allocated storage.
//

#ifndef SCFX_STRING_H
#define SCFX_STRING_H

#include <cstdio>


namespace sc_dt
{

// classes defined in this module
class scfx_string;


// ----------------------------------------------------------------------------
//  CLASS : scfx_string
//
//  Simple string class for internal use.
// ----------------------------------------------------------------------------

class scfx_string
{
    void resize( std::size_t );

public:

    scfx_string();

    ~scfx_string();

    int length() const;

    void clear();

    char& operator [] ( int );

    void append( int );
    void discard( int );
    void remove( int );

    void operator += ( char );
    void operator += ( const char* );

    operator const char* ();

private:

    std::size_t m_len;
    std::size_t m_alloc;
    char*  m_buffer;
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
scfx_string::resize( std::size_t i )
{
    do {
	m_alloc *= 2;
    } while( i >= m_alloc );

    char* temp = new char[m_alloc];

    for( int j = 0; j < (int) m_len; ++ j ) {
	temp[j] = m_buffer[j];
    }
    temp[m_len] = 0;

    delete [] m_buffer;
    m_buffer = temp;
}


inline
scfx_string::scfx_string()
: m_len( 0 ), m_alloc( BUFSIZ ), m_buffer( new char[m_alloc] )
{
    m_buffer[m_len] = 0;
}


inline
scfx_string::~scfx_string()
{
    delete [] m_buffer;
}


inline
int
scfx_string::length() const
{
    return m_len;
}


inline
void
scfx_string::clear()
{
    m_len = 0;
    m_buffer[m_len] = 0;
}


inline
char&
scfx_string::operator [] ( int i )
{
    if( i >= (int) m_alloc ) {
	resize( i );
    }
    return m_buffer[i];
}


inline
void
scfx_string::append( int n )
{
    m_len += n;
    m_buffer[m_len] = 0;
}

inline
void
scfx_string::discard( int n )
{
    m_len -= n;
    m_buffer[m_len] = 0;
}

inline
void
scfx_string::remove( int i )
{
    for( int j = i + 1; j < (int) m_len; ++ j )
	m_buffer[j - 1] = m_buffer[j];
    -- m_len;
    m_buffer[m_len] = 0;
}


inline
void
scfx_string::operator += ( char c )
{
    this->operator [] ( m_len ) = c;
    m_len ++;
    this->operator [] ( m_len ) = 0;
}

inline
void
scfx_string::operator += ( const char* s )
{
    while( *s )
	(*this) += *s ++;
}


inline
scfx_string::operator const char*()
{
    m_buffer[m_len] = 0;
    return m_buffer;
}

} // namespace sc_dt


#endif

// Taf!
