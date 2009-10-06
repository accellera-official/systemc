// Copyright David Abrahams 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SC_IS_INCREMENTABLE_DWA200415_HPP
# define SC_IS_INCREMENTABLE_DWA200415_HPP

# include <sysc/packages/boost/type_traits/detail/bool_trait_def.hpp>
# include <sysc/packages/boost/type_traits/detail/template_arity_spec.hpp>
# include <sysc/packages/boost/type_traits/remove_cv.hpp>
# include <sysc/packages/boost/mpl/aux_/lambda_support.hpp>
# include <sysc/packages/boost/mpl/bool.hpp>
# include <sysc/packages/boost/detail/workaround.hpp>

namespace sc_boost { namespace sc_detail { 

// is_incrementable<T> metafunction
//
// Requires: Given x of type T&, if the expression ++x is well-formed
// it must have complete type; otherwise, it must neither be ambiguous
// nor violate access.

// This namespace ensures that ADL doesn't mess things up.
namespace sc_is_incrementable_
{
  // a type returned from operator++ when no increment is found in the
  // type's own namespace
  struct tag {};
  
  // any soaks up implicit conversions and makes the following
  // operator++ less-preferred than any other such operator that
  // might be found via ADL.
  struct any { template <class T> any(T const&); };

  // This is a last-resort operator++ for when none other is found
# if SC_BOOST_WORKAROUND(__GNUC__, == 4) && __GNUC_MINOR__ == 0 && __GNUC_PATCHLEVEL__ == 2
  
}

namespace sc_is_incrementable_2
{
  is_incrementable_::tag operator++(is_incrementable_::any const&);
  is_incrementable_::tag operator++(is_incrementable_::any const&,int);
}
using namespace is_incrementable_2;

namespace sc_is_incrementable_
{
  
# else
  
  tag operator++(any const&);
  tag operator++(any const&,int);
  
# endif 

# if SC_BOOST_WORKAROUND(__MWERKS__, SC_BOOST_TESTED_AT(0x3202)) \
    || SC_BOOST_WORKAROUND(SC_BOOST_MSVC, <= 1300)
#  define SC_BOOST_comma(a,b) (a)
# else 
  // In case an operator++ is found that returns void, we'll use ++x,0
  tag operator,(tag,int);  
#  define SC_BOOST_comma(a,b) (a,b)
# endif 
  
  // two check overloads help us identify which operator++ was picked
  char (& check(tag) )[2];
  
  template <class T>
  char check(T const&);
  

  template <class T>
  struct impl
  {
      static typename sc_boost::remove_cv<T>::type& x;

      SC_BOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(sc_is_incrementable_::check(SC_BOOST_comma(++x,0))) == 1
      );
  };

  template <class T>
  struct postfix_impl
  {
      static typename sc_boost::remove_cv<T>::type& x;

      SC_BOOST_STATIC_CONSTANT(
          bool
        , value = sizeof(sc_is_incrementable_::check(SC_BOOST_comma(x++,0))) == 1
      );
  };
}

# undef SC_BOOST_comma

template<typename T> 
struct is_incrementable 
SC_BOOST_TT_AUX_BOOL_C_BASE(::sc_boost::sc_detail::is_incrementable_::impl<T>::value)
{ 
    SC_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(::sc_boost::sc_detail::sc_is_incrementable_::impl<T>::value)
    SC_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_incrementable,(T))
};

template<typename T> 
struct is_postfix_incrementable 
SC_BOOST_TT_AUX_BOOL_C_BASE(::sc_boost::sc_detail::sc_is_incrementable_::impl<T>::value)
{ 
    SC_BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(::sc_boost::sc_detail::sc_is_incrementable_::postfix_impl<T>::value)
    SC_BOOST_MPL_AUX_LAMBDA_SUPPORT(1,is_postfix_incrementable,(T))
};

} // namespace sc_detail

SC_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1, ::sc_boost::sc_detail::sc_is_incrementable)
SC_BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1, ::sc_boost::sc_detail::is_postfix_incrementable)

} // namespace sc_boost


#endif // SC_IS_INCREMENTABLE_DWA200415_HPP
