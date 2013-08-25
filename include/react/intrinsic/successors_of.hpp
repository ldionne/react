/*!
 * @file
 * This file defines the `react::successors_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_SUCCESSORS_OF_HPP
#define REACT_INTRINSIC_SUCCESSORS_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/set.hpp>


namespace react {
namespace successors_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(successors)

    template <typename Computation>
    struct nested_successors {
        using type = typename Computation::successors;
    };
} // end namespace successors_of_detail

namespace extension {
    template <typename RawComputation, typename Enable = void>
    struct successors_of_impl {
        template <typename>
        struct apply
            : boost::mpl::eval_if<
                successors_of_detail::has_successors<RawComputation>,
                successors_of_detail::nested_successors<RawComputation>,
                boost::mpl::set<>
            >
        { };
    };
} // end namespace extension

template <typename Computation>
struct successors_of
    : extension::successors_of_impl<
        typename detail::strip<Computation>::type
    >::template apply<Computation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_SUCCESSORS_OF_HPP
