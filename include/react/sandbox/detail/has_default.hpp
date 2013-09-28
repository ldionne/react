/*!
 * @file
 * Defines `react::detail::has_default`.
 */

#ifndef REACT_DETAIL_HAS_DEFAULT_HPP
#define REACT_DETAIL_HAS_DEFAULT_HPP

#include <react/sandbox/default_of.hpp>

#include <boost/mpl/bool.hpp>


namespace react {
namespace has_default_detail {
    template <typename Requirement>
    boost::mpl::true_
    has_default_impl(typename default_of<Requirement>::type*);

    template <typename Requirement>
    boost::mpl::false_ has_default_impl(...);
} // end namespace has_default_detail

namespace detail {
    //! Returns whether a `Requirement` has a default implementation.
    template <typename Requirement>
    struct has_default
        : decltype(has_default_detail::has_default_impl<Requirement>(nullptr))
    { };
} // end namespace detail
} // end namespace react

#endif // !REACT_DETAIL_HAS_DEFAULT_HPP
