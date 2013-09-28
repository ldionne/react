/*!
 * @file
 * Defines `react::detail::feature_execution_graph`.
 */

#ifndef REACT_DETAIL_FEATURE_EXECUTION_GRAPH_HPP
#define REACT_DETAIL_FEATURE_EXECUTION_GRAPH_HPP

#include <react/feature_of.hpp>

#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/transform_view.hpp>
#include <type_traits>


namespace react {
namespace graph_detail {
    using namespace boost::mpl;

    template <typename Graph>
    struct edges;

    template <typename From, typename To>
    struct edge {
        using source = From;
        using target = To;
    };

    template <typename Edge>
    struct source {
        using type = typename Edge::source;
    };

    template <typename Edge>
    struct target {
        using type = typename Edge::target;
    };

    template <typename Vertex, typename Graph>
    struct out_edges
        : filter_view<
            typename edges<Graph>::type,
            std::is_same<Vertex, source<_1>>
        >
    { };

    template <typename Vertex, typename Graph>
    struct in_edges
        : filter_view<
            typename edges<Graph>::type,
            std::is_same<Vertex, target<_1>>
        >
    { };
} // end namespace graph_detail

namespace feg_detail {
    using namespace boost::mpl;

    template <typename Computation>
    struct direct_successors_of {

    };

    template <typename Computation>
    struct direct_predecessors_of {

    };

    template <typename Computation>
    struct edges_added_by
        : joint_view<
            transform_view<
                typename direct_successors_of<Computation>::type,
                edge<typename feature_of<Computation>::type, _1>
            >,
            transform_view<
                typename direct_predecessors_of<Computation>::type,
                edge<_1, typename feature_of<Computation>::type>
            >
        >
    { };

    template <typename Computations>
    struct feature_execution_graph {
        using computations = Computations;
    };

    template <typename Graph>
    struct edges
        : fold<typename Graph::computations,
            set<>,
            set_insert_range<_1, edges_added_by<_2>>
        >
    { };
} // end namespace feg_detail

namespace detail {

} // end namespace detail
} // end namespace react

#endif // !REACT_DETAIL_FEATURE_EXECUTION_GRAPH_HPP
