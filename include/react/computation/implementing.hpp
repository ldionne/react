/*!
 * @file
 * This file defines `react::computation::implementing`.
 */

#ifndef REACT_COMPUTATION_IMPLEMENTING_HPP
#define REACT_COMPUTATION_IMPLEMENTING_HPP

#include <react/computation/noop.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/mpl/always.hpp>


namespace react {
namespace computation {
    /*!
     * Wrapper associating a specific feature to another computation.
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
    struct implementing : Computation {
        using Computation::Computation;
        using Computation::operator=;
        using feature = Feature;
    };
} // end namespace computation

namespace extension {
    template <typename Feature, typename Computation>
    struct feature_of_impl<computation::implementing<Feature, Computation>>
        : boost::mpl::always<Feature>
    { };
} // end namespace extension
} // end namespace react

#endif // !REACT_COMPUTATION_IMPLEMENTING_HPP
