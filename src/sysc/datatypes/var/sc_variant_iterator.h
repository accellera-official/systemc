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

#ifndef SC_DATATYPES_VARIANT_ITERATOR_H_INCLUDED_
#define SC_DATATYPES_VARIANT_ITERATOR_H_INCLUDED_

#include "sysc/kernel/sc_cmnhdr.h"
#include <iterator> // std::reverse_iterator

/**
 * @file   sc_variant_iterator.h
 * @author Philipp A. Hartmann, Intel
 */

namespace sc_dt {

// forward declarations
class sc_variant;
class sc_variant_cref;
class sc_variant_ref;
class sc_variant_list_cref;
class sc_variant_list_ref;
class sc_variant_map_cref;
class sc_variant_map_ref;
class sc_variant_map_elem_ref;
class sc_variant_map_elem_cref;
template<typename T> class sc_variant_iterator;

//@cond SC_HIDDEN_FROM_DOXYGEN
namespace sc_variant_impl {

/// helper class to avoid dangling pointers to sc_variant reference objects
template<typename T> struct proxy_ptr
{
  typedef T element_type;

  explicit proxy_ptr(const T& ref) : ref_(ref) {}

  T& operator*()  { return ref_; }
  T* operator->() { return ptr(); }

  bool operator==(const proxy_ptr& that) const { return ref_.is_same() == (*that).is_same(); }
  bool operator!=(const proxy_ptr& that) const { return !(*this == that); }

  static proxy_ptr pointer_to(element_type& elem) { return pointer(elem); }

private:
  // avoid addressof operator
  T* ptr() { return reinterpret_cast<T*>(&reinterpret_cast<unsigned char&>(ref_)); }
  T ref_; // extend lifetime of reference
};

/// helper class to implement a sc_variant_iterator
template<typename T> class iterator_impl
{
public:
  // std::iterator_traits types
  typedef T reference;          // return by value!
  typedef proxy_ptr<T> pointer; // return proxy pointer
  typedef typename T::value_type value_type; // "real" value type
  typedef std::ptrdiff_t difference_type;
  typedef std::random_access_iterator_tag iterator_category;

protected:
  typedef void* impl_type; //  type-punned pointer for now

  iterator_impl(impl_type r = NULL)
    : impl_(r) {}

  void increment(difference_type n = 1) { update(advance(n));  }
  void decrement(difference_type n = 1) { update(advance(-n)); }
  impl_type advance(difference_type n) const;

  difference_type distance(impl_type) const;

  int compare(impl_type) const;

  pointer   ptr()   const { return pointer(deref()); }
  reference deref() const { return reference(raw()); }
  impl_type raw()   const { return impl_; }

private:
  void update(impl_type i) { impl_ = i; }

  impl_type  impl_; // underlying iterator,
};

} // namespace sc_variant_impl
//@endcond

/**
 * @brief Generic iterator impementation for \ref sc_variant lists and maps
 *
 * @see sc_variant_list, sc_variant_map
 */
template<typename T>
class sc_variant_iterator
  : protected sc_variant_impl::iterator_impl<T>
{
  typedef sc_variant_impl::iterator_impl<T> impl;
  typedef sc_variant_iterator<typename T::const_reference> const_type;
  typedef sc_variant_iterator<typename T::reference>       nonconst_type;

  friend class sc_variant_list_cref;
  friend class sc_variant_list_ref;
  friend class sc_variant_map_cref;
  friend class sc_variant_map_ref;
  template<typename U> friend class sc_variant_iterator;

private:
  /// internal constructor
  sc_variant_iterator(typename impl::impl_type r) : impl(r) {}

public:
  /// @name C++ standard iterator types
  ///@{
  typedef typename impl::value_type        value_type;
  typedef typename impl::pointer           pointer;
  typedef typename impl::reference         reference;
  typedef typename impl::difference_type   difference_type;
  typedef typename impl::iterator_category iterator_category;
  ///@}

  /// constructs an invalid iterator (non-dereferencable, non-incrementable)
  sc_variant_iterator() : impl() {}
  /// constructs an iterator from a potentially less \c const iterator
  sc_variant_iterator(const nonconst_type & that)
    : impl(that.raw()) {}

  /// @name Unary increment/decrement operators
  ///@{
  sc_variant_iterator& operator++(){ impl::increment(); return *this; }
  sc_variant_iterator& operator--(){ impl::decrement(); return *this; }
  sc_variant_iterator  operator++(int)
    { sc_variant_iterator old(*this); impl::increment(); return old; }
  sc_variant_iterator  operator--(int)
    { sc_variant_iterator old(*this); impl::decrement(); return old; }
  ///@}

  /// @name Binary increment/decrement operators
  ///@{
  sc_variant_iterator  operator+( difference_type n ) const
    { return sc_variant_iterator( impl::advance(n) ); }
  sc_variant_iterator  operator-( difference_type n ) const
    { return sc_variant_iterator( impl::advance(-n) ); }

  sc_variant_iterator& operator+=( difference_type n )
    { impl::increment(n); return *this; }
  sc_variant_iterator& operator-=( difference_type n )
    { impl::decrement(n); return *this; }
  ///@}

  /** @name Comparison between iterators
   *  @note These operator support comparisons between compatible const/non-const iterators.
   */
  ///@{
  bool operator==( const_type that ) const { return impl::compare(that.raw()) == 0; }
  bool operator!=( const_type that ) const { return impl::compare(that.raw()) != 0; }
  bool operator<=( const_type that ) const { return impl::compare(that.raw()) <= 0; }
  bool operator>=( const_type that ) const { return impl::compare(that.raw()) >= 0; }
  bool operator< ( const_type that ) const { return impl::compare(that.raw()) <  0; }
  bool operator> ( const_type that ) const { return impl::compare(that.raw()) >  0; }
  ///@}

  /// @name Dereferencing the iterator
  ///@{
  pointer   operator&()  const { return impl::ptr(); }
  pointer   operator->() const { return impl::ptr(); }
  reference operator*()  const { return impl::deref();  }
  reference operator[]( difference_type n ) const { return (*this + n).deref(); }
  ///@}

  /// Computing the distance between two iterators (from the same range)
  difference_type operator-( const_type that ) const
    { return impl::distance(that.raw()); }

}; // class sc_variant_iterator

} // namespace sc_dt

#endif // SC_DATATYPES_VARIANT_ITERATOR_H_INCLUDED_
