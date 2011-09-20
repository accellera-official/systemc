//
//  boost/assert.hpp - SC_BOOST_ASSERT(expr)
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  Note: There are no include guards. This is intentional.
//
//  See http://www.boost.org/libs/utility/assert.html for documentation.
//

#undef SC_BOOST_ASSERT

#if defined(SC_BOOST_DISABLE_ASSERTS)

# define SC_BOOST_ASSERT(expr) ((void)0)

#elif defined(SC_BOOST_ENABLE_ASSERT_HANDLER)

#include <boost/current_function.hpp>

namespace sc_boost
{

void assertion_failed(char const * expr, char const * function, char const * file, long line); // user defined

} // namespace sc_boost

#define SC_BOOST_ASSERT(expr) ((expr)? ((void)0): ::sc_boost::assertion_failed(#expr, SC_BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
# include <assert.h> // .h to support old libraries w/o <cassert> - effect is the same
# define SC_BOOST_ASSERT(expr) assert(expr)
#endif
