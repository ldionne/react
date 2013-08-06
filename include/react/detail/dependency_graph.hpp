/*!
 * @file
 * This file defines `react::detail::dependency_graph`.
 */

#ifndef REACT_DETAIL_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_DEPENDENCY_GRAPH_HPP

#include <react/computation/depends_on.hpp>
#include <react/traits.hpp>

#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/tag_of.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
namespace dependency_graph_detail {
    template <typename Computations>
    struct default_getter {
        using IndexedByName = typename boost::mpl::make_index_of<
            Computations, name_of<boost::mpl::_1>
        >::type;

        template <typename Name>
        struct apply : boost::mpl::at<IndexedByName, Name> {
            static constexpr bool found = boost::mpl::has_key<
                IndexedByName, Name
            >::value;

            BOOST_MPL_ASSERT_MSG(
                found,
unable_to_find_the_computation_associated_to_some_name_in_the_dependency_graph,
                (Name, IndexedByName)
            );

            static_assert(found,
            "unable to find the computation associated to "
            "some name in the dependency graph ");
        };
    };
} // end namespace dependency_graph_detail

/*!
 * Type representing a compile-time graph of the dependencies between
 * computations.
 *
 * @tparam Computations
 *         A Boost.MPL `ForwardSequence` of computations whose
 *         dependencies shall generate the graph.
 *
 * @tparam GetComputation
 *         A Boost.MPL `LambdaExpression` returning the computation
 *         associated to the computation name (an arbitrary type) it is
 *         invoked with. It defaults to fetching the result in a map `M`
 *         such that `boost::mpl::at<M, name_of<C>::type>::type` is `C`
 *         for every `C` in `Computations`.
 */
template <
    typename Computations,
    typename GetComputation =
                        dependency_graph_detail::default_getter<Computations>
>
class dependency_graph {
    using ComputationNames = boost::mpl::transform_view<
        Computations, name_of<boost::mpl::_1>
    >;
    using FakeRoot = computation::depends_on<ComputationNames>;

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
                apply_wrap1<typename lambda<GetComputation>::type, _1>
            >;
        };
    };
} // end namespace extension
}} // end namespace boost::mpl

#endif // !REACT_DETAIL_DEPENDENCY_GRAPH_HPP
