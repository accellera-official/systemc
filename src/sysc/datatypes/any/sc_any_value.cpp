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
 * @file   sc_any_value.cpp
 * @author Philipp A. Hartmann, OFFIS/Intel
 */

#include "sysc/datatypes/any/sc_any_value.h"
#include "sysc/datatypes/any/sc_rapidjson.h"

#include <algorithm> // std::swap
#include <sstream> //std::stringstream

namespace rapidjson = RAPIDJSON_NAMESPACE;

using namespace sc_core;

namespace sc_dt {

//@cond SC_HIDDEN_FROM_DOXYGEN
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

#define ANY_VALUE_ASSERT( Cond, Msg ) \
  do { if( SC_UNLIKELY_(!(Cond)) ) \
    SC_REPORT_ERROR( sc_core::SC_ID_ANY_VALUE_ACCESS_FAILED_, \
      Msg "\nFailing condition: " #Cond ); \
  } while( false )
//@endcond SC_HIDDEN_FROM_DOXYGEN

// ----------------------------------------------------------------------------
// sc_any_value_cref

bool
operator == ( sc_any_value_cref const & left, sc_any_value_cref const & right )
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

sc_any_value_category sc_any_value_cref::category() const
{
  if (!THIS)
    return SC_ANY_VALUE_NULL;

  switch(THIS->GetType())
  {
  case rapidjson::kFalseType:
  case rapidjson::kTrueType:
    return SC_ANY_VALUE_BOOL;

  case rapidjson::kNumberType:
    return THIS->IsDouble() ? SC_ANY_VALUE_REAL : SC_ANY_VALUE_INT;

  case rapidjson::kStringType:
    return SC_ANY_VALUE_STRING;

  case rapidjson::kArrayType:
    return SC_ANY_VALUE_LIST;

  case rapidjson::kObjectType:
    return SC_ANY_VALUE_MAP;

  case rapidjson::kNullType:
  default:
    return SC_ANY_VALUE_NULL;
  }
}

bool sc_any_value_cref::is_null() const
  { return !THIS || THIS->IsNull(); }

bool sc_any_value_cref::is_bool() const
  { return THIS && THIS->IsBool(); }

bool sc_any_value_cref::is_int() const
  { return THIS && THIS->IsInt(); }

bool sc_any_value_cref::is_int64() const
  { return THIS && THIS->IsInt64(); }

bool sc_any_value_cref::is_uint() const
  { return THIS && THIS->IsUint(); }

bool sc_any_value_cref::is_uint64() const
  { return THIS && THIS->IsUint64(); }

bool sc_any_value_cref::is_double() const
  { return THIS && THIS->IsDouble(); }

bool sc_any_value_cref::is_string() const
  { return THIS && THIS->IsString(); }

bool sc_any_value_cref::is_list() const
  { return THIS && THIS->IsArray(); }

bool sc_any_value_cref::is_map() const
  { return THIS && THIS->IsObject(); }

#define ASSERT_TYPE( Cond ) \
  ANY_VALUE_ASSERT( Cond, "invalid type" )

bool sc_any_value_cref::get_bool() const
{
  ASSERT_TYPE(is_bool());
  return THIS->GetBool();
}

int sc_any_value_cref::get_int() const
{
  ASSERT_TYPE(is_int());
  return THIS->GetInt();
}

unsigned sc_any_value_cref::get_uint() const
{
  ASSERT_TYPE(is_uint());
  return THIS->GetUint();
}

int64 sc_any_value_cref::get_int64() const
{
  ASSERT_TYPE(is_int64());
  return THIS->GetInt64();
}

uint64 sc_any_value_cref::get_uint64() const
{
  ASSERT_TYPE(is_uint64());
  return THIS->GetUint64();
}

double sc_any_value_cref::get_double() const
{
  ASSERT_TYPE(is_number());
  return THIS->GetDouble();
}

sc_zstring_view sc_any_value_cref::get_string() const
{
  ASSERT_TYPE(is_string());
  return sc_zstring_view(THIS->GetString());
}

sc_any_value_list_cref sc_any_value_cref::get_list() const
{
  ASSERT_TYPE(is_list());
  return sc_any_value_list_cref(pimpl_);
}

sc_any_value_map_cref sc_any_value_cref::get_map() const
{
  ASSERT_TYPE(is_map());
  return sc_any_value_map_cref(pimpl_);
}

std::ostream& operator<<( std::ostream& os, sc_any_value_cref const& v )
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
// sc_any_value_ref

sc_any_value
sc_any_value_ref::move()
{
  sc_any_value ret;
  ret.init();         // ensure target validity
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  return ret;
}

void
sc_any_value_ref::swap( sc_any_value_ref& that )
{
  ANY_VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

sc_any_value_ref
sc_any_value_ref::operator=( const sc_any_value_cref& that )
{
  if( that.is_null() )
    set_null();
  else if (THIS != PIMPL(that) ) {
    sc_assert( THIS );
    THIS->CopyFrom( DEREF(that), json_allocator );
  }
  return *this;
}

sc_any_value_ref
sc_any_value_ref::set_null()
{
  if( THIS ) THIS->SetNull();
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_bool(bool v)
{
  sc_assert( THIS );
  THIS->SetBool(v);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_int(int v)
{
  sc_assert( THIS );
  THIS->SetInt(v);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_uint(unsigned v)
{
  sc_assert( THIS );
  THIS->SetUint(v);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_int64(int64 v)
{
  sc_assert( THIS );
  THIS->SetInt64(v);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_uint64(uint64 v)
{
  sc_assert( THIS );
  THIS->SetUint64(v);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_double(double d)
{
  sc_assert( THIS );
  THIS->SetDouble(d);
  return this_type( THIS );
}

sc_any_value_ref
sc_any_value_ref::set_string( sc_string_view s )
{
  sc_assert( THIS );
  THIS->SetString(s.data(), static_cast<rapidjson::SizeType>(s.size()), json_allocator);
  return this_type( THIS );
}

sc_any_value_list_ref
sc_any_value_ref::set_list()
{
  sc_assert( THIS );
  THIS->SetArray();
  return sc_any_value_list_ref( THIS );
}

sc_any_value_map_ref
sc_any_value_ref::set_map()
{
  sc_assert( THIS );
  THIS->SetObject();
  return sc_any_value_map_ref( THIS );
}

std::istream& operator>>( std::istream& is, sc_any_value_ref v )
{
  sc_assert( PIMPL(v) );
  json_document d;
  rapidjson::IStreamWrapper wis(is);

  try
  {
    d.ParseStream< rapidjson::kParseStopWhenDoneFlag >( wis );
    DEREF(v).Swap( d );
  }
  catch ( rapidjson::ParseException const& ex )
  {
    std::stringstream ss;
    ss << "JSON parse error: "
       << rapidjson::GetParseError_En( ex.Code() )
       << " (offset: " << ex.Offset() << ")";
    SC_REPORT_WARNING( sc_core::SC_ID_ANY_VALUE_PARSING_FAILED_, ss.str().c_str() );
    is.setstate( std::istream::failbit );
  }
  return is;
}

// ----------------------------------------------------------------------------

//@cond SC_HIDDEN_FROM_DOXYGEN
namespace sc_any_value_impl {

struct iterator_list_tag {};
struct iterator_map_tag {};

template<typename T> struct iterator_tag;
template<> struct iterator_tag<sc_any_value_cref> : iterator_list_tag {};
template<> struct iterator_tag<sc_any_value_ref>  : iterator_list_tag {};
template<> struct iterator_tag<sc_any_value_map_elem_cref> : iterator_map_tag {};
template<> struct iterator_tag<sc_any_value_map_elem_ref>  : iterator_map_tag {};

template<typename T>
struct iterator_impl_json
{
  typedef void* impl_type; // type-punned pointer for now
  typedef std::ptrdiff_t difference_type;

  static impl_type advance(impl_type it, difference_type n, iterator_list_tag)
    { return static_cast<json_value_iter>(it) + n; }

  static impl_type advance(impl_type it, difference_type n, iterator_map_tag)
    { return static_cast<json_member_iter>(it) + n; }

  static difference_type distance(impl_type a, impl_type b, iterator_list_tag)
    { return static_cast<json_value_iter>(b) - static_cast<json_value_iter>(a); }

  static difference_type distance(impl_type a, impl_type b, iterator_map_tag)
    { return static_cast<json_member_iter>(b) - static_cast<json_member_iter>(a); }
};

template<typename T>
typename iterator_impl<T>::impl_type
iterator_impl<T>::advance(difference_type n) const
  { return iterator_impl_json<T>::advance( impl_, n, iterator_tag<T>() ); }

template<typename T>
int iterator_impl<T>::compare(impl_type other_impl) const
{
  // comparing internal pointers is sufficient here
  if (impl_ < other_impl) return -1;
  if (impl_ > other_impl) return 1;
  return 0;
}

template<typename T>
typename iterator_impl<T>::difference_type
iterator_impl<T>::distance(impl_type that_impl) const
  { return iterator_impl_json<T>::distance( that_impl, impl_, iterator_tag<T>() ); }

template class iterator_impl<sc_any_value_cref>;
template class iterator_impl<sc_any_value_ref>;
template class iterator_impl<sc_any_value_map_elem_cref>;
template class iterator_impl<sc_any_value_map_elem_ref>;
} // namespace sc_any_value_impl
///@endcond

template class sc_any_value_iterator<sc_any_value_cref>;
template class sc_any_value_iterator<sc_any_value_ref>;
template class sc_any_value_iterator<sc_any_value_map_elem_cref>;
template class sc_any_value_iterator<sc_any_value_map_elem_ref>;

// ----------------------------------------------------------------------------
// sc_any_value_list_cref

sc_any_value_list_cref::size_type
sc_any_value_list_cref::size() const
  { return THIS->Size(); }

sc_any_value_list_cref::difference_type
sc_any_value_list_cref::ssize() const
  { return static_cast<difference_type>(THIS->Size()); }

sc_any_value_list_cref::size_type
sc_any_value_list_cref::capacity() const
  { return THIS->Capacity(); }

sc_any_value_cref
sc_any_value_list_cref::at( size_type index ) const
{
  ANY_VALUE_ASSERT( index < size(), "index out of bounds" );
  return sc_any_value_cref( &(*THIS)[static_cast<rapidjson::SizeType>(index)] );
}

sc_any_value_cref
sc_any_value_list_cref::operator[]( size_type index ) const
  { return sc_any_value_cref( &(*THIS)[static_cast<rapidjson::SizeType>(index)] ); }

sc_any_value_list_cref::const_iterator
sc_any_value_list_cref::cbegin() const
  { return const_iterator(THIS->Begin()); }

sc_any_value_list_cref::const_iterator
sc_any_value_list_cref::cend() const
  { return const_iterator(THIS->End()); }

// ----------------------------------------------------------------------------
// sc_any_value_list_ref

sc_any_value
sc_any_value_list_ref::move()
{
  sc_any_value ret;
  ret.set_list();
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  THIS->SetArray();
  return ret;
}

void
sc_any_value_list_ref::swap(this_type & that)
{
  ANY_VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

sc_any_value_list_ref
sc_any_value_list_ref::clear()
{
  THIS->Clear();
  return *this;
}

sc_any_value_list_ref
sc_any_value_list_ref::reserve( size_type new_capacity )
{
  THIS->Reserve( static_cast<rapidjson::SizeType>(new_capacity)
               , json_allocator );
  return *this;
}

sc_any_value_list_ref
sc_any_value_list_ref::push_back( const_reference value )
{
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->PushBack( v, json_allocator );
  return *this;
}

#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
sc_any_value_list_ref
sc_any_value_list_ref::push_back( sc_any_value&& value )
{
  json_value v;
  if( PIMPL(value) )
    v = DEREF(value); // RapidJSON has move semantics upon plain assignment
  THIS->PushBack( v, json_allocator );
  return *this;
}
#endif // rvalue refs - C++11

sc_any_value_list_ref::iterator
sc_any_value_list_ref::begin()
  { return iterator(THIS->Begin()); }

sc_any_value_list_ref::iterator
sc_any_value_list_ref::end()
  { return iterator(THIS->End()); }

sc_any_value_list_ref::iterator
sc_any_value_list_ref::insert( const_iterator pos, const_reference value )
{
  return insert(pos, 1u, value);
}

sc_any_value_list_ref::iterator
sc_any_value_list_ref::insert( const_iterator pos, size_type count, const_reference value )
{
  // RapidJSON doesn't support Insert, yet
  json_value_iter json_pos = static_cast<json_value_iter>(pos.raw());
  size_type       offset   = json_pos - THIS->Begin();

  ANY_VALUE_ASSERT( offset <= THIS->Size(), "invalid insertion position" );

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

sc_any_value_list_ref::iterator
sc_any_value_list_ref::erase(const_iterator pos)
{
  json_value_iter json_pos = static_cast<json_value_iter>(pos.raw());
  return iterator( THIS->Erase(json_pos) );
}

sc_any_value_list_ref::iterator
sc_any_value_list_ref::erase(const_iterator first, const_iterator last)
{
  json_value_iter json_first = static_cast<json_value_iter>(first.raw());
  json_value_iter json_last  = static_cast<json_value_iter>(last.raw());
  return iterator( THIS->Erase(json_first, json_last) );
}

void
sc_any_value_list_ref::pop_back()
  { THIS->PopBack(); }

// ----------------------------------------------------------------------------
// sc_any_value_map_cref

sc_any_value_map_elem_cref::sc_any_value_map_elem_cref(void* raw)
  : key  ( static_cast<json_member*>(raw)->name.GetString() )
  , value( &static_cast<json_member*>(raw)->value )
  , pimpl_(raw)
{}

sc_any_value_map_cref::size_type
sc_any_value_map_cref::size() const
  { return THIS->MemberCount(); }

sc_any_value_map_cref::difference_type
sc_any_value_map_cref::ssize() const
  { return static_cast<difference_type>(THIS->MemberCount()); }

sc_any_value_cref::impl_type
sc_any_value_map_cref::do_lookup( sc_string_view key
                          , lookup_mode mode /* = KEY_REQUIRED */ ) const
{
  json_value kv( rapidjson::StringRef(key.data(), key.size()) );
  json_value::ConstMemberIterator it = THIS->FindMember(kv);

  if( it != THIS->MemberEnd() )
    return const_cast<json_value*>(&it->value);

  if( mode == KEY_OPTIONAL )
    return NULL;

  if( mode == KEY_CREATE )
  {
    json_value k;
    k.SetString( kv.GetString(), kv.GetStringLength(), json_allocator ); // copy key
    THIS->AddMember( k, json_value().Move(), json_allocator );
    it = THIS->FindMember(kv);
    sc_assert( it != THIS->MemberEnd() );
    return const_cast<json_value*>(&it->value);
  }

  std::stringstream ss;
  ss << "sc_any_value map has no element with key '" << key << "'";
  SC_REPORT_ERROR( sc_core::SC_ID_ANY_VALUE_ACCESS_FAILED_, ss.str().c_str() );
  return NULL;
}

sc_any_value_map_cref::const_iterator
sc_any_value_map_cref::find(sc_string_view key) const
{
  return const_iterator(THIS->FindMember(rapidjson::StringRef(key.data(), key.size())));
}

sc_any_value_map_cref::const_iterator
sc_any_value_map_cref::cbegin() const
  { return const_iterator(THIS->MemberBegin()); }

sc_any_value_map_cref::const_iterator
sc_any_value_map_cref::cend() const
  { return const_iterator(THIS->MemberEnd()); }

// ----------------------------------------------------------------------------
// sc_any_value_map_ref

sc_any_value_map_elem_ref::sc_any_value_map_elem_ref(void* raw)
  : key  ( static_cast<json_member*>(raw)->name.GetString() )
  , value( &static_cast<json_member*>(raw)->value )
  , pimpl_(raw)
{}

sc_any_value
sc_any_value_map_ref::move()
{
  sc_any_value ret;
  ret.set_map();
  DEREF(ret) = *THIS; // RapidJSON has move semantics upon plain assignment
  THIS->SetObject();
  return ret;
}

void
sc_any_value_map_ref::swap(this_type & that)
{
  ANY_VALUE_ASSERT( pimpl_ && that.pimpl_, "swap with invalid value failed" );
  THIS->Swap( DEREF(that) );
}

sc_any_value_map_ref
sc_any_value_map_ref::clear()
{
  THIS->RemoveAllMembers();
  return *this;
}

sc_any_value_map_ref
sc_any_value_map_ref::push_entry( sc_string_view key
                          , sc_any_value::const_reference value )
{
  json_value k( key.data(), static_cast<rapidjson::SizeType>(key.size()), json_allocator );
  json_value v;
  if( PIMPL(value) )
    v.CopyFrom( DEREF(value), json_allocator );
  THIS->AddMember( k, v, json_allocator );
  return *this;
}

#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11
sc_any_value_map_ref
sc_any_value_map_ref::push_entry( sc_string_view key, sc_any_value&& value )
{
  json_value k( key.data(), static_cast<rapidjson::SizeType>(key.size()), json_allocator );
  json_value v;
  if( PIMPL(value) )
    v = DEREF(value); // RapidJSON has move semantics upon plain assignment
  THIS->AddMember( k, v, json_allocator );
  return *this;
}
#endif // rvalue refs - C++11

sc_any_value_map_ref::iterator
sc_any_value_map_ref::begin()
  { return iterator(THIS->MemberBegin()); }

sc_any_value_map_ref::iterator
sc_any_value_map_ref::end()
  { return iterator(THIS->MemberEnd()); }

sc_any_value_map_ref::size_type
sc_any_value_map_ref::erase(sc_string_view key)
{
  json_value json_key(rapidjson::StringRef(key.data(), key.size()));
  size_type  count = 0;
  for( json_member_iter it = THIS->FindMember(json_key);
       it != THIS->MemberEnd(); it = THIS->FindMember(json_key) )
  {
    THIS->EraseMember(it);
    count++;
  }
  return count;
}

sc_any_value_map_ref::iterator
sc_any_value_map_ref::erase(const_iterator pos)
{
  json_member_iter json_pos = static_cast<json_member_iter>(pos.raw());
  return iterator( THIS->EraseMember(json_pos) );
}

sc_any_value_map_ref::iterator
sc_any_value_map_ref::erase(const_iterator first, const_iterator last)
{
  json_member_iter json_first = static_cast<json_member_iter>(first.raw());
  json_member_iter json_last  = static_cast<json_member_iter>(last.raw());
  return iterator( THIS->EraseMember(json_first, json_last) );
}

// ----------------------------------------------------------------------------
// sc_any_value(, _list, _map ) -- owning wrapper implementations

#define WRAPPER_ASSIGN_PRECOND_(Kind) \
  WRAPPER_ASSIGN_PRECOND_FOR_ ## Kind
#define WRAPPER_ASSIGN_PRECOND_FOR_sc_any_value \
    if( that.is_null() ) { set_null(); return *this; } \
    init()
#define WRAPPER_ASSIGN_PRECOND_FOR_sc_any_value_list \
  sc_assert( is_list() && that.is_list() )
#define WRAPPER_ASSIGN_PRECOND_FOR_sc_any_value_map \
  sc_assert( is_map() && that.is_map() )

#define WRAPPER_DO_INIT_(Kind) \
  WRAPPER_DO_INIT_ ## Kind
#define WRAPPER_DO_INIT_sc_any_value \
  ((void)0)
#define WRAPPER_DO_INIT_sc_any_value_list \
  (THIS)->SetArray()
#define WRAPPER_DO_INIT_sc_any_value_map \
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

DEFINE_WRAPPER_(sc_any_value)
DEFINE_WRAPPER_(sc_any_value_list)
DEFINE_WRAPPER_(sc_any_value_map)

#if SC_CPLUSPLUS >= 201103L // rvalue refs - C++11

sc_any_value::sc_any_value( this_type && that )
  : sc_any_value_ref(std::move(that.pimpl_))
  , own_pimpl_(std::move(that.own_pimpl_))
{
  that.pimpl_ = NULL;
  that.own_pimpl_ = NULL;
}

sc_any_value::sc_any_value( sc_any_value_list && that )
  : own_pimpl_()
{
  do_init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetArray();
}

sc_any_value_list::sc_any_value_list( this_type && that )
  : own_pimpl_()
{
  do_init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetArray();
}

sc_any_value::sc_any_value( sc_any_value_map && that )
  : own_pimpl_()
{
  do_init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetObject();
}

sc_any_value_map::sc_any_value_map( this_type && that )
  : own_pimpl_()
{
  do_init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetObject();
}

sc_any_value& sc_any_value::operator=( this_type && that )
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  impl_pool::deallocate(impl_cast(own_pimpl_));
  pimpl_ = own_pimpl_ = that.own_pimpl_;
  that.pimpl_ = that.own_pimpl_ = NULL;
  return *this;
}

sc_any_value& sc_any_value::operator=( sc_any_value_list && that )
{
  init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetArray();
  return *this;
}

sc_any_value_list& sc_any_value_list::operator=(this_type && that)
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  *THIS = std::move(DEREF(that));
  DEREF(that).SetArray();
  return *this;
}

sc_any_value& sc_any_value::operator=(sc_any_value_map && that)
{
  init();
  *THIS = std::move(DEREF(that));
  DEREF(that).SetObject();
  return *this;
}

sc_any_value_map& sc_any_value_map::operator=(this_type && that)
{
  if (own_pimpl_ == that.own_pimpl_) return *this;
  *THIS = std::move(DEREF(that));
  DEREF(that).SetObject();
  return *this;
}

#endif // rvalue refs - C++11

// ----------------------------------------------------------------------------
// JSON (de)serialize

std::string
sc_any_value_cref::to_json() const
{
  std::string dst;
  rapidjson::StringOutputStream str(dst);
  rapidjson::Writer<rapidjson::StringOutputStream> writer(str);
  if (!THIS) {
    writer.Null();
  } else {
    THIS->Accept(writer);
  }
  ANY_VALUE_ASSERT(writer.IsComplete(), "incomplete JSON sequence");
  return dst;
}

sc_any_value
sc_any_value::from_json( sc_string_view json )
{
  sc_any_value v;
  json_document doc;
  try
  {
    doc.Parse( json.data(), json.size() );
    v.init();
    DEREF(v).Swap( doc );
  }
  catch ( rapidjson::ParseException const & ex )
  {
    std::stringstream ss;
    ss << "JSON parse error: "
       << rapidjson::GetParseError_En( ex.Code() )
       << " (offset: " << ex.Offset() << ")";
    SC_REPORT_ERROR( sc_core::SC_ID_ANY_VALUE_PARSING_FAILED_, ss.str().c_str() );
  }
  return v;
}

} // namespace sc_dt
// Taf!
