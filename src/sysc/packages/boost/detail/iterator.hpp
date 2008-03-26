// (C) Copyright David Abrahams 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Boost versions of
//
//    std::iterator_traits<>::iterator_category
//    std::iterator_traits<>::difference_type
//    std::distance()
//
// ...for all compilers and iterators
//
// Additionally, if X is a pointer
//    std::iterator_traits<X>::pointer

// Otherwise, if partial specialization is supported or X is not a pointer
//    std::iterator_traits<X>::value_type
//    std::iterator_traits<X>::pointer
//    std::iterator_traits<X>::reference
//
// CAVEAT: When using the VC6 standard library, an iterator derived from
// std::iterator but not boost::iterator or from one supplied by the standard
// will always have pointer == const value_type* and reference == const
// value_type&, whether that's correct or not.

// See http://www.boost.org for most recent version including documentation.

// Revision History
// 04 Mar 2001 - More attempted fixes for Intel C++ (David Abrahams)
// 03 Mar 2001 - Put all implementation into namespace
//               boost::detail::iterator_traits_. Some progress made on fixes
//               for Intel compiler. (David Abrahams)
// 02 Mar 2001 - Changed BOOST_MSVC to BOOST_MSVC_STD_ITERATOR in a few
//               places. (Jeremy Siek)
// 19 Feb 2001 - Improved workarounds for stock MSVC6; use yes_type and
//               no_type from type_traits.hpp; stopped trying to remove_cv
//               before detecting is_pointer, in honor of the new type_traits
//               semantics. (David Abrahams)
// 13 Feb 2001 - Make it work with nearly all standard-conforming iterators
//               under raw VC6. The one category remaining which will fail is
//               that of iterators derived from std::iterator but not
//               boost::iterator and which redefine difference_type.
// 11 Feb 2001 - Clean away code which can never be used (David Abrahams)
// 09 Feb 2001 - Always have a definition for each traits member, even if it
//               can't be properly deduced. These will be incomplete types in
//               some cases (undefined<void>), but it helps suppress MSVC errors
//               elsewhere (David Abrahams)
// 07 Feb 2001 - Support for more of the traits members where possible, making
//               this useful as a replacement for std::iterator_traits<T> when
//               used as a default template parameter.
// 06 Feb 2001 - Removed useless #includes of standard library headers
//               (David Abrahams)

#ifndef ITERATOR_DWA122600_HPP_
# define ITERATOR_DWA122600_HPP_

# include <sysc/packages/boost/config.hpp>
# include <sysc/packages/boost/type_traits.hpp>
# include <sysc/packages/boost/iterator.hpp>
# include <iterator>
# include <cstddef>

# if defined(BOOST_MSVC_STD_ITERATOR) && !defined(__SGI_STL_PORT)
#  include <xtree>
#  include <deque>
#  include <list>
# endif


// STLPort 4.0 and betas have a bug when debugging is enabled and there is no
// partial specialization: instead of an iterator_category typedef, the standard
// container iterators have _Iterator_category.
//
// Also, whether debugging is enabled or not, there is a broken specialization
// of std::iterator<output_iterator_tag,void,void,void,void> which has no
// typedefs but iterator_category.
# if defined(__SGI_STL_PORT) && (__SGI_STL_PORT <= 0x410) && !defined(__STL_CLASS_PARTIAL_SPECIALIZATION)

#  ifdef __STL_DEBUG
#   define BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
#  endif

#  define BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION

# endif // STLPort <= 4.1b4 && no partial specialization

