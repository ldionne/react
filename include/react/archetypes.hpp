/*!
 * @file
 * This file defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/bind.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/concept_archetype.hpp>
#include <boost/mpl/set.hpp>
#include <utility>


namespace react {
    //! Archetype for the `Environment` concept.
    template <typename Base = boost::null_archetype<>>
    struct environment_archetype : Base { };

    namespace extension {
        template <typename Base>
        struct bind_impl<environment_archetype<Base>> {
            template <typename Name, typename Env, typename Computation>
            static auto call(Env&& env, Computation&&) REACT_AUTO_RETURN(
                std::forward<Env>(env)
            )
        };

        template <typename Base>
        struct execute_impl<environment_archetype<Base>> {
            template <typename Env>
            static boost::null_archetype<>& call(Env&& env) {
                return *(boost::null_archetype<>*)0;
            }
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
        static boost::null_archetype<>& execute(Self&&, Env&&) {
            return *(boost::null_archetype<>*)0;
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
