/*!
 * @file
 * This file defines the concepts used in the library.
 */

#ifndef REACT_CONCEPTS_HPP
#define REACT_CONCEPTS_HPP

#include <react/archetypes.hpp>
#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>
#include <react/traits.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_void.hpp>
#include <utility>


namespace react {
template <typename ...Pairs>
using dependency_results = typename boost::mpl::map<Pairs...>::type;

template <typename ...Computations>
using implemented_computations = typename boost::mpl::vector<
    Computations...
>::type;

/*!
 * Specification of the `Environment` concept.
 *
 *
 * ## Notation
 * | Expression        | Description
 * | ----------        | -----------
 * | `env`             | An arbitrary `Environment`
 * | `Name`            | An arbitrary type
 * | `computations...` | An arbitrary sequence of `Computation`s
 *
 *
 * ## Valid expressions
 * | Expression                      | Return type      | Semantics
 * | ----------                      | -----------      | ---------
 * | `retrieve<Name>(env)`           | Any type         | Return the result of the computation associated to the name `Name` in `env`. If there is no such computation in the environment, the expression shall be ill-formed.
 * | `update(env)`                   | An `Environment` | Execute all of the computations in `env` in an order such that all the dependencies of a computations are executed before it. The result of executing the computations is folded into a new environment, which is returned. For any computation `c`, if `execute(c, env)` returns `void` or is ill-formed, the result is folded as if `execute(c, env)` had returned `augment(env, c)`.
 * | `augment(env, computations...)` | An `Environment` | Return `env` with `computations...` available in it.
 *
 *
 * @tparam Env
 *         The type to be tested for modeling of the `Environment` concept.
 *
 * @tparam ImplementedComputations
 *         A Boost.MPL `ForwardSequence` of computation or computation names
 *         that can be retrieved from `Env`.
 */
template <typename Env, typename ImplementedComputations>
class Environment {
    static Env& env;

    using ComputationArchetype = computation_archetype<
        boost::copy_constructible_archetype<
            boost::default_constructible_archetype<>>
    >;

    static constexpr struct {
        template <typename Name>
        void operator()(Name*) const {
            retrieve<Name>(env);
        }
    } do_retrieve{};

    template <typename E>
    struct basic_env_check {
        BOOST_CONCEPT_USAGE(basic_env_check) {
            update(e);
            augment(e, ComputationArchetype{});
        }
        static E& e;
    };

public:
    BOOST_CONCEPT_USAGE(Environment) {
        boost::mpl::for_each<
            boost::mpl::transform_view<
                ImplementedComputations,
                boost::add_pointer<name_of<boost::mpl::_1>>
            >
        >(do_retrieve);

        update(env);
        BOOST_CONCEPT_ASSERT((basic_env_check<
            decltype(update(env))
        >));

        augment(env, ComputationArchetype{});
        BOOST_CONCEPT_ASSERT((basic_env_check<
            decltype(augment(env, ComputationArchetype{}))
        >));
    }
};

namespace computation_detail {
    template <typename DependenciesResults>
    struct environment : environment_archetype<> { };
}

namespace extensions {
    template <typename DependenciesResults>
    struct retrieve<computation_detail::environment<DependenciesResults>> {
        template <typename Name, typename Env>
        static typename boost::mpl::at<DependenciesResults, Name>::type
        call(Env&&);
    };

    template <typename DependenciesResults>
    struct update<computation_detail::environment<DependenciesResults>>
        : update<environment_archetype<>>
    { };

    template <typename DependenciesResults>
    struct augment<computation_detail::environment<DependenciesResults>>
        : augment<environment_archetype<>>
    { };
} // end namespace extensions

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
 * | Expression                      | Return type                       | Semantics
 * | ----------                      | -----------                       | ---------
 * | `execute(c, env)`               | `void` or an `Environment`        | Execute the computation with an environment and return an updated environment. See `execute` for details.
 * | `c.retrieve(env)`<sub>opt</sub> | Any type                          | Return the result of the computation.
 * | `dependencies_of<C>::type`      | A Boost.MPL `AssociativeSequence` | The names of the computations required in an `Environment` in order for this computation to be available. See `dependencies_of` for details.
 * | `name_of<C>::type`              | Any type                          | The name associated to `C`. See `name_of` for details.
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
        get<C_>().retrieve(get<Env>())
    )

    template <typename C_, typename Env>
    static void try_retrieve(...) { }

    using GoodEnoughEnv = computation_detail::environment<DependenciesResults>;

public:
    BOOST_CONCEPT_USAGE(Computation) {
        execute(get<C>(), get<GoodEnoughEnv>());

        using ExecuteResult = decltype(execute(get<C>(), get<GoodEnoughEnv>()));
        BOOST_CONCEPT_ASSERT((typename boost::mpl::if_<
            boost::is_void<ExecuteResult>,
            null_concept,
            Environment<ExecuteResult, implemented_computations<>>
        >::type));

        try_retrieve<C, GoodEnoughEnv>(0);

        using Dependencies = typename dependencies_of<C>::type;
        static_assert(boost::mpl::is_sequence<Dependencies>::value, "");

        using Name = typename name_of<C>::type;
    }
};
} // end namespace react

#endif // !REACT_CONCEPTS_HPP
