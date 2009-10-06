#ifndef SC_BOOST_BIND_ARG_HPP_INCLUDED
#define SC_BOOST_BIND_ARG_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  bind/arg.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

namespace sc_boost
{

template<int I> class arg
{
};

template<int I> bool operator==(arg<I> const &, arg<I> const &)
{
    return true;
}

} // namespace sc_boost

#endif // #ifndef SC_BOOST_BIND_ARG_HPP_INCLUDED