namespace boost { namespace detail {
# if !defined(BOOST_NO_STD_ITERATOR_TRAITS) && !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR)
using std::iterator_traits;
using std::distance;
# else

namespace iterator_traits_ {

// Workarounds for less-capable implementations
template <bool is_ptr> struct iterator_traits_select;

template <class T> struct undefined;
template <> struct iterator_traits_select<true>
{
    template <class Ptr>
    struct traits
    {
        typedef std::ptrdiff_t difference_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef Ptr pointer;
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
// Keeps MSVC happy under certain circumstances. It seems class template default
// arguments are partly instantiated even when not used when the class template
// is the return type of a function template.
        typedef undefined<void> value_type;
        typedef undefined<void> reference;
#endif
    };
};


typedef char yes_type;
typedef double no_type;

# ifdef BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF

no_type bad_category_helper(...);
template <class C, class T> yes_type bad_category_helper(std::_DBG_iter<C,T>*);

template <bool has_bad_category_typedef> struct bad_category_select;
template <>
struct bad_category_select<true>
{
    template <class Iterator>
    struct category { typedef typename Iterator::_Iterator_category type; };
};
template <>
struct bad_category_select<false>
{
    template <class Iterator>
    struct category { typedef typename Iterator::iterator_category type; };
};

template <class Iterator>
struct iterator_category_select
{
 private:
    static Iterator p;
    enum { has_bad_category
           = sizeof(bad_category_helper(&p)) == sizeof(yes_type) };
    typedef bad_category_select<has_bad_category> category_select;
 public:
    typedef typename category_select::template category<Iterator>::type type;
};

# endif

# ifdef BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION
template <bool is_bad_output_iterator> struct bad_output_iterator_select;
template <>
struct bad_output_iterator_select<true>
{
    template <class Iterator>
    struct non_category_traits {
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;
    };
};
template <>
struct bad_output_iterator_select<false>
{
    template <class Iterator>
    struct non_category_traits {
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };
};
# endif

# if defined(BOOST_MSVC_STD_ITERATOR) && !defined(__SGI_STL_PORT)

// We'll sort iterator types into one of these classifications, from which we
// can determine the difference_type, pointer, reference, and value_type
enum {
    not_msvc_stdlib_iterator,
    msvc_stdlib_const_iterator,
    msvc_stdlib_mutable_iterator,
    msvc_stdlib_ostream_iterator
};
       
template <unsigned> struct msvc_traits_select;

template <> struct msvc_traits_select<not_msvc_stdlib_iterator>
{
    template <class Iterator>
    struct traits_     // calling this "traits" will confuse VC.
    {
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };
};

template <> struct msvc_traits_select<msvc_stdlib_mutable_iterator>
{
    template <class Iterator>
    struct traits_
    {
        typedef typename Iterator::distance_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
    };
};

template <> struct msvc_traits_select<msvc_stdlib_const_iterator>
{
    template <class Iterator>
    struct traits_
    {
        typedef typename Iterator::distance_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;
    };
};

template <> struct msvc_traits_select<msvc_stdlib_ostream_iterator>
{
    template <class Iterator>
    struct traits_
    {
        typedef typename Iterator::distance_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef void pointer;
        typedef void reference;
    };
};

// These functions allow us to detect which classification a given iterator type
// falls into.

// Is the iterator derived from std::iterator?
no_type is_std_iterator_helper(...);
template <class V, class D, class C>
yes_type is_std_iterator_helper(const volatile std::iterator<V,D,C>*);

// Is the iterator derived from boost::iterator?
template <class C, class T, class D, class P, class R>
yes_type is_boost_iterator_helper(const volatile boost::iterator<C,T,D,P,R>*);
no_type is_boost_iterator_helper(...);

// Is the iterator one of the known mutable container iterators?
template<class K, class Ty, class Kfn, class Pr, class A>
yes_type is_mutable_iterator_helper(const volatile typename std::_Tree<K,Ty,Kfn,Pr,A>::iterator*);
template<class Ty, class A>
yes_type is_mutable_iterator_helper(const volatile typename std::list<Ty,A>::iterator*);
template<class Ty, class A>
yes_type is_mutable_iterator_helper(const volatile typename std::deque<Ty,A>::iterator*);
no_type is_mutable_iterator_helper(...);

// Is the iterator an ostream_iterator?
template<class T, class CharT, class Traits>
yes_type is_ostream_iterator_helper(const volatile std::ostream_iterator<T,CharT,Traits>*);
no_type is_ostream_iterator_helper(...);

template <class T>
struct msvc_iterator_classification {
    BOOST_STATIC_CONSTANT(unsigned,
        value = (sizeof(is_ostream_iterator_helper((T*)0)) == sizeof(yes_type))
          ? msvc_stdlib_ostream_iterator
        : (sizeof(is_mutable_iterator_helper((T*)0)) == sizeof(yes_type))
          ? msvc_stdlib_mutable_iterator
        : (sizeof(is_std_iterator_helper((T*)0)) == sizeof(yes_type)
           && sizeof(is_boost_iterator_helper((T*)0)) == sizeof(no_type))
        ? msvc_stdlib_const_iterator
        : not_msvc_stdlib_iterator
        );
};
# endif

template <> struct iterator_traits_select<false>
{
    template <class Iterator>
    struct traits
    {
#   if defined(BOOST_MSVC_STD_ITERATOR) && !defined(__SGI_STL_PORT)
        typedef msvc_traits_select<(
            msvc_iterator_classification<Iterator>::value
        )>::template traits_<Iterator> inner_traits;
        
        typedef typename inner_traits::difference_type difference_type;
        typedef typename inner_traits::value_type value_type;
        typedef typename inner_traits::pointer pointer;
        typedef typename inner_traits::reference reference;
#   elif !defined(BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION)
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
#   else
        typedef bad_output_iterator_select<
          is_convertible<const volatile Iterator*,
            const volatile std::iterator<std::output_iterator_tag,void,void,void,void>*
          >::value> non_category_traits_select;
        typedef non_category_traits_select::template non_category_traits<Iterator> non_category_traits;
     public:
        typedef typename non_category_traits::value_type value_type;
        typedef typename non_category_traits::difference_type difference_type;
        typedef typename non_category_traits::pointer pointer;
        typedef typename non_category_traits::reference reference;
#   endif
        
#   if !defined(BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF)
        typedef typename Iterator::iterator_category iterator_category;
#   else
        typedef typename iterator_category_select<Iterator>::type iterator_category;
#   endif
    };
};

} // namespace boost::detail::iterator_traits_

template <class Iterator>
struct iterator_traits
    : iterator_traits_::iterator_traits_select<is_pointer<Iterator>::value>::template traits<Iterator>
{
 private:
    typedef typename iterator_traits_::iterator_traits_select<
        is_pointer<remove_cv<Iterator>::type>::value>::template traits<Iterator> traits;
 public:
    // Why do I need to define these typedefs? It keeps MSVC happy somehow.
    // Why don't I need to define the other typedefs? Who knows?!?
    typedef typename traits::difference_type difference_type;
    typedef typename traits::iterator_category iterator_category;
};

namespace iterator_traits_ {

template <class Category>
struct distance_select {
    template <class Iterator>
    static typename ::boost::detail::iterator_traits<Iterator>::difference_type
    distance(Iterator i1, const Iterator i2)
    {
        typename ::boost::detail::iterator_traits<Iterator>::difference_type result = 0;
        while (i1 != i2)
        {
            ++i1;
            ++result;
        }
        return result;
    }
};

template <>
struct distance_select<std::random_access_iterator_tag> {
    template <class Iterator>
    static typename ::boost::detail::iterator_traits<Iterator>::difference_type
    distance(const Iterator i1, const Iterator i2)
    {
        return i2 - i1;
    }
};

} // namespace boost::detail::iterator_traits_

template <class Iterator>
inline typename ::boost::detail::iterator_traits<Iterator>::difference_type
distance(const Iterator& first, const Iterator& last)
{
    typedef typename ::boost::detail::iterator_traits<Iterator>::iterator_category iterator_category;
    return iterator_traits_::distance_select<iterator_category>::distance(first, last);
}
# endif // workarounds

}} // namespace boost::detail

# undef BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
# undef BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION

#endif // ITERATOR_DWA122600_HPP_
