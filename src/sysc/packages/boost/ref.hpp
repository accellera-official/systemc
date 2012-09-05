#ifndef SC_BOOST_REF_HPP_INCLUDED
#define SC_BOOST_REF_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <sysc/packages/boost/config.hpp>
#include <sysc/packages/boost/utility/addressof.hpp>
#include <sysc/packages/boost/mpl/bool.hpp>

//
//  ref.hpp - ref/cref, useful helper functions
//
//  Copyright (C) 1999, 2000 Jaakko Järvi (jaakko.jarvi@cs.utu.fi)
//  Copyright (C) 2001, 2002 Peter Dimov
//  Copyright (C) 2002 David Abrahams
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/ref.html for documentation.
//

namespace sc_boost
{

template<class T> class reference_wrapper
{ 
public:
    typedef T type;

#if defined(SC_BOOST_MSVC) && (SC_BOOST_MSVC < 1300)

    explicit reference_wrapper(T& t): t_(&t) {}

#else

    explicit reference_wrapper(T& t): t_(sc_boost::addressof(t)) {}

#endif

    operator T& () const { return *t_; }

    T& get() const { return *t_; }

    T* get_pointer() const { return t_; }

private:

    T* t_;
};

# if defined(__BORLANDC__) && (__BORLANDC__ <= 0x570)
#  define SC_BOOST_REF_CONST
# else
#  define SC_BOOST_REF_CONST const
# endif

template<class T> inline reference_wrapper<T> SC_BOOST_REF_CONST ref(T & t)
{ 
    return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> SC_BOOST_REF_CONST cref(T const & t)
{
    return reference_wrapper<T const>(t);
}

# undef SC_BOOST_REF_CONST

# ifndef SC_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename T>
class is_reference_wrapper
    : public mpl::false_
{
};

template<typename T>
class unwrap_reference
{
 public:
    typedef T type;
};

#  define SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(X) \
template<typename T> \
class is_reference_wrapper< X > \
    : public mpl::true_ \
{ \
}; \
\
template<typename T> \
class unwrap_reference< X > \
{ \
 public: \
    typedef T type; \
}; \
/**/

SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(reference_wrapper<T>)
#if !defined(SC_BOOST_NO_CV_SPECIALIZATIONS)
SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(reference_wrapper<T> const)
SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(reference_wrapper<T> volatile)
SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(reference_wrapper<T> const volatile)
#endif

#  undef SC_AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF

# else // no partial specialization

} // namespace sc_boost

#include <sysc/packages/boost/type.hpp>

namespace sc_boost
{

namespace detail
{
  typedef char (&yes_reference_wrapper_t)[1];
  typedef char (&no_reference_wrapper_t)[2];
      
  no_reference_wrapper_t is_reference_wrapper_test(...);

  template<typename T>
  yes_reference_wrapper_t is_reference_wrapper_test(type< reference_wrapper<T> >);

  template<bool wrapped>
  struct reference_unwrapper
  {
      template <class T>
      struct apply
      {
          typedef T type;
      };
  };

  template<>
  struct reference_unwrapper<true>
  {
      template <class T>
      struct apply
      {
          typedef typename T::type type;
      };
  };
}

template<typename T>
class is_reference_wrapper
{
 public:
    SC_BOOST_STATIC_CONSTANT(
        bool, value = (
             sizeof(detail::is_reference_wrapper_test(type<T>()))
            == sizeof(detail::yes_reference_wrapper_t)));
    
    typedef ::sc_boost::mpl::bool_<value> type;
};

template <typename T>
class unwrap_reference
    : public detail::reference_unwrapper<
        is_reference_wrapper<T>::value
      >::template apply<T>
{};

# endif // SC_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace sc_boost

#endif // #ifndef SC_BOOST_REF_HPP_INCLUDED
