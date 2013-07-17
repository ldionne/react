/*!
 * @file
 * This file defines `react::detail::dependency_graph`.
 */

#ifndef REACT_DETAIL_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_DEPENDENCY_GRAPH_HPP

#include <react/computations/depends_on.hpp>
#include <react/traits.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/type_traits/is_same.hpp>


namespace react { namespace detail {
namespace dependency_graph_detail {
    using namespace boost;

    template <typename ComputationName>
    using is_computation_bundle = mpl::false_;

    template <typename ComputationBundle>
    using computations_in = ComputationBundle;

    struct graph_tag;

    template <typename ComputationOf, typename ComputationNames>
    class graph {
        template <typename ComputationName>
        struct computation_of
            : mpl::apply<ComputationOf, ComputationName>
        { };

        template <typename Computation>
        class vertex {
            using computation = Computation;
            friend mpl::at_impl<graph_tag>;

        public:
            using open_neighborhood = mpl::transform_view<
                typename dependencies_of<computation>::type,
                mpl::inherit<
                    mpl::if_<is_computation_bundle<mpl::_1>,
                        graph<ComputationOf, computations_in<mpl::_1>>,
                        mpl::empty_base
                    >,
                    vertex<computation_of<mpl::_1>>
                >
            >;
        };

        using fake_root_vertex = vertex<
            computations::depends_on<ComputationNames>
        >;

    public:
        using tag = graph_tag; // for mpl::sequence_tag required by mpl::at

        using vertices = typename mpl::erase_key<
            typename mpl::reachable_set<fake_root_vertex>::type,
            fake_root_vertex
        >::type;
    };

    template <typename Computations>
    struct find_computation_by_name {
        using NotFound = typename mpl::end<Computations>::type;

        template <typename Name>
        struct apply {
            using Iter = typename mpl::find_if<
                Computations,
                is_same<Name, name_of<mpl::_1>>
            >::type;

            static constexpr bool was_found = !is_same<Iter, NotFound>::value;

            BOOST_MPL_ASSERT_MSG(
                was_found,
                unable_to_find_some_computation_in_the_set_of_computations_spanning_the_dependency_graph,
                (Name, Computations)
            );

            static_assert(was_found,
            "unable to find some computation in the set of computations "
            "spanning the dependency graph");

            using type = typename mpl::deref<Iter>::type;
        };
    };
} // end namespace dependency_graph_detail

/*!
 * Type representing a compile-time graph of the dependencies between
 * computations.
 *
 * The `dependency_graph` also supports `boost::mpl::at`, which maps any of
 * its vertices to the `Computation` it represents.
 *
 * @tparam Computations
 *         A Boost.MPL `ForwardSequence` of `Computation`s.
 */
template <typename Computations>
struct dependency_graph
    : dependency_graph_detail::graph<
        dependency_graph_detail::find_computation_by_name<Computations>,
        boost::mpl::transform_view<Computations, name_of<boost::mpl::_1>>
    >
{ };
}} // end namespace react::detail

namespace boost { namespace mpl {
    template <>
    struct at_impl<react::detail::dependency_graph_detail::graph_tag> {
        template <typename Graph, typename Vertex>
        struct apply {
            using type = typename Vertex::computation;
        };
    };
}} // end namespace boost::mpl

#endif // !REACT_DETAIL_DEPENDENCY_GRAPH_HPP
