/*!
 * @file
 * This file defines `react::detail::complete_dependencies`.
 */

#ifndef REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
#define REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP

#include <react/detail/computation_of.hpp>
#include <react/detail/dependency_graph.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
    /*!
     * Given a set of computations, generate a superset of it which also
     * satisfies all of its dependencies.
     *
     * @tparam Implementations
     *         A Boost.MPL `ForwardSequence` of types modeling the
     *         `Implementation` concept. They represent the computations
     *         whose dependencies must be satisfied. The actual type of which
     *         the dependencies are satisfied is `computation_of<C, M>::type`
     *         for all `C` in `UnboundCustomizedComputations`, with `M` being
     *         a Boost.MPL `AssociativeSequence` mapping `feature_of<C>::type`
     *         to `C` for all `C` in `UnboundCustomizedComputations`.
     *
     * @return A Boost.MPL `ForwardSequence` of `Computation`s.
     */
    template <typename Implementations>
    class complete_dependencies {
        // Note:
        // We use inheritance here to hide the map from any lambda
        // substitution mechanism, since the map could contain lambda
        // placeholders that we want to preserve.
        template <typename Feature>
        struct computation_of
            : detail::computation_of<
                Feature,
                typename boost::mpl::make_index_of<
                    Implementations, feature_of<boost::mpl::_1>
                >::type
            >
        { };

        using CustomizedComputations = boost::mpl::transform_view<
            Implementations,
            computation_of<feature_of<boost::mpl::_1>>
        >;

        using SpannedDependencyGraph = dependency_graph<
            CustomizedComputations, computation_of<boost::mpl::_1>
        >;

    public:
        using type = typename boost::mpl::vertices_of<
            SpannedDependencyGraph
        >::type;
    };
}} // end namespace react::detail

#endif // !REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
