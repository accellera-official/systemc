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

  sc_concatref.h -- Concatenation support.
 
  Original Author: Andy Goodrich, Forte Design, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  Andy Goodrich, Forte Design Systems, 17 Nov 2002
  Creation of sc_concatref class by merging the capabilities of 
  sc_int_concref, sc_int_concref, sc_uint_concref, sc_uint_concref, 
  and implementing the capabilities of sc_signed_concref, sc_signed_concref, 
  sc_unsigned_concref, and sc_unsigned_concref. The resultant class allows 
  mixed mode concatenations on the left and right sides of an assignment.

 *****************************************************************************/

// $Log: sc_concatref.h,v $
// Revision 1.1.1.1  2006/12/15 20:31:36  acg
// SystemC 2.2
//
// Revision 1.3  2006/01/13 18:54:01  acg
// Andy Goodrich: added $Log command so that CVS comments are reproduced in
// the source.
//

#ifndef SC_CONCATREF_H
#define SC_CONCATREF_H

#include "sysc/kernel/sc_object.h"
#include "sysc/datatypes/misc/sc_value_base.h"
#include "sysc/utils/sc_temporary.h"
#include "sysc/utils/sc_string.h"
#include "sysc/datatypes/bit/sc_bv.h"
#include "sysc/datatypes/bit/sc_lv.h"
#include "sysc/datatypes/int/sc_int_base.h"
#include "sysc/datatypes/int/sc_uint_base.h"
#include "sysc/datatypes/int/sc_signed.h"
#include "sysc/datatypes/int/sc_unsigned.h"

namespace sc_core {
    extern sc_byte_heap sc_temp_heap; // Temporary storage.
} // namespace sc_core

namespace sc_dt
{

// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_concatref
//
//  Proxy class for sized bit concatenation.
// ----------------------------------------------------------------------------

class sc_concatref : public sc_generic_base<sc_concatref>, public sc_value_base
{
public:
    friend class sc_core::sc_vpool<sc_concatref>;

    inline void initialize( 
        sc_value_base& left, sc_value_base& right )
        {    
            bool left_xz;   // True if x's and/or z's found in left.
            bool right_xz;  // True if x's and/or z's found in right.
            
            m_left_p = (sc_value_base*)&left;
            m_right_p = (sc_value_base*)&right;
            m_len_r = right.concat_length(&right_xz);
            m_len = left.concat_length(&left_xz) + m_len_r;
            m_flags = ( left_xz || right_xz ) ? cf_xz_present : cf_none;
        }


    inline void initialize( 
        const sc_value_base& left, const sc_value_base& right )
        {    
            bool left_xz;   // True if x's and/or z's found in left.
            bool right_xz;  // True if x's and/or z's found in right.

            m_left_p = (sc_value_base*)&left;
            m_right_p = (sc_value_base*)&right;
            m_len_r = right.concat_length(&right_xz);
            m_len = left.concat_length(&left_xz) + m_len_r;
            m_flags = ( left_xz || right_xz ) ? cf_xz_present : cf_none;
        }

    // destructor

    virtual ~sc_concatref()
    {}


    // capacity

    unsigned int length() const
        { return m_len; }

#ifdef SC_DT_DEPRECATED
    int bitwidth() const
        { return length(); }
#endif

    // concatenation

    virtual int concat_length( bool* xz_present_p ) const
    { 
        if ( xz_present_p ) 
            *xz_present_p = m_flags & cf_xz_present ? true : false;
        return m_len; 
    }

    virtual void concat_clear_data( bool to_ones )
    { 
        m_left_p->concat_clear_data(to_ones); 
        m_right_p->concat_clear_data(to_ones); 
    }

    virtual bool concat_get_ctrl( sc_digit* dst_p, int low_i ) const
    {
        bool rnz = m_right_p->concat_get_ctrl( dst_p, low_i );
        bool lnz = m_left_p->concat_get_ctrl( dst_p, low_i+m_len_r );
        return rnz || lnz;
    }

    virtual bool concat_get_data( sc_digit* dst_p, int low_i ) const
    {
        bool rnz = m_right_p->concat_get_data( dst_p, low_i );
        bool lnz = m_left_p->concat_get_data( dst_p, low_i+m_len_r );
        return rnz || lnz;
    }

    virtual uint64 concat_get_uint64() const
    {
        if ( m_len_r >= 64 )
            return m_right_p->concat_get_uint64();
        else
        {
            return (m_left_p->concat_get_uint64() << m_len_r) | 
                m_right_p->concat_get_uint64();
        }
    }

