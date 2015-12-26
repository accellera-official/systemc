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

 ****************************************************************************/

#ifndef CCI_CCI_VALUE_H_INCLUDED_
#define CCI_CCI_VALUE_H_INCLUDED_

#include "cci_core/cci_core_types.h"
#include "cci_core/cci_value_iterator.h"
#include "cci_core/systemc.h" // sc_dt::(u)int64, potentially strip out

#include <cstring> // std::strlen

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4231) // extern template
#endif // _MSC_VER

/**
 * @file   cci_value.h
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

CCI_OPEN_NAMESPACE_

// define our own typedefs to avoid SystemC dependency?
using sc_dt::int64;
using sc_dt::uint64;

// forward declarations
class cci_value;
class cci_value_cref;
class cci_value_ref;
class cci_value_string_cref;
class cci_value_string_ref;
class cci_value_list;
class cci_value_list_cref;
class cci_value_list_ref;
class cci_value_map;
class cci_value_map_cref;
class cci_value_map_ref;
class cci_value_map_elem_ref;
class cci_value_map_elem_cref;

template<typename T> struct cci_value_converter;

/**
*  Enumeration for data type categories, whose rough getting and setting is
*  supported by base class directly
*/
enum cci_value_category {
	/// Indicates that there is no data
	CCI_NULL_VALUE = 0,
	/// A boolean valued component of data
	CCI_BOOL_VALUE,
	/// An integer number component of data
	CCI_INTEGRAL_VALUE,
	/// A real number number component of data
	CCI_REAL_VALUE,
	/// A string component of data
	CCI_STRING_VALUE,
	/// A list component of data
	CCI_LIST_VALUE,
	/// A component of data that doesn't fit the other categories
	CCI_OTHER_VALUE
};

#ifndef CCI_DOXYGEN_IS_RUNNING
# define CCI_VALUE_CONVERTER_(Type) \
    typename cci_value_converter<Type>::type
#define CCI_VALUE_CHECKED_CONVERTER_(Type) \
    CCI_VALUE_CONVERTER_(Type) *
# define CCI_VALUE_REQUIRES_CONVERTER_(Type) \
    CCI_VALUE_CHECKED_CONVERTER_(Type) = 0
#else
# define CCI_VALUE_CONVERTER_(Type) Type
#endif // CCI_DOXYGEN_IS_RUNNING

/// @ref cci_value comparisons
bool operator==( cci_value_cref const &, cci_value_cref const & );

/// @ref cci_value ostream insertion
std::ostream& operator<<( std::ostream&, cci_value_cref const & );

/// @ref cci_value istream extraction
std::istream& operator>>( std::istream&, cci_value_ref );

// --------------------------------------------------------------------------

/// reference to a constant (nested) @ref cci_value
class cci_value_cref
{
  friend class cci_value_ref;
  friend class cci_value_list_cref;
  friend class cci_value_list_ref;
  friend class cci_value_map_cref;
  friend class cci_value_map_ref;
  friend class cci_value_map_elem_cref;
  template<typename U> friend class cci_impl::value_iterator_impl;
  friend bool operator==( cci_value_cref const &, cci_value_cref const & );
  friend std::ostream& operator<<( std::ostream&, cci_value_cref const & );

protected:
  typedef void* impl_type; // use type-punned pointer for now
  typedef cci_impl::value_ptr<cci_value_cref> proxy_ptr;

  explicit cci_value_cref(impl_type i = NULL)
    : pimpl_(i) {}

public:
  typedef cci_value      value_type;
  typedef cci_value_cref const_reference;
  typedef cci_value_ref  reference;

  /** @name Type queries */
  ///@{
  cci_value_category category() const;
  bool is_null()    const;

  bool is_bool()    const;
  bool is_false()   const
    { return is_bool() && !get_bool(); }
  bool is_true()    const
    { return is_bool() &&  get_bool(); }

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
  //@}

  /**
   * @name Get complex value
   * These functions return (constant) references to the complex value types
   * that can be stored in a cci_value (strings, lists, maps).
   */
  //@{
  cci_value_string_cref get_string() const;
  cci_value_list_cref   get_list() const;
  cci_value_map_cref    get_map()  const;
  //@}

  /** @name Get arbitrarily typed value */
  //@{
  /// try to get a value of a @ref cci_value_converter enabled type
  template<typename T>
  bool try_get( T& dst
#ifndef CCI_DOXYGEN_IS_RUNNING
              , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
              ) const;

  /// get a value of a @ref cci_value_converter enabled type
  template<typename T>
  CCI_VALUE_CONVERTER_(T) get() const;
  //@}

  /// convert value to JSON
  std::string to_json() const;

