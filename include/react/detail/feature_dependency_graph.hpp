/*!
 * @file
 * This file defines `react::detail::feature_dependency_graph`.
 */

#ifndef REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP

#include <react/archetypes.hpp>
#include <react/traits.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
namespace feature_dependency_graph_detail {
    namespace mpl = boost::mpl;

    /*!
     * Compile-time graph representing dependencies between the features
     * of a `FeatureSet`.
     */
    template <typename ComputationOf, typename TopLevelComputations>
    class feature_dependency_graph {
        template <typename ...Args>
        struct computation_of
            : mpl::apply<ComputationOf, Args...>
        { };

        //! A vertex in the feature dependency graph represents a computation
        //! to be implemented in a feature set.
        template <typename Computation>
        struct vertex {
            using computation = Computation;

            using adjacent_vertices = mpl::transform_view<
                typename dependencies_of<Computation>::type,
                vertex<computation_of<mpl::_1>>
            >;
        };

        struct pseudo_root_computation : incremental_computation_archetype<> {
            using dependencies = typename mpl::fold<
                TopLevelComputations,
                mpl::set<>,
                mpl::insert<mpl::_1, feature_of<mpl::_2>>
            >::type;
        };

        using root_vertex = vertex<pseudo_root_computation>;

    public:
        //! Set of all the vertices in the feature dependency graph.
        using vertices = typename mpl::reachable_set<root_vertex>::type;
    };
} // end namespace feature_dependency_graph_detail

using feature_dependency_graph_detail::feature_dependency_graph;
}} // end namespace react::detail

#endif // !REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP
