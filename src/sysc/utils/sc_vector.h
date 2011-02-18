/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_vector.h - A vector of named (SystemC) objects (modules, ports, channels)

  Original Author: Philipp A. Hartmann, OFFIS

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SC_VECTOR_H_INCLUDED_
#define SC_VECTOR_H_INCLUDED_

#include <vector>
#include <iterator>
#include <string>

#include "sysc/kernel/sc_object.h"

//#define SC_VECTOR_HEADER_ONLY_

namespace sc_core {

// forward declarations
template< typename >           class sc_vector;
template< typename, typename > class sc_vector_assembly;
template< typename, typename > class sc_vector_iter;

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

class sc_vector_base
  : public sc_object
{

  template<typename,typename> friend class sc_vector_iter;
  template< typename Container, typename ArgumentIterator >
  friend typename Container::iterator
      sc_vector_do_bind( Container & cont
                       , ArgumentIterator  first
                       , ArgumentIterator  last
                       , typename Container::iterator from );
  template< typename Container, typename ArgumentIterator >
  friend typename Container::iterator
     sc_vector_do_paren( Container & cont
                       , ArgumentIterator  first
                       , ArgumentIterator  last
                       , typename Container::iterator from );


public:

  typedef std::vector< sc_object* >     storage_type;
  typedef storage_type::size_type       size_type;
  typedef storage_type::difference_type difference_type;

  const char * kind() const { return "sc_vector"; }

  storage_type const & get_elements() const
    { return vec_; }

  size_type size() const
    { return vec_.size(); }

protected:

  // begin implementation defined

  typedef storage_type::iterator        iterator;
  typedef storage_type::const_iterator  const_iterator;

  sc_vector_base();

  sc_vector_base( const char* prefix )
    : sc_object( prefix )
    , vec_()
  {}

  sc_object * & at( size_type i )
    { return vec_[i]; }

  sc_object const * at( size_type i ) const
    { return vec_[i]; }

  void reserve( size_type n )
    { vec_.reserve(n); }

  void clear()
    { vec_.clear(); }

  void push_back( sc_object* item )
    { vec_.push_back(item); }

  void check_index( size_type i ) const;
  bool check_init( size_type n )  const;

  static std::string make_name( const char* prefix, size_type index );

  iterator begin() { return vec_.begin(); }
  iterator end()   { return vec_.end();   }

  const_iterator begin() const { return vec_.begin(); }
  const_iterator end()   const { return vec_.end();   }

  // TODO: useful?
  // void report_incomplete_loop( const char* );

public: // @@@@#### Philipp has this private

  void report_empty_bind( const char* kind_, bool dst_range_ ) const;

private:
  storage_type vec_;

  // disabled
  sc_vector_base( const sc_vector_base& );
  sc_vector_base& operator=( const sc_vector_base& );

}; // sc_vector_base

// iterator access adapters
template< typename ElementType >
struct sc_direct_access
{
  typedef ElementType type;
  type* get( type* this_ ) const
    { return this_; }
};

// iterator access adapters
template< typename ElementType
        , typename AccessType   >
struct sc_member_access
{
  typedef ElementType element_type;
  typedef AccessType  access_type;
  typedef access_type (element_type::*member_type);
  typedef access_type type;

  sc_member_access( member_type ptr )
    : ptr_(ptr) {}