  /** @brief overloaded addressof operator
   *
   * The \c addressof operator is replaced in the reference proxy classes
   * \c cci_value_*ref to avoid taking an address of a (usually)
   * temporary object.
   *
   * All reference objects provide the replacement, only the top-level
   * objects (cci_value, cci_value_list, cci_value_map) provide default
   * \c addressof semantics again.
   */
  proxy_ptr operator&() const { return proxy_ptr(*this); }

  /// Does this reference point to the same value as another reference?
  bool is_same(cci_value_cref that) const { return pimpl_ == that.pimpl_; }

protected:
  void
  report_error( const char* msg
              , const char* file = NULL, int line = 0 ) const;

protected:
  impl_type pimpl_;

private:
  /// constant reference, disabled assignment
  cci_value_cref operator=( cci_value_cref const& ) /* = delete */;
};

template<typename T>
bool
cci_value_cref::try_get( T& dst
#ifndef CCI_DOXYGEN_IS_RUNNING
                       , CCI_VALUE_CHECKED_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                       ) const
{
  typedef cci_value_converter<T> conv;
  return conv::unpack( dst, *this );
}

template<typename T>
CCI_VALUE_CONVERTER_(T) cci_value_cref::get() const
{
  T result;
  if( !try_get(result) ) {
    report_error("conversion from cci_value failed", __FILE__, __LINE__);
  }
  return result;
}

// --------------------------------------------------------------------------

/// reference to a mutable (nested) @ref cci_value
class cci_value_ref
  : public cci_value_cref
{
  friend class cci_value_string_ref;
  friend class cci_value_list_ref;
  friend class cci_value_map_ref;
  friend class cci_value_map_elem_ref;
  template<typename U> friend class cci_impl::value_iterator_impl;
  friend std::istream& operator>>( std::istream&, cci_value_ref );
  typedef cci_value_cref base_type;
  typedef cci_value_ref  this_type;

  typedef cci_impl::value_ptr<this_type> proxy_ptr;
protected:
  explicit cci_value_ref(impl_type i = NULL)
    : cci_value_cref(i) {}

public:
  /// move contents to another value (becomes @c null afterwards)
  cci_value move();

  /// exchange contents with another cci_value
  void swap( this_type& that );
  friend void swap(this_type a, this_type b) { a.swap(b); }

  this_type operator=( const base_type& );
  this_type operator=( const this_type& );

  /** @name Set basic value
   *
   * The various @c set_* functions update the represented value
   * (and its @ref category()) accordingly.
   *
   * These functions provide a "fluent interface" by returning a reference
   * to the surrounding variant object.  This enables chained function calls,
   * which is especially convenient for complex values like lists and maps.
   */
  ///@{

  /// set value to @c null
  cci_value_ref set_null();
  /// set boolean value (@c true, @c false)
  cci_value_ref set_bool( bool v );
  /// set integer value
  cci_value_ref set_int( int v );
  /// set unsigned integer value
  cci_value_ref set_uint( unsigned v );
  /// set 64-bit integer value
  cci_value_ref set_int64( int64 v );
  /// set unsigned 64-bit integer value
  cci_value_ref set_uint64( uint64 v );
  /// set double value
  cci_value_ref set_double( double v );
  /// set double value (alias for set_double())
  cci_value_ref set_number( double v ) { return set_double(v); }

  /// set string value from null-terminated string
  cci_value_string_ref set_string( const char* s );
  /// set string value from std::string
  cci_value_string_ref set_string( const std::string& s );
  /// set string value from cci_value::const_string_reference
  cci_value_string_ref set_string( cci_value_string_cref s );
  /// set string value from null-terminated string and length
  cci_value_string_ref set_string( const char* s, size_t len );

  /// set value to an (empty) list
  cci_value_list_ref   set_list();
  /// set value to an (empty) map
  cci_value_map_ref    set_map();
  ///@}

  /** @name Get complex value
   *
   * The functions return a reference to a (mutable) @ref cci_value object
   * of the corresponding complex value type (string, list, map).
   */

  ///@{
  using base_type::get_string;
  cci_value_string_ref get_string();

  using base_type::get_list;
  cci_value_list_ref get_list();

  using base_type::get_map;
  cci_value_map_ref  get_map();
  ///@}

  /** @name Set arbitrarily typed value */
  //@{
  /// set value to cci_value_converter enabled type
  template<typename T>
  cci_value_ref set(T const & dst
#ifndef CCI_DOXYGEN_IS_RUNNING
                   , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                   );
  /// try to set value to cci_value_converter enabled type
  template<typename T>
  bool try_set(T const & dst
#ifndef CCI_DOXYGEN_IS_RUNNING
              , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
              );
  ///@}

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
protected:
  /// try to set the value from a JSON-encoded string
  bool json_deserialize( std::string const& );
};