    virtual void concat_set( int64 src, int low_i ) 
    { 
        m_right_p->concat_set( src, low_i );
        m_left_p->concat_set( src, low_i+m_len_r);
    }

    virtual void concat_set( const sc_signed& src, int low_i ) 
    {
        m_right_p->concat_set( src, low_i );
        m_left_p->concat_set( src, low_i+m_len_r);
    }

    virtual void concat_set( const sc_unsigned& src, int low_i ) 
    { 
        m_right_p->concat_set( src, low_i );
        m_left_p->concat_set( src, low_i+m_len_r);
    }

    virtual void concat_set( uint64 src, int low_i )
    { 
        m_right_p->concat_set( src, low_i );
        m_left_p->concat_set( src, low_i+m_len_r);
    }


    // explicit conversions

    uint64 to_uint64() const 
        {
            uint64 mask;
            uint64 result;

            result = m_right_p->concat_get_uint64();
            if ( m_len_r < 64 )
            {
                mask = ~0;
                result = (m_left_p->concat_get_uint64() << m_len_r) | 
                            (result & ~(mask << m_len_r));
            }
            if ( m_len < 64 )
            {
                mask = ~0;
                result = result & ~(mask << m_len);
            }
            return result;
        }

    const sc_unsigned& value() const
        {
            bool           left_non_zero;
            sc_unsigned*   result_p = sc_unsigned::m_pool.allocate();
            bool           right_non_zero;

            result_p->nbits = result_p->num_bits(m_len);
            result_p->ndigits = (result_p->nbits+BITS_PER_DIGIT-1) / 
                BITS_PER_DIGIT;
            result_p->digit = (sc_digit*)sc_core::sc_temp_heap.allocate( 
                sizeof(sc_digit)*result_p->ndigits );
            right_non_zero = m_right_p->concat_get_data( result_p->digit, 0 );
            left_non_zero = m_left_p->concat_get_data(result_p->digit, m_len_r); 
            if ( left_non_zero || right_non_zero ) 
                result_p->sgn = SC_POS;
            else
                result_p->sgn = SC_ZERO;
            return *result_p;
        }

    int64 to_int64() const
        { 
            return (int64)to_uint64();
        }
    int to_int() const
        { return (int)to_int64(); }
    unsigned int  to_uint() const
        { return (unsigned int)to_uint64(); }
    long to_long() const
        { return (long)to_int64(); }
    unsigned long to_ulong() const
        { return (unsigned long)to_uint64(); }
    double to_double() const
        { return value().to_double(); }

    void to_sc_signed( sc_signed& target ) const
        { target = value(); }

    void to_sc_unsigned( sc_unsigned& target ) const
        { target = value(); }

    // implicit conversions:

    operator  uint64 () const 
        { return to_uint64(); }

    operator const sc_unsigned& () const
        { return value(); }

    // unary operators:

    sc_unsigned operator + () const
        { return value(); } 

    sc_signed operator - () const
        { return -value(); } 

    sc_unsigned operator ~ () const
        { return ~value(); } 

    // explicit conversion to character string

    const std::string to_string( sc_numrep numrep = SC_DEC ) const
        { return value().to_string(numrep); }

    const std::string to_string( sc_numrep numrep, bool w_prefix ) const
        { return value().to_string(numrep,w_prefix); }



    // assignments

    inline const sc_concatref& operator = ( int v )
    {
        m_right_p->concat_set((int64)v, 0);
        m_left_p->concat_set((int64)v, m_len_r);
        return *this;
    }

    inline const sc_concatref& operator = ( long v )
    {
        m_right_p->concat_set((int64)v, 0);
        m_left_p->concat_set((int64)v, m_len_r);
        return *this;
    }

    inline const sc_concatref& operator = ( int64 v )
    {
        m_right_p->concat_set(v, 0);
        m_left_p->concat_set(v, m_len_r);
        return *this;
    }

    inline const sc_concatref& operator = ( unsigned int v )
    {
        m_right_p->concat_set((uint64)v, 0);
        m_left_p->concat_set((uint64)v, m_len_r);
        return *this;
    }

    inline const sc_concatref& operator = ( unsigned long v )
    {
        m_right_p->concat_set((uint64)v, 0);
        m_left_p->concat_set((uint64)v, m_len_r);
        return *this;
    }

