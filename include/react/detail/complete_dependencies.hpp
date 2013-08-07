/*!
 * @file
 * This file defines `react::detail::complete_dependencies`.
 */

#ifndef REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
#define REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP

#include <react/detail/dependency_graph.hpp>
#include <react/intrinsic/default_implementation_of.hpp>
#include <react/intrinsic/name_of.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/at_default.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/graph_intrinsics.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/placeholders.hpp>


namespace react { namespace detail {
    /*!
     * Given a set of computations, generate a superset of it which also
     * satisfies all of its dependencies.
     *
     * If a computation is required to complete the set but is not specified
     * in `Computations`, the default implementation of the computation is
     * generated using the `default_implementation_of` intrinsic.
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

        BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

        template <typename Name, bool always_false = false>
        struct helpful_failure {
            BOOST_MPL_ASSERT_MSG(
                always_false,
        MISSING_A_DEFAULT_IMPLEMENTATION_FOR_THE_FOLLOWING_COMPUTATION_NAME,
                (Name)
            );

            static_assert(always_false,
            "Missing a default implementation for some computation name. "
            "The implementation for that computation name must be provided "
            "explicitly or the computation name must define a default "
            "implementation using the `react::default_implementation_of` "
            "intrinsic.");

            struct type;
        };

        template <typename Name>
        struct GetOrGenerateComputation
            : boost::mpl::lazy_at_default<
                ProvidedComputations, Name,
                boost::mpl::eval_if<has_type<default_implementation_of<Name>>,
                    default_implementation_of<Name>,
                    helpful_failure<Name>
                >
            >
        { };

        using SpannedDependencyGraph = dependency_graph<
            Computations,
            GetOrGenerateComputation<boost::mpl::_1>
        >;

    public:
        using type = typename boost::mpl::vertices_of<
            SpannedDependencyGraph
        >::type;
    };
}} // end namespace react::detail

#endif // !REACT_DETAIL_COMPLETE_DEPENDENCIES_HPP
