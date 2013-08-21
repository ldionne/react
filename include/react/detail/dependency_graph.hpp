/*!
 * @file
 * This file defines `react::detail::dependency_graph`.
 */

#ifndef REACT_DETAIL_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_DEPENDENCY_GRAPH_HPP

#include <react/computation/depends_on.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/feature_of.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/filter_view.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/tag_of.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
namespace dependency_graph_detail { struct default_; }

/*!
 * Type representing a compile-time graph of the dependencies between
 * computations.
 *
 * When `GetComputation` is not specified, the generated graph is such that
 * it contains only computations in `Computations`. Otherwise, it contains
 * all of `Computations` and their dependencies, as mapped by `GetComputation`.
 *
 * @tparam Computations
 *         A Boost.MPL `ForwardSequence` of computations whose
 *         dependencies shall generate the graph.
 *
 * @tparam GetComputation
 *         A Boost.MPL `LambdaExpression` returning the computation
 *         associated to the feature it is invoked with.
 */
template <
    typename Computations,
    typename GetComputation = dependency_graph_detail::default_
>
class dependency_graph {
    using Features = boost::mpl::transform_view<
        Computations, feature_of<boost::mpl::_1>
    >;
    using FakeRoot = computation::depends_on<Features>;

public:
    using vertices = typename boost::mpl::erase_key<
        typename boost::mpl::reachable_set<dependency_graph, FakeRoot>::type,
        FakeRoot
    >::type;
};
}} // end namespace react::detail

namespace boost { namespace mpl {
// We need to specialize `mpl::tag_of` because it fails otherwise.
// I think it is due to `vertices` using `mpl::tag_of` while
// `dependency_graph` is still incomplete.
template <typename Computations, typename GetComputation>
struct tag_of<react::detail::dependency_graph<Computations, GetComputation>> {
    using type = react::detail::dependency_graph<Computations, GetComputation>;
};

namespace extension {
    template <typename Computations, typename GetComputation>
    struct open_neighborhood_of_impl<
        react::detail::dependency_graph<Computations, GetComputation>
    > {
        template <typename Graph, typename Computation>
        struct apply {
            using type = transform_view<
                typename react::dependencies_of<Computation>::type,
                GetComputation
            >;
        };
    };

    template <typename Computations>
    struct open_neighborhood_of_impl<
        react::detail::dependency_graph<
            Computations, react::detail::dependency_graph_detail::default_
        >
    > {
        using IndexedByFeature = typename make_index_of<
            Computations, react::feature_of<_1>
        >::type;

        template <typename Graph, typename Computation>
        struct apply {
            using type = transform_view<
                filter_view<
                    typename react::dependencies_of<Computation>::type,
                    has_key<IndexedByFeature, _1>
                >,
                at<IndexedByFeature, _1>
            >;
        };
    };
} // end namespace extension
}} // end namespace boost::mpl

#endif // !REACT_DETAIL_DEPENDENCY_GRAPH_HPP