    inline const sc_concatref& operator = ( uint64 v )
    {
        m_right_p->concat_set(v, 0);
        m_left_p->concat_set(v, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const sc_concatref& v )
    {
        sc_unsigned temp(v.length());
        temp = v.value();
        m_right_p->concat_set(temp, 0);
        m_left_p->concat_set(temp, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const sc_signed& v )
    {
        m_right_p->concat_set(v, 0);
        m_left_p->concat_set(v, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const sc_unsigned& v )
    {
        m_right_p->concat_set(v, 0);
        m_left_p->concat_set(v, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const char* v_p )
    {
        sc_unsigned v(strlen(v_p));
        v = v_p;
        m_right_p->concat_set(v, 0);
        m_left_p->concat_set(v, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const sc_bv_base& v )
    {
        sc_unsigned temp(v.length());
        temp = v;
        m_right_p->concat_set(temp, 0);
        m_left_p->concat_set(temp, m_len_r);
        return *this;
    }

    const sc_concatref& operator = ( const sc_lv_base& v )
    {
        sc_unsigned data(v.length());
        data = v;
        m_right_p->concat_set(data, 0);
        m_left_p->concat_set(data, m_len_r);
        return *this;
    }


    // reduce methods

    bool and_reduce() const
        { return value().and_reduce(); }

    bool nand_reduce() const
        { return value().nand_reduce(); }

    bool or_reduce() const
        { return value().or_reduce(); }

    bool nor_reduce() const
        { return value().nor_reduce(); }

    bool xor_reduce() const
        { return value().xor_reduce(); }

    bool xnor_reduce() const
        { return value().xnor_reduce(); }

    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
        { os << this->value(); }

    void scan( ::std::istream& is ) 
    { 
        std::string s; 
        is >> s; 
        *this = s.c_str(); 
    } 

public:
    static sc_core::sc_vpool<sc_concatref> m_pool; // Pool of temporary objects.

public:
    enum concat_flags {
        cf_none = 0,        // Normal value. 
        cf_xz_present = 1   // X and/or Z values present.
    };

protected:
    sc_value_base*  m_left_p;    // Left hand operand of concatenation.
    sc_value_base*  m_right_p;   // Right hand operand of concatenation.
    int             m_len;       // Length of concatenation.
    int             m_len_r;     // Length of m_rightt_p.
    concat_flags    m_flags;     // Value is read only.

private:
    sc_concatref(const sc_concatref&);
    sc_concatref() { }
};


// functional notation for the reduce methods

inline
bool
and_reduce( const sc_concatref& a )
{
    return a.and_reduce();
}

inline
bool
nand_reduce( const sc_concatref& a )
{
    return a.nand_reduce();
}

inline
bool
or_reduce( const sc_concatref& a )
{
    return a.or_reduce();
}

inline
bool
nor_reduce( const sc_concatref& a )
{
    return a.nor_reduce();
}

inline
bool
xor_reduce( const sc_concatref& a )
{
    return a.xor_reduce();
}

inline
bool
xnor_reduce( const sc_concatref& a )
{
    return a.xnor_reduce();
}


// SHIFT OPERATORS FOR sc_concatref OBJECT INSTANCES:
//
// Because sc_concatref has implicit casts to both uint64 and sc_unsigned
// it is necessary to disambiguate the use of the shift operators. We do
// this in favor of sc_unsigned so that precision is not lost. To get an
// integer-based result use a cast to uint64 before performing the shift.

inline const sc_unsigned operator << (const sc_concatref& target, uint64 shift)
{
    return target.value() << (int)shift;
}

inline const sc_unsigned operator << (const sc_concatref& target, int64 shift)
{
    return target.value() << (int)shift;
}

inline const sc_unsigned operator << ( 
    const sc_concatref& target, unsigned long shift )
{
    return target.value() << (int)shift;
}

inline const sc_unsigned operator << ( 
    const sc_concatref& target, unsigned int shift )
{
    return target.value() << (int)shift;
}

inline const sc_unsigned operator << ( const sc_concatref& target, long shift )
{
    return target.value() << (int)shift;
}

inline const sc_unsigned operator >> (const sc_concatref& target, uint64 shift)
{
    return target.value() >> (int)shift;
}

inline const sc_unsigned operator >> (const sc_concatref& target, int64 shift)
{
    return target.value() >> (int)shift;
}

inline const sc_unsigned operator >> ( 
    const sc_concatref& target, unsigned long shift )
{
    return target.value() >> (int)shift;
}

inline const sc_unsigned operator >> ( 
    const sc_concatref& target, unsigned int shift )
{
    return target.value() >> (int)shift;
}

inline const sc_unsigned operator >> ( const sc_concatref& target, long shift )
{
    return target.value() >> (int)shift;
}


// STREAM OPERATORS FOR sc_concatref OBJECT INSTANCES:

inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_concatref& v )
{ 
    return os << v.value();
}

inline
::std::istream&
operator >> ( ::std::istream& is, sc_concatref& a )
{
    sc_unsigned temp(a.concat_length(0));   
    temp.scan( is );
    a = temp;
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS TEMPLATE : sc_concat_bool
//
//  Proxy class for read-only boolean values in concatenations.
// ----------------------------------------------------------------------------

class sc_concat_bool : public sc_value_base
{
  protected:
    static sc_core::sc_vpool<sc_concat_bool> m_pool;  // Temporaries pool.
    bool                                     m_value; // Value for this obj.

  public:

    // destructor:

    virtual ~sc_concat_bool() 
        { }

    // allocation of temporary object:

    static inline sc_concat_bool* allocate( bool v )
    {
        sc_concat_bool* result_p = m_pool.allocate();
        result_p->m_value = v;
        return result_p;
    }

    // concatenation:

    virtual int concat_length( bool* xz_present_p ) const
    { 
        if ( xz_present_p ) *xz_present_p = false;
        return 1; 
    }

    virtual bool concat_get_ctrl( sc_digit* dst_p, int low_i ) const
    {
        int bit = 1 << (low_i % BITS_PER_DIGIT); 
        int word_i = low_i / BITS_PER_DIGIT;
        dst_p[word_i] &= ~bit;
        return false;
    }

    virtual bool concat_get_data( sc_digit* dst_p, int low_i ) const
    {
        int bit = 1 << (low_i % BITS_PER_DIGIT); 
        int word_i = low_i / BITS_PER_DIGIT;
        if ( m_value )
            dst_p[word_i] |= bit;
        else 
            dst_p[word_i] &= ~bit;
        return m_value;
    }

    virtual uint64 concat_get_uint64() const
    {
        return m_value ? 1 : 0;
    }
};


// ----------------------------------------------------------------------------
// ARITHMETIC AND LOGIC OPERATORS FOR sc_concatref
// ----------------------------------------------------------------------------

#define SC_CONCAT_OP_TYPE(RESULT,OP,OTHER_TYPE) \
    inline RESULT operator OP ( const sc_concatref& a, OTHER_TYPE b ) \
    { \
        return a.value() OP b; \
    } \
    inline RESULT operator OP ( OTHER_TYPE a, const sc_concatref& b ) \
    { \
        return a OP b.value(); \
    } 


#define SC_CONCAT_OP(RESULT,OP) \
    inline RESULT operator OP ( const sc_concatref& a, const sc_concatref& b ) \
    { \
        return a.value() OP b.value(); \
    }  \
    SC_CONCAT_OP_TYPE(const sc_signed,OP,int) \
    SC_CONCAT_OP_TYPE(const sc_signed,OP,long) \
    SC_CONCAT_OP_TYPE(const sc_signed,OP,int64) \
    SC_CONCAT_OP_TYPE(RESULT,OP,unsigned int) \
    SC_CONCAT_OP_TYPE(RESULT,OP,unsigned long) \
    SC_CONCAT_OP_TYPE(RESULT,OP,uint64) \
    SC_CONCAT_OP_TYPE(const sc_signed,OP,const sc_int_base&) \
    SC_CONCAT_OP_TYPE(RESULT,OP,const sc_uint_base&) \
    SC_CONCAT_OP_TYPE(const sc_signed,OP,const sc_signed&) \
    SC_CONCAT_OP_TYPE(RESULT,OP,const sc_unsigned&) \
    inline RESULT operator OP ( const sc_concatref& a, bool b ) \
    { \
        return a.value() OP (int)b; \
    } \
    inline RESULT operator OP ( bool a, const sc_concatref& b ) \
    { \
        return (int)a OP b.value(); \
    } 

#define SC_CONCAT_BOOL_OP(OP) \
    inline bool operator OP ( const sc_concatref& a, const sc_concatref& b ) \
    { \
        return a.value() OP b.value(); \
    }  \
    SC_CONCAT_OP_TYPE(bool,OP,int) \
    SC_CONCAT_OP_TYPE(bool,OP,long) \
    SC_CONCAT_OP_TYPE(bool,OP,int64) \
    SC_CONCAT_OP_TYPE(bool,OP,unsigned int) \
    SC_CONCAT_OP_TYPE(bool,OP,unsigned long) \
    SC_CONCAT_OP_TYPE(bool,OP,uint64) \
    SC_CONCAT_OP_TYPE(bool,OP,const sc_int_base&) \
    SC_CONCAT_OP_TYPE(bool,OP,const sc_uint_base&) \
    SC_CONCAT_OP_TYPE(bool,OP,const sc_signed&) \
    SC_CONCAT_OP_TYPE(bool,OP,const sc_unsigned&) \
    inline bool operator OP ( const sc_concatref& a, bool b ) \
    { \
        return a.value() OP (int)b; \
    } \
    inline bool operator OP ( bool a, const sc_concatref& b ) \
    { \
        return (int)a OP b.value(); \
    } 

SC_CONCAT_OP(const sc_unsigned,+)
SC_CONCAT_OP(const sc_signed,-)
SC_CONCAT_OP(const sc_unsigned,*)
SC_CONCAT_OP(const sc_unsigned,/)
SC_CONCAT_OP(const sc_unsigned,%)
SC_CONCAT_OP(const sc_unsigned,&)
SC_CONCAT_OP(const sc_unsigned,|)
SC_CONCAT_OP(const sc_unsigned,^)
SC_CONCAT_BOOL_OP(==)
SC_CONCAT_BOOL_OP(<=)
SC_CONCAT_BOOL_OP(>=)
SC_CONCAT_BOOL_OP(!=)
SC_CONCAT_BOOL_OP(>)
SC_CONCAT_BOOL_OP(<)

#undef SC_CONCAT_OP
#undef SC_CONCAT_OP_TYPE


// ----------------------------------------------------------------------------
// CONCATENATION FUNCTION AND OPERATOR FOR STANDARD SYSTEM C DATA TYPES:
// ----------------------------------------------------------------------------

inline sc_dt::sc_concatref& concat(
    sc_dt::sc_value_base& a, sc_dt::sc_value_base& b)
{
    sc_dt::sc_concatref* result_p;     // Proxy for the concatenation.

    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, b );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& concat(
    const sc_dt::sc_value_base& a, const sc_dt::sc_value_base& b)
{
    sc_dt::sc_concatref* result_p;     // Proxy for the concatenation.

    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, b );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& concat(const sc_dt::sc_value_base& a, bool b)
{
    const sc_dt::sc_concat_bool* b_p;        // Proxy for boolean value.
    sc_dt::sc_concatref*         result_p;   // Proxy for the concatenation.

    b_p = sc_dt::sc_concat_bool::allocate(b);
    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, *b_p );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& concat(bool a, const sc_dt::sc_value_base& b)
{
    const sc_dt::sc_concat_bool* a_p;        // Proxy for boolean value.
    sc_dt::sc_concatref*         result_p;   // Proxy for the concatenation.

    a_p = sc_dt::sc_concat_bool::allocate(a);
    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( *a_p, b );
    return *result_p;
}

inline sc_dt::sc_concatref& operator , (
    sc_dt::sc_value_base& a, sc_dt::sc_value_base& b)
{
    sc_dt::sc_concatref* result_p;     // Proxy for the concatenation.

    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, b );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& operator , (
    const sc_dt::sc_value_base& a, const sc_dt::sc_value_base& b)
{
    sc_dt::sc_concatref* result_p;     // Proxy for the concatenation.

    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, b );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& operator , (const sc_dt::sc_value_base& a, bool b)
{
    const sc_dt::sc_concat_bool* b_p;      // Proxy for boolean value.
    sc_dt::sc_concatref*         result_p; // Proxy for the concatenation.

    b_p = sc_dt::sc_concat_bool::allocate(b);
    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( a, *b_p );
    return *result_p;
}

inline 
const
sc_dt::sc_concatref& operator , (bool a, const sc_dt::sc_value_base& b)
{
    const sc_dt::sc_concat_bool* a_p;      // Proxy for boolean value.
    sc_dt::sc_concatref*         result_p; // Proxy for the concatenation.

    a_p = sc_dt::sc_concat_bool::allocate(a);
    result_p = sc_dt::sc_concatref::m_pool.allocate();
    result_p->initialize( *a_p, b );
    return *result_p;
}

} // namespace sc_dt

#endif //  SC_CONCATREF_H

