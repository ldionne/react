/*!
 * @file
 * This file defines the `react::Computation` concept.
 */

#ifndef REACT_CONCEPT_COMPUTATION_HPP
#define REACT_CONCEPT_COMPUTATION_HPP

#include <react/archetypes.hpp>
#include <react/concept/implementation.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/concept/usage.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/utility/enable_if.hpp>


namespace react {
template <typename Dependency>
struct fake_result_of {
    template <typename Result>
    using with = boost::mpl::pair<Dependency, Result>;
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
 *
 *
 * ## Valid expressions
 * | Expression                 | Return type                                     | Semantics
 * | ----------                 | -----------                                     | ---------
 * | `execute(c, env)`          | Any type                                        | Execute `c` with `env` as `Environment`. See `execute` for details.
 * | `retrieve(c, env)`         | Any type                                        | Return the result of `c` with `env` as `Environment`. See `retrieve` for details.
 * | `dependencies_of<C>::type` | A Boost.MPL `AssociativeSequence` of `Feature`s | The features of the computations that must be executed before `c` when an `Environment` is executed. See `dependencies_of` for details.
 *
 *
 * @tparam C
 *         The type to be tested for modeling of the `Computation` concept.
 *
 * @tparam ResultsOfDependencies...
 *         A sequence of instantiations of `fake_result_of<>::with`
 *         associating each dependency of `C` to its result type.
 */
template <typename C, typename ...ResultsOfDependencies>
class Computation : Implementation<C> {
    using Results = typename boost::mpl::map<ResultsOfDependencies...>::type;

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

        using Dependencies = typename dependencies_of<C>::type;
        static_assert(boost::mpl::is_sequence<Dependencies>::value, "");
    }
};
} // end namespace react

#endif // !REACT_CONCEPT_COMPUTATION_HPP
