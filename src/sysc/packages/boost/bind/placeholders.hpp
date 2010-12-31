#ifndef SC_BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
#define SC_BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  bind/placeholders.hpp - _N definitions
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

#include <sysc/packages/boost/bind/arg.hpp>
#include <sysc/packages/boost/config.hpp>

namespace sc_unnamed
{

namespace { // anonymous namespace

#if defined(__BORLANDC__) || defined(__GNUC__)

static inline sc_boost::arg<1> _1() { return sc_boost::arg<1>(); }
static inline sc_boost::arg<2> _2() { return sc_boost::arg<2>(); }
static inline sc_boost::arg<3> _3() { return sc_boost::arg<3>(); }
static inline sc_boost::arg<4> _4() { return sc_boost::arg<4>(); }
static inline sc_boost::arg<5> _5() { return sc_boost::arg<5>(); }
static inline sc_boost::arg<6> _6() { return sc_boost::arg<6>(); }
static inline sc_boost::arg<7> _7() { return sc_boost::arg<7>(); }
static inline sc_boost::arg<8> _8() { return sc_boost::arg<8>(); }
static inline sc_boost::arg<9> _9() { return sc_boost::arg<9>(); }

#elif defined(SC_BOOST_MSVC) || (defined(__DECCXX_VER) && __DECCXX_VER <= 60590031) || defined(__MWERKS__)

static sc_boost::arg<1> _1;
static sc_boost::arg<2> _2;
static sc_boost::arg<3> _3;
static sc_boost::arg<4> _4;
static sc_boost::arg<5> _5;
static sc_boost::arg<6> _6;
static sc_boost::arg<7> _7;
static sc_boost::arg<8> _8;
static sc_boost::arg<9> _9;

#else

sc_boost::arg<1> _1;
sc_boost::arg<2> _2;
sc_boost::arg<3> _3;
sc_boost::arg<4> _4;
sc_boost::arg<5> _5;
sc_boost::arg<6> _6;
sc_boost::arg<7> _7;
sc_boost::arg<8> _8;
sc_boost::arg<9> _9;

#endif

} // anonymous namespace
} // namespace sc_unnamed

#endif // #ifndef SC_BOOST_BIND_PLACEHOLDERS_HPP_INCLUDED
