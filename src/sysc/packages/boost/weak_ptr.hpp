#ifndef BOOST_WEAK_PTR_HPP_INCLUDED
#define BOOST_WEAK_PTR_HPP_INCLUDED

//
//  weak_ptr.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/weak_ptr.htm for documentation.
//

#include <sysc/packages/boost/shared_ptr.hpp>

#ifdef BOOST_MSVC  // moved here to work around VC++ compiler crash
# pragma warning(push)
# pragma warning(disable:4284) // odd return type for operator->
#endif

namespace boost
{

template<typename T> class weak_ptr
{
private:

    // Borland 5.5.1 specific workarounds
    typedef weak_ptr<T> this_type;

public:

    typedef T element_type;

    weak_ptr(): px(0), pn()
    {
    }

//  generated copy constructor, assignment, destructor are fine

    template<typename Y>
    weak_ptr(weak_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
    {
    }

    template<typename Y>
    weak_ptr(shared_ptr<Y> const & r): px(r.px), pn(r.pn) // never throws
    {
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

    template<typename Y>
    weak_ptr & operator=(weak_ptr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

    template<typename Y>
    weak_ptr & operator=(shared_ptr<Y> const & r) // never throws
    {
        px = r.px;
        pn = r.pn;
        return *this;
    }

#endif

    void reset()
    {
        this_type().swap(*this);
    }

    T * get() const // never throws; deprecated, removal pending, don't use
    {
        return pn.use_count() == 0? 0: px;
    }

    long use_count() const // never throws
    {
        return pn.use_count();
    }

    bool expired() const // never throws
    {
        return pn.use_count() == 0;
    }

    void swap(this_type & other) // never throws
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    bool less(this_type const & rhs) const // implementation detail, never throws
    {
        return pn < rhs.pn;
    }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

private:

    template<typename Y> friend class weak_ptr;
    template<typename Y> friend class shared_ptr;

#endif

    T * px;                     // contained pointer
    detail::weak_count pn;      // reference counter

};  // weak_ptr

template<class T, class U> inline bool operator==(weak_ptr<T> const & a, weak_ptr<U> const & b)
{
    return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(weak_ptr<T> const & a, weak_ptr<U> const & b)
{
    return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<typename T> inline bool operator!=(weak_ptr<T> const & a, weak_ptr<T> const & b)
{
    return a.get() != b.get();
}

#endif

template<class T> inline bool operator<(weak_ptr<T> const & a, weak_ptr<T> const & b)
{
    return a.less(b);
}

template<class T> void swap(weak_ptr<T> & a, weak_ptr<T> & b)
{
    a.swap(b);
}

template<class T> shared_ptr<T> make_shared(weak_ptr<T> const & r) // never throws
{
    // optimization: avoid throw overhead
    if(r.use_count() == 0)
    {
        return shared_ptr<T>();
    }

    try
    {
        return shared_ptr<T>(r);
    }
    catch(use_count_is_zero const &)
    {
        return shared_ptr<T>();
    }
}

// Note: there is no get_pointer overload for weak_ptr.
// This is intentional. Even get() will disappear in a
// future release; these accessors are too error-prone.

} // namespace boost

#ifdef BOOST_MSVC
# pragma warning(pop)
#endif    

#endif  // #ifndef BOOST_WEAK_PTR_HPP_INCLUDED
