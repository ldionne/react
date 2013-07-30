/*!
 * @file
 * This file defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>
#include <utility>


namespace react {
    //! Archetype for the `Environment` concept.
    template <typename Base = boost::null_archetype<>>
    struct environment_archetype : Base { };

    namespace extensions {
        template <typename Base>
        struct augment<environment_archetype<Base>> {
            template <typename Env, typename ...Computations>
            static auto call(Env&& env, Computations&& ...)
            REACT_AUTO_RETURN(
                std::forward<Env>(env)
            )
        };

        template <typename Base>
        struct update<environment_archetype<Base>> {
            template <typename Env>
            static auto call(Env&& env)
            REACT_AUTO_RETURN(
                std::forward<Env>(env)
            )
        };

        template <typename Base>
        struct retrieve<environment_archetype<Base>> {
            template <typename Name, typename Env>
            static boost::null_archetype<>& call(Env&&) {
                return *(boost::null_archetype<>*)0;
            }
        };
    } // end namespace extensions


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
        struct name;
    };
} // end namespace react

#endif // !REACT_ARCHETYPES_HPP
