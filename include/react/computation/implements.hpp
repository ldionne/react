/*!
 * @file
 * This file defines `react::computation::implements`.
 */

#ifndef REACT_COMPUTATION_IMPLEMENTS_HPP
#define REACT_COMPUTATION_IMPLEMENTS_HPP

#include <react/computation/noop.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/mpl/always.hpp>


namespace react {
namespace computation {
    /*!
     * Computation associating a specific feature to a computation.
     *
     * @tparam Feature
     *         The feature representing the computation.
     *
     * @tparam Computation
     *         The computation to wrap in such a way that
     *         `feature_of<Computation>::type` is the same
     *         as `Feature`. It defaults to `noop`.
     */
    template <typename Feature, typename Computation = noop>
    struct implements : Computation {
        using Computation::Computation;
        using Computation::operator=;
        using feature = Feature;
    };
} // end namespace computation

namespace extension {
    template <typename Feature, typename Computation>
    struct feature_of_impl<computation::implements<Feature, Computation>>
        : boost::mpl::always<Feature>
    { };
} // end namespace extension
} // end namespace react

#endif // !REACT_COMPUTATION_IMPLEMENTS_HPP
