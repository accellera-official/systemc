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

#ifndef SC_DATATYPES_ANY_VALUE_H_INCLUDED_
#define SC_DATATYPES_ANY_VALUE_H_INCLUDED_

/**
 * @file   sc_any_value.h
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

#include "sysc/utils/sc_meta.h"
#include "sysc/utils/sc_string_view.h"
#include "sysc/datatypes/int/sc_nbdefs.h" // sc_dt::(u)int64, potentially strip out
#include "sysc/datatypes/any/sc_any_value_ids.h"
#include "sysc/datatypes/any/sc_any_value_iterator.h"

#include <cstring> // std::strlen

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4231) // extern template
#endif // _MSC_VER

namespace sc_dt {

namespace sc_meta = sc_core::sc_meta;
using sc_core::sc_string_view;
using sc_core::sc_zstring_view;

// forward declarations
class sc_any_value;
class sc_any_value_cref;
class sc_any_value_ref;
class sc_any_value_list;
class sc_any_value_list_cref;
class sc_any_value_list_ref;
class sc_any_value_map;
class sc_any_value_map_cref;
class sc_any_value_map_ref;
class sc_any_value_map_elem_ref;
class sc_any_value_map_elem_cref;

template<typename T> struct sc_any_value_converter;
template<typename T> struct sc_any_value_has_pack;
template<typename T> struct sc_any_value_has_unpack;

#ifndef SC_DOXYGEN_IS_RUNNING
# define SC_ANY_VALUE_REQUIRES_PACK_(Type,ReturnType) \
  typename sc_meta::enable_if<sc_any_value_has_pack<T>::value,ReturnType>::type
# define SC_ANY_VALUE_REQUIRES_UNPACK_(Type,ReturnType) \
  typename sc_meta::enable_if<sc_any_value_has_unpack<T>::value,ReturnType>::type
#else
# define SC_ANY_VALUE_REQUIRES_PACK_(Type,ReturnType)   ReturnType
# define SC_ANY_VALUE_REQUIRES_UNPACK_(Type,ReturnType) ReturnType
#endif // SC_DOXYGEN_IS_RUNNING

// --------------------------------------------------------------------------

/**
 *  Enumeration for @ref sc_any_value data type categories
 */
enum sc_any_value_category
{
  SC_ANY_VALUE_NULL = 0,
  SC_ANY_VALUE_BOOL,
  SC_ANY_VALUE_INT,
  SC_ANY_VALUE_REAL,
  SC_ANY_VALUE_STRING,
  SC_ANY_VALUE_LIST,
  SC_ANY_VALUE_MAP
};

/// @ref sc_any_value comparisons
bool operator==( sc_any_value_cref const &, sc_any_value_cref const & );

/// @ref sc_any_value ostream insertion
std::ostream& operator<<( std::ostream&, sc_any_value_cref const & );

/// @ref sc_any_value istream extraction
std::istream& operator>>( std::istream&, sc_any_value_ref );

// --------------------------------------------------------------------------

/// reference to a constant (nested) @ref sc_any_value
class sc_any_value_cref
{
  friend class sc_any_value_ref;
  friend class sc_any_value_list_cref;
  friend class sc_any_value_list_ref;
  friend class sc_any_value_map_cref;
  friend class sc_any_value_map_ref;
  friend class sc_any_value_map_elem_cref;
  template<typename U> friend class sc_any_value_impl::iterator_impl;
  friend bool operator==( sc_any_value_cref const &, sc_any_value_cref const & );
  friend std::ostream& operator<<( std::ostream&, sc_any_value_cref const & );

protected:
  typedef void* impl_type; // use type-punned pointer for now
  typedef sc_any_value_impl::proxy_ptr<sc_any_value_cref> proxy_ptr;

  explicit sc_any_value_cref(impl_type i = NULL)
    : pimpl_(i) {}

public:
  typedef sc_any_value      value_type;
  typedef sc_any_value_cref const_reference;
  typedef sc_any_value_ref  reference;
  typedef sc_any_value_cref this_type;

#if SC_CPLUSPLUS >= 201103L
  sc_any_value_cref( this_type const & other ) = default;
#endif // C++11

  /** @name Type queries */
  ///@{
  sc_any_value_category category() const;

  bool is_null()    const;

  bool is_bool()    const;
  bool is_false()   const { return is_bool() && !get_bool(); }
  bool is_true()    const { return is_bool() &&  get_bool(); }

  bool is_number()  const
    { return is_int64() || is_uint64() || is_double(); }

  bool is_int()     const;
  bool is_uint()    const;
  bool is_int64()   const;
  bool is_uint64()  const;
  bool is_double()  const;

  bool is_string()  const;

  bool is_map()     const;

  bool is_list()    const;
  //@}

  /** @name Get basic value */
  //@{

  /// get boolean value
  bool get_bool() const;

  /// get integer value
  int      get_int()     const;
  /// get unsigned integer value
  unsigned get_uint()    const;
  /// get 64-bit integer value
  int64    get_int64()   const;
  /// get 64-bit unsigned integer value
  uint64   get_uint64()  const;
  /// get floating point value
  double   get_double()  const;
  /// get the numeric value (alias for get_double())
  double   get_number()  const { return get_double(); }

