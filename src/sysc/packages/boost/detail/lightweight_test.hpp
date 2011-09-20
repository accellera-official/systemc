#ifndef SC_BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED
#define SC_BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/lightweight_test.hpp - lightweight test library
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  SC_BOOST_TEST(expression)
//  SC_BOOST_ERROR(message)
//
//  int boost::report_errors()
//

#include <sysc/packages/boost/current_function.hpp>
#include <iostream>

namespace sc_boost
{

namespace sc_detail
{

inline int & test_errors()
{
    static int x = 0;
    return x;
}

inline void test_failed_impl(char const * expr, char const * file, int line, char const * function)
{
    std::cerr << file << "(" << line << "): test '" << expr << "' failed in function '" << function << "'" << std::endl;
    ++test_errors();
}

inline void error_impl(char const * msg, char const * file, int line, char const * function)
{
    std::cerr << file << "(" << line << "): " << msg << " in function '" << function << "'" << std::endl;
    ++test_errors();
}

} // namespace sc_detail

inline int report_errors()
{
    int errors = sc_detail::test_errors();

    if(errors == 0)
    {
        std::cerr << "No errors detected." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << errors << " error" << (errors == 1? "": "s") << " detected." << std::endl;
        return 1;
    }
}

} // namespace sc_boost

#define SC_BOOST_TEST(expr) ((expr)? (void)0: ::sc_boost::sc_detail::test_failed_impl(#expr, __FILE__, __LINE__, SC_BOOST_CURRENT_FUNCTION))
#define SC_BOOST_ERROR(msg) ::sc_boost::sc_detail::error_impl(msg, __FILE__, __LINE__, SC_BOOST_CURRENT_FUNCTION)

#endif // #ifndef SC_BOOST_DETAIL_LIGHTWEIGHT_TEST_HPP_INCLUDED
