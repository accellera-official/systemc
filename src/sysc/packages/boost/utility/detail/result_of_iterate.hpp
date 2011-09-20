// Boost result_of library

//  Copyright Douglas Gregor 2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org/libs/utility
#if !defined(SC_BOOST_PP_IS_ITERATING)
# error Boost result_of - do not include this file!
#endif

// CWPro8 requires an argument in a function type specialization
#if SC_BOOST_WORKAROUND(__MWERKS__, SC_BOOST_TESTED_AT(0x3002)) && SC_BOOST_PP_ITERATION() == 0
# define SC_BOOST_RESULT_OF_ARGS void
#else
# define SC_BOOST_RESULT_OF_ARGS SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),T)
#endif

#if !SC_BOOST_WORKAROUND(__BORLANDC__, SC_BOOST_TESTED_AT(0x551))
template<typename F SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<F(SC_BOOST_RESULT_OF_ARGS)>
    : sc_detail::result_of<F, F(SC_BOOST_RESULT_OF_ARGS)> {};
#endif

namespace sc_detail {

template<typename R,  typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (*)(SC_BOOST_RESULT_OF_ARGS), FArgs>
{
  typedef R type;
};

template<typename R,  typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (&)(SC_BOOST_RESULT_OF_ARGS), FArgs>
{
  typedef R type;
};

#undef SC_BOOST_RESULT_OF_ARGS

#if SC_BOOST_PP_ITERATION() > 1 && !SC_BOOST_WORKAROUND(__BORLANDC__, SC_BOOST_TESTED_AT(0x551))
template<typename R, typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (T0::*)
                     (SC_BOOST_PP_ENUM_SHIFTED_PARAMS(SC_BOOST_PP_ITERATION(),T)),
                 FArgs>
{
  typedef R type;
};

template<typename R, typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (T0::*)
                     (SC_BOOST_PP_ENUM_SHIFTED_PARAMS(SC_BOOST_PP_ITERATION(),T))
                     const,
                 FArgs>
{
  typedef R type;
};

template<typename R, typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (T0::*)
                     (SC_BOOST_PP_ENUM_SHIFTED_PARAMS(SC_BOOST_PP_ITERATION(),T))
                     volatile,
                 FArgs>
{
  typedef R type;
};

template<typename R, typename FArgs SC_BOOST_PP_COMMA_IF(SC_BOOST_PP_ITERATION())
         SC_BOOST_PP_ENUM_PARAMS(SC_BOOST_PP_ITERATION(),typename T)>
struct result_of<R (T0::*)
                     (SC_BOOST_PP_ENUM_SHIFTED_PARAMS(SC_BOOST_PP_ITERATION(),T))
                     const volatile,
                 FArgs>
{
  typedef R type;
};
#endif

}
