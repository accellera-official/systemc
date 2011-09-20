// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#ifndef SC_BOOST_RESULT_OF_HPP
#define SC_BOOST_RESULT_OF_HPP

#include <sysc/packages/boost/config.hpp>
#include <sysc/packages/boost/type_traits/ice.hpp>
#include <sysc/packages/boost/type.hpp>
#include <sysc/packages/boost/preprocessor.hpp>
#include <sysc/packages/boost/detail/workaround.hpp>
#include <sysc/packages/boost/mpl/has_xxx.hpp>

#ifndef SC_BOOST_RESULT_OF_NUM_ARGS
#  define SC_BOOST_RESULT_OF_NUM_ARGS 10
#endif

namespace sc_boost {

template<typename F> struct result_of;

#if !defined(SC_BOOST_NO_SFINAE) && !defined(SC_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
namespace sc_detail {

SC_BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

template<typename F, typename FArgs, bool HasResultType> struct get_result_of;

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, true>
{
  typedef typename F::result_type type;
};

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, false>
{
  typedef typename F::template result<FArgs>::type type;
};

template<typename F>
struct get_result_of<F, F(void), false>
{
  typedef void type;
};

template<typename F, typename FArgs>
struct result_of : get_result_of<F, FArgs, (has_result_type<F>::value)> {};

} // end namespace sc_detail

#define SC_BOOST_PP_ITERATION_PARAMS_1 (3,(0,SC_BOOST_RESULT_OF_NUM_ARGS,<sysc/packages/boost/utility/detail/result_of_iterate.hpp>))
#include SC_BOOST_PP_ITERATE()

#else
#  define SC_BOOST_NO_RESULT_OF 1
#endif

}

#endif // SC_BOOST_RESULT_OF_HPP