inline cci_value_ref
cci_value_ref::operator=( const this_type & that )
  { return *this = base_type(that); }

template<typename T>
bool
cci_value_ref::try_set( T const & src
#ifndef CCI_DOXYGEN_IS_RUNNING
                      , CCI_VALUE_CHECKED_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                      )
{
  typedef cci_value_converter<T> conv;
  return conv::pack( *this, src );
}

template<typename T>
cci_value_ref
cci_value_ref::set( T const& src
#ifndef CCI_DOXYGEN_IS_RUNNING
                  , CCI_VALUE_CHECKED_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                  )
{
  if( !try_set(src) ) {
    report_error("conversion to cci_value failed", __FILE__, __LINE__);
  }
  return *this;
}

// --------------------------------------------------------------------------

/// reference to constant cci_value string value
class cci_value_string_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  friend class cci_value_map_elem_cref;
  friend class cci_value_map_elem_ref;
  typedef cci_value_cref        base_type;
  typedef cci_value_string_cref this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;

protected:
  explicit cci_value_string_cref(impl_type i = NULL)
    : base_type(i) {}

public:
  typedef size_t size_type;

  /// empty string?
  bool      empty()  const { return size() == 0;  }
  size_type length() const { return size(); }
  size_type size()   const;

  /// implicit conversion to std::string
  operator std::string() const { return std::string( c_str(), length() ); }
  /// obtain underlying null-terminated string
  char const * c_str()   const;

  /// character access by index
  char operator[]( size_type index ) const
    { return c_str()[index]; }

  /** @name string equality comparisons */
  //@{
  bool operator==(cci_value_string_cref const &) const;
  bool operator==(const char* s) const;
  bool operator==(std::string const & ) const;

  friend bool operator==( const char * a, cci_value_string_cref const & b)
    { return b == a; }
  friend bool operator==( std::string const & a, cci_value_string_cref const & b )
    { return b == a; }
  //@}

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

private:
  // exclude non-string value functions
  using base_type::get_bool;
  using base_type::get_int;
  using base_type::get_uint;
  using base_type::get_int64;
  using base_type::get_uint64;
  using base_type::get_double;
  using base_type::get_list;
  using base_type::get_map;

private:
  // constant reference, no assignment
  this_type& operator=( this_type const& ) /* = delete */;
};

/// specialisation for printing a plain string reference (not JSON encoded)
inline std::ostream&
operator<<( std::ostream& os, cci_value_string_cref const& s )
{
  os.write( s.c_str(), static_cast<std::streamsize>(s.length()) );
  return os;
}

// --------------------------------------------------------------------------

/// reference to mutable cci_value string value
class cci_value_string_ref
  : public cci_value_string_cref
{
  friend class cci_value_ref;
  typedef cci_value_string_cref base_type;
  typedef cci_value_string_ref  this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;

protected:
  explicit cci_value_string_ref(impl_type i = NULL)
    : base_type(i) {}

public:
  /// move contents to another value (becomes empty string afterwards)
  cci_value move();

  /// exchange contents with another string value
  friend void swap(this_type a, this_type b) { a.swap(b); }
  void swap( this_type& that );

  /** @name assign string contents */
  //@{
  this_type operator=( this_type const& );
  this_type operator=( cci_value_string_cref s );
  this_type operator=( const char * s );
  this_type operator=( std::string const & s );
  //@}

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
};

