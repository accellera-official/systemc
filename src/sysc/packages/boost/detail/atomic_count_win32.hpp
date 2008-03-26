#ifndef BOOST_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
#define BOOST_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/atomic_count_win32.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <sysc/packages/boost/detail/winapi.hpp>

namespace boost
{

namespace detail
{

class atomic_count
{
public:

    explicit atomic_count(long v): value_(v)
    {
    }

    long operator++()
    {
        return winapi::InterlockedIncrement(&value_);
    }

    long operator--()
    {
        return winapi::InterlockedDecrement(&value_);
    }

    operator long() const
    {
        return value_;
    }

private:

    atomic_count(atomic_count const &);
    atomic_count & operator=(atomic_count const &);

    volatile long value_;
};

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_ATOMIC_COUNT_WIN32_HPP_INCLUDED