  /// get string value
  sc_zstring_view get_string() const;
  //@}

  /**
   * @name Get composite value
   * These functions return (constant) references to the composite value types
   * that can be stored in a sc_any_value (lists, maps).
   */
  //@{
  sc_any_value_list_cref   get_list() const;
  sc_any_value_map_cref    get_map()  const;
  //@}

  /** @name Get arbitrarily typed value */
  //@{
  /**
   * @brief Try to get a value of a conversion-enabled type
   * @param[out] dst store converted value in given reference
   * @return @c true, iff conversion was successful
   *
   * Type conversion from @ref sc_any_value is defined by an unqualified free
   * function call of @c sc_any_value_unpack with the following signature:
   * @code
   *  bool sc_any_value_unpack(T&, sc_any_value::const_reference);
   * @endcode
   *
   * For custom types, this function can be overloaded.
   * If no such function is available for a type T, the
   * @ref sc_any_value_ref::get function will not participate
   * in the overload resolution.
   *
   * @see get, sc_any_value_unpack, sc_any_value_converter
   */
  template<typename T>
  SC_ANY_VALUE_REQUIRES_UNPACK_(T,bool) try_get( T& dst ) const;

  /**
   * @brief Get a value of a conversion-enabled type
   * @return converted value
   * @throw  SC_REPORT_ERROR - if conversion was not successful
   *
   * Type conversion from @ref sc_any_value is defined by an unqualified free
   * function call of @c sc_any_value_unpack with the following signature:
   * @code
   *  bool sc_any_value_unpack(T&, sc_any_value::const_reference);
   * @endcode
   *
   * For custom types, this function can be overloaded.
   * If no such function is available for a type T, the
   * @ref sc_any_value_ref::try_get function will not participate
   * in the overload resolution.
   *
   * @see try_get, sc_any_value_unpack, sc_any_value_converter
   */
  template<typename T>
  SC_ANY_VALUE_REQUIRES_UNPACK_(T,T) get() const;
  //@}

  /// convert value to JSON
  std::string to_json() const;

  /** @brief overloaded addressof operator
   *
   * The \c addressof operator is replaced in the reference proxy classes
   * \c sc_any_value_*ref to avoid taking an address of a (usually)
   * temporary object.
   *
   * All reference objects provide the replacement, only the top-level
   * objects (sc_any_value, sc_any_value_list, sc_any_value_map) provide default
   * \c addressof semantics again.
   */
  proxy_ptr operator&() const { return proxy_ptr(*this); }

  /// Does this reference point to the same value as another reference?
  bool is_same(sc_any_value_cref that) const { return pimpl_ == that.pimpl_; }

protected:
  impl_type pimpl_;

private:
  /// constant reference, disabled assignment
  sc_any_value_cref operator=( sc_any_value_cref const& ) /* = delete */;
};

template<typename T>
SC_ANY_VALUE_REQUIRES_UNPACK_( T, bool )
sc_any_value_cref::try_get( T& dst ) const
{
  return sc_any_value_unpack( dst, *this );
}

template<typename T>
SC_ANY_VALUE_REQUIRES_UNPACK_( T, T )
sc_any_value_cref::get() const
{
  T result;
  if( !try_get(result) ) {
    SC_REPORT_ERROR( sc_core::SC_ID_ANY_VALUE_CONVERSION_FAILED_,
      "sc_any_value::get<T> could not convert value to requested type" );
  }
  return result;
}

// --------------------------------------------------------------------------

