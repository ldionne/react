/*!
 * @file
 * This file defines `react::detail::complete_dependencies`.
 */

#ifndef REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
#define REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP

#include <react/detail/dependency_graph.hpp>
#include <react/traits.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>


namespace react { namespace detail {
    /*!
     * Given a set of computations, generate a superset of it which also
     * satisfies all of its dependencies.
     *
     * If a computation is required to complete the set but is not specified
     * in `Computations`, the default implementation of the computation is
     * generated using the `default_computation` trait.
     *
     * @tparam Computations
     *         A Boost.MPL `ForwardSequence` of `Computation`s representing
     *         the computations whose dependencies must be satisfied.
     *
     * @return A Boost.MPL `ForwardSequence` of `Computation`s.
     */
    template <typename Computations>
    class complete_dependencies {
        using ProvidedComputations = typename boost::mpl::make_index_of<
            Computations, name_of<boost::mpl::_1>
        >::type;

        template <typename Name>
        struct get_computation
            : boost::mpl::eval_if<
                boost::mpl::has_key<ProvidedComputations, Name>,
                boost::mpl::at<ProvidedComputations, Name>,
                default_computation<Name>
            >
        { };

        using SpannedDependencyGraph = dependency_graph<
            Computations, get_computation<boost::mpl::_1>
        >;

    public:
        using type = typename boost::mpl::vertices_of<
            SpannedDependencyGraph
        >::type;
    };
}} // end namespace react::detail

#endif // !REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
