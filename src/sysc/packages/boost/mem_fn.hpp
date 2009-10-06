#ifndef SC_BOOST_MEM_FN_HPP_INCLUDED
#define SC_BOOST_MEM_FN_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  mem_fn.hpp - a generalization of std::mem_fun[_ref]
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2001 David Abrahams
//  Copyright (c) 2003-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/mem_fn.html for documentation.
//

#include <sysc/packages/boost/config.hpp>
#include <sysc/packages/boost/get_pointer.hpp>
#include <sysc/packages/boost/detail/workaround.hpp>

namespace sc_boost
{

#if defined(SC_BOOST_NO_VOID_RETURNS)

#define SC_BOOST_MEM_FN_CLASS_F , class F
#define SC_BOOST_MEM_FN_TYPEDEF(X)

namespace sc_mfi // mem_fun_impl
{

template<class V> struct mf
{

#define SC_BOOST_MEM_FN_RETURN return

#define SC_BOOST_MEM_FN_NAME(X) inner_##X
#define SC_BOOST_MEM_FN_CC

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#ifdef SC_BOOST_MEM_FN_ENABLE_CDECL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_cdecl
#define SC_BOOST_MEM_FN_CC __cdecl

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_STDCALL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_stdcall
#define SC_BOOST_MEM_FN_CC __stdcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_FASTCALL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_fastcall
#define SC_BOOST_MEM_FN_CC __fastcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#undef SC_BOOST_MEM_FN_RETURN

}; // struct mf<V>

template<> struct mf<void>
{

#define SC_BOOST_MEM_FN_RETURN

#define SC_BOOST_MEM_FN_NAME(X) inner_##X
#define SC_BOOST_MEM_FN_CC

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#ifdef SC_BOOST_MEM_FN_ENABLE_CDECL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_cdecl
#define SC_BOOST_MEM_FN_CC __cdecl

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_STDCALL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_stdcall
#define SC_BOOST_MEM_FN_CC __stdcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_FASTCALL

#define SC_BOOST_MEM_FN_NAME(X) inner_##X##_fastcall
#define SC_BOOST_MEM_FN_CC __fastcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#undef SC_BOOST_MEM_FN_RETURN

}; // struct mf<void>

#undef SC_BOOST_MEM_FN_CLASS_F
#undef SC_BOOST_MEM_FN_TYPEDEF_F

#define SC_BOOST_MEM_FN_NAME(X) X
#define SC_BOOST_MEM_FN_NAME2(X) inner_##X
#define SC_BOOST_MEM_FN_CC

#include <sysc/packages/boost/bind/mem_fn_vw.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_NAME2
#undef SC_BOOST_MEM_FN_CC

#ifdef SC_BOOST_MEM_FN_ENABLE_CDECL

#define SC_BOOST_MEM_FN_NAME(X) X##_cdecl
#define SC_BOOST_MEM_FN_NAME2(X) inner_##X##_cdecl
#define SC_BOOST_MEM_FN_CC __cdecl

#include <sysc/packages/boost/bind/mem_fn_vw.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_NAME2
#undef SC_BOOST_MEM_FN_CC

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_STDCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_stdcall
#define SC_BOOST_MEM_FN_NAME2(X) inner_##X##_stdcall
#define SC_BOOST_MEM_FN_CC __stdcall

#include <sysc/packages/boost/bind/mem_fn_vw.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_NAME2
#undef SC_BOOST_MEM_FN_CC

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_FASTCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_fastcall
#define SC_BOOST_MEM_FN_NAME2(X) inner_##X##_fastcall
#define SC_BOOST_MEM_FN_CC __fastcall

#include <sysc/packages/boost/bind/mem_fn_vw.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_NAME2
#undef SC_BOOST_MEM_FN_CC

#endif

} // namespace sc_mfi

#else // #ifdef SC_BOOST_NO_VOID_RETURNS

#define SC_BOOST_MEM_FN_CLASS_F
#define SC_BOOST_MEM_FN_TYPEDEF(X) typedef X;

namespace sc_mfi
{

#define SC_BOOST_MEM_FN_RETURN return

#define SC_BOOST_MEM_FN_NAME(X) X
#define SC_BOOST_MEM_FN_CC

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#ifdef SC_BOOST_MEM_FN_ENABLE_CDECL

#define SC_BOOST_MEM_FN_NAME(X) X##_cdecl
#define SC_BOOST_MEM_FN_CC __cdecl

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_STDCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_stdcall
#define SC_BOOST_MEM_FN_CC __stdcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_FASTCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_fastcall
#define SC_BOOST_MEM_FN_CC __fastcall

#include <sysc/packages/boost/bind/mem_fn_template.hpp>

#undef SC_BOOST_MEM_FN_CC
#undef SC_BOOST_MEM_FN_NAME

#endif

#undef SC_BOOST_MEM_FN_RETURN

} // namespace sc_mfi

#undef SC_BOOST_MEM_FN_CLASS_F
#undef SC_BOOST_MEM_FN_TYPEDEF

#endif // #ifdef SC_BOOST_NO_VOID_RETURNS

#define SC_BOOST_MEM_FN_NAME(X) X
#define SC_BOOST_MEM_FN_CC

#include <sysc/packages/boost/bind/mem_fn_cc.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_CC

#ifdef SC_BOOST_MEM_FN_ENABLE_CDECL

#define SC_BOOST_MEM_FN_NAME(X) X##_cdecl
#define SC_BOOST_MEM_FN_CC __cdecl

#include <sysc/packages/boost/bind/mem_fn_cc.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_CC

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_STDCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_stdcall
#define SC_BOOST_MEM_FN_CC __stdcall

#include <sysc/packages/boost/bind/mem_fn_cc.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_CC

#endif

#ifdef SC_BOOST_MEM_FN_ENABLE_FASTCALL

#define SC_BOOST_MEM_FN_NAME(X) X##_fastcall
#define SC_BOOST_MEM_FN_CC __fastcall

#include <sysc/packages/boost/bind/mem_fn_cc.hpp>

#undef SC_BOOST_MEM_FN_NAME
#undef SC_BOOST_MEM_FN_CC

#endif

// data member support

namespace sc_mfi
{

template<class R, class T> class dm
{
public:

    typedef R const & result_type;
    typedef T const * argument_type;

private:
    
    typedef R (T::*F);
    F f_;

    template<class U> R const & call(U & u, T const *) const
    {
        return (u.*f_);
    }

    template<class U> R & call(U & u, T *) const
    {
        return (u.*f_);
    }

    template<class U> R const & call(U & u, void const *) const
    {
        return (get_pointer(u)->*f_);
    }

public:
    
    explicit dm(F f): f_(f) {}

    R & operator()(T * p) const
    {
        return (p->*f_);
    }

    R const & operator()(T const * p) const
    {
        return (p->*f_);
    }

    template<class U> R const & operator()(U & u) const
    {
        return call(u, &u);
    }

#if !SC_BOOST_WORKAROUND(SC_BOOST_MSVC, <= 1300) && !SC_BOOST_WORKAROUND(__MWERKS__, < 0x3200)

    R & operator()(T & t) const
    {
        return (t.*f_);
    }

#endif

    R const & operator()(T const & t) const
    {
        return (t.*f_);
    }

    bool operator==(dm const & rhs) const
    {
        return f_ == rhs.f_;
    }

    bool operator!=(dm const & rhs) const
    {
        return f_ != rhs.f_;
    }
};

} // namespace sc_mfi

template<class R, class T> sc_mfi::dm<R, T> mem_fn(R T::*f)
{
    return sc_mfi::dm<R, T>(f);
}

} // namespace sc_boost

#endif // #ifndef SC_BOOST_MEM_FN_HPP_INCLUDED
