/*!
 * @file
 * This file defines `react::detail::topological_indexing`.
 */

#ifndef REACT_DETAIL_TOPOLOGICAL_INDEXING_HPP
#define REACT_DETAIL_TOPOLOGICAL_INDEXING_HPP

#include <react/detail/dependency_graph.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/topological_sort.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
namespace topological_indexing_detail {
    using namespace boost;

    //! Return the index of an element in a Boost.MPL `ForwardSequence`.
    template <typename ForwardSequence, typename Element>
    struct index_of
        : mpl::distance<
            typename mpl::begin<ForwardSequence>::type,
            typename mpl::find<ForwardSequence, Element>::type
        >
    { };

    template <typename Computations>
    struct impl {
        using Graph = dependency_graph<Computations>;

        using OrderedVertices = typename mpl::topological_sort<Graph>::type;

        using OrderedComputations = mpl::transform_view<
            OrderedVertices,
            mpl::at<Graph, mpl::_1>
        >;

        using Indices = mpl::transform_view<
            OrderedComputations,
            index_of<Computations, mpl::_1>
        >;
    };
} // end namespace topological_indexing_detail

/*!
 * Return an indexing of a sequence of computations such that every
 * computation appears after all of its dependencies.
 *
 * @tparam Computations
 *         A Boost.MPL `ForwardSequence` of `Computation`s.
 */
template <typename Computations>
struct topological_indexing {
    using type = typename topological_indexing_detail::impl<
        Computations
    >::Indices;
};
}} // end namespace react::detail

#endif // !REACT_DETAIL_TOPOLOGICAL_INDEXING_HPP
