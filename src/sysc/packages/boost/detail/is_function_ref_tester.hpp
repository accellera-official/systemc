
// (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, 
// Aleksey Gurtovoy, Howard Hinnant & John Maddock 2000.  
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SC_BOOST_PP_IS_ITERATING)

///// header body

#ifndef SC_BOOST_DETAIL_IS_FUNCTION_REF_TESTER_HPP_INCLUDED
#define SC_BOOST_DETAIL_IS_FUNCTION_REF_TESTER_HPP_INCLUDED

#include "sysc/packages/boost/type_traits/detail/yes_no_type.hpp"
#include "sysc/packages/boost/type_traits/config.hpp"

#if defined(SC_BOOST_TT_PREPROCESSING_MODE)
#   include "sysc/packages/boost/preprocessor/iterate.hpp"
#   include "sysc/packages/boost/preprocessor/enum_params.hpp"
#   include "sysc/packages/boost/preprocessor/comma_if.hpp"
#endif

namespace sc_boost {
namespace sc_detail {
namespace sc_is_function_ref_tester_ {

template <class T>
sc_boost::type_traits::no_type SC_BOOST_TT_DECL is_function_ref_tester(T& ...);

#if !defined(SC_BOOST_TT_PREPROCESSING_MODE)
// preprocessor-generated part, don't edit by hand!

template <class R>
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(), int);

template <class R,class T0 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0), int);

template <class R,class T0,class T1 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1), int);

template <class R,class T0,class T1,class T2 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2), int);

template <class R,class T0,class T1,class T2,class T3 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3), int);

template <class R,class T0,class T1,class T2,class T3,class T4 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19,class T20 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19,class T20,class T21 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19,class T20,class T21,class T22 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19,class T20,class T21,class T22,class T23 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23), int);

template <class R,class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10,class T11,class T12,class T13,class T14,class T15,class T16,class T17,class T18,class T19,class T20,class T21,class T22,class T23,class T24 >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24), int);

#else

#define SC_BOOST_PP_ITERATION_PARAMS_1 \
    (3, (0, 25, "sysc/packages/boost/type_traits/detail/is_function_ref_tester.hpp"))
#include SC_BOOST_PP_ITERATE()

#endif // SC_BOOST_TT_PREPROCESSING_MODE

} // namespace sc_detail
} // namespace python
} // namespace sc_boost

#endif // SC_BOOST_DETAIL_IS_FUNCTION_REF_TESTER_HPP_INCLUDED

///// iteration

#else
#define i SC_BOOST_PP_FRAME_ITERATION(1)

template <class R SC_BOOST_PP_COMMA_IF(i) SC_BOOST_PP_ENUM_PARAMS(i,class T) >
sc_boost::type_traits::yes_type is_function_ref_tester(R (&)(SC_BOOST_PP_ENUM_PARAMS(i,T)), int);

#undef i
#endif // SC_BOOST_PP_IS_ITERATING