/// reference to a mutable (nested) @ref sc_any_value
class sc_any_value_ref
  : public sc_any_value_cref
{
  friend class sc_any_value_list_ref;
  friend class sc_any_value_map_ref;
  friend class sc_any_value_map_elem_ref;
  friend std::istream& operator>>( std::istream&, sc_any_value_ref );
  template<typename U> friend class sc_any_value_impl::iterator_impl;
  typedef sc_any_value_cref base_type;
  typedef sc_any_value_ref  this_type;

  typedef sc_any_value_impl::proxy_ptr<this_type> proxy_ptr;
protected:
  explicit sc_any_value_ref(impl_type i = NULL)
    : sc_any_value_cref(i) {}

public:

#if SC_CPLUSPLUS >= 201103L
  sc_any_value_ref( this_type const& other ) = default;
#endif // C++11

  /// move contents to another value (becomes @c null afterwards)
  sc_any_value move();

  /// exchange contents with another sc_any_value
  void swap( this_type& that );
  friend void swap(this_type a, this_type b) { a.swap(b); }

  this_type operator=( const base_type& );
  this_type operator=( const this_type& );

  /** @name Set basic value
   *
   * The various @c set_* functions update the represented value.
   *
   * These functions provide a "fluent interface" by returning a reference
   * to the surrounding variant object.  This enables chained function calls,
   * which is especially convenient for complex values like lists and maps.
   */
  ///@{

  /// set value to @c null
  sc_any_value_ref set_null();
  /// set boolean value (@c true, @c false)
  sc_any_value_ref set_bool( bool v );
  /// set integer value
  sc_any_value_ref set_int( int v );
  /// set unsigned integer value
  sc_any_value_ref set_uint( unsigned v );
  /// set 64-bit integer value
  sc_any_value_ref set_int64( int64 v );
  /// set unsigned 64-bit integer value
  sc_any_value_ref set_uint64( uint64 v );
  /// set double value
  sc_any_value_ref set_double( double v );
  /// set double value (alias for set_double())
  sc_any_value_ref set_number( double v ) { return set_double(v); }

  /// set string value from string reference
  sc_any_value_ref set_string( sc_string_view s );
  /// set string value from char pointer and length
  sc_any_value_ref set_string( const char* s, size_t len );

  /// set value to an (empty) list
  sc_any_value_list_ref   set_list();
  /// set value to an (empty) map
  sc_any_value_map_ref    set_map();
  ///@}

  /** @name Get composite value
   *
   * The functions return a reference to a (mutable) @ref sc_any_value object
   * of the corresponding composite value type (list, map).
   */

  ///@{
  using base_type::get_list;
  sc_any_value_list_ref get_list();

  using base_type::get_map;
  sc_any_value_map_ref  get_map();
  ///@}

  /** @name Set arbitrarily typed value */
  //@{
  /**
   * @brief Set value to conversion-enabled type
   * @return reference to itself
   * @throw  SC_REPORT_ERROR - if conversion was not successful
   *
   * Type conversion to @ref sc_any_value is defined by an unqualified free
   * function call of @c sc_any_value_pack with the following signature:
   * @code
   *  bool sc_any_value_pack(sc_any_value::reference, const T&);
   * @endcode
   *
   * For custom types, this function can be overloaded.
   * If no such function is available for a type T, the
   * @ref sc_any_value_ref::set function will not participate
   * in the overload resolution.
   *
   * @see try_set, sc_any_value_pack, sc_any_value_converter
   */
  template<typename T>
  SC_ANY_VALUE_REQUIRES_PACK_(T,sc_any_value_ref) set(T const & dst );
  /**
   * @brief Set value to conversion-enabled type
   * @return @c true, iff conversion was successful
   *
   * Type conversion to @ref sc_any_value is defined by an unqualified free
   * function call of @c sc_any_value_pack with the following signature:
   * @code
   *  bool sc_any_value_pack(sc_any_value::reference, const T&);
   * @endcode
   *
   * For custom types, this function can be overloaded.
   * If no such function is available for a type T, the
   * @ref sc_any_value_ref::try_set function will not participate
   * in the overload resolution.
   *
   * @see set, sc_any_value_pack, sc_any_value_converter
   */
  template<typename T>
  SC_ANY_VALUE_REQUIRES_PACK_(T,bool) try_set(T const & dst );
  ///@}

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
};

inline sc_any_value_ref
sc_any_value_ref::operator=( const this_type & that )
  { return *this = base_type(that); }

template<typename T>
SC_ANY_VALUE_REQUIRES_PACK_( T, bool )
sc_any_value_ref::try_set( T const & src )
{
  return sc_any_value_pack( *this, src );
}

template<typename T>
SC_ANY_VALUE_REQUIRES_PACK_(T,sc_any_value_ref)
sc_any_value_ref::set( T const& src )
{
  if( !try_set(src) ) {
    SC_REPORT_ERROR( sc_core::SC_ID_ANY_VALUE_CONVERSION_FAILED_,
      "sc_any_value:set<T> could not convert given value of type T to sc_any_value" );
  }
  return *this;
}

// --------------------------------------------------------------------------

//@cond SC_HIDDEN_FROM_DOXYGEN
// iterator implementations in sc_any_value.cpp
namespace sc_any_value_impl {
SC_TPLEXTERN_ template class iterator_impl<sc_any_value_cref>;
SC_TPLEXTERN_ template class iterator_impl<sc_any_value_ref>;
} // namespace sc_any_value_impl
SC_TPLEXTERN_ template class sc_any_value_iterator<sc_any_value_cref>;
SC_TPLEXTERN_ template class sc_any_value_iterator<sc_any_value_ref>;
///@endcond

