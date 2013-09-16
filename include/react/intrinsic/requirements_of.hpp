/*!
 * @file
 * This file defines the `react::requirements_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_REQUIREMENTS_OF_HPP
#define REACT_INTRINSIC_REQUIREMENTS_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/set.hpp>


namespace react {
namespace requirements_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(requirements)

    template <typename Computation>
    struct nested_requirements {
        using type = typename Computation::requirements;
    };
} // end namespace requirements_of_detail

namespace extension {
    template <typename RawComputation, typename Enable = void>
    struct requirements_of_impl {
        template <typename>
        struct apply
            : boost::mpl::eval_if<
                requirements_of_detail::has_requirements<RawComputation>,
                requirements_of_detail::nested_requirements<RawComputation>,
                boost::mpl::set<>
            >
        { };
    };
} // end namespace extension

/*!
 * @ingroup intrinsics
 *
 * The features that must be satisfied in an environment containing
 * `Computation`.
 */
template <typename Computation>
struct requirements_of
    : extension::requirements_of_impl<
        typename detail::strip<Computation>::type
    >::template apply<Computation>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_REQUIREMENTS_OF_HPP
