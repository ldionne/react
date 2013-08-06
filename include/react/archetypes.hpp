/*!
 * @file
 * This file defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/retrieve.hpp>
#include <react/intrinsic/update.hpp>

#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>
#include <utility>


namespace react {
    //! Archetype for the `Environment` concept.
    template <typename Base = boost::null_archetype<>>
    struct environment_archetype : Base { };

    namespace extension {
        template <typename Base>
        struct augment_impl<environment_archetype<Base>> {
            template <typename Env, typename ...Computations>
            static auto call(Env&& env, Computations&& ...)
            REACT_AUTO_RETURN(
                std::forward<Env>(env)
            )
        };

        template <typename Base>
        struct update_impl<environment_archetype<Base>> {
            template <typename Env>
            static auto call(Env&& env)
            REACT_AUTO_RETURN(
                std::forward<Env>(env)
            )
        };

        template <typename Base>
        struct retrieve_impl<environment_archetype<Base>> {
            template <typename Name, typename Env>
            static boost::null_archetype<>& call(Env&&) {
                return *(boost::null_archetype<>*)0;
            }
        };
    } // end namespace extension


    template <typename Base = boost::null_archetype<>>
    struct computation_name_archetype;

    //! Archetype for the `Computation` concept.
    template <typename Base = boost::null_archetype<>>
    struct computation_archetype : Base {
        template <typename Self, typename Env>
        static environment_archetype<>& execute(Self&&, Env&&) {
            return *(environment_archetype<>*)0;
        }

        template <typename Self, typename Env>
        static boost::null_archetype<>& retrieve(Self&&, Env&&) {
            return *(boost::null_archetype<>*)0;
        }

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
