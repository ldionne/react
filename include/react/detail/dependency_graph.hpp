/*!
 * @file
 * This file defines `react::detail::dependency_graph`.
 */

#ifndef REACT_DETAIL_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_DEPENDENCY_GRAPH_HPP

#include <react/computations/depends_on.hpp>
#include <react/traits.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
struct dependency_graph_tag;

/*!
 * Type representing a compile-time graph of the dependencies between
 * computations.
 *
 * The `dependency_graph` also supports `boost::mpl::at`, which maps any
 * computation name to the `Computation` associated to it in the graph.
 *
 * @tparam Computations
 *         A Boost.MPL `ForwardSequence` of `Computation`s.
 */
template <typename Computations>
struct dependency_graph {
    // for graph intrinsics
    struct mpl { using tag = dependency_graph_tag; };

    // for mpl::sequence_tag required by mpl::at
    using tag = typename mpl::tag;

private:
    using ComputationNames = boost::mpl::transform_view<
        Computations, name_of<boost::mpl::_1>
    >;
    using Root = computations::depends_on<ComputationNames>;

    using ComputationsByName = typename boost::mpl::make_index_of<
        Computations, name_of<boost::mpl::_1>
    >::type;

    friend boost::mpl::at_impl<dependency_graph_tag>;
    template <typename Name>
    class at : public boost::mpl::at<ComputationsByName, Name> {
        static constexpr bool found = boost::mpl::has_key<
            ComputationsByName, Name
        >::value;

        BOOST_MPL_ASSERT_MSG(
            found,
            unable_to_find_some_computation_in_the_set_of_computations_spanning_the_dependency_graph,
            (Name, ComputationsByName)
        );

        static_assert(found,
        "unable to find some computation in the set of computations "
        "spanning the dependency graph");
    };

public:
    using vertices = typename boost::mpl::erase_key<
        typename boost::mpl::reachable_set<dependency_graph, Root>::type, Root
    >::type;
};
}} // end namespace react::detail

namespace boost { namespace mpl {
// We need to specialize `mpl::tag_of` because `vertices` uses it while
// `dependency_graph` is still incomplete. Because of this, it appears
// that `tag_of` can't fetch the nested `mpl::tag`.
template <typename Computations>
struct tag_of<react::detail::dependency_graph<Computations>> {
    using type = react::detail::dependency_graph_tag;
};

template <>
struct at_impl<react::detail::dependency_graph_tag> {
    template <typename Graph, typename Name>
    struct apply
        : Graph::template at<Name>
    { };
};

namespace extension {
    template <>
    struct open_neighborhood_of_impl<react::detail::dependency_graph_tag> {
        template <typename Graph, typename Computation>
        struct apply {
            using type = transform_view<
                typename react::dependencies_of<Computation>::type,
                at<Graph, _1>
            >;
        };
    };
} // end namespace extension
}} // end namespace boost::mpl

#endif // !REACT_DETAIL_DEPENDENCY_GRAPH_HPP
