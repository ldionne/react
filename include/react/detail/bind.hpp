/*!
 * @file
 * This file defines `react::detail::bind`.
 */

#ifndef REACT_DETAIL_BIND_HPP
#define REACT_DETAIL_BIND_HPP

#include <react/detail/dependency_graph.hpp>
#include <react/placeholders.hpp>
#include <react/traits.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/topological_sort.hpp>
#include <boost/mpl/value_view.hpp>
#include <boost/type_traits/is_same.hpp>


namespace react { namespace detail {
namespace bind_detail {
    namespace mpl = boost::mpl;

    template <typename UnboundComputations>
    using in_topological_order = typename mpl::topological_sort<
        dependency_graph<UnboundComputations>
    >::type;

    template <typename UnboundComputation, typename BoundDependencyMap>
    struct substitute_dependencies {
        using type = UnboundComputation;
    };

    template <template <typename ...> class UnboundComputation, typename ...T,
              typename BoundDependencyMap>
    struct substitute_dependencies<
        UnboundComputation<T...>, BoundDependencyMap
    > {
        using type = UnboundComputation<
            typename mpl::if_<boost::is_same<T, placeholders::dependencies>,
                BoundDependencyMap, T
            >::type...
        >;
    };

    template <typename UnboundComputations>
    struct bound_computation_map
        : mpl::fold<
            in_topological_order<UnboundComputations>,
            mpl::map<>,
            mpl::insert<mpl::_1,
                mpl::pair<
                    name_of<substitute_dependencies<mpl::_2, mpl::_1>>,
                            substitute_dependencies<mpl::_2, mpl::_1>
                >
            >
        >
    { };
} // end namespace bind_detail

/*!
 * Substitute special placeholders in a sequence of computations according
 * to the semantics of each placeholder.
 *
 * @note To perform the substitution, all the dependencies of each computation
 *       must be satisfied in the sequence.
 *
 * @tparam UnboundComputations
 *         A Boost.MPL `ForwardSequence` of `Computation`s or
 *         `LambdaExpression`s containing placeholders to be
 *         substituted.
 *
 * @return A Boost.MPL `ForwardSequence` of `Computation`s. Note that the
 *         order is not guaranteed to be the same as that of the initial
 *         sequence.
 */
template <typename UnboundComputations>
struct bind {
    using type = boost::mpl::value_view<
        typename bind_detail::bound_computation_map<UnboundComputations>::type
    >;
};
}} // end namespace react::detail

#endif // !REACT_DETAIL_BIND_HPP
