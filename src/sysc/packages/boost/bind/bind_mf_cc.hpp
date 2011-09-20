//
//  bind/bind_mf_cc.hpp - support for different calling conventions
//
//  Do not include this header directly.
//
//  Copyright (c) 2001 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

// 0

template<class R, class T,
    class A1>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf0)<R, T>, typename sc_bi::list_av_1<A1>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (), A1 a1)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf0)<R, T> F;
    typedef typename sc_bi::list_av_1<A1>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1));
}

template<class R, class T,
    class A1>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf0)<R, T>, typename sc_bi::list_av_1<A1>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) () const, A1 a1)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf0)<R, T> F;
    typedef typename sc_bi::list_av_1<A1>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1));
}

// 1

template<class R, class T,
    class B1,
    class A1, class A2>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf1)<R, T, B1>, typename sc_bi::list_av_2<A1, A2>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1), A1 a1, A2 a2)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf1)<R, T, B1> F;
    typedef typename sc_bi::list_av_2<A1, A2>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2));
}

template<class R, class T,
    class B1,
    class A1, class A2>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf1)<R, T, B1>, typename sc_bi::list_av_2<A1, A2>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1) const, A1 a1, A2 a2)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf1)<R, T, B1> F;
    typedef typename sc_bi::list_av_2<A1, A2>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2));
}

// 2

template<class R, class T,
    class B1, class B2,
    class A1, class A2, class A3>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf2)<R, T, B1, B2>, typename sc_bi::list_av_3<A1, A2, A3>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2), A1 a1, A2 a2, A3 a3)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf2)<R, T, B1, B2> F;
    typedef typename sc_bi::list_av_3<A1, A2, A3>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3));
}

template<class R, class T,
    class B1, class B2,
    class A1, class A2, class A3>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf2)<R, T, B1, B2>, typename sc_bi::list_av_3<A1, A2, A3>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2) const, A1 a1, A2 a2, A3 a3)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf2)<R, T, B1, B2> F;
    typedef typename sc_bi::list_av_3<A1, A2, A3>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3));
}

// 3

template<class R, class T,
    class B1, class B2, class B3,
    class A1, class A2, class A3, class A4>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf3)<R, T, B1, B2, B3>, typename sc_bi::list_av_4<A1, A2, A3, A4>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3), A1 a1, A2 a2, A3 a3, A4 a4)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf3)<R, T, B1, B2, B3> F;
    typedef typename sc_bi::list_av_4<A1, A2, A3, A4>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4));
}

template<class R, class T,
    class B1, class B2, class B3,
    class A1, class A2, class A3, class A4>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf3)<R, T, B1, B2, B3>, typename sc_bi::list_av_4<A1, A2, A3, A4>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3) const, A1 a1, A2 a2, A3 a3, A4 a4)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf3)<R, T, B1, B2, B3> F;
    typedef typename sc_bi::list_av_4<A1, A2, A3, A4>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4));
}

// 4

template<class R, class T,
    class B1, class B2, class B3, class B4,
    class A1, class A2, class A3, class A4, class A5>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf4)<R, T, B1, B2, B3, B4>, typename sc_bi::list_av_5<A1, A2, A3, A4, A5>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf4)<R, T, B1, B2, B3, B4> F;
    typedef typename sc_bi::list_av_5<A1, A2, A3, A4, A5>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5));
}

template<class R, class T,
    class B1, class B2, class B3, class B4,
    class A1, class A2, class A3, class A4, class A5>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf4)<R, T, B1, B2, B3, B4>, typename sc_bi::list_av_5<A1, A2, A3, A4, A5>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf4)<R, T, B1, B2, B3, B4> F;
    typedef typename sc_bi::list_av_5<A1, A2, A3, A4, A5>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5));
}

// 5

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5,
    class A1, class A2, class A3, class A4, class A5, class A6>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf5)<R, T, B1, B2, B3, B4, B5>, typename sc_bi::list_av_6<A1, A2, A3, A4, A5, A6>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf5)<R, T, B1, B2, B3, B4, B5> F;
    typedef typename sc_bi::list_av_6<A1, A2, A3, A4, A5, A6>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6));
}

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5,
    class A1, class A2, class A3, class A4, class A5, class A6>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf5)<R, T, B1, B2, B3, B4, B5>, typename sc_bi::list_av_6<A1, A2, A3, A4, A5, A6>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf5)<R, T, B1, B2, B3, B4, B5> F;
    typedef typename sc_bi::list_av_6<A1, A2, A3, A4, A5, A6>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6));
}

// 6

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf6)<R, T, B1, B2, B3, B4, B5, B6>, typename sc_bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf6)<R, T, B1, B2, B3, B4, B5, B6> F;
    typedef typename sc_bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7));
}

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf6)<R, T, B1, B2, B3, B4, B5, B6>, typename sc_bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf6)<R, T, B1, B2, B3, B4, B5, B6> F;
    typedef typename sc_bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7));
}

// 7

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6, class B7,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf7)<R, T, B1, B2, B3, B4, B5, B6, B7>, typename sc_bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6, B7), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf7)<R, T, B1, B2, B3, B4, B5, B6, B7> F;
    typedef typename sc_bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7, a8));
}

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6, class B7,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf7)<R, T, B1, B2, B3, B4, B5, B6, B7>, typename sc_bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6, B7) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf7)<R, T, B1, B2, B3, B4, B5, B6, B7> F;
    typedef typename sc_bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7, a8));
}

// 8

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(mf8)<R, T, B1, B2, B3, B4, B5, B6, B7, B8>, typename sc_bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6, B7, B8), A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(mf8)<R, T, B1, B2, B3, B4, B5, B6, B7, B8> F;
    typedef typename sc_bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}

template<class R, class T,
    class B1, class B2, class B3, class B4, class B5, class B6, class B7, class B8,
    class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    sc_bi::bind_t<R, sc_mfi::SC_BOOST_BIND_MF_NAME(cmf8)<R, T, B1, B2, B3, B4, B5, B6, B7, B8>, typename sc_bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type>
    SC_BOOST_BIND(R (SC_BOOST_BIND_MF_CC T::*f) (B1, B2, B3, B4, B5, B6, B7, B8) const, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    typedef sc_mfi::SC_BOOST_BIND_MF_NAME(cmf8)<R, T, B1, B2, B3, B4, B5, B6, B7, B8> F;
    typedef typename sc_bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type list_type;
    return sc_bi::bind_t<R, F, list_type>(F(f), list_type(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}
