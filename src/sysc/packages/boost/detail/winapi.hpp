#ifndef BOOST_DETAIL_WINAPI_HPP_INCLUDED
#define BOOST_DETAIL_WINAPI_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/detail/winapi.hpp - a lightweight version of <windows.h>
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

namespace boost
{

namespace detail
{

namespace winapi
{

typedef long long_type;
typedef unsigned long dword_type;
typedef void * handle_type;

#if defined(_WIN64)

typedef __int64 int_ptr_type;
typedef unsigned __int64 uint_ptr_type;
typedef __int64 long_ptr_type;
typedef unsigned __int64 ulong_ptr_type;

#else

typedef int int_ptr_type;
typedef unsigned int uint_ptr_type;
typedef long long_ptr_type;
typedef unsigned long ulong_ptr_type;

#endif

struct critical_section
{
    struct critical_section_debug * DebugInfo;
    long_type LockCount;
    long_type RecursionCount;
    handle_type OwningThread;
    handle_type LockSemaphore;
    ulong_ptr_type SpinCount;
};

#if defined(_WIN64)

// Intel 6.0 on Win64 version, posted by Tim Fenders to [boost-users]

extern "C" long_type __cdecl _InterlockedIncrement(long_type volatile *);
extern "C" long_type __cdecl _InterlockedDecrement(long_type volatile *);
extern "C" long_type __cdecl _InterlockedExchange(long_type volatile *, long_type);

#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)

inline long_type InterlockedIncrement(long_type volatile * lp)
{ 
    return _InterlockedIncrement(lp);
}

inline long_type InterlockedDecrement(long_type volatile* lp)
{ 
    return _InterlockedDecrement(lp);
}

inline long_type InterlockedExchange(long_type volatile* lp, long_type l)
{
    return _InterlockedExchange(lp, l);
}

#else

extern "C" __declspec(dllimport) long_type __stdcall InterlockedIncrement(long_type volatile *);
extern "C" __declspec(dllimport) long_type __stdcall InterlockedDecrement(long_type volatile *);
extern "C" __declspec(dllimport) long_type __stdcall InterlockedExchange(long_type volatile *, long_type);

#endif

extern "C" __declspec(dllimport) void __stdcall Sleep(dword_type);

extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(critical_section *);
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(critical_section *);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(critical_section *);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(critical_section *);

} // namespace winapi

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_WINAPI_HPP_INCLUDED
