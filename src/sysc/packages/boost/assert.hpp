#ifndef BOOST_ASSERT_HPP_INCLUDED
#define BOOST_ASSERT_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  boost/assert.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

//
//  When BOOST_DEBUG is not defined, it defaults to 0 (off)
//  for compatibility with programs that do not expect asserts
//  in the smart pointer class templates.
//
//  This default may be changed after an initial transition period.
//

#ifndef BOOST_DEBUG
#define BOOST_DEBUG 0
#endif

#if BOOST_DEBUG

#include <assert.h>

#ifndef BOOST_ASSERT

#include "sysc/packages/boost/current_function.hpp"

bool boost_error(char const * expr, char const * func, char const * file, long line);

# define BOOST_ASSERT(expr) ((expr) || !boost_error(#expr, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__) || (assert(expr), true))

#endif // #ifndef BOOST_ASSERT

#else // #if BOOST_DEBUG

#undef BOOST_ASSERT
#define BOOST_ASSERT(expr) ((void)0)

#endif // #if BOOST_DEBUG

#endif // #ifndef BOOST_ASSERT_HPP_INCLUDED
