/*!
 * @file
 * This file defines the `react::Computation` concept.
 */

#ifndef REACT_CONCEPT_COMPUTATION_HPP
#define REACT_CONCEPT_COMPUTATION_HPP

#include <react/archetypes.hpp>
#include <react/concept/implementation.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/predecessors_of.hpp>
#include <react/intrinsic/requirements_of.hpp>
#include <react/intrinsic/retrieve.hpp>
#include <react/intrinsic/successors_of.hpp>

#include <boost/concept/usage.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/utility/enable_if.hpp>


namespace react {
template <typename Requirement>
struct fake_result_of {
    template <typename Result>
    using with = boost::mpl::pair<Requirement, Result>;
};

/*!
 * Specification of the `Computation` concept. `Computation` is a refinement
 * of the `Implementation` concept.
 *
 *
 * ## Notation
 * | Expression | Description
 * | ---------- | -----------
 * | `C`        | A type modeling the `Computation` concept
 * | `c`        | An object of type `C`
 * | `env`      | An arbitrary `Environment`
 * | `F`        | A type modeling the `Feature` concept
 *
 *
 * ## Valid expressions
 * | Expression                 | Return type                                     | Semantics
 * | ----------                 | -----------                                     | ---------
 * | `execute(c, env)`          | Any type                                        | Execute `c` with `env` as `Environment`. See `execute` for details.
 * | `retrieve(c, env)`         | Any type                                        | Return the result of `c` with `env` as `Environment`. See `retrieve` for details.
 * | `requirements_of<C>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s | The features that must be satisfied in an environment containing `C`. See `requirements_of` for details.
 * | `predecessors_of<C>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s | The features of the computations that must be executed before `C` when an `Environment` is executed. See `predecessors_of` for details.
 * | `successors_of<C>::type`   | A Boost.MPL `AssociativeSequence` of `Feature`s | The features of the computations that must be executed after `C` when an `Environment` is executed. See `successors_of` for details.
 *
 *
 * @tparam C
 *         The type to be tested for modeling of the `Computation` concept.
 *
 * @tparam ResultsOfRequirements...
 *         A sequence of instantiations of `fake_result_of<>::with`
 *         associating each requirement of `C` to its result type.
 */
template <typename C, typename ...ResultsOfRequirements>
class Computation : Implementation<C> {
    using Results = typename boost::mpl::map<ResultsOfRequirements...>::type;

    struct GoodEnoughEnv : environment_archetype<> {
        template <typename Feature, typename Self>
        static typename boost::lazy_enable_if<
            boost::mpl::has_key<Results, Feature>,
            boost::mpl::at<Results, Feature>
        >::type retrieve(Self&&);
    };

    static GoodEnoughEnv& env;
    static C& c;

public:
    BOOST_CONCEPT_USAGE(Computation) {
        execute(c, env);
        retrieve(c, env);

        using Predecessors = typename predecessors_of<C>::type;
        static_assert(boost::mpl::is_sequence<Predecessors>::value, "");

        using Successors = typename successors_of<C>::type;
        static_assert(boost::mpl::is_sequence<Successors>::value, "");

        using Requirements = typename requirements_of<C>::type;
        static_assert(boost::mpl::is_sequence<Requirements>::value, "");
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_COMPUTATION_HPP
