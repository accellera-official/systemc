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

/**
 * @file   cci_value.cpp
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

#if defined(_MSC_VER) && _MSC_VER <= 1600
# pragma warning(disable:4661)
# define CCI_TPLEXTERN_
#endif // excluded from MSVC'2010

#include "cci_core/cci_value.h"
#include "cci_core/rapidjson.h"
#include "cci_cfg/cci_report_handler.h"

#include <algorithm> // std::swap
#include <sstream> //std::stringstream

namespace rapidjson = RAPIDJSON_NAMESPACE;

CCI_OPEN_NAMESPACE_

///@cond CCI_HIDDEN_FROM_DOXYGEN
namespace /* anonymous */ {

typedef rapidjson::CrtAllocator allocator_type;
typedef rapidjson::UTF8<>       encoding_type;
typedef rapidjson::GenericValue<encoding_type, allocator_type>    json_value;
typedef rapidjson::GenericMember<encoding_type, allocator_type>   json_member;
typedef rapidjson::GenericDocument<encoding_type, allocator_type> json_document;

typedef json_value::ValueIterator   json_value_iter;
typedef json_value::MemberIterator  json_member_iter;

static allocator_type json_allocator;

// wrapper implementation is simply a RapidJSON value
typedef json_value impl_type;

// helper to convert to wrapper implementation
static inline impl_type* impl_cast(void* p)
  { return static_cast<impl_type*>(p); }

// simple memory pool for impl wrapper objects (non-deallocating)
//  - this pool handles the "top-level" objects only
//  - RapidJSON allocation is still done via a plain malloc/free allocator
struct impl_pool
{
  static impl_type* allocate()
  {
    impl_type* ret = free_list_;
    if (free_list_ != NULL) {
      free_list_ = *reinterpret_cast<impl_type**>(free_list_);
      new (ret) impl_type(); // initialize JSON object (placement new)
    } else {
      ret = new impl_type();
    }
    return ret;
  }

  static void deallocate(impl_type* elem) {
    if (elem == NULL) return; // delete NULL is no-op
    elem->~impl_type();       // release internal memory (not pooled)
     *reinterpret_cast<impl_type**>(elem) = free_list_;
    free_list_ = elem;
  }
private:
  static impl_type* free_list_;
};
impl_type* impl_pool::free_list_;

} // anonymous namespace

#define PIMPL( x ) \
  (impl_cast((x).pimpl_))

#define DEREF( x ) \
  (*PIMPL(x))

#define THIS \
  (PIMPL(*this))

