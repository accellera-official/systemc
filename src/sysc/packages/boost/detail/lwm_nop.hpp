#ifndef SC_BOOST_DETAIL_LWM_NOP_HPP_INCLUDED
#define SC_BOOST_DETAIL_LWM_NOP_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/lwm_nop.hpp
//
//  Copyright (c) 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

namespace sc_boost
{

namespace sc_detail
{

class lightweight_mutex
{
public:

    typedef lightweight_mutex scoped_lock;
};

} // namespace sc_detail

} // namespace sc_boost

#endif // #ifndef SC_BOOST_DETAIL_LWM_NOP_HPP_INCLUDED
