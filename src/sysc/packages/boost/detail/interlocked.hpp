#ifndef SC_BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED
#define SC_BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/interlocked.hpp
//
//  Copyright 2005 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <sysc/packages/boost/config.hpp>

#if defined( SC_BOOST_USE_WINDOWS_H )

# include <windows.h>

# define SC_BOOST_INTERLOCKED_INCREMENT InterlockedIncrement
# define SC_BOOST_INTERLOCKED_DECREMENT InterlockedDecrement
# define SC_BOOST_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange

#elif defined( SC_BOOST_MSVC ) || defined( SC_BOOST_INTEL_WIN )

extern "C" long __cdecl _InterlockedIncrement( long volatile * );
extern "C" long __cdecl _InterlockedDecrement( long volatile * );
extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );

# pragma intrinsic( _InterlockedIncrement )
# pragma intrinsic( _InterlockedDecrement )
# pragma intrinsic( _InterlockedCompareExchange )

# define SC_BOOST_INTERLOCKED_INCREMENT _InterlockedIncrement
# define SC_BOOST_INTERLOCKED_DECREMENT _InterlockedDecrement
# define SC_BOOST_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ )

namespace sc_boost
{

namespace sc_detail
{

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement( long volatile * );
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement( long volatile * );
extern "C" __declspec(dllimport) long __stdcall InterlockedCompareExchange( long volatile *, long, long );

} // namespace sc_detail

} // namespace sc_boost

# define SC_BOOST_INTERLOCKED_INCREMENT InterlockedIncrement
# define SC_BOOST_INTERLOCKED_DECREMENT InterlockedDecrement
# define SC_BOOST_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange

#else

# error "Interlocked intrinsics not available"

#endif

#endif // #ifndef SC_BOOST_DETAIL_INTERLOCKED_HPP_INCLUDED
