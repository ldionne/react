/*!
 * @file
 * This file defines the `react::Feature` concept.
 */

#ifndef REACT_CONCEPT_FEATURE_HPP
#define REACT_CONCEPT_FEATURE_HPP

#include <react/intrinsic/default_implementation_of.hpp>
#include <react/intrinsic/has_default_implementation.hpp>

#include <boost/concept/usage.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/void.hpp>


namespace react {
/*!
 * Specification of the `Feature` concept.
 *
 * A `Feature` represents an abstract functionality provided by a
 * `Computation`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `F`        | A type modeling the `Feature` concept
 *
 *
 * ## Valid expressions
 * | Expression                                         | Return type                            | Semantics
 * | ----------                                         | -----------                            | ---------
 * | `default_implementation_of<F>::type<sub>opt</sub>` | A `Computation`                        | Return the default implementation of the computation represented by `F`. See `default_implementation_of` for details.
 * | `has_default_implementation<F>::type`              | A Boost.MPL boolean `IntegralConstant` | Return whether `F` has a default implementation. See `has_default_implementation` for details.
 *
 *
 * @tparam F
 *         The type to be tested for modeling of the `Feature` concept.
 */
template <typename F>
struct Feature {
    BOOST_CONCEPT_USAGE(Feature) {
        using HasDefault = typename has_default_implementation<F>::type;

        using Default = typename boost::mpl::eval_if<HasDefault,
            default_implementation_of<F>,
            boost::mpl::void_
        >::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_FEATURE_HPP