/// reference to constant sc_any_value list value
class sc_any_value_list_cref
  : public sc_any_value_cref
{
  friend class sc_any_value_cref;
  typedef sc_any_value_cref      base_type;
  typedef sc_any_value_list_cref this_type;
  typedef sc_any_value_impl::proxy_ptr<this_type> proxy_ptr;

protected:
  explicit sc_any_value_list_cref(impl_type i = NULL)
    : base_type(i) {}

public:
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef sc_any_value_iterator<reference>        iterator;
  typedef sc_any_value_iterator<const_reference>  const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

#if SC_CPLUSPLUS >= 201103L
  sc_any_value_list_cref( this_type const& other ) = default;
#endif // C++11

  /** @name list queries */
  //@{
  bool      empty() const { return size() == 0;  }
  /// number of elements in the list
  size_type size()  const;
  /// number of entries in the map (signed value)
  difference_type ssize() const;
  /// capacity of underlying storage
  size_type capacity() const;
  //@}

  /** @name (constant) element access by index
   *  @see sc_any_value_list_ref::at, sc_any_value_list_ref::operator[]
   */
  //@{
  /** @brief unchecked element access
   *  @warning Given index is not checked and leads to undefined behavior
   *           for out-of-bounds accesses
   */
  const_reference operator[]( size_type index ) const;
  /** @brief checked element access
   *  @note Given index is bounds-checked, i.e. reports an error
   *        for attempted out-of-bounds accesses
   */
  const_reference at( size_type index ) const;

  /// return first element in the list
  const_reference front() const
    { return (*this)[0]; }

  /// return last element in the list
  const_reference back() const
    { return (*this)[size() - 1]; }
  //@}

  /** @name (constant) iterator interface */
  //@{
  const_iterator cbegin() const;
  const_iterator cend() const;

  const_iterator begin() const
    { return cbegin(); }
  const_iterator end() const
    { return cend(); }

  const_reverse_iterator rbegin() const
    { return crbegin(); }
  const_reverse_iterator rend() const
    { return crend(); }

  const_reverse_iterator crbegin() const
    { return const_reverse_iterator(cend()); }
  const_reverse_iterator crend() const
    { return const_reverse_iterator(cbegin()); }
  //@}

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

private:
  // exclude non-list value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_string;
  using base_type::get_map;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

// --------------------------------------------------------------------------

/// reference to mutable sc_any_value list value
class sc_any_value_list_ref
  : public sc_any_value_list_cref
{
  friend class sc_any_value_ref;
  typedef sc_any_value_list_cref base_type;
  typedef sc_any_value_list_ref  this_type;
  typedef sc_any_value_impl::proxy_ptr<this_type> proxy_ptr;

protected:
  explicit sc_any_value_list_ref(impl_type i = NULL)
    : base_type(i) {}

public:
#if SC_CPLUSPLUS >= 201103L
  sc_any_value_list_ref( this_type const& other ) = default;
#endif // C++11

  this_type operator=( this_type const& );
  this_type operator=( base_type const& );

  /// move contents to another value (becomes empty list afterwards)
  sc_any_value move();

  /// exchange contents with another list value
  void swap( this_type& );
  friend void swap(this_type a, this_type b) { a.swap(b); }

  /// reserve space in underlying storage
  this_type reserve( size_type );

  /// clear list elements
  this_type clear();

  /** @name (mutable) element access by index
   *  @see sc_any_value_list_cref::at, sc_any_value_list_cref::operator[]
   */
  //@{
  using base_type::operator[];
  ///@copydoc sc_any_value_list_cref::operator[]
  reference operator[]( size_type index );

  using base_type::at;
  ///@copydoc sc_any_value_list_cref::at
  reference at( size_type index );

  using base_type::front;
  ///@copydoc sc_any_value_list_cref::front
  reference front()
    { return (*this)[0]; }

  using base_type::back;
  ///@copydoc sc_any_value_list_cref::back
  reference back()
    { return (*this)[size() - 1]; }
  //@}

  /** @name (mutable) iterator interface */
  //@{
  using base_type::begin;
  iterator begin();

  using base_type::end;
  iterator end();

  using base_type::rbegin;
  reverse_iterator rbegin()
    { return reverse_iterator(end()); }

  using base_type::rend;
  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  //@}

  /** @name push new elements to the end of the list */
  //@{

  /// append value obtained from a constant sc_any_value reference
  this_type push_back( const_reference v );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  /// append value obtained from temporary sc_any_value rvalues
  this_type push_back( sc_any_value && v );
#endif // rvalue refs - C++11
  /// append arbitrary sc_any_value_converter enabled value
  template<typename T>
  SC_ANY_VALUE_REQUIRES_PACK_(T,this_type) push_back( const T & v );
  //@}

  /** @name insert elements into the list */
  //@{
  iterator insert( const_iterator pos, const_reference value );
  iterator insert( const_iterator pos, size_type count, const_reference value );
  template< class InputIt >
  iterator insert( const_iterator pos, InputIt first, InputIt last );
  //@}

  /** @name erase elements from the list */
  //@{
  iterator erase( const_iterator pos );
  iterator erase( const_iterator first, const_iterator last );

  void pop_back();
  //@}

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
};

inline sc_any_value_list_ref
sc_any_value_list_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline sc_any_value_list_ref
sc_any_value_list_ref::operator=( base_type const & that )
  { sc_any_value_ref v(pimpl_); v = that; return *this; }

inline sc_any_value_list_ref::reference
sc_any_value_list_ref::operator[]( size_type index )
  { return reference( base_type::operator[](index).pimpl_ ); }

inline sc_any_value_list_ref::reference
sc_any_value_list_ref::at( size_type index )
  { return reference( base_type::at(index).pimpl_ ); }

inline sc_any_value_list_ref
sc_any_value_ref::get_list()
  { return sc_any_value_list_ref( base_type::get_list().pimpl_ ); }

// --------------------------------------------------------------------------

/// reference to a constant sc_any_value map element
class sc_any_value_map_elem_cref
{
  template<typename U> friend class sc_any_value_impl::iterator_impl;
  typedef sc_any_value_impl::proxy_ptr<sc_any_value_map_elem_cref> proxy_ptr;
  typedef void value_type; // TODO: add  explicit value_type
public:
  typedef sc_any_value_map_elem_cref const_reference;
  typedef sc_any_value_map_elem_ref  reference;

  /// constant reference to the element's key
  sc_zstring_view key;
  /// constant reference to the element's value
  sc_any_value_cref value;

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

  /// Does this reference point to the same value as another reference?
  bool is_same(sc_any_value_map_elem_cref that) const { return pimpl_ == that.pimpl_; }

protected:
  typedef void* impl_type; // use type-punned pointer for now
  impl_type pimpl_;
  sc_any_value_map_elem_cref(impl_type i = NULL);
};

/// reference to a mutable sc_any_value map element
class sc_any_value_map_elem_ref
{
  template<typename U> friend class sc_any_value_impl::iterator_impl;
  typedef sc_any_value_impl::proxy_ptr<sc_any_value_map_elem_ref> proxy_ptr;
  typedef void value_type; // TODO: add  explicit value_type
public:
  typedef sc_any_value_map_elem_cref const_reference;
  typedef sc_any_value_map_elem_ref  reference;

  /// constant reference to the element's key
  sc_zstring_view key;
  /// mutable reference to the element's value
  sc_any_value_ref  value;

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

protected:
  typedef void* impl_type; // use type-punned pointer for now
  impl_type pimpl_;
  sc_any_value_map_elem_ref(impl_type i = NULL);
};

//@cond SC_HIDDEN_FROM_DOXYGEN
// iterator implementations in sc_any_value.cpp
namespace sc_any_value_impl {
SC_TPLEXTERN_ template class iterator_impl<sc_any_value_map_elem_cref>;
SC_TPLEXTERN_ template class iterator_impl<sc_any_value_map_elem_ref>;
} // namespace sc_any_value_impl
SC_TPLEXTERN_ template class sc_any_value_iterator<sc_any_value_map_elem_cref>;
SC_TPLEXTERN_ template class sc_any_value_iterator<sc_any_value_map_elem_ref>;
//@endcond

// --------------------------------------------------------------------------

/// reference to constant sc_any_value map
class sc_any_value_map_cref
  : public sc_any_value_cref
{
  friend class sc_any_value_cref;
  typedef sc_any_value_cref     base_type;
  typedef sc_any_value_map_cref this_type;
  typedef sc_any_value_impl::proxy_ptr<this_type> proxy_ptr;

protected:
  explicit sc_any_value_map_cref(impl_type i = NULL)
    : base_type(i) {}

public:
  typedef std::size_t    size_type;
  typedef std::ptrdiff_t difference_type;
  typedef sc_any_value_map_elem_ref  element_reference;
  typedef sc_any_value_map_elem_cref const_element_reference;
  typedef sc_any_value_iterator<element_reference>       iterator;
  typedef sc_any_value_iterator<const_element_reference> const_iterator;
  typedef std::reverse_iterator<iterator>              reverse_iterator;
  typedef std::reverse_iterator<const_iterator>        const_reverse_iterator;

#if SC_CPLUSPLUS >= 201103L
  sc_any_value_map_cref(this_type const& other) = default;
#endif // C++11

  /** @name map queries */
  //@{
  bool      empty()    const { return size() == 0;  }
  /// number of entries in the map
  size_type size()     const;
  /// number of entries in the map (signed value)
  difference_type ssize() const;
  //@}

  /** @name map element queries
   * Check for the existence of an entry with a given key
   */
  //@{
  bool has_entry( sc_string_view key ) const
    { return NULL != do_lookup( key, KEY_OPTIONAL ); }
  //@}

  /** @name (constant) map value access
   *
   * Accessing values in a constant map is only possible via the @ref at
   * functions, which require the existence of an entry with the given key.
   *
   * @see has_entry, find, sc_any_value_map_ref::operator[]
   */
  //@{
  /** @brief checked value access
   *  @note Access is checked, i.e. reports an error
   *        for value accesses through non-existent keys
   */
  const_reference at( sc_string_view key ) const
    { return const_reference( do_lookup( key ) ); }
  const_reference operator[]( sc_string_view key ) const
    { return const_reference( do_lookup( key ) ); }
  ///@}

  /** @name (constant) iterator interface */
  //@{
  const_iterator cbegin() const;
  const_iterator cend() const;

  const_iterator begin() const
    { return cbegin(); }
  const_iterator end() const
    { return cend(); }

  const_reverse_iterator rbegin() const
    { return crbegin(); }
  const_reverse_iterator rend() const
    { return crend(); }

  const_reverse_iterator crbegin() const
    { return const_reverse_iterator(cend()); }
  const_reverse_iterator crend() const
    { return const_reverse_iterator(cbegin()); }
  //@}

  /** @name find (constant) entries in the map
   *
   * These overloads return a const_iterator pointing
   * to an entry in the map and \ref end() otherwise.
   *
   * @see sc_any_value_map_ref::find
   */
  //@{
  const_iterator find( sc_string_view key ) const;
  //@}

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

protected:
  enum lookup_mode { KEY_REQUIRED = 0, KEY_OPTIONAL, KEY_CREATE };
  impl_type do_lookup( sc_string_view key
                     , lookup_mode mode = KEY_REQUIRED ) const;

private:
  // exclude non-map value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_string;
  using base_type::get_list;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

// --------------------------------------------------------------------------

/// reference to mutable sc_any_value map
class sc_any_value_map_ref
  : public sc_any_value_map_cref
{
  friend class sc_any_value_ref;
  typedef sc_any_value_map_cref base_type;
  typedef sc_any_value_map_ref  this_type;
  typedef sc_any_value_impl::proxy_ptr<this_type> proxy_ptr;

protected:
  explicit sc_any_value_map_ref(impl_type i = NULL)
    : base_type(i) {}

public:
#if SC_CPLUSPLUS >= 201103L
  sc_any_value_map_ref( this_type const& other ) = default;
#endif // C++11

  this_type operator=( base_type const& );
  this_type operator=( this_type const& );

  /// move contents to another value (becomes empty map afterwards)
  sc_any_value move();

  /// exchange contents with another map
  void swap( this_type& );
  friend void swap(this_type a, this_type b) { a.swap(b); }

  /// clear map entries
  this_type clear();

  /** @name (mutable) map value access
   *
   * Accessing values in a mutable map is possible via @ref at() or
   * @ref operator[], differing in their behavior for non-existent keys.
   *
   * @see has_entry, find, sc_any_value_map_cref::at
   */
  //@{
  using base_type::at;
  /// @copydoc sc_any_value_map_cref::at
  reference at( sc_string_view key )
    { return reference( do_lookup(key) ); }

  /** @brief inserting value access
   *  @note If there is no entry with the requested key, a new entry
   *        with a @c null value will be created.
   */
  reference operator[]( sc_string_view key )
    { return reference( do_lookup( key, KEY_CREATE ) ); }
  //@}

  /** @name (mutable) iterator interface */
  //@{
  using base_type::begin;
  iterator begin();

  using base_type::end;
  iterator end();

  using base_type::rbegin;
  reverse_iterator rbegin()
    { return reverse_iterator(end()); }

  using base_type::rend;
  reverse_iterator rend()
    { return reverse_iterator(begin()); }
  //@}

  /** @name find (mutable) elements in the map
   *
   * These overloads return an iterator pointing
   * to an element in the map and \ref end() otherwise.
   *
   * @see sc_any_value_map_cref::find
   */
  //@{
  iterator find( sc_string_view key )
    { return iterator( base_type::find(key).raw() ); }

  //@}

  ///@name map element addition
  //@{
  /// add value obtained from a constant sc_any_value reference
  this_type push_entry( sc_string_view key, const_reference value );

#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  /// add value obtained from a temporary sc_any_value rvalue reference
  this_type push_entry( sc_string_view key, sc_any_value&& value );
#endif // rvalue refs - C++11

  /// add an arbitrary sc_any_value_converter enabled value
  template<typename T>
  SC_ANY_VALUE_REQUIRES_PACK_(T,this_type)
  push_entry( sc_string_view key, const T & value );
  //@}

  /** @name erase elements from the map */
  //@{
  size_type erase( sc_string_view key );
  iterator  erase( const_iterator pos );
  iterator  erase( const_iterator first, const_iterator last );
  //@}

  /// @copydoc sc_any_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
};

inline sc_any_value_map_ref
sc_any_value_map_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline sc_any_value_map_ref
sc_any_value_map_ref::operator=( base_type const & that )
  { sc_any_value_ref v(pimpl_); v = that; return *this; }

inline sc_any_value_map_ref
sc_any_value_ref::get_map()
  { return sc_any_value_map_ref( base_type::get_map().pimpl_ ); }

} // namespace sc_dt

// --------------------------------------------------------------------------

//@cond SC_HIDDEN_FROM_DOXYGEN
namespace sc_any_value_no_adl {

typedef bool yes_type;
typedef bool no_type[2];

const no_type& sc_any_value_pack(...);   // catch-all overload
const no_type& sc_any_value_unpack(...); // catch-all overload

template<typename T>
class has_pack_impl
{
    static sc_dt::sc_any_value_ref make_ref();
    template<typename U> static U& make();
public:
    static const bool value = sizeof(sc_any_value_pack(make_ref(),make<T>())) == sizeof(yes_type);
};

template<typename T>
class has_unpack_impl
{
    static sc_dt::sc_any_value_cref make_ref();
    template<typename U> static U& make();
public:
    static const bool value = sizeof(sc_any_value_unpack(make<T>(),make_ref())) == sizeof(yes_type);
};

} // namespace sc_any_value_no_adl
//@endcond SC_HIDDEN_FROM_DOXYGEN

namespace sc_dt {

template<typename T>
struct sc_any_value_has_pack
  : sc_meta::bool_constant< sc_any_value_no_adl::has_pack_impl<T>::value > {};
template<typename T>
struct sc_any_value_has_unpack
  : sc_meta::bool_constant< sc_any_value_no_adl::has_unpack_impl<T>::value > {};

// --------------------------------------------------------------------------

/**
 * @brief generic variant type
 *
 * This class provides the first-class type for representing arbitrary values.
 * It can represent its values via a set of primitive types:
 *  * @c null (default)
 *  * boolean values (@c true, @c false)
 *  * numeric values (signed/unsigned integral or floating-point numbers)
 *  * strings
 *  * lists of values
 *  * maps of (key, value) pairs
 *
 * First-class objects of this class have strict value semantics, i.e. each
 * value represents a distinct object.  Due to the hierarchical nature of the
 * data structure, values embedded somewhere in a list or map are referenced
 * by dedicated reference objects (sc_any_value_cref, sc_any_value_ref, and their
 * specialized variants for strings, lists and maps), with or without constness.
 *
 * Users can add automatic conversions from/to sc_any_value objects by providing
 * an implementation (or specialisation) of the sc_any_value_converter class.
 * Corresponding specializations for the builtin types, the SystemC data types
 * and some freuquently used standard types are provided by default already.
 *
 * \see sc_any_value_list, sc_any_value_map, sc_any_value_converter
 */
class sc_any_value
  : public sc_any_value_ref
{
  typedef sc_any_value this_type;
  friend class sc_any_value_ref;
public:
  /// reference to a constant value
  typedef sc_any_value_cref        const_reference;
  /// reference to a mutable value
  typedef sc_any_value_ref         reference;
  /// reference to a constant list value
  typedef sc_any_value_list_cref   const_list_reference;
  /// reference to a mutable list value
  typedef sc_any_value_list_ref    list_reference;
  /// reference to a constant map value
  typedef sc_any_value_map_cref    const_map_reference;
  /// reference to a mutable map value
  typedef sc_any_value_map_ref     map_reference;

  /// default constructor
  sc_any_value()
    : sc_any_value_ref(), own_pimpl_() {}

  sc_any_value( this_type const & that );
  sc_any_value( const_reference that );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  sc_any_value( this_type&& that );
  sc_any_value( sc_any_value_list&& that );
  sc_any_value( sc_any_value_map&& that );
#endif // rvalue refs - C++11

  /// constructor from arbitrary sc_any_value_converter enabled value
  template<typename T>
  explicit
  sc_any_value( T const & src
#ifndef SC_DOXYGEN_IS_RUNNING
           , SC_ANY_VALUE_REQUIRES_PACK_(T,void)* = 0
#endif // SC_DOXYGEN_IS_RUNNING
           );

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  this_type& operator=( this_type&& );
  this_type& operator=( sc_any_value_list&& );
  this_type& operator=( sc_any_value_map&& );
#endif // rvalue refs - C++11

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { init(); reference::swap( that ); }
  void swap( sc_any_value & that );

  ~sc_any_value();

  /** @name Set value functions
   * \see sc_any_value_ref
   */
  //@{
  /// @copydoc sc_any_value_ref::set
  template< typename T >
  SC_ANY_VALUE_REQUIRES_PACK_(T,reference) set( T const & v )
    { init(); return reference::set(v); }

  /// @copydoc sc_any_value_ref::try_set
  template< typename T >
  SC_ANY_VALUE_REQUIRES_PACK_(T,bool) try_set( T const & v )
    { init(); return reference::try_set(v); }

  /// @copydoc sc_any_value_ref::set_null
  reference set_null()
    { init(); return reference::set_null(); }

  /// @copydoc sc_any_value_ref::set_bool
  reference set_bool( bool v )
    { init(); return reference::set_bool(v); }

  /// @copydoc sc_any_value_ref::set_int
  reference set_int( int v )
    { init(); return reference::set_int(v); }
  /// @copydoc sc_any_value_ref::set_uint
  reference set_uint( unsigned v )
    { init(); return reference::set_uint(v); }
  /// @copydoc sc_any_value_ref::set_int64
  reference set_int64( int64 v )
    { init(); return reference::set_int64(v); }
  /// @copydoc sc_any_value_ref::set_uint64
  reference set_uint64(uint64 v)
    { init(); return reference::set_uint64(v); }

  /// @copydoc sc_any_value_ref::set_double
  reference set_double(double v)
    { init(); return reference::set_double(v); }
  /// @copydoc sc_any_value_ref::set_number
  reference set_number( double v )
    { return set_double(v); }

  /// @copydoc sc_any_value_ref::set_string
  reference set_string( sc_string_view s )
    { init(); return reference::set_string(s); }

  /// @copydoc sc_any_value_ref::set_list
  list_reference set_list()
    { init(); return sc_any_value_ref::set_list(); }

  /// @copydoc sc_any_value_ref::set_map
  map_reference set_map()
    { init(); return sc_any_value_ref::set_map(); }
  //@}

  /** @name JSON (de)serialization
   */

  using const_reference::to_json;
  static sc_any_value from_json( sc_string_view json );

  friend std::istream& operator>>( std::istream& is, this_type & v )
    { v.init(); return is >> reference(v); }
  //@}

  /// @copydoc sc_any_value_cref::operator&
  const sc_any_value * operator&() const { return this; }
  /// @copydoc sc_any_value_cref::operator&
  sc_any_value * operator&() { return this; }

private:
  impl_type init();
  impl_type do_init();
  impl_type own_pimpl_;
};

template<typename T>
sc_any_value::sc_any_value( T const & v
#ifndef SC_DOXYGEN_IS_RUNNING
                      , SC_ANY_VALUE_REQUIRES_PACK_(T,void)*
#endif // SC_DOXYGEN_IS_RUNNING
                    )
  : sc_any_value_ref(), own_pimpl_()
{
  do_init();
  set(v);
}

inline
sc_any_value::sc_any_value( this_type const & that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline
sc_any_value::sc_any_value( const_reference that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline sc_any_value &
sc_any_value::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

inline sc_any_value::impl_type
sc_any_value::init()
{
  if( !pimpl_ )
    pimpl_ = do_init();
  return pimpl_;
}

// --------------------------------------------------------------------------
// The following functions depend on the completeness of the sc_any_value
// class, enforced by some compilers (e.g. Clang).

template<typename InputIt>
sc_any_value_list_ref::iterator
sc_any_value_list_ref::insert( const_iterator pos, InputIt first, InputIt last )
{
  iterator::difference_type offs = pos - begin();
  while( first != last ) {
    // TODO: optimize multi-element insertion
    pos = insert( pos, sc_any_value(*first++) ) + 1;
  }
  return begin() + offs;
}

template<typename T>
SC_ANY_VALUE_REQUIRES_PACK_( T, sc_any_value_list_ref::this_type )
sc_any_value_list_ref::push_back( const T& value )
{
  return push_back( sc_any_value(value) );
}

template<typename T>
SC_ANY_VALUE_REQUIRES_PACK_( T, sc_any_value_map_ref::this_type )
sc_any_value_map_ref::push_entry( sc_string_view key, const T& value )
{
  return push_entry( key, sc_any_value(value) );
}

// --------------------------------------------------------------------------

/**
 * @brief list of sc_any_value values
 *
 * This class is equivalent to a sc_any_value after calling @c set_list().
 * @see sc_any_value, sc_any_value_list_cref, sc_any_value_list_ref
 */
class sc_any_value_list
  : public sc_any_value_list_ref
{
  friend class sc_any_value;
  typedef sc_any_value_list this_type;
public:
  typedef sc_any_value_list_cref const_reference;
  typedef sc_any_value_list_ref  reference;

  sc_any_value_list();

  sc_any_value_list( this_type const & );
  sc_any_value_list( const_reference );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  sc_any_value_list( this_type&& );
#endif // rvalue refs - C++11

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  this_type& operator=( this_type && );
#endif // rvalue refs - C++11

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { reference::swap( that ); }
  void swap( this_type & );

  ~sc_any_value_list();

  ///@copydoc sc_any_value_cref::operator&
  const sc_any_value_list * operator&() const { return this; }
  ///@copydoc sc_any_value_cref::operator&
  sc_any_value_list * operator&() { return this; }

private:
  impl_type do_init();
  impl_type own_pimpl_;
};

inline
sc_any_value_list::sc_any_value_list()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
sc_any_value_list::sc_any_value_list( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
sc_any_value_list::sc_any_value_list( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline sc_any_value_list &
sc_any_value_list::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

// --------------------------------------------------------------------------

/**
 * @brief map of (key, sc_any_value) pairs
 *
 * This class is equivalent to a sc_any_value after calling @c set_map().
 * @see sc_any_value, sc_any_value_map_cref, sc_any_value_map_ref
 */
class sc_any_value_map
  : public sc_any_value_map_ref
{
  friend class sc_any_value;
  typedef sc_any_value_map this_type;
public:
  typedef sc_any_value_map_cref const_reference;
  typedef sc_any_value_map_ref  reference;

  sc_any_value_map();

  sc_any_value_map( this_type const & );
  sc_any_value_map( const_reference );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  sc_any_value_map( this_type && );
#endif // rvalue refs - C++11

  this_type& operator=( this_type const& );
  this_type& operator=( const_reference );
#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
  this_type& operator=( this_type && );
#endif // rvalue refs - C++11

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { reference::swap( that ); }
  void swap( this_type & );

  ~sc_any_value_map();

  /// @copydoc sc_any_value_cref::operator&
  const sc_any_value_map * operator&() const { return this; }
  /// @copydoc sc_any_value_cref::operator&
  sc_any_value_map * operator&() { return this; }

private:
  impl_type do_init();
  impl_type own_pimpl_;
};

inline
sc_any_value_map::sc_any_value_map()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
sc_any_value_map::sc_any_value_map( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
sc_any_value_map::sc_any_value_map( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline sc_any_value_map &
sc_any_value_map::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

// --------------------------------------------------------------------------

} // namespace sc_dt

#undef SC_ANY_VALUE_REQUIRES_PACK_
#undef SC_ANY_VALUE_REQUIRES_UNPACK_

#endif // SC_DATATYPES_ANY_VALUE_H_INCLUDED_
// Taf!
