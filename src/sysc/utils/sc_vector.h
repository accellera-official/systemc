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

/*****************************************************************************

  sc_vector.h - A vector of named (SystemC) objects (modules, ports, channels)

  Original Author: Philipp A. Hartmann, OFFIS

  CHANGE LOG AT END OF FILE
 *****************************************************************************/

#ifndef SC_VECTOR_H_INCLUDED_
#define SC_VECTOR_H_INCLUDED_

#include <algorithm> // std::swap
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "sysc/kernel/sc_object.h"
#include "sysc/kernel/sc_stage_callback_if.h"

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(push)
#pragma warning(disable: 4251) // DLL import for std::vector
#endif

namespace sc_core {
// forward declarations
template< typename T >              class sc_vector;
template< typename T, typename MT > class sc_vector_assembly;
template< typename T, typename MT > class sc_vector_iter;

// implementation-defined
template< typename Container, typename ArgumentIterator >
typename Container::iterator
sc_vector_do_bind( Container & cont
                 , ArgumentIterator  first
                 , ArgumentIterator  last
                 , typename Container::iterator from );

// implementation-defined
template< typename Container, typename ArgumentIterator >
typename Container::iterator
sc_vector_do_operator_paren( Container & cont
                           , ArgumentIterator  first
                           , ArgumentIterator  last
                           , typename Container::iterator from );

enum sc_vector_init_policy
{
    SC_VECTOR_LOCK_AFTER_INIT,
    SC_VECTOR_LOCK_AFTER_ELABORATION
};

class sc_vector_element;  // opaque pointer

class SC_API sc_vector_base
  : public sc_object
  , public sc_stage_callback_if
{

  template<typename,typename> friend class sc_vector_assembly;
  template<typename,typename> friend class sc_vector_iter;

public:

  typedef sc_vector_element*            handle_type;
  typedef std::vector< handle_type >    storage_type;
  typedef storage_type::size_type       size_type;
  typedef storage_type::difference_type difference_type;

  const char * kind() const { return "sc_vector"; }

  std::vector<sc_object*> const & get_elements() const;

  size_type size() const
    { return vec_.size(); }

  void lock() { locked = true; };

  bool is_locked() const { return locked; }

protected:

  // begin implementation defined

  typedef storage_type::iterator        iterator;
  typedef storage_type::const_iterator  const_iterator;

  sc_vector_base();

  sc_vector_base( const char* prefix )
    : sc_object( prefix )
    , vec_()
    , objs_vec_(0)
    , locked(false)
  {
      init_lock_cb();
  }

  ~sc_vector_base()
    { delete objs_vec_; }

  void * at( size_type i )
    { return vec_[i]; }

  void const * at( size_type i ) const
    { return vec_[i]; }

  void reserve( size_type n )
    { vec_.reserve(n); }

  void clear()
    { vec_.clear(); }

  void push_back( void* item )
    { vec_.push_back( static_cast<handle_type>(item) ); }

  bool check_locked() const;
  void check_index( size_type i ) const;
  bool check_init( size_type n )  const;

  static std::string make_name( const char* prefix, size_type index );

  iterator begin() { return vec_.begin(); }
  iterator end()   { return vec_.end();   }

  const_iterator begin() const { return vec_.begin(); }
  const_iterator end()   const { return vec_.end();   }

  virtual sc_object* object_cast( void* ) const = 0;

  sc_object* implicit_cast( sc_object* p ) const { return p; }
  sc_object* implicit_cast( ... /* incompatible */ )  const;

  void init_lock_cb();
  void stage_callback(const sc_stage & stage); // implement callback

public: 
  void report_empty_bind( const char* kind_, bool dst_range_ ) const;

private:
  storage_type vec_;
  mutable std::vector< sc_object* >* objs_vec_;
  bool locked;

  // disabled
  sc_vector_base( const sc_vector_base& );
  sc_vector_base& operator=( const sc_vector_base& );

}; // sc_vector_base

// helper check for constness
template< typename CT, typename T >
inline constexpr bool sc_is_more_const_v =
     std::is_same_v<std::remove_const_t<CT>, std::remove_const_t<T>> &&
     std::is_const_v<CT> >= std::is_const_v<T>;

// iterator access adapters
template< typename ElementType >
struct sc_direct_access
{
  typedef ElementType               element_type;
  typedef element_type              type;
  typedef std::remove_const_t<type> plain_type;