  access_type * get( element_type* this_ ) const
    { return &(this_->*ptr_); }

private:
  member_type ptr_;
}; // sc_member_access


template< typename ElementType
        , typename AccessPolicy = sc_direct_access<ElementType> >
class sc_vector_iter
  : public std::iterator< std::random_access_iterator_tag
                        , typename AccessPolicy::type >
  , private AccessPolicy
{
  typedef ElementType  element_type;
  typedef AccessPolicy access_policy;
  typedef typename access_policy::type access_type;

  // simplistic version to avoid Boost et.al.
  template< typename U > struct remove_const { typedef U type; };
  template< typename U > struct remove_const<const U> { typedef U type; };
  typedef typename remove_const<ElementType>::type plain_type;

  friend class sc_vector< plain_type >;
  template< typename, typename > friend class sc_vector_assembly;

  typedef std::iterator< std::random_access_iterator_tag, access_type > base_type;
  typedef sc_vector_iter               this_type;
  typedef sc_vector<plain_type>        vector_type;
  typedef sc_vector_base::storage_type storage_type;

  // select correct base-type iterator
  template< typename U > struct select_iter
    { typedef typename storage_type::iterator type;  };
  template< typename U > struct select_iter< const U >
    { typedef typename storage_type::const_iterator type;  };

  typedef typename select_iter<ElementType>::type raw_iterator;
  raw_iterator it_;

  sc_vector_iter( raw_iterator it, access_policy acc = access_policy() )
    : access_policy(acc), it_(it) {}

public:
  // interface for Random Access Iterator category,
  // see ISO/IEC 14882:2003(E), 24.1 [lib.iterator.requirements]

  typedef typename base_type::difference_type difference_type;
  typedef typename base_type::reference       reference;
  typedef typename base_type::pointer         pointer;

  sc_vector_iter() : access_policy(), it_() {}

  // step
  this_type& operator++(){ ++it_; return *this; }
  this_type& operator--(){ --it_; return *this; }
  this_type  operator++(int){ this_type old(*this); ++it_; return old; }
  this_type  operator--(int){ this_type old(*this); --it_; return old; }

  // advance
  this_type  operator+( difference_type n ) { return it_ + n; }
  this_type  operator-( difference_type n ) { return it_ - n; }
  this_type& operator+=( difference_type n ) { it_+=n; return *this; }
  this_type& operator-=( difference_type n ) { it_-=n; return *this; }

  // relations
  bool operator== ( const this_type& that ) const { return it_ == that.it_; }
  bool operator!= ( const this_type& that ) const { return it_ != that.it_; }
  bool operator<= ( const this_type& that ) const { return it_ <= that.it_; }
  bool operator>= ( const this_type& that ) const { return it_ >= that.it_; }
  bool operator<  ( const this_type& that ) const { return it_ < that.it_; }
  bool operator>  ( const this_type& that ) const { return it_ > that.it_; }

  // dereference
  reference operator*() const
    { return *access_policy::get( static_cast<element_type*>(*it_) ); }
  pointer   operator->() const
    { return access_policy::get( static_cast<element_type*>(*it_) ); }
  reference operator[]( difference_type n ) const
    { return *access_policy::get( static_cast<element_type*>(it_[n]) ); }

  // distance
  difference_type operator-( this_type const& that ) const
    { return it_-that.it_; }

}; // sc_vector_iter

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

  explicit sc_vector( const char* prefix, size_type n )
    : base_type( prefix )
    { init(n); }

  template< typename Creator >
  sc_vector( const char* prefix, size_type n, Creator creator )
    : base_type( prefix )
  {
    init( n, creator );
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

  void init( size_type n )
    { init( n, &this_type::create_element ); }

  template< typename Creator >
  void init( size_type n, Creator c );

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
  sc_vector_assembly<T,MT> assemble( MT (element_type::*member_ptr) )
    { return sc_vector_assembly<T,MT>( *this, member_ptr ); }

protected:

  void clear();

};

template< typename T, typename MT >
class sc_vector_assembly
{
  template< typename > friend class sc_vector;

  template< typename Container, typename ArgumentIterator >
  friend typename Container::iterator
      sc_vector_do_bind( Container & cont
                       , ArgumentIterator  first
                       , ArgumentIterator  last
                       , typename Container::iterator from );
  template< typename Container, typename ArgumentIterator >
  friend typename Container::iterator
     sc_vector_do_paren( Container & cont
                       , ArgumentIterator  first
                       , ArgumentIterator  last
                       , typename Container::iterator from );

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


  typedef access_type (element_type::*member_type);

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

  sc_vector_assembly( const sc_vector_assembly & other )
    : vec_( other.vec_ )
    , ptr_( other.ptr_ )
    , child_vec_(0)
  {}

  sc_vector_assembly& operator=( const sc_vector_assembly& other )
  {
    vec_ = other.vec_;
    ptr_ = other.ptr_;
    delete child_vec_;
    child_vec_ = 0;
  }

  ~sc_vector_assembly()
    { delete child_vec_; }

private:
  sc_vector_assembly( base_type & v, member_type ptr )
    : vec_(&v)
    , ptr_(ptr)
    , child_vec_(0)
  {}
  void report_empty_bind( const char* kind_, bool dst_empty_ ) const
    { vec_->report_empty_bind( kind_, dst_empty_ ); }

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
sc_vector<T>::init( size_type n, Creator c )
{
  if ( base_type::check_init(n) )
  {
    base_type::reserve( n );
    try
    {
      for ( size_type i = 0; i<n; ++i )
      {
        // this workaround is needed for SystemC 2.2/2.3 sc_bind
        std::string  name  = make_name( basename(), i );
        const char*  cname = name.c_str();

        sc_object* p = c( cname, i ) ; // call Creator
        base_type::push_back(p);
      }
    }
    catch ( ... )
    {
      clear();
      throw;
    }
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
    base_type::at(i) = 0;
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
  {
    sc_object * p = static_cast<sc_object*>( const_cast<MT*>( &*it ) );
    child_vec_->push_back( p );
  }
  return *child_vec_;
}

} // namespace sc_core

#endif // SC_VECTOR_H_INCLUDED_
// Taf!
