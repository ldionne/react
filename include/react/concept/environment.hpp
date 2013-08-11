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
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/add_pointer.hpp>


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
} // end namespace react

#endif // !REACT_CONCEPT_ENVIRONMENT_HPP