  typedef sc_direct_access< type >             policy;
  typedef sc_direct_access< plain_type >       non_const_policy;
  typedef sc_direct_access< const plain_type > const_policy;

  sc_direct_access(){}
  // convert from any policy to (const) direct policy
  template< typename U
          , typename = std::enable_if_t<sc_is_more_const_v<type, typename U::policy::element_type>>>
  sc_direct_access( const U& ) {}

  type* get( type* this_ ) const
    { return this_; }
};

// iterator access adapters
template< typename ElementType
        , typename AccessType   >
class sc_member_access
{
  public:
    template< typename, typename > friend class sc_member_access;

    typedef ElementType element_type;
    typedef AccessType  access_type;
    typedef access_type (ElementType::*member_type);
    typedef access_type type;
    typedef std::remove_const_t<type> plain_type;
    typedef std::remove_const_t<ElementType> plain_elem_type;

    typedef sc_member_access< element_type, access_type > policy;
    typedef sc_member_access< plain_elem_type, plain_type >
      non_const_policy;
    typedef sc_member_access< const plain_elem_type, const plain_type >
      const_policy;
  
    sc_member_access( member_type ptr )
      : ptr_(ptr) {}
  
    sc_member_access( const non_const_policy& other )
      : ptr_(other.ptr_)
    {}
  
    access_type * get( element_type* this_ ) const
      { return &(this_->*ptr_); }
  
