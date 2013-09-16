/*!
 * @file
 * This file defines the `react::Environment` concept.
 */

#ifndef REACT_CONCEPT_ENVIRONMENT_HPP
#define REACT_CONCEPT_ENVIRONMENT_HPP

#include <react/archetypes.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/concept_archetype.hpp>


namespace react {
namespace environment_detail {
    template <int>
    struct comp
        : computation_archetype<
          boost::copy_constructible_archetype<
          boost::default_constructible_archetype<>>>
    { using feature = comp; };

    template <typename Env, typename ...AvailableFeatures>
    struct BasicEnvironment {
        BOOST_CONCEPT_USAGE(BasicEnvironment) {
            execute(env);

            augment(env, comp<0>{});
            augment(env, comp<0>{}, comp<1>{});
            augment(env, comp<0>{}, comp<0>{});

            allow_expansion((retrieve<AvailableFeatures>(env), 0)...);
        }

    private:
        static Env& env;

        template <typename ...Args>
        static void allow_expansion(Args&& ...);
    };
} // end namespace environment_detail

/*!
 * Specification of the `Environment` concept.
 *
 *
 * ## Notation
 * | Expression        | Description
 * | ----------        | -----------
 * | `env`             | An arbitrary `Environment`
 * | `F`               | A type modeling the `Feature` concept
 * | `computations...` | An arbitrary sequence of `Computation`s
 *
 *
 * ## Valid expressions
 * | Expression                      | Return type
 * | ----------                      | -----------
 * | `retrieve<F>(env)`              | Any type
 * | `execute(env)`                  | Any type
 * | `augment(env, computations...)` | An `Environment`
 *
 *
 * @tparam Env
 *         The type to be tested for modeling of the `Environment` concept.
 *
 * @tparam AvailableFeatures...
 *         A sequence of `Feature`s that can be retrieved from `Env`.
 */
template <typename Env, typename ...AvailableFeatures>
struct Environment
    : environment_detail::BasicEnvironment<Env, AvailableFeatures...>
{
    BOOST_CONCEPT_USAGE(Environment) {
        using namespace environment_detail;

        BOOST_CONCEPT_ASSERT((BasicEnvironment<
            decltype(augment(env, comp<0>{})),
            AvailableFeatures..., comp<0>
        >));

        BOOST_CONCEPT_ASSERT((BasicEnvironment<
            decltype(augment(env, comp<0>{}, comp<1>{})),
            AvailableFeatures..., comp<0>, comp<1>
        >));

        BOOST_CONCEPT_ASSERT((BasicEnvironment<
            decltype(augment(env, comp<0>{}, comp<0>{})),
            AvailableFeatures..., comp<0>
        >));
    }

private:
    static Env& env;
};

template <typename ...Features>
struct available_features;

template <typename Env, typename ...AvailableFeatures>
struct Environment<Env, available_features<AvailableFeatures...>>
    : Environment<Env, AvailableFeatures...>
{ };
} // end namespace react

#endif // !REACT_CONCEPT_ENVIRONMENT_HPP
