/*!
 * @file
 * This file defines the concepts used in the library.
 */

#ifndef REACT_CONCEPTS_HPP
#define REACT_CONCEPTS_HPP

#include <react/archetypes.hpp>
#include <react/detail/auto_return.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/name_of.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <utility>


namespace react {
namespace environment_detail {
    template <typename Sequence>
    using pointers_to = boost::mpl::transform_view<
        Sequence,
        boost::add_pointer<boost::mpl::_1>
    >;

    template <typename Env, typename AvailableNames>
    class BasicEnvironmentCheck {
    protected:
        template <int>
        struct self_named_computation
            : computation_archetype<
                boost::copy_constructible_archetype<
                    boost::default_constructible_archetype<>
                >
            >
        {
            using name = self_named_computation;
        };

        static Env& env;

        static constexpr struct {
            template <typename Name>
            void operator()(Name*) const {
                retrieve<Name>(env);
            }
        } do_retrieve{};

    public:
        BOOST_CONCEPT_USAGE(BasicEnvironmentCheck) {
            execute(env);
            augment(env, self_named_computation<0>{});
            augment(env, self_named_computation<0>{},
                         self_named_computation<1>{});
            augment(env, self_named_computation<0>{},
                         self_named_computation<0>{});
            boost::mpl::for_each<pointers_to<AvailableNames>>(do_retrieve);
        }
    };
} // end namespace environment_detail

template <typename ...ComputationNames>
using available_names = typename boost::mpl::vector<
    ComputationNames...
>::type;

/*!
 * Specification of the `Environment` concept.
 *
 *
 * ## Notation
 * | Expression        | Description
 * | ----------        | -----------
 * | `env`             | An arbitrary `Environment`
 * | `Name`            | A type modeling `ComputationName`
 * | `computations...` | An arbitrary sequence of `Computation`s
 *
 *
 * ## Valid expressions
 * | Expression                      | Return type      | Semantics
 * | ----------                      | -----------      | ---------
 * | `retrieve<Name>(env)`           | Any type         | Return the result of the computation associated to the name `Name` in `env`. If there is no such computation in the environment, the expression shall be ill-formed. See `retrieve` for details.
 * | `execute(env)`                  | Any type         | Execute all of the computations in `env` in an order such that all the dependencies of a computations are executed before it. See `execute` for details.
 * | `augment(env, computations...)` | An `Environment` | Return `env` with `name_of<C>::type` implemented by `C` for all `C` in `computations...`. See `augment` for details.
 *
 *
 * @tparam Env
 *         The type to be tested for modeling of the `Environment` concept.
 *
 * @tparam AvailableNames
 *         A Boost.MPL `ForwardSequence` of `ComputationName`s that can be
 *         retrieved from `Env`.
 */
template <typename Env, typename AvailableNames>
class Environment
    : environment_detail::BasicEnvironmentCheck<Env, AvailableNames>
{
    using Base = environment_detail::BasicEnvironmentCheck<Env, AvailableNames>;

    template <int i>
    using self_named_computation =
                            typename Base::template self_named_computation<i>;

    template <typename Sequence, typename T>
    using push_back = typename boost::mpl::push_back<Sequence, T>::type;

public:
    BOOST_CONCEPT_USAGE(Environment) {
        BOOST_CONCEPT_ASSERT((environment_detail::BasicEnvironmentCheck<
            decltype(augment(Base::env, self_named_computation<0>{})),
            push_back<AvailableNames, self_named_computation<0>>
        >));

        BOOST_CONCEPT_ASSERT((environment_detail::BasicEnvironmentCheck<
            decltype(augment(Base::env, self_named_computation<0>{},
                                        self_named_computation<1>{})),
            push_back<
                push_back<AvailableNames, self_named_computation<0>>,
                self_named_computation<1>
            >
        >));

        BOOST_CONCEPT_ASSERT((environment_detail::BasicEnvironmentCheck<
            decltype(augment(Base::env, self_named_computation<0>{},
                                        self_named_computation<0>{})),
            push_back<AvailableNames, self_named_computation<0>>
        >));
    }
};

/*!
 * Specification of the `ComputationName` concept.
 *
 * A `ComputationName` is an arbitrary type used to represent a `Computation`.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `CN`       | A type modeling the `ComputationName` concept
 *
 *
 * ## Valid expressions
 * | Expression                                          | Return type     | Semantics
 * | ----------                                          | -----------     | ---------
 * | `default_implementation_of<CN>::type<sub>opt</sub>` | A `Computation` | Return the default implementation of the computation represented by `CN`. See `default_implementation_of` for details.
 *
 *
 * @tparam CN
 *         The type to be tested for modeling of the `ComputationName` concept.
 */
template <typename CN>
struct ComputationName {
    BOOST_CONCEPT_USAGE(ComputationName) {
        // Optional requirement:
        // using Default = typename default_implementation_of<CN>::type;
    }
};

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

#endif // !REACT_CONCEPTS_HPP
