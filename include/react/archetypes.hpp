/*!
 * @file
 * This file defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>
#include <utility>


namespace react {
    //! Archetype for the `Environment` concept.
    template <typename Base = boost::null_archetype<>>
    struct environment_archetype : Base {
        template <typename Self>
        static boost::copy_constructible_archetype<> execute(Self&&);

        template <typename ComputationName, typename Self>
        static boost::copy_constructible_archetype<> retrieve(Self&&);

        template <typename Self, typename ...Computations>
        static environment_archetype<boost::copy_constructible_archetype<>>
        augment(Self&&, Computations&& ...);
    };


    template <typename Base = boost::null_archetype<>>
    struct computation_name_archetype;

    //! Archetype for the `Computation` concept.
    template <typename Base = boost::null_archetype<>>
    struct computation_archetype : Base {
        template <typename Self, typename Env>
        static boost::copy_constructible_archetype<> execute(Self&&, Env&&);

        template <typename Self, typename Env>
        static boost::copy_constructible_archetype<> retrieve(Self&&, Env&&);

        using dependencies = typename boost::mpl::set<>::type;
        using name = computation_name_archetype<>;
    };

    //! Archetype for the `ComputationName` concept.
    template <typename Base>
    struct computation_name_archetype : Base {
        using default_implementation = computation_archetype<>;
    };
} // end namespace react

#endif // !REACT_ARCHETYPES_HPP