  private:
    member_type ptr_;
}; // sc_member_access
  

template< typename ElementType
        , typename AccessPolicy = sc_direct_access<ElementType> >
class sc_vector_iter
  : private AccessPolicy
{
  typedef ElementType  element_type;
  typedef typename AccessPolicy::policy            access_policy;
  typedef typename AccessPolicy::non_const_policy  non_const_policy;
  typedef typename AccessPolicy::const_policy      const_policy;

  typedef std::remove_const_t<ElementType> plain_type;
  typedef const plain_type                 const_plain_type;
  typedef typename sc_direct_access<plain_type>::const_policy
    const_direct_policy;

  friend class sc_vector< plain_type >;
  template< typename, typename > friend class sc_vector_assembly;
  template< typename, typename > friend class sc_vector_iter;

  typedef sc_vector_iter               this_type;
  typedef sc_vector<plain_type>        vector_type;
  typedef sc_vector_base::storage_type storage_type;

  // select correct base-type iterator
  template< typename U > struct select_iter
    { typedef typename storage_type::iterator       type; };
  template< typename U > struct select_iter< const U >
    { typedef typename storage_type::const_iterator type; };

  typedef typename select_iter<ElementType>::type          raw_iterator;
  typedef sc_vector_iter< const_plain_type, const_policy > const_iterator;
  typedef sc_vector_iter< const_plain_type, const_direct_policy >
    const_direct_iterator;

  // underlying vector iterator

  raw_iterator it_;

  sc_vector_iter( raw_iterator it, access_policy acc = access_policy() )
    : access_policy(acc), it_(it) {}

  access_policy const & get_policy() const { return *this; }

public:
  // interface for Random Access Iterator category,
  // see ISO/IEC 14882:2003(E), 24.1 [lib.iterator.requirements]

  typedef std::random_access_iterator_tag iterator_category;
  typedef std::ptrdiff_t                  difference_type;
  typedef typename access_policy::type    value_type;
  typedef value_type &                    reference;
  typedef value_type *                    pointer;

  sc_vector_iter() : access_policy(), it_() {}

  // iterator conversions to more const, and/or direct iterators
  //
  // Note: There is a minor risk to match unrelated classes (i.e. not sc_vector_iter<T,POL>),
  //       but MSVC 2005 does not correctly consider a restricted conversion constructor
  //       sc_vector_iter( const sc_vector_iter<OtherType,OtherPolicy>, SC_ENABLE_IF_ ...).
  //       To reduce this risk, the types used in the enable-if condition could be further
  //       tailored towards sc_vector(_iter), should the need arise.
  //
  // See also: sc_direct_access conversion constructor
  template< typename It
          , typename = std::enable_if_t<sc_is_more_const_v<element_type, typename It::policy::element_type>>>
  sc_vector_iter( const It& it )
    : access_policy( it.get_policy() ), it_( it.it_ )
  {}

  // step
  this_type& operator++(){ ++it_; return *this; }
  this_type& operator--(){ --it_; return *this; }
  this_type  operator++(int){ this_type old(*this); ++it_; return old; }
  this_type  operator--(int){ this_type old(*this); --it_; return old; }

  // advance
  this_type  operator+( difference_type n ) const
    { return this_type( it_ + n, get_policy()); }
  this_type  operator-( difference_type n ) const
    { return this_type( it_ - n, get_policy()); }

  this_type& operator+=( difference_type n ) { it_+=n; return *this; }
  this_type& operator-=( difference_type n ) { it_-=n; return *this; }

  // dereference
  reference operator*() const
    { return *access_policy::get( static_cast<element_type*>((void*)*it_) ); }
  pointer   operator->() const
    { return access_policy::get( static_cast<element_type*>((void*)*it_) ); }
  reference operator[]( difference_type n ) const
    { return *access_policy::get( static_cast<element_type*>((void*)it_[n]) ); }

  // distance
  difference_type operator-( const_direct_iterator const& that ) const
    { return it_-that.it_; }

  raw_iterator raw() const { return it_; }

}; // sc_vector_iter


// sc_vector_iter relations
template< typename T1, typename Pol1, typename T2, typename Pol2
        , typename = std::enable_if_t<std::is_same_v<std::remove_const_t<T1>, std::remove_const_t<T2>>>>
inline bool
operator==( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b )
  { return a.raw() == b.raw(); }

template< typename T1, typename Pol1, typename T2, typename Pol2 >
inline auto
operator!=( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b ) -> decltype( !(a == b) )
  { return !(a == b); }

template< typename T1, typename Pol1, typename T2, typename Pol2
        , typename = std::enable_if_t<std::is_same_v<std::remove_const_t<T1>, std::remove_const_t<T2>>>>
inline bool
operator<( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b )
  { return a.raw() < b.raw(); }

template< typename T1, typename Pol1, typename T2, typename Pol2 >
inline auto
operator>=( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b ) -> decltype( !(a < b) )
  { return !(a < b); }

template< typename T1, typename Pol1, typename T2, typename Pol2 >
inline auto
operator>( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b ) -> decltype( (b < a) )
  { return b < a; }

template< typename T1, typename Pol1, typename T2, typename Pol2 >
inline auto
operator<=( const sc_vector_iter<T1,Pol1>& a, const sc_vector_iter<T2,Pol2> & b ) -> decltype( !(b < a) )
  { return !(b < a); }


template< typename T >
class sc_vector
  : public sc_vector_base
{
  typedef sc_vector_base base_type;
  typedef sc_vector<T>   this_type;

public:
  typedef T element_type;
  typedef sc_vector_iter< element_type >       iterator;
  typedef sc_vector_iter< const element_type > const_iterator;

  sc_vector(){}

  explicit sc_vector( const char* prefix )
    : base_type( prefix )
  {}

  sc_vector( const char* prefix, size_type n,
             sc_vector_init_policy init_pol = SC_VECTOR_LOCK_AFTER_INIT)
    : base_type( prefix )
  {
    init(n, init_pol);
  }

  template< typename Creator >
  sc_vector( const char* prefix, size_type n, Creator creator,
             sc_vector_init_policy init_pol = SC_VECTOR_LOCK_AFTER_INIT )
    : base_type( prefix )
  {
    init( n, creator, init_pol );
  }

  virtual ~sc_vector();

  element_type& operator[]( size_type i )
    { return *static_cast<element_type*>( base_type::at(i) ); }

  element_type& at( size_type i )
    { check_index(i); return (*this)[i]; }

  const element_type& operator[]( size_type i ) const
    { return *static_cast<element_type const *>( base_type::at(i) ); }

  const element_type& at( size_type i ) const
    { check_index(i); return (*this)[i]; }

  void init( size_type n,
             sc_vector_init_policy init_pol = SC_VECTOR_LOCK_AFTER_INIT )
    { init( n, &this_type::create_element, init_pol ); }

  template< typename Creator >
  void init( size_type n, Creator c,
             sc_vector_init_policy init_pol = SC_VECTOR_LOCK_AFTER_INIT);

  // Append element with automatically generated name
  // T(generated_name, args...)
  template< typename... Args >
  void emplace_back( Args&&... args );

  // Append element with user-defined name
  template< typename... Args >
  void emplace_back_with_name(Args &&... args);

  static element_type * create_element( const char* prefix, size_type index );

  iterator begin() { return base_type::begin(); }
  iterator end()   { return base_type::end();   }

  const_iterator begin()  const { return base_type::begin(); }
  const_iterator end()    const { return base_type::end(); }

  const_iterator cbegin() const { return base_type::begin(); }
  const_iterator cend()   const { return base_type::end(); }

  template< typename ContainerType, typename ArgumentType >
  iterator bind( sc_vector_assembly<ContainerType,ArgumentType> c )
    { return bind( c.begin(), c.end() ); }

  template< typename BindableContainer >
  iterator bind( BindableContainer & c )
    { return bind( c.begin(), c.end() ); }

  template< typename BindableIterator >
  iterator bind( BindableIterator first, BindableIterator last )
    { return bind( first, last, this->begin() ); }

  template< typename BindableIterator >
  iterator bind( BindableIterator first, BindableIterator last
               , iterator from )
    { return sc_vector_do_bind( *this, first, last, from ); }

  template< typename ContainerType, typename ArgumentType >
  iterator operator()( sc_vector_assembly<ContainerType,ArgumentType> c )
    { return operator()( c.begin(), c.end() ); }

  template< typename ArgumentContainer >
  iterator operator()( ArgumentContainer & c )
    { return operator()( c.begin(), c.end() ); }

  template< typename ArgumentIterator >
  iterator operator()( ArgumentIterator first, ArgumentIterator last )
    { return operator()( first, last, this->begin() ); }

  template< typename ArgumentIterator >
  iterator operator()( ArgumentIterator first, ArgumentIterator last
                     , iterator from )
    { return sc_vector_do_operator_paren( *this, first, last, from ); }

  // member-wise access

  template< typename MT >
  sc_vector_assembly<T,MT> assemble( MT (T::*member_ptr) )
    { return sc_vector_assembly<T,MT>( *this, member_ptr ); }

protected:

  void clear();

  virtual sc_object* object_cast( void* p ) const
    { return implicit_cast( static_cast<element_type*>(p) ); }

};

template< typename T, typename MT >
class sc_vector_assembly
{
  template< typename U > friend class sc_vector;

public:

