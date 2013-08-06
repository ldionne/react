/*!
 * @file
 * This file defines `react::tag_of`.
 */

#ifndef REACT_TAG_OF_HPP
#define REACT_TAG_OF_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react {
namespace tag_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(react_tag)

    template <typename T>
    struct nested_react_tag {
        using type = typename T::react_tag;
    };

    template <typename T>
    struct tag_of_impl
        : boost::mpl::eval_if<
            has_react_tag<T>,
            nested_react_tag<T>,
            boost::mpl::identity<T>
        >
    { };
} // end namespace tag_of_detail

template <typename T>
struct tag_of
    : tag_of_detail::tag_of_impl<
        typename boost::remove_cv<
            typename boost::remove_reference<T>::type
        >::type
    >
{ };
} // end namespace react

#endif // !REACT_TAG_OF_HPP
