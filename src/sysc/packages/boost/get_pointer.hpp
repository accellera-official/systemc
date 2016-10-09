// Copyright Peter Dimov and David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef SC_GET_POINTER_DWA20021219_HPP
# define SC_GET_POINTER_DWA20021219_HPP

# include <memory>

namespace sc_boost { 

// get_pointer(p) extracts a ->* capable pointer from p

template<class T> T * get_pointer(T * p)
{
    return p;
}

// get_pointer(shared_ptr<T> const & p) has been moved to shared_ptr.hpp

#if !defined( SC_BOOST_NO_AUTO_PTR )

#if defined( __GNUC__ ) && (defined( __GXX_EXPERIMENTAL_CXX0X__ ) || (__cplusplus >= 201103L))
#if defined( SC_BOOST_GCC )
#if SC_BOOST_GCC >= 40600
#define SC_BOOST_CORE_DETAIL_DISABLE_LIBSTDCXX_DEPRECATED_WARNINGS
#endif // SC_BOOST_GCC >= 40600
#elif defined( __clang__ ) && defined( __has_warning )
#if __has_warning("-Wdeprecated-declarations")
#define SC_BOOST_CORE_DETAIL_DISABLE_LIBSTDCXX_DEPRECATED_WARNINGS
#endif // __has_warning("-Wdeprecated-declarations")
#endif
#endif // defined( __GNUC__ ) && (defined( __GXX_EXPERIMENTAL_CXX0X__ ) || (__cplusplus >= 201103L))

#if defined( SC_BOOST_CORE_DETAIL_DISABLE_LIBSTDCXX_DEPRECATED_WARNINGS )
// Disable libstdc++ warnings about std::auto_ptr being deprecated in C++11 mode
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#define SC_BOOST_CORE_DETAIL_DISABLED_DEPRECATED_WARNINGS
#endif

template<class T> T * get_pointer(std::auto_ptr<T> const& p)
{
    return p.get();
}

#if defined( SC_BOOST_CORE_DETAIL_DISABLE_LIBSTDCXX_DEPRECATED_WARNINGS )
#pragma GCC diagnostic pop
#undef SC_BOOST_CORE_DETAIL_DISABLE_LIBSTDCXX_DEPRECATED_WARNINGS
#endif

#endif // !defined( BOOST_NO_AUTO_PTR )

} // namespace sc_boost

#endif // SC_GET_POINTER_DWA20021219_HPP