  typedef sc_vector<T> base_type;

  typedef sc_vector_iter< T, sc_member_access<T, MT> >          iterator;
  typedef sc_vector_iter< const T
                        , sc_member_access<const T, const MT> > const_iterator;

  typedef T  element_type;
  typedef MT access_type;

  typedef typename base_type::size_type       size_type;
  typedef typename base_type::difference_type difference_type;
  typedef typename iterator::reference        reference;
  typedef typename iterator::pointer          pointer;
  typedef typename const_iterator::reference  const_reference;
  typedef typename const_iterator::pointer    const_pointer;


  typedef access_type (T::*member_type);

  const char* name() const { return vec_->name(); }
  const char* kind() const { return "sc_vector_assembly"; }

  iterator begin()
    { return iterator( (*vec_).begin().it_, ptr_ ); }
  iterator end()
    { return iterator( (*vec_).end().it_, ptr_ ); }

  const_iterator cbegin() const
    { return const_iterator( (*vec_).cbegin().it_, ptr_ ); }
  const_iterator cend() const
    { return const_iterator( (*vec_).cend().it_, ptr_ ); }

  const_iterator begin() const
    { return const_iterator( (*vec_).begin().it_, ptr_ ); }
  const_iterator end()   const
    { return const_iterator( (*vec_).end().it_, ptr_ ); }

