//  boost utility/base_from_member.hpp header file  --------------------------//

//  Copyright 2001, 2003, 2004 Daryle Walker.  Use, modification, and
//  distribution are subject to the Boost Software License, Version 1.0.  (See
//  accompanying file LICENSE_1_0.txt or a copy at
//  <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/utility/> for the library's home page.

#ifndef SC_BOOST_UTILITY_BASE_FROM_MEMBER_HPP
#define SC_BOOST_UTILITY_BASE_FROM_MEMBER_HPP

#include <sysc/packages/boost/preprocessor/arithmetic/inc.hpp>
#include <sysc/packages/boost/preprocessor/repetition/enum_binary_params.hpp>
#include <sysc/packages/boost/preprocessor/repetition/enum_params.hpp>
#include <sysc/packages/boost/preprocessor/repetition/repeat_from_to.hpp>


//  Base-from-member arity configuration macro  ------------------------------//

// The following macro determines how many arguments will be in the largest
// constructor template of base_from_member.  Constructor templates will be
// generated from one argument to this maximum.  Code from other files can read
// this number if they need to always match the exact maximum base_from_member
// uses.  The maximum constructor length can be changed by overriding the
// #defined constant.  Make sure to apply the override, if any, for all source
// files during project compiling for consistency.

// Contributed by Jonathan Turkanis

#ifndef SC_BOOST_BASE_FROM_MEMBER_MAX_ARITY
#define SC_BOOST_BASE_FROM_MEMBER_MAX_ARITY  10
#endif


//  An iteration of a constructor template for base_from_member  -------------//

// A macro that should expand to:
//     template < typename T1, ..., typename Tn >
//     base_from_member( T1 x1, ..., Tn xn )
//         : member( x1, ..., xn )
//         {}
// This macro should only persist within this file.

#define SC_BOOST_PRIVATE_CTR_DEF( z, n, data )                            \
    template < SC_BOOST_PP_ENUM_PARAMS(n, typename T) >                   \
    explicit base_from_member( SC_BOOST_PP_ENUM_BINARY_PARAMS(n, T, x) )  \
        : member( SC_BOOST_PP_ENUM_PARAMS(n, x) )                         \
        {}                                                             \
    /**/


namespace sc_boost
{

//  Base-from-member class template  -----------------------------------------//

// Helper to initialize a base object so a derived class can use this
// object in the initialization of another base class.  Used by
// Dietmar Kuehl from ideas by Ron Klatcho to solve the problem of a
// base class needing to be initialized by a member.

// Contributed by Daryle Walker

template < typename MemberType, int UniqueID = 0 >
class base_from_member
{
protected:
    MemberType  member;

    base_from_member()
        : member()
        {}

    SC_BOOST_PP_REPEAT_FROM_TO( 1, SC_BOOST_PP_INC(SC_BOOST_BASE_FROM_MEMBER_MAX_ARITY),
     SC_BOOST_PRIVATE_CTR_DEF, _ )

};  // sc_boost::base_from_member

}  // namespace sc_boost


// Undo any private macros
#undef SC_BOOST_PRIVATE_CTR_DEF


#endif  // SC_BOOST_UTILITY_BASE_FROM_MEMBER_HPP
