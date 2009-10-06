//  abi_prefix header  -------------------------------------------------------//

// © Copyright John Maddock 2003
   
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef SC_BOOST_CONFIG_ABI_PREFIX_HPP
# define SC_BOOST_CONFIG_ABI_PREFIX_HPP
#else
# error double inclusion of header boost/config/abi_prefix.hpp is an error
#endif

#include <sysc/packages/boost/config.hpp>

// this must occur after all other includes and before any code appears:
#ifdef SC_BOOST_HAS_ABI_HEADERS
#  include SC_BOOST_ABI_PREFIX
#endif
