/*!
 * @file
 * This file defines the `react::Computation` concept.
 */

#ifndef REACT_CONCEPT_COMPUTATION_HPP
#define REACT_CONCEPT_COMPUTATION_HPP

#include <react/archetypes.hpp>
#include <react/concept/computation_name.hpp>
#include <react/detail/auto_return.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/name_of.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <utility>


namespace react {
namespace computation_detail {
    template <typename DependenciesResults>
    struct environment : environment_archetype<> { };
} // end namespace computation_detail

namespace extension {
    template <typename DependenciesResults>
    struct retrieve_impl<computation_detail::environment<DependenciesResults>>
    {
        template <typename Name, typename Env>
        static typename boost::mpl::at<DependenciesResults, Name>::type
        call(Env&&);
    };

    template <typename _>
    struct execute_impl<computation_detail::environment<_>> {
        static auto call(computation_detail::environment<_> const& env)
        REACT_AUTO_RETURN(
            execute(static_cast<environment_archetype<> const&>(env))
        )

        static auto call(computation_detail::environment<_>& env)
        REACT_AUTO_RETURN(
            execute(static_cast<environment_archetype<>&>(env))
        )
    };

    template <typename _>
    struct augment_impl<computation_detail::environment<_>> {
        template <typename ...Computations>
        static auto
        call(computation_detail::environment<_>& env, Computations&& ...c)
        REACT_AUTO_RETURN(
            augment(static_cast<environment_archetype<>&>(env),
                    std::forward<Computations>(c)...)
        )

        template <typename ...Computations>
        static auto call(computation_detail::environment<_> const& env,
                         Computations&& ...c)
        REACT_AUTO_RETURN(
            augment(static_cast<environment_archetype<> const&>(env),
                    std::forward<Computations>(c)...)
        )
    };
} // end namespace extension

template <typename ...Pairs>
using dependencies_results = typename boost::mpl::map<Pairs...>::type;

/*!
 * Specification of the `Computation` concept.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `C`        | A type modeling the `Computation` concept
 * | `c`        | An object of type `C`
 * | `env`      | An arbitrary `Environment`
 *
 *
 * ## Valid expressions
 * | Expression                       | Return type                       | Semantics
 * | ----------                       | -----------                       | ---------
 * | `execute(c, env)`                | Any type                          | Execute the computation with an environment. See `execute` for details.
 * | `retrieve(c, env)<sub>opt</sub>` | Any type                          | Return the result of the computation. See `retrieve` for details.
 * | `dependencies_of<C>::type`       | A Boost.MPL `AssociativeSequence` | The names of the computations required in an `Environment` in order for this computation to be available. See `dependencies_of` for details.
 * | `name_of<C>::type`               | A `ComputationName`               | The name associated to `C`. See `name_of` for details.
 *
 *
 * @tparam C
 *         The type to be tested for modeling of the `Computation` concept.
 *
 * @tparam DependenciesResults
 *         A Boost.MPL `AssociativeSequence` mapping each dependency of `C`
 *         to its result type.
 */
template <typename C, typename DependenciesResults>
class Computation {
    struct null_concept { BOOST_CONCEPT_USAGE(null_concept) { } };

    template <typename T> static T get();

    template <typename C_, typename Env>
    static auto try_retrieve(int) REACT_AUTO_RETURN(
        retrieve(get<C_>(), get<Env>())
    )

    template <typename C_, typename Env>
    static void try_retrieve(...) { }

    using GoodEnoughEnv = computation_detail::environment<DependenciesResults>;

public:
    BOOST_CONCEPT_USAGE(Computation) {
        execute(get<C>(), get<GoodEnoughEnv>());
        try_retrieve<C, GoodEnoughEnv>(0);

        using Dependencies = typename dependencies_of<C>::type;
        static_assert(boost::mpl::is_sequence<Dependencies>::value, "");

        using Name = typename name_of<C>::type;
        BOOST_CONCEPT_ASSERT((ComputationName<Name>));
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_COMPUTATION_HPP
