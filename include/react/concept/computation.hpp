/*!
 * @file
 * Defines the `react::Computation` concept.
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
#include <type_traits>


namespace react {
template <typename Requirement>
struct fake_result_of {
    template <typename Result>
    using with = boost::mpl::pair<Requirement, Result>;
};

/*!
 * Specification of the `Computation` concept.
 *
 *
 * ## Refinement of
 * `Implementation`
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
 * | Expression                 | Return type
 * | ----------                 | -----------
 * | `execute(c, env)`          | Any type
 * | `retrieve(c, env)`         | Any type
 * | `requirements_of<C>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s
 * | `predecessors_of<C>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s
 * | `successors_of<C>::type`   | A Boost.MPL `AssociativeSequence` of `Feature`s
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
        static typename std::enable_if<
            boost::mpl::has_key<Results, Feature>::type::value,
            boost::mpl::at<Results, Feature>
        >::type::type retrieve(Self&&);
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