  size_type size() const { return vec_->size(); }
  const std::vector< sc_object* > & get_elements() const;

  reference operator[]( size_type idx )
    { return (*vec_)[idx].*ptr_; }
  reference at( size_type idx )
    { return vec_->at(idx).*ptr_; }
  const_reference operator[]( size_type idx ) const
    { return (*vec_)[idx].*ptr_; }
  const_reference at( size_type idx ) const
    { return vec_->at(idx).*ptr_; }

  template< typename ContainerType, typename ArgumentType >
  iterator bind( sc_vector_assembly<ContainerType,ArgumentType> c )
    { return bind( c.begin(), c.end() ); }

  template< typename BindableContainer >
  iterator bind( BindableContainer & c )
    { return bind( c.begin(), c.end() ); }

  template< typename BindableIterator >
  iterator bind( BindableIterator first, BindableIterator last )
    { return bind( first, last, this->begin() ); }

  template< typename BindableIterator >
  iterator bind( BindableIterator first, BindableIterator last
               , iterator from )
    { return sc_vector_do_bind( *this, first, last, from ); }

  template< typename BindableIterator >
  iterator bind( BindableIterator first, BindableIterator last
               , typename base_type::iterator from )
    { return bind( first, last, iterator(from.it_, ptr_) ); }

  template< typename ContainerType, typename ArgumentType >
  iterator operator()( sc_vector_assembly<ContainerType,ArgumentType> c )
    { return operator()( c.begin(), c.end() ); }

  template< typename ArgumentContainer >
  iterator operator()( ArgumentContainer & c )
    { return operator()( c.begin(), c.end() ); }

  template< typename ArgumentIterator >
  iterator operator()( ArgumentIterator first, ArgumentIterator last )
    { return operator()( first, last, this->begin() ); }

  template< typename ArgumentIterator >
  iterator operator()( ArgumentIterator first, ArgumentIterator last
                     , iterator from )
    { return sc_vector_do_operator_paren( *this, first, last, from ); }

  template< typename ArgumentIterator >
  iterator operator()( ArgumentIterator first, ArgumentIterator last
                     , typename base_type::iterator from )
    { return operator()( first, last, iterator(from.it_, ptr_) ); }

  sc_vector_assembly( const sc_vector_assembly & other )
    : vec_( other.vec_ )
    , ptr_( other.ptr_ )
    , child_vec_(0)
  {}

  sc_vector_assembly& operator=( sc_vector_assembly other_copy )
  {
    swap( other_copy );
    return *this;
  }

  void swap( sc_vector_assembly & that )
  {
    using std::swap;
    swap( vec_,       that.vec_ );
    swap( ptr_,       that.ptr_ );
    swap( child_vec_, that.child_vec_ );
  }

  void report_empty_bind( const char* kind_, bool dst_empty_ ) const
    { vec_->report_empty_bind( kind_, dst_empty_ ); }

  ~sc_vector_assembly()
    { delete child_vec_; }

private:

  sc_vector_assembly( base_type & v, member_type ptr )
    : vec_(&v)
    , ptr_(ptr)
    , child_vec_(0)
  {}

  sc_object* object_cast( pointer p ) const
    { return vec_->implicit_cast( p ); }

  base_type * vec_;
  member_type ptr_;

