/*!
 * @file
 * This file defines `react::detail::fusion_fold`.
 */

#ifndef REACT_DETAIL_FUSION_FOLD_HPP
#define REACT_DETAIL_FUSION_FOLD_HPP

#include <react/detail/auto_return.hpp>

#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/fusion/include/next.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace detail {
namespace fusion_fold_detail {
template <typename Dummy = void>
struct impl {
    template <typename First, typename Last, typename State, typename F,
        typename = typename boost::disable_if<
            boost::fusion::result_of::equal_to<First, Last>
        >::type>
    static auto call(First const& first, Last const& last,
                     State&& state, F const& f)
    REACT_AUTO_RETURN(
        impl<First>::call(
            boost::fusion::next(first),
            last,
            f(std::forward<State>(state), *first),
            f
        )
    )

    template <typename First, typename Last, typename State, typename F,
        typename = typename boost::enable_if<
            boost::fusion::result_of::equal_to<First, Last>
        >::type>
    static auto call(First const&, Last const&, State&& state, F const&)
    REACT_AUTO_RETURN(
        std::forward<State>(state)
    )
};
} // end namespace fusion_fold_detail

//! Reimplementation of `boost::fusion::fold` that works better with C++11.
static constexpr struct {
    template <typename Sequence, typename State, typename F>
    auto operator()(Sequence&& seq, State&& state, F&& f) const
    REACT_AUTO_RETURN(
        fusion_fold_detail::impl<>::call(
            boost::fusion::begin(seq),
            boost::fusion::end(seq),
            std::forward<State>(state),
            std::forward<F>(f)
        )
    )
} fusion_fold{};
}} // end namespace react::detail

#endif // !REACT_DETAIL_FUSION_FOLD_HPP
