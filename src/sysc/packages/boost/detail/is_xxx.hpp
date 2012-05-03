// Copyright David Abrahams 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef SC_BOOST_DETAIL_IS_XXX_DWA20051011_HPP
# define SC_BOOST_DETAIL_IS_XXX_DWA20051011_HPP

# include <sysc/packages/boost/config.hpp>
# include <sysc/packages/boost/mpl/bool.hpp>
# include <sysc/packages/boost/preprocessor/enum_params.hpp>

# if defined(SC_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
# include <sysc/packages/boost/type_traits/is_reference.hpp>
# include <sysc/packages/boost/type_traits/add_reference.hpp>

#  define SC_BOOST_DETAIL_IS_XXX_DEF(name, qualified_name, nargs)          \
template <class X_>                                                     \
struct is_##name                                                        \
{                                                                       \
    typedef char yes;                                                   \
    typedef char (&no)[2];                                              \
                                                                        \
    static typename add_reference<X_>::type dummy;                      \
                                                                        \
    struct helpers                                                      \
    {                                                                   \
        template < SC_BOOST_PP_ENUM_PARAMS_Z(1, nargs, class U) >          \
        static yes test(                                                \
           qualified_name< SC_BOOST_PP_ENUM_PARAMS_Z(1, nargs, U) >&, int  \
        );                                                              \
                                                                        \
        template <class U>                                              \
        static no test(U&, ...);                                        \
    };                                                                  \
                                                                        \
    SC_BOOST_STATIC_CONSTANT(                                              \
        bool, value                                                     \
        = !is_reference<X_>::value                                      \
        & (sizeof(helpers::test(dummy, 0)) == sizeof(yes)));            \
                                                                        \
    typedef mpl::bool_<value> type;                                     \
};

# else

#  define SC_BOOST_DETAIL_IS_XXX_DEF(name, qualified_name, nargs)  \
template <class T>                                              \
struct is_##name : mpl::false_                                  \
{                                                               \
};                                                              \
                                                                \
template < SC_BOOST_PP_ENUM_PARAMS_Z(1, nargs, class T) >          \
struct is_##name<                                               \
   qualified_name< SC_BOOST_PP_ENUM_PARAMS_Z(1, nargs, T) >        \
>                                                               \
   : mpl::true_                                                 \
{                                                               \
};

# endif

#endif // SC_BOOST_DETAIL_IS_XXX_DWA20051011_HPP