  mutable std::vector< sc_object* >* child_vec_;
};

template< typename T, typename MT >
sc_vector_assembly<T,MT>
sc_assemble_vector( sc_vector<T> & vec, MT (T::*ptr) )
{
  return vec.assemble( ptr );
}

template< typename T >
typename sc_vector<T>::element_type *
sc_vector<T>::create_element( const char* name, size_type /* idx */ )
{
  return new T( name );
}

template< typename T >
template< typename Creator >
void
sc_vector<T>::init( size_type n, Creator c, sc_vector_init_policy init_pol )
{
  if ( base_type::check_init(n) )
  {
    base_type::reserve( n );

    // restore SystemC hierarchy context, if needed
    sc_hierarchy_scope scope( get_hierarchy_scope() );
    try
    {
      for ( size_type i = 0; i<n; ++i )
      {
        // this workaround is needed for SystemC 2.2/2.3 sc_bind
        std::string  name  = make_name( basename(), i );
        const char*  cname = name.c_str();

        element_type* p = c( cname, i ) ; // call Creator
        base_type::push_back(p);
      }
    }
    catch ( ... )
    {
      clear();
      throw;
    }

    if (init_pol == SC_VECTOR_LOCK_AFTER_INIT)
        base_type::lock();

  }
}

template< typename T >
template< typename... Args >
void sc_vector<T>::emplace_back( Args&&... args ) {
  if (check_locked()) {
    sc_hierarchy_scope scope( get_hierarchy_scope() );
    T* p = new T( make_name( basename(), size()).c_str() ,
                  std::forward<Args>(args)...);
    base_type::push_back(p);
  }
}

template< typename T >
template< typename... Args >
void sc_vector<T>::emplace_back_with_name(Args &&... args) {
  if (check_locked()) {
    sc_hierarchy_scope scope( get_hierarchy_scope() );
    T *p = new T(std::forward<Args>(args)...);
    base_type::push_back(p);
  }
}

template< typename T >
void
sc_vector<T>::clear()
{
  size_type i = size();
  while ( i --> 0 )
  {
    delete &( (*this)[i] );
  }
  base_type::clear();
}

template< typename Container, typename ArgumentIterator >
typename Container::iterator
sc_vector_do_bind( Container & cont
                 , ArgumentIterator  first
                 , ArgumentIterator  last
                 , typename Container::iterator from )
{
  typename Container::iterator end = cont.end();

  if( !cont.size() || from == end || first == last )
      cont.report_empty_bind( cont.kind(), from == end );

  while( from!=end && first != last )
    (*from++).bind( *first++ );
  return from;
}

template< typename Container, typename ArgumentIterator >
typename Container::iterator
sc_vector_do_operator_paren( Container& cont
                           , ArgumentIterator  first
                           , ArgumentIterator  last
                           , typename Container::iterator from )
{
  typename Container::iterator end = cont.end();

  if( !cont.size() || from == end || first == last )
      cont.report_empty_bind( cont.kind(), from == end );

  while( from!=end && first != last )
    (*from++)( *first++ );
  return from;
}

template< typename T >
sc_vector<T>::~sc_vector()
{
  clear();
}

template< typename T, typename MT >
std::vector< sc_object* > const &
sc_vector_assembly<T,MT>::get_elements() const
{
  if( !child_vec_ )
    child_vec_ = new std::vector< sc_object* >;

  if( child_vec_->size() || !size() )
    return *child_vec_;

  child_vec_->reserve( size() );
  for( const_iterator it=begin(); it != end(); ++it )
    if( sc_object * obj = object_cast( const_cast<MT*>(&*it) ) )
      child_vec_->push_back( obj );

  return *child_vec_;
}

} // namespace sc_core

#undef SC_RPTYPE_
#undef SC_ENABLE_IF_

#if defined(_MSC_VER) && !defined(SC_WIN_DLL_WARN)
#pragma warning(pop)
#endif

// $Log: sc_vector.h,v $
// Revision 1.17  2011/08/26 20:46:20  acg
//  Andy Goodrich: moved the modification log to the end of the file to
//  eliminate source line number skew when check-ins are done.
//
// Revision 1.16  2011/07/25 10:21:17  acg
//  Andy Goodrich: check in aftermath of call to automake.
//
// Revision 1.15  2011/04/02 00:04:32  acg
//  Philipp A. Hartmann: fix distance from member iterators, and
//  add iterator conversions.
//
// Revision 1.14  2011/04/01 22:35:19  acg
//  Andy Goodrich: spelling fix.
//
// Revision 1.13  2011/03/28 13:03:09  acg
//  Andy Goodrich: Philipp's latest update.
//
// Revision 1.12  2011/03/23 16:16:28  acg
//  Philipp A. Hartmann: rebase implementation on void*
//      - supports virtual inheritance from sc_object again
//      - build up get_elements result on demand
//      - still requires element type to be derived from sc_object
//
// Revision 1.11  2011/02/19 16:46:36  acg
//  Andy Goodrich: finally get the update from Philipp correct!
//

#endif // SC_VECTOR_H_INCLUDED_
// Taf!
