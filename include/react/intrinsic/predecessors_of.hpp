/*!
 * @file
 * This file defines the `react::predecessors_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_PREDECESSORS_OF_HPP
#define REACT_INTRINSIC_PREDECESSORS_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/set.hpp>


namespace react {
namespace predecessors_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(predecessors)

    template <typename Computation>
    struct nested_predecessors {
        using type = typename Computation::predecessors;
    };
} // end namespace predecessors_of_detail

namespace extension {
    template <typename RawComputation, typename Enable = void>
    struct predecessors_of_impl {
        template <typename>
        struct apply
            : boost::mpl::eval_if<
                predecessors_of_detail::has_predecessors<RawComputation>,
                predecessors_of_detail::nested_predecessors<RawComputation>,
                boost::mpl::set<>
            >
        { };
    };
} // end namespace extension

/*!
 * @ingroup intrinsics
 *
 * The features of the computations that must be executed before `Computation`
 * when an environment is executed.
 */
template <typename Computation>
struct predecessors_of
    : extension::predecessors_of_impl<
        typename detail::strip<Computation>::type
    >::template apply<Computation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_PREDECESSORS_OF_HPP