#define VALUE_ASSERT( Cond, Msg ) \
  do { if( !(Cond) ) { \
    cci_report_handler::cci_value_failure \
      ( Msg "\n Condition: " #Cond, __FILE__, __LINE__ ); \
    cci_abort(); /* cannot recover from here */ \
  } } while( false )
///@endcond

// ----------------------------------------------------------------------------
// cci_value_cref

void
cci_value_cref::report_error( const char* msg, const char* file, int line ) const
{
    cci_report_handler::cci_value_failure( msg, file, line );
}

bool
operator == ( cci_value_cref const & left, cci_value_cref const & right )
{
  if( PIMPL(left) == PIMPL(right) )
    return true;

  if( !PIMPL(left) || !PIMPL(right) ) { // only one is set
    // equal, if both are conceptually null
    return ( PIMPL(left)  && DEREF(left).IsNull() )
        || ( PIMPL(right) && DEREF(right).IsNull() ) ;
  }

  return DEREF(left) == DEREF(right);
}

cci_value_category
cci_value_cref::category() const
{
  if( !THIS )
    return CCI_NULL_VALUE;

  switch(THIS->GetType())
  {
  case rapidjson::kFalseType:
  case rapidjson::kTrueType:
    return CCI_BOOL_VALUE;

  case rapidjson::kNumberType:
    return THIS->IsDouble() ? CCI_REAL_VALUE : CCI_INTEGRAL_VALUE;

  case rapidjson::kStringType:
    return CCI_STRING_VALUE;

  case rapidjson::kArrayType:
    return CCI_LIST_VALUE;

  case rapidjson::kObjectType:
    return CCI_OTHER_VALUE;

  case rapidjson::kNullType:
  default:
    return CCI_NULL_VALUE;
  }
}

bool cci_value_cref::is_null() const
  { return !THIS || THIS->IsNull(); }

bool cci_value_cref::is_bool() const
  { return THIS && THIS->IsBool(); }

bool cci_value_cref::is_int() const
  { return THIS && THIS->IsInt(); }

bool cci_value_cref::is_int64() const
  { return THIS && THIS->IsInt64(); }

bool cci_value_cref::is_uint() const
  { return THIS && THIS->IsUint(); }

bool cci_value_cref::is_uint64() const
  { return THIS && THIS->IsUint64(); }

bool cci_value_cref::is_double() const
  { return THIS && THIS->IsDouble(); }

bool cci_value_cref::is_string() const
  { return THIS && THIS->IsString(); }

bool cci_value_cref::is_list() const
  { return THIS && THIS->IsArray(); }

bool cci_value_cref::is_map() const
  { return THIS && THIS->IsObject(); }

#define ASSERT_TYPE( Cond ) \
  VALUE_ASSERT( Cond, "invalid type access" )

bool cci_value_cref::get_bool() const
{
  ASSERT_TYPE(is_bool());
  return THIS->GetBool();
}

int cci_value_cref::get_int() const
{
  ASSERT_TYPE(is_int());
  return THIS->GetInt();
}

unsigned cci_value_cref::get_uint() const
{
  ASSERT_TYPE(is_uint());
  return THIS->GetUint();
}

int64 cci_value_cref::get_int64() const
{
  ASSERT_TYPE(is_int64());
  return THIS->GetInt64();
}

uint64 cci_value_cref::get_uint64() const
{
  ASSERT_TYPE(is_uint64());
  return THIS->GetUint64();
}

double cci_value_cref::get_double() const
{
  ASSERT_TYPE(is_number());
  return THIS->GetDouble();
}

cci_value_string_cref cci_value_cref::get_string() const
{
  ASSERT_TYPE(is_string());
  return cci_value_string_cref(pimpl_);
}

cci_value_list_cref cci_value_cref::get_list() const
{
  ASSERT_TYPE(is_list());
  return cci_value_list_cref(pimpl_);
}

cci_value_map_cref cci_value_cref::get_map() const
{
  ASSERT_TYPE(is_map());
  return cci_value_map_cref(pimpl_);
}

std::ostream& operator<<( std::ostream& os, cci_value_cref const& v )
{
  if( v.is_null() ) {
    os << "null";
  } else {
    rapidjson::OStreamWrapper wos(os);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(wos);
    DEREF(v).Accept( writer );
  }
  return os;
}

// ----------------------------------------------------------------------------
// cci_value_ref

cci_value
cci_value_ref::move()
{
  cci_value ret;
  ret.init();         // ensure target validity
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  return ret;
}

void
cci_value_ref::swap( cci_value_ref& that )
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_ref
cci_value_ref::operator=( const cci_value_cref &that )
{
  if( that.is_null() )
    set_null();
  else if (THIS != PIMPL(that) ) {
    sc_assert( THIS );
    THIS->CopyFrom( DEREF(that), json_allocator );
  }
  return *this;
}

cci_value_ref
cci_value_ref::set_null()
{
  if( THIS ) THIS->SetNull();
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_bool(bool v)
{
  sc_assert( THIS );
  THIS->SetBool(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_int(int v)
{
  sc_assert( THIS );
  THIS->SetInt(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_uint(unsigned v)
{
  sc_assert( THIS );
  THIS->SetUint(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_int64(int64 v)
{
  sc_assert( THIS );
  THIS->SetInt64(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_uint64(uint64 v)
{
  sc_assert( THIS );
  THIS->SetUint64(v);
  return this_type( THIS );
}

cci_value_ref
cci_value_ref::set_double(double d)
{
  sc_assert( THIS );
  THIS->SetDouble(d);
  return this_type( THIS );
}

cci_value_string_ref
cci_value_ref::set_string( const char* s, size_t len )
{
  sc_assert( THIS );
  THIS->SetString(s, static_cast<rapidjson::SizeType>(len), json_allocator);
  return cci_value_string_ref(THIS);
}

cci_value_list_ref
cci_value_ref::set_list()
{
  sc_assert( THIS );
  THIS->SetArray();
  return cci_value_list_ref( THIS );
}

cci_value_map_ref
cci_value_ref::set_map()
{
  sc_assert( THIS );
  THIS->SetObject();
  return cci_value_map_ref( THIS );
}

std::istream& operator>>( std::istream& is, cci_value_ref v )
{
  sc_assert( PIMPL(v) );
  json_document d;
  rapidjson::IStreamWrapper wis(is);

  d.ParseStream< rapidjson::kParseStopWhenDoneFlag >( wis );
  // VALUE_ASSERT( !d.HasParseError(), "cci_value stream extraction failed" );
  if( !d.HasParseError() )
    DEREF(v).Swap( d );
  else
    is.setstate( std::istream::failbit );

  return is;
}

// ----------------------------------------------------------------------------
// cci_value_string_cref

cci_value_string_cref::size_type
cci_value_string_cref::size() const
  { return THIS->GetStringLength(); }

const char*
cci_value_string_cref::c_str() const
  { return THIS->GetString(); }

bool
cci_value_string_cref::operator==( cci_value_string_cref const & s ) const
  { return *THIS == DEREF(s); }

bool
cci_value_string_cref::operator==( const char * s ) const
  { return !s ? false : *THIS == s; }

bool
cci_value_string_cref::operator==( const std::string& s ) const
  { return *THIS == rapidjson::StringRef( s.c_str(), s.size() ); }

// ----------------------------------------------------------------------------
// cci_value_string_ref

cci_value
cci_value_string_ref::move()
{
  cci_value ret;
  ret.init();         // ensure target validity
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  THIS->SetString("", 0);
  return ret;
}

void
cci_value_string_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

// ----------------------------------------------------------------------------
// cci_value_list/map::(const_)iterator

///@cond CCI_HIDDEN_FROM_DOXYGEN
namespace cci_impl {

struct value_iterator_list_tag {};
struct value_iterator_map_tag {};

template<typename T> struct value_iterator_tag;
template<> struct value_iterator_tag<cci_value_cref> : value_iterator_list_tag {};
template<> struct value_iterator_tag<cci_value_ref>  : value_iterator_list_tag {};
template<> struct value_iterator_tag<cci_value_map_elem_cref> : value_iterator_map_tag {};
template<> struct value_iterator_tag<cci_value_map_elem_ref>  : value_iterator_map_tag {};

template<typename T>
struct value_iterator_impl_json
{
  typedef void* impl_type; // type-punned pointer for now
  typedef std::ptrdiff_t difference_type;

  static impl_type advance(impl_type it, difference_type n, value_iterator_list_tag)
    { return static_cast<json_value_iter>(it) + n; }

  static impl_type advance(impl_type it, difference_type n, value_iterator_map_tag)
    { return static_cast<json_member_iter>(it) + n; }

  static difference_type distance(impl_type a, impl_type b, value_iterator_list_tag)
    { return static_cast<json_value_iter>(b) - static_cast<json_value_iter>(a); }

  static difference_type distance(impl_type a, impl_type b, value_iterator_map_tag)
    { return static_cast<json_member_iter>(b) - static_cast<json_member_iter>(a); }
};

template<typename T>
typename value_iterator_impl<T>::impl_type
value_iterator_impl<T>::advance(difference_type n) const
  { return value_iterator_impl_json<T>::advance( impl_, n, value_iterator_tag<T>() ); }

template<typename T>
int value_iterator_impl<T>::compare(impl_type other_impl) const
{
  // comparing internal pointers is sufficient here
  if (impl_ < other_impl) return -1;
  if (impl_ > other_impl) return 1;
  return 0;
}

template<typename T>
typename value_iterator_impl<T>::difference_type
value_iterator_impl<T>::distance(impl_type that_impl) const
  { return value_iterator_impl_json<T>::distance( that_impl, impl_, value_iterator_tag<T>() ); }

template class value_iterator_impl<cci_value_cref>;
template class value_iterator_impl<cci_value_ref>;
template class value_iterator_impl<cci_value_map_elem_cref>;
template class value_iterator_impl<cci_value_map_elem_ref>;
} // namespace cci_impl
///@endcond

template class cci_value_iterator<cci_value_cref>;
template class cci_value_iterator<cci_value_ref>;
template class cci_value_iterator<cci_value_map_elem_cref>;
template class cci_value_iterator<cci_value_map_elem_ref>;

// ----------------------------------------------------------------------------
// cci_value_list_cref

cci_value_list_cref::size_type
cci_value_list_cref::size() const
  { return THIS->Size(); }

cci_value_list_cref::size_type
cci_value_list_cref::capacity() const
  { return THIS->Capacity(); }

cci_value_cref
cci_value_list_cref::at( size_type index ) const
{
  VALUE_ASSERT( index < size(), "index out of bounds" );
  return cci_value_cref( &(*THIS)[static_cast<rapidjson::SizeType>(index)] );
}

cci_value_cref
cci_value_list_cref::operator[]( size_type index ) const
  { return cci_value_cref( &(*THIS)[static_cast<rapidjson::SizeType>(index)] ); }

cci_value_list_cref::const_iterator
cci_value_list_cref::cbegin() const
  { return const_iterator(THIS->Begin()); }

cci_value_list_cref::const_iterator
cci_value_list_cref::cend() const
  { return const_iterator(THIS->End()); }

// ----------------------------------------------------------------------------
// cci_value_list_ref

cci_value
cci_value_list_ref::move()
{
  cci_value ret;
  ret.set_list();
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  THIS->SetArray();
  return ret;
}

void
cci_value_list_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_list_ref
cci_value_list_ref::clear()
{
  THIS->Clear();
  return *this;
}

cci_value_list_ref
cci_value_list_ref::reserve( size_type new_capacity )
{
  THIS->Reserve( static_cast<rapidjson::SizeType>(new_capacity)
               , json_allocator );
  return *this;
}

cci_value_list_ref
cci_value_list_ref::push_back( const_reference value )
{
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->PushBack( v, json_allocator );
  return *this;
}

#ifdef CCI_HAS_CXX_RVALUE_REFS
cci_value_list_ref
cci_value_list_ref::push_back( cci_value&& value )
{
  json_value v;
  if( PIMPL(value) )
    v = DEREF(value); // RapidJSON has move semantics upon plain assignment
  THIS->PushBack( v, json_allocator );
  return *this;
}
#endif // CCI_HAS_CXX_RVALUE_REFS

cci_value_list_ref::iterator
cci_value_list_ref::begin()
  { return iterator(THIS->Begin()); }

cci_value_list_ref::iterator
cci_value_list_ref::end()
  { return iterator(THIS->End()); }

cci_value_list_ref::iterator
cci_value_list_ref::insert( const_iterator pos, const_reference value )
{
  return insert(pos, 1u, value);
}

cci_value_list_ref::iterator
cci_value_list_ref::insert( const_iterator pos, size_type count, const_reference value )
{
  // RapidJSON doesn't support Insert, yet
  json_value_iter json_pos = static_cast<json_value_iter>(pos.raw());
  size_type       offset   = json_pos - THIS->Begin();

  VALUE_ASSERT( offset <= THIS->Size(), "invalid insertion position" );

  if (!count) // nothing to insert
    return iterator(json_pos);

  json_value new_val(rapidjson::kArrayType);
  new_val.Reserve(static_cast<rapidjson::SizeType>(size() + count), json_allocator);

  json_value_iter it = THIS->Begin();
  while ( it != json_pos ) // move prefix values into new array
    new_val.PushBack(*it++, json_allocator);

  while( count-- > 0u ) { // copy values at new position
    json_value v;
    if( PIMPL(value) )
      v.CopyFrom( DEREF(value), json_allocator );
    new_val.PushBack( v, json_allocator );
  }

  while ( it != THIS->End() ) // move suffix values into new array
    new_val.PushBack(*it++, json_allocator);

  THIS->Swap( new_val ); // update current value
  return iterator(THIS->Begin() + offset); // iterator to first inserted element
}

cci_value_list_ref::iterator
cci_value_list_ref::erase(const_iterator pos)
{
  json_value_iter json_pos = static_cast<json_value_iter>(pos.raw());
  return iterator( THIS->Erase(json_pos) );
}

cci_value_list_ref::iterator
cci_value_list_ref::erase(const_iterator first, const_iterator last)
{
  json_value_iter json_first = static_cast<json_value_iter>(first.raw());
  json_value_iter json_last  = static_cast<json_value_iter>(last.raw());
  return iterator( THIS->Erase(json_first, json_last) );
}

void
cci_value_list_ref::pop_back()
  { THIS->PopBack(); }

// ----------------------------------------------------------------------------
// cci_value_map_cref

cci_value_map_elem_cref::cci_value_map_elem_cref(void* raw)
  : key  ( &static_cast<json_member*>(raw)->name )
  , value( &static_cast<json_member*>(raw)->value )
  , pimpl_(raw)
{}

cci_value_map_cref::size_type
cci_value_map_cref::size() const
  { return THIS->MemberCount(); }

cci_value_cref::impl_type
cci_value_map_cref::do_lookup( const char* key, size_type keylen
                             , lookup_mode mode /* = KEY_REQUIRED */ ) const
{
  json_value kv( rapidjson::StringRef(key, keylen) );
  json_value::ConstMemberIterator it = THIS->FindMember(kv);

  if( it != THIS->MemberEnd() )
    return const_cast<json_value*>(&it->value);

  if( mode == KEY_OPTIONAL )
    return NULL;

  if( mode == KEY_CREATE )
  {
    json_value k( key, static_cast<rapidjson::SizeType>(keylen), json_allocator );
    THIS->AddMember( k, json_value().Move(), json_allocator );
    it = THIS->FindMember(kv);
    sc_assert( it != THIS->MemberEnd() );
    return const_cast<json_value*>(&it->value);
  }

  std::stringstream ss;
  ss << "cci_value map has no element with key '" << key << "'";
  report_error( ss.str().c_str(), __FILE__, __LINE__ );
  return NULL;
}

cci_value_map_cref::const_iterator
cci_value_map_cref::do_find(const char* key, size_type keylen) const
{
  return const_iterator(THIS->FindMember(rapidjson::StringRef(key, keylen)));
}

cci_value_map_cref::const_iterator
cci_value_map_cref::cbegin() const
  { return const_iterator(THIS->MemberBegin()); }

cci_value_map_cref::const_iterator
cci_value_map_cref::cend() const
  { return const_iterator(THIS->MemberEnd()); }

// ----------------------------------------------------------------------------
// cci_value_map_ref

cci_value_map_elem_ref::cci_value_map_elem_ref(void* raw)
  : key  ( &static_cast<json_member*>(raw)->name )
  , value( &static_cast<json_member*>(raw)->value )
  , pimpl_(raw)
{}

cci_value
cci_value_map_ref::move()
{
  cci_value ret;
  ret.set_map();
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  THIS->SetObject();
  return ret;
}

void
cci_value_map_ref::swap(this_type & that)
{
  VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

cci_value_map_ref
cci_value_map_ref::clear()
{
  THIS->RemoveAllMembers();
  return *this;
}

cci_value_map_ref
cci_value_map_ref::do_push( const char * key, size_type keylen
                          , cci_value::const_reference value )
{
  json_value k( key, static_cast<rapidjson::SizeType>(keylen), json_allocator );
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->AddMember( k, v, json_allocator );
  return *this;
}

#ifdef CCI_HAS_CXX_RVALUE_REFS
cci_value_map_ref
cci_value_map_ref::do_push( const char * key, size_type keylen
                          , cci_value&& value )
{
  json_value k( key, keylen, json_allocator );
  json_value v;
  if( PIMPL(value) )
    v = DEREF(value); // RapidJSON has move semantics upon plain assignment
  THIS->AddMember( k, v, json_allocator );
  return *this;
}
#endif // CCI_HAS_CXX_RVALUE_REFS

cci_value_map_ref::iterator
cci_value_map_ref::begin()
  { return iterator(THIS->MemberBegin()); }

cci_value_map_ref::iterator
cci_value_map_ref::end()
  { return iterator(THIS->MemberEnd()); }

cci_value_map_ref::size_type
cci_value_map_ref::do_erase(const char* key, size_type keylen)
{
  json_value json_key(rapidjson::StringRef(key, keylen));
  size_type  count = 0;
  for( json_member_iter it = THIS->FindMember(json_key);
       it != THIS->MemberEnd(); it = THIS->FindMember(json_key) )
  {
    THIS->EraseMember(it);
    count++;
  }
  return count;
}

cci_value_map_ref::iterator
cci_value_map_ref::erase(const_iterator pos)
{
  json_member_iter json_pos = static_cast<json_member_iter>(pos.raw());
  return iterator( THIS->EraseMember(json_pos) );
}

cci_value_map_ref::iterator
cci_value_map_ref::erase(const_iterator first, const_iterator last)
{
  json_member_iter json_first = static_cast<json_member_iter>(first.raw());
  json_member_iter json_last  = static_cast<json_member_iter>(last.raw());
  return iterator( THIS->EraseMember(json_first, json_last) );
}

// ----------------------------------------------------------------------------
// cci_value(, _list, _map ) -- owning wrapper implementations

#define WRAPPER_ASSIGN_PRECOND_(Kind) \
  WRAPPER_ASSIGN_PRECOND_FOR_ ## Kind
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value \
    if( that.is_null() ) { set_null(); return *this; } \
    init()
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value_list \
  sc_assert( is_list() && that.is_list() )
#define WRAPPER_ASSIGN_PRECOND_FOR_cci_value_map \
  sc_assert( is_map() && that.is_map() )

#define WRAPPER_DO_INIT_(Kind) \
  WRAPPER_DO_INIT_ ## Kind
#define WRAPPER_DO_INIT_cci_value \
  ((void)0)
#define WRAPPER_DO_INIT_cci_value_list \
  (THIS)->SetArray()
#define WRAPPER_DO_INIT_cci_value_map \
  (THIS)->SetObject()

#define DEFINE_WRAPPER_(Kind)                          \
  Kind::this_type &                                    \
  Kind::operator=( const_reference that )              \
  {                                                    \
    WRAPPER_ASSIGN_PRECOND_( Kind );                   \
    reference::operator=( that );                      \
    return *this;                                      \
  }                                                    \
                                                       \
  void                                                 \
  Kind::swap( this_type & that )                       \
  {                                                    \
    using std::swap;                                   \
    swap( pimpl_    , that.pimpl_ );                   \
    swap( own_pimpl_, that.own_pimpl_ );               \
  }                                                    \
                                                       \
  Kind::impl_type                                      \
  Kind::do_init()                                      \
  {                                                    \
    sc_assert( !own_pimpl_ );                          \
    pimpl_ = own_pimpl_ = impl_pool::allocate();       \
    WRAPPER_DO_INIT_( Kind );                          \
    return THIS;                                       \
  }                                                    \
                                                       \
  Kind::~Kind()                                        \
  {                                                    \
    impl_pool::deallocate(impl_cast(own_pimpl_));      \
  }

DEFINE_WRAPPER_(cci_value)
DEFINE_WRAPPER_(cci_value_list)
DEFINE_WRAPPER_(cci_value_map)

#ifdef CCI_HAS_CXX_RVALUE_REFS

cci_value::cci_value( this_type && that )
  : cci_value_ref(CCI_MOVE_(that.pimpl_))
  , own_pimpl_(CCI_MOVE_(that.own_pimpl_))
{
  that.pimpl_ = NULL;
  that.own_pimpl_ = NULL;
}

cci_value::cci_value( cci_value_list && that )
  : own_pimpl_()
{
  do_init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetArray();
}

cci_value_list::cci_value_list( this_type && that )
  : own_pimpl_()
{
  do_init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetArray();
}

cci_value::cci_value( cci_value_map && that )
  : own_pimpl_()
{
  do_init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetObject();
}

cci_value_map::cci_value_map( this_type && that )
  : own_pimpl_()
{
  do_init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetObject();
}

cci_value& cci_value::operator=( this_type && that )
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  impl_pool::deallocate(impl_cast(own_pimpl_));
  pimpl_ = own_pimpl_ = that.own_pimpl_;
  that.pimpl_ = that.own_pimpl_ = NULL;
  return *this;
}

cci_value& cci_value::operator=( cci_value_list && that )
{
  init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetArray();
  return *this;
}

cci_value_list& cci_value_list::operator=(this_type && that)
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetArray();
  return *this;
}

cci_value& cci_value::operator=(cci_value_map && that)
{
  init();
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetObject();
  return *this;
}

cci_value_map& cci_value_map::operator=(this_type && that)
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  *THIS = CCI_MOVE_(DEREF(that));
  DEREF(that).SetObject();
  return *this;
}

#endif // CCI_HAS_CXX_RVALUE_REFS

// ----------------------------------------------------------------------------
// JSON (de)serialize

bool
cci_value_ref::json_deserialize( std::string const & src )
{
  json_document doc;
  try {
    doc.Parse( src.c_str() );
  }
  catch ( rapidjson::ParseException const & )
  {
    return false;
  }
  THIS->Swap( doc );
  return true;
}

std::string
cci_value_cref::to_json() const
{
  std::string dst;
  rapidjson::StringOutputStream str(dst);
  rapidjson::Writer<rapidjson::StringOutputStream> writer(str);
  if (!THIS) {
    writer.Null();
  } else {
    THIS->Accept(writer);
  }
  VALUE_ASSERT(writer.IsComplete(), "incomplete JSON sequence");
  return dst;
}

CCI_CLOSE_NAMESPACE_
