/*!
 * @file
 * Defines the archetypes for all the concepts of the library.
 */

#ifndef REACT_ARCHETYPES_HPP
#define REACT_ARCHETYPES_HPP

namespace react {
    namespace archetypes_detail { struct dummy_constructor; }

    template <typename = void>
    struct null_archetype {
        null_archetype(archetypes_detail::dummy_constructor);
        null_archetype() = delete;
        ~null_archetype() = delete;
    };

    template <typename Base = null_archetype<>>
    struct destructible_archetype : Base {
        ~destructible_archetype();
    };

    template <typename Base = null_archetype<>>
    struct default_constructible_archetype : Base {
        default_constructible_archetype();
    };

    template <typename Base = null_archetype<>>
    struct copy_constructible_archetype : Base {
        copy_constructible_archetype();
        copy_constructible_archetype(copy_constructible_archetype const&);
    };

    //! Archetype for the `Environment` concept.
    template <typename Base = null_archetype<>>
    struct environment_archetype : Base {
        template <typename Self>
        static copy_constructible_archetype<> execute(Self&&);

        template <typename Feature, typename Self>
        static copy_constructible_archetype<> retrieve(Self&&);

        template <typename Self, typename ...Computations>
        static environment_archetype<copy_constructible_archetype<>>
        augment(Self&&, Computations&& ...);
    };

    //! Archetype for the `Computation` concept.
    template <typename Base = null_archetype<>>
    struct computation_archetype : Base {
        template <typename Self, typename Env>
        static copy_constructible_archetype<> execute(Self&&, Env&&);

        template <typename Self, typename Env>
        static copy_constructible_archetype<> retrieve(Self&&, Env&&);
    };

    //! Archetype for the `Feature` concept.
    template <typename Base = null_archetype<>>
    struct feature_archetype : Base {
        // Optional requirement:
        // using default_implementation = computation_archetype<>;
    };

    //! Archetype for the `Implementation` concept.
    template <typename Base = null_archetype<>>
    struct implementation_archetype : Base {

    };
} // end namespace react

#endif // !REACT_ARCHETYPES_HPP
