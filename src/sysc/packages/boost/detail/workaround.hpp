// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef SC_WORKAROUND_DWA2002126_HPP
# define SC_WORKAROUND_DWA2002126_HPP

// Compiler/library version workaround macro
//
// Usage:
//
//     #if SC_BOOST_WORKAROUND(SC_BOOST_MSVC, <= 1200)
//        ... // workaround code here
//     #endif
//
// When SC_BOOST_STRICT_CONFIG is defined, expands to 0. Otherwise, the
// first argument must be undefined or expand to a numeric
// value. The above expands to:
//
//     (SC_BOOST_MSVC) != 0 && (SC_BOOST_MSVC) <= 1200
//
// When used for workarounds that apply to the latest known version 
// and all earlier versions of a compiler, the following convention 
// should be observed:
//
//     #if SC_BOOST_WORKAROUND(SC_BOOST_MSVC, SC_BOOST_TESTED_AT(1301))
//
// The version number in this case corresponds to the last version in
// which the workaround was known to have been required. When
// SC_BOOST_DETECT_OUTDATED_WORKAROUNDS is not the defined, the macro
// SC_BOOST_TESTED_AT(x) expands to "!= 0", which effectively activates
// the workaround for any version of the compiler. When
// SC_BOOST_DETECT_OUTDATED_WORKAROUNDS is defined, a compiler warning or
// error will be issued if the compiler version exceeds the argument
// to SC_BOOST_TESTED_AT().  This can be used to locate workarounds which
// may be obsoleted by newer versions.

# ifndef SC_BOOST_STRICT_CONFIG

#  define SC_BOOST_WORKAROUND(symbol, test)                \
        ((symbol != 0) && (1 % (( (symbol test) ) + 1)))
//                              ^ ^           ^ ^
// The extra level of parenthesis nesting above, along with the
// SC_BOOST_OPEN_PAREN indirection below, is required to satisfy the
// broken preprocessor in MWCW 8.3 and earlier.
//
// The basic mechanism works as follows:
//      (symbol test) + 1        =>   if (symbol test) then 2 else 1
//      1 % ((symbol test) + 1)  =>   if (symbol test) then 1 else 0
//
// The complication with % is for cooperation with SC_BOOST_TESTED_AT().
// When "test" is SC_BOOST_TESTED_AT(x) and
// SC_BOOST_DETECT_OUTDATED_WORKAROUNDS is #defined,
//
//      symbol test              =>   if (symbol <= x) then 1 else -1
//      (symbol test) + 1        =>   if (symbol <= x) then 2 else 0
//      1 % ((symbol test) + 1)  =>   if (symbol <= x) then 1 else divide-by-zero
//

#  ifdef SC_BOOST_DETECT_OUTDATED_WORKAROUNDS
#   define SC_BOOST_OPEN_PAREN (
#   define SC_BOOST_TESTED_AT(value)  > value) ?(-1): SC_BOOST_OPEN_PAREN 1
#  else
#   define SC_BOOST_TESTED_AT(value) != ((value)-(value))
#  endif

# else

#  define SC_BOOST_WORKAROUND(symbol, test) 0

# endif 

#endif // SC_WORKAROUND_DWA2002126_HPP