inline cci_value_string_ref
cci_value_string_ref::operator=( this_type const& s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( cci_value_string_cref s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( const char * s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_string_ref::operator=( std::string const & s )
  { return cci_value_ref(pimpl_).set_string(s); }

inline cci_value_string_ref
cci_value_ref::get_string()
  { return cci_value_string_ref( base_type::get_string().pimpl_ ); }

inline cci_value_string_ref
cci_value_ref::set_string(char const * s)
  { return set_string( s ? s : "", s ? std::strlen(s) : 0u ); }

inline cci_value_string_ref
cci_value_ref::set_string(cci_value_string_cref s)
  { return set_string( s.c_str(), s.length() ); }

inline cci_value_string_ref
cci_value_ref::set_string(std::string const & s)
  { return set_string( s.c_str(), s.length() ); }

// --------------------------------------------------------------------------

///@cond CCI_HIDDEN_FROM_DOXYGEN
// iterator implementations in cci_value.cpp
namespace cci_impl {
CCI_TPLEXTERN_ template class value_iterator_impl<cci_value_ref>;
CCI_TPLEXTERN_ template class value_iterator_impl<cci_value_cref>;
} // namespace cci_impl
CCI_TPLEXTERN_ template class cci_value_iterator<cci_value_cref>;
CCI_TPLEXTERN_ template class cci_value_iterator<cci_value_ref>;
///@endcond

/// reference to constant cci_value list value
class cci_value_list_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  typedef cci_value_cref      base_type;
  typedef cci_value_list_cref this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;

protected:
  explicit cci_value_list_cref(impl_type i = NULL)
    : base_type(i) {}

public:
  typedef size_t size_type;
  typedef cci_value_iterator<reference>         iterator;
  typedef cci_value_iterator<const_reference>   const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /** @name list queries */
  //@{
  bool      empty() const { return size() == 0;  }
  /// number of elements in the list
  size_type size()  const;
  /// capacity of underlying storage
  size_type capacity() const;
  //@}

  /** @name (constant) element access by index
   *  @see cci_value_list_ref::at, cci_value_list_ref::operator[]
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

  /// @copydoc cci_value_cref::operator&
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

/// reference to mutable cci_value list value
class cci_value_list_ref
  : public cci_value_list_cref
{
  friend class cci_value_ref;
  typedef cci_value_list_cref base_type;
  typedef cci_value_list_ref  this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;

protected:
  explicit cci_value_list_ref(impl_type i = NULL)
    : base_type(i) {}

public:
  this_type operator=( this_type const& );
  this_type operator=( base_type const& );

  /// move contents to another value (becomes empty list afterwards)
  cci_value move();

  /// exchange contents with another list value
  void swap( this_type& );
  friend void swap(this_type a, this_type b) { a.swap(b); }

  /// reserve space in underlying storage
  this_type reserve( size_type );

  /// clear list elements
  this_type clear();

  /** @name (mutable) element access by index
   *  @see cci_value_list_cref::at, cci_value_list_cref::operator[]
   */
  //@{
  using base_type::operator[];
  ///@copydoc cci_value_list_cref::operator[]
  reference operator[]( size_type index );

  using base_type::at;
  ///@copydoc cci_value_list_cref::at
  reference at( size_type index );

  using base_type::front;
  ///@copydoc cci_value_list_cref::front
  reference front()
    { return (*this)[0]; }

  using base_type::back;
  ///@copydoc cci_value_list_cref::back
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
  /// append value obtained from a constant cci_value reference
  this_type push_back( const_reference v );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  /// append value obtained from temporary cci_value rvalues
  this_type push_back( cci_value && v );
#endif // CCI_HAS_CXX_RVALUE_REFS
  /// append arbitrary cci_value_converter enabled value
  template<typename T>
  this_type push_back( const T & v
#ifndef CCI_DOXYGEN_IS_RUNNING
                     , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                     );
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

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }
};

inline cci_value_list_ref
cci_value_list_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline cci_value_list_ref
cci_value_list_ref::operator=( base_type const & that )
  { cci_value_ref v(pimpl_); v = that; return *this; }

inline cci_value_list_ref::reference
cci_value_list_ref::operator[]( size_type index )
  { return reference( base_type::operator[](index).pimpl_ ); }

inline cci_value_list_ref::reference
cci_value_list_ref::at( size_type index )
  { return reference( base_type::at(index).pimpl_ ); }

inline cci_value_list_ref
cci_value_ref::get_list()
  { return cci_value_list_ref( base_type::get_list().pimpl_ ); }

// --------------------------------------------------------------------------

/// reference to a constant cci_value map element
class cci_value_map_elem_cref
{
  template<typename U> friend class cci_impl::value_iterator_impl;
  typedef cci_impl::value_ptr<cci_value_map_elem_cref> proxy_ptr;

  typedef void value_type; // TODO: add  explicit value_type 
public:
  typedef cci_value_map_elem_cref const_reference;
  typedef cci_value_map_elem_ref  reference;

  /// constant reference to the element's key
  cci_value_string_cref key;
  /// constant reference to the element's value
  cci_value_cref        value;

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

  /// Does this reference point to the same value as another reference?
  bool is_same(cci_value_map_elem_cref that) const { return pimpl_ == that.pimpl_; }

protected:
  typedef void* impl_type; // use type-punned pointer for now
  impl_type pimpl_;
  cci_value_map_elem_cref(impl_type i = NULL);
};

/// reference to a mutable cci_value map element
class cci_value_map_elem_ref
{
  template<typename U> friend class cci_impl::value_iterator_impl;
  typedef cci_impl::value_ptr<cci_value_map_elem_ref> proxy_ptr;
  typedef void value_type; // TODO: add  explicit value_type
public:
  typedef cci_value_map_elem_cref const_reference;
  typedef cci_value_map_elem_ref  reference;

  /// constant reference to the element's key
  cci_value_string_cref key;
  /// mutable reference to the element's value
  cci_value_ref         value;

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

protected:
  typedef void* impl_type; // use type-punned pointer for now
  impl_type pimpl_;
  cci_value_map_elem_ref(impl_type i = NULL);
};

///@cond CCI_HIDDEN_FROM_DOXYGEN
// iterator implementations in cci_value.cpp
namespace cci_impl {
CCI_TPLEXTERN_ template class value_iterator_impl<cci_value_map_elem_cref>;
CCI_TPLEXTERN_ template class value_iterator_impl<cci_value_map_elem_ref>;
} // namespace cci_impl
CCI_TPLEXTERN_ template class cci_value_iterator<cci_value_map_elem_cref>;
CCI_TPLEXTERN_ template class cci_value_iterator<cci_value_map_elem_ref>;
///@endcond

/// reference to constant cci_value map
class cci_value_map_cref
  : public cci_value_cref
{
  friend class cci_value_cref;
  typedef cci_value_cref     base_type;
  typedef cci_value_map_cref this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;

protected:
  explicit cci_value_map_cref(impl_type i = NULL)
    : base_type(i) {}

public:
  typedef size_t size_type;
  typedef cci_value_iterator<cci_value_map_elem_ref>  iterator;
  typedef cci_value_iterator<cci_value_map_elem_cref> const_iterator;
  typedef std::reverse_iterator<iterator>             reverse_iterator;
  typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

  /** @name map queries */
  //@{
  bool      empty()    const { return size() == 0;  }
  /// number of entries in the map
  size_type size()     const;
  //@}

  /** @name map element queries
   * Check for the existence of an entry with a given key
   */
  //@{
  bool has_entry( const char * key ) const
    { return NULL != do_lookup( key, std::strlen(key), KEY_OPTIONAL ); }
  bool has_entry( std::string const & key ) const
    { return NULL != do_lookup( key.c_str(), key.length(), KEY_OPTIONAL ); }
  bool has_entry( cci_value_string_cref key ) const
    { return NULL != do_lookup( key.c_str(), key.length(), KEY_OPTIONAL ); }
  //@}

  /** @name (constant) map value access
   *
   * Accessing values in a constant map is only possible via the @ref at
   * functions, which require the existence of an entry with the given key.
   *
   * @see has_entry, find, cci_value_map_ref::operator[]
   */
  //@{
  /** @brief checked value access
   *  @note Access is checked, i.e. reports an error
   *        for value accesses through non-existent keys
   */
  const_reference at( const char* key ) const
    { return const_reference( do_lookup( key, std::strlen(key) ) ); }
  ///@copydoc cci_value_map_cref::at(const char*) const
  const_reference at( std::string const& key ) const
    { return const_reference( do_lookup( key.c_str(), key.length() ) ); }
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
   * @see cci_value_map_ref::find
   */
  //@{
  const_iterator find( const char* key ) const
    { return do_find( key, std::strlen(key) ); }
  const_iterator find( const std::string& key ) const
    { return do_find( key.c_str(), key.length() ); }
  //@}

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

protected:
  enum lookup_mode { KEY_REQUIRED = 0, KEY_OPTIONAL, KEY_CREATE };
  impl_type do_lookup( const char* key, size_type keylen
                     , lookup_mode mode = KEY_REQUIRED ) const;
  const_iterator do_find(const char* key, size_type keylen) const;

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

/// reference to mutable cci_value map
class cci_value_map_ref
  : public cci_value_map_cref
{
  friend class cci_value_ref;
  typedef cci_value_map_cref base_type;
  typedef cci_value_map_ref  this_type;
  typedef cci_impl::value_ptr<this_type> proxy_ptr;
protected:
  explicit cci_value_map_ref(impl_type i = NULL)
    : base_type(i) {}

public:

  this_type operator=( base_type const& );
  this_type operator=( this_type const& );

  /// move contents to another value (becomes empty map afterwards)
  cci_value move();

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
   * @see has_entry, find, cci_value_map_cref::at
   */
  //@{
  using base_type::at;
  /// @copydoc cci_value_map_cref::at
  reference at( const char* key )
    { return reference( do_lookup( key, std::strlen(key) ) ); }
  /// @copydoc cci_value_map_cref::at
  reference at( std::string const& key )
    { return reference( do_lookup( key.c_str(), key.length() ) ); }
  /** @brief inserting value access
   *  @note If there is no entry with the requested key, a new entry
   *        with a @c null value will be created.
   */
  reference operator[]( const char* key )
    { return reference( do_lookup( key, std::strlen(key), KEY_CREATE ) ); }
  ///@copydoc cci_value_map_ref::operator[](const char*)
  reference operator[]( std::string const& key )
    { return reference( do_lookup( key.c_str(), key.length(), KEY_CREATE ) ); }
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
   * @see cci_value_map_cref::find
   */
  //@{
  iterator find( const char* key )
    { return iterator( do_find( key, std::strlen(key) ).raw() ); }
  iterator find( const std::string& key )
    { return iterator( do_find( key.c_str(), key.length() ).raw() ); }
  //@}

  ///@name map element addition
  //@{
  /// add value obtained from a constant cci_value reference
  this_type push_entry( const char* key, const_reference value )
    { return do_push( key, std::strlen(key), value ); }

  /// add value obtained from a constant cci_value reference
  this_type push_entry( std::string const& key, const_reference value )
    { return do_push( key.c_str(), key.length(), value ); }

#ifdef CCI_HAS_CXX_RVALUE_REFS
  /// add value obtained from a temporary cci_value rvalue reference
  this_type push_entry( const char* key, cci_value&& value )
    { return do_push( key, std::strlen(key), CCI_MOVE_(value) ); }

  /// add value obtained from a rvalue cci_value rvalue reference
  this_type push_entry( std::string const& key, cci_value&& value )
    { return do_push( key.c_str(), key.length(), CCI_MOVE_(value) ); }
#endif // CCI_HAS_CXX_RVALUE_REFS

  /// add an arbitrary cci_value_converter enabled value
  template<typename T>
  this_type push_entry( const char* key, const T & value
#ifndef CCI_DOXYGEN_IS_RUNNING
                      , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif
                      )
    { return do_push( key, std::strlen(key), value ); }

  /// add an arbitrary cci_value_converter enabled value
  template<typename T>
  this_type push_entry( std::string const & key, const T & value
#ifndef CCI_DOXYGEN_IS_RUNNING
                      , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif
                      )
    { return do_push( key.c_str(), key.length(), value ); }
  //@}

  /** @name erase elements from the map */
  //@{
  size_type erase( const char* key )
    { return do_erase( key, std::strlen(key) ); }
  size_type erase( const std::string& key )
    { return do_erase( key.c_str(), key.length() ); }

  iterator erase( const_iterator pos );
  iterator erase( const_iterator first, const_iterator last );
  //@}

  /// @copydoc cci_value_cref::operator&
  proxy_ptr operator&() const { return proxy_ptr(*this); }

private:
  template<typename T>
  this_type do_push(const char* key, size_type keylen, const T& value);
  this_type do_push(const char* key, size_type keylen, const_reference value);
#ifdef CCI_HAS_CXX_RVALUE_REFS
  this_type do_push(const char* key, size_type keylen, cci_value&& value);
#endif // CCI_HAS_CXX_RVALUE_REFS
  size_type do_erase(const char* key, size_type keylen);
};

inline cci_value_map_ref
cci_value_map_ref::operator=( this_type const & that )
  { return *this = base_type(that); }

inline cci_value_map_ref
cci_value_map_ref::operator=( base_type const & that )
  { cci_value_ref v(pimpl_); v = that; return *this; }

inline cci_value_map_ref
cci_value_ref::get_map()
  { return cci_value_map_ref( base_type::get_map().pimpl_ ); }

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
 * by dedicated reference objects (cci_value_cref, cci_value_ref, and their
 * specialized variants for strings, lists and maps), with or without constness.
 *
 * Users can add automatic conversions from/to cci_value objects by providing
 * an implementation (or specialisation) of the cci_value_converter class.
 * Corresponding specializations for the builtin types, the SystemC data types
 * and some freuquently used standard types are provided by default already.
 *
 * \see cci_value_list, cci_value_map, cci_value_converter
 */
class cci_value
  : public cci_value_ref
{
  typedef cci_value this_type;
  friend class cci_value_ref;
  friend class cci_value_string_ref;
public:
  /// reference to a constant value
  typedef cci_value_cref        const_reference;
  /// reference to a mutable value
  typedef cci_value_ref         reference;
  /// reference to a constant string value
  typedef cci_value_string_cref const_string_reference;
  /// reference to a mutable string value
  typedef cci_value_string_ref  string_reference;
  /// reference to a constant list value
  typedef cci_value_list_cref   const_list_reference;
  /// reference to a mutable list value
  typedef cci_value_list_ref    list_reference;
  /// reference to a constant map value
  typedef cci_value_map_cref    const_map_reference;
  /// reference to a mutable map value
  typedef cci_value_map_ref     map_reference;

  /// default constructor
  cci_value()
    : cci_value_ref(), own_pimpl_() {}

  /// constructor from arbitrary cci_value_converter enabled value
  template<typename T>
  explicit
  cci_value( T const & src
#ifndef CCI_DOXYGEN_IS_RUNNING
           , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
           );

  cci_value( this_type const & that );
  cci_value( const_reference that );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  cci_value( this_type&& that );
  cci_value( cci_value_list&& that );
  cci_value( cci_value_map&& that );
#endif // CCI_HAS_CXX_RVALUE_REFS

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  this_type& operator=( this_type&& );
  this_type& operator=( cci_value_list&& );
  this_type& operator=( cci_value_map&& );
#endif // CCI_HAS_CXX_RVALUE_REFS

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { init(); reference::swap( that ); }
  void swap( cci_value & that );

  ~cci_value();

  /** @name Set value functions
   * \see cci_value_ref
   */
  //@{
  /// @copydoc cci_value_ref::set
  template< typename T >
  reference  set( T const & v
#ifndef CCI_DOXYGEN_IS_RUNNING
                , CCI_VALUE_REQUIRES_CONVERTER_(T)
#endif //  CCI_DOXYGEN_IS_RUNNING
                )
    { init(); return reference::set(v); }

  /// @copydoc cci_value_ref::set_null
  reference set_null()
    { init(); return reference::set_null(); }

  /// @copydoc cci_value_ref::set_bool
  reference set_bool( bool v )
    { init(); return reference::set_bool(v); }

  /// @copydoc cci_value_ref::set_int
  reference set_int( int v )
    { init(); return reference::set_int(v); }
  /// @copydoc cci_value_ref::set_uint
  reference set_uint( unsigned v )
    { init(); return reference::set_uint(v); }
  /// @copydoc cci_value_ref::set_int64
  reference set_int64( int64 v )
    { init(); return reference::set_int64(v); }
  /// @copydoc cci_value_ref::set_uint64
  reference set_uint64(uint64 v)
    { init(); return reference::set_uint64(v); }

  /// @copydoc cci_value_ref::set_double
  reference set_double(double v)
    { init(); return cci_value_ref::set_double(v); }
  /// @copydoc cci_value_ref::set_number
  reference set_number( double v )
    { return set_double(v); }

  /// @copydoc cci_value_ref::set_string(const char*)
  string_reference set_string( const char* s )
    { init(); return reference::set_string(s); }
  /// @copydoc cci_value_ref::set_string(const std::string&)
  string_reference set_string( const std::string& s )
    { init(); return reference::set_string(s); }
  /// @copydoc cci_value_ref::set_string(cci_value_string_cref)
  string_reference set_string( const_string_reference s )
    { init(); return reference::set_string(s); }

  /// @copydoc cci_value_ref::set_list
  list_reference set_list()
    { init(); return cci_value_ref::set_list(); }

  /// @copydoc cci_value_ref::set_map
  map_reference set_map()
    { init(); return cci_value_ref::set_map(); }
  //@}

  /** @name JSON (de)serialization
   */

  using const_reference::to_json;
  static cci_value from_json( std::string const & json );

  friend std::istream& operator>>( std::istream& is, this_type & v )
    { v.init(); return is >> reference(v); }
  //@}

  /// @copydoc cci_value_cref::operator&
  const cci_value * operator&() const { return this; }
  /// @copydoc cci_value_cref::operator&
  cci_value * operator&() { return this; }

private:
  impl_type init();
  impl_type do_init();

  bool json_deserialize( std::string const & src )
    { init(); return reference::json_deserialize( src ); }

  impl_type own_pimpl_;
};

template<typename T>
cci_value::cci_value( T const & v
#ifndef CCI_DOXYGEN_IS_RUNNING
                    , CCI_VALUE_CHECKED_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                    )
  : cci_value_ref(), own_pimpl_()
{
  do_init();
  set(v);
}

inline
cci_value::cci_value( this_type const & that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline
cci_value::cci_value( const_reference that )
  : reference(), own_pimpl_()
{
  *this = that;
}

inline cci_value &
cci_value::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

inline cci_value::impl_type
cci_value::init()
{
  if( !pimpl_ )
    pimpl_ = do_init();
  return pimpl_;
}

inline cci_value
cci_value::from_json( std::string const & json )
{
  cci_value v;
  bool ok = v.json_deserialize( json );
  sc_assert( ok );
  return v;
}

// --------------------------------------------------------------------------
// The following two functions depend on the completeness of the cci_value
// class, enforced by some compilers (e.g. Clang).


///@cond CCI_HIDDEN_FROM_DOXYGEN
#ifdef CCI_HAS_CXX_RVALUE_REFS
# define CCI_VALUE_MOVE_(v) CCI_MOVE_(v)
#else
# define CCI_VALUE_MOVE_(v) cci_value::const_reference(v)
#endif // CCI_HAS_CXX_RVALUE_REFS
///@endcond

template<typename T>
cci_value_list_ref::this_type
cci_value_list_ref::push_back( const T& value
#ifndef CCI_DOXYGEN_IS_RUNNING
                             , CCI_VALUE_CHECKED_CONVERTER_(T)
#endif // CCI_DOXYGEN_IS_RUNNING
                             )
{
  cci_value v(value);
  return push_back( CCI_VALUE_MOVE_(v) );
}

template<typename T>
cci_value_map_ref
cci_value_map_ref::do_push( const char* key, size_type keylen, const T& value )
{
  cci_value v(value);
  return push_entry( key, CCI_VALUE_MOVE_(v) );
}

// --------------------------------------------------------------------------

/**
 * @brief list of cci_value values
 *
 * This class is equivalent to a cci_value after calling @c set_list().
 * @see cci_value, cci_value_list_cref, cci_value_list_ref
 */
class cci_value_list
  : public cci_value_list_ref
{
  friend class cci_value;
  typedef cci_value_list this_type;
public:
  typedef cci_value_list_cref const_reference;
  typedef cci_value_list_ref  reference;

  cci_value_list();

  cci_value_list( this_type const & );
  cci_value_list( const_reference );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  cci_value_list( this_type&& );
#endif // CCI_HAS_CXX_RVALUE_REFS

  this_type& operator=( this_type const & );
  this_type& operator=( const_reference );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  this_type& operator=( this_type && );
#endif // CCI_HAS_CXX_RVALUE_REFS

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { reference::swap( that ); }
  void swap( this_type & );

  ~cci_value_list();

  ///@copydoc cci_value_cref::operator&
  const cci_value_list * operator&() const { return this; }
  ///@copydoc cci_value_cref::operator&
  cci_value_list * operator&() { return this; }

private:
  impl_type do_init();
  impl_type own_pimpl_;
};

inline
cci_value_list::cci_value_list()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
cci_value_list::cci_value_list( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
cci_value_list::cci_value_list( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline cci_value_list &
cci_value_list::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

// --------------------------------------------------------------------------

/**
 * @brief map of (key, cci_value) pairs
 *
 * This class is equivalent to a cci_value after calling @c set_map().
 * @see cci_value, cci_value_map_cref, cci_value_map_ref
 */
class cci_value_map
  : public cci_value_map_ref
{
  friend class cci_value;
  typedef cci_value_map this_type;
public:
  typedef cci_value_map_cref const_reference;
  typedef cci_value_map_ref  reference;

  cci_value_map();

  cci_value_map( this_type const & );
  cci_value_map( const_reference );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  cci_value_map( this_type && );
#endif // CCI_HAS_CXX_RVALUE_REFS

  this_type& operator=( this_type const& );
  this_type& operator=( const_reference );
#ifdef CCI_HAS_CXX_RVALUE_REFS
  this_type& operator=( this_type && );
#endif // CCI_HAS_CXX_RVALUE_REFS

  friend void swap(this_type& a, this_type& b) { a.swap(b); }
  void swap( reference that ) { reference::swap( that ); }
  void swap( this_type & );

  ~cci_value_map();

  /// @copydoc cci_value_cref::operator&
  const cci_value_map * operator&() const { return this; }
  /// @copydoc cci_value_cref::operator&
  cci_value_map * operator&() { return this; }

private:
  impl_type do_init();
  impl_type own_pimpl_;
};

inline
cci_value_map::cci_value_map()
  : reference(), own_pimpl_()
{
  do_init();
}

inline
cci_value_map::cci_value_map( this_type const & that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline
cci_value_map::cci_value_map( const_reference that )
  : reference(), own_pimpl_()
{
  do_init();
  *this = that;
}

inline cci_value_map &
cci_value_map::operator=( this_type const & that )
{
  return *this = const_reference(that);
}

// --------------------------------------------------------------------------

template<typename InputIt>
cci_value_list_ref::iterator
cci_value_list_ref::insert( const_iterator pos, InputIt first, InputIt last )
{
  iterator::difference_type offs = pos - begin();
  while( first != last )
    pos = insert( pos, cci_value(*first++) ) + 1;
  return begin() + offs;
}

CCI_CLOSE_NAMESPACE_

#undef CCI_VALUE_CONVERTER_
#undef CCI_VALUE_CHECKED_CONVERTER_
#undef CCI_VALUE_REQUIRES_CONVERTER_
#undef CCI_VALUE_MOVE_

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // CCI_CCI_VALUE_H_INCLUDED_
