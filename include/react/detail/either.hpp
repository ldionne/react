/*!
 * @file
 * Defines `react::detail::either`.
 */

#ifndef REACT_DETAIL_EITHER_HPP
#define REACT_DETAIL_EITHER_HPP

namespace react {
namespace either_detail {
    template <typename Left, typename Right>
    auto either_impl(typename Left::type*) -> Left;

    template <typename Left, typename Right>
    auto either_impl(...) -> Right;
} // end namespace either_detail

namespace detail {
    template <typename Left, typename Right>
    struct either
        : decltype(either_detail::either_impl<Left, Right>(nullptr))
    { };
} // end namespace detail
} // end namespace react

#endif // !REACT_DETAIL_EITHER_HPP
