/*!
 * @file
 * Defines `REACT_CONCEPT_ASSERT`.
 */

#ifndef REACT_CONCEPT_ASSERT_HPP
#define REACT_CONCEPT_ASSERT_HPP

namespace react {
    namespace concept_assert_detail {
        template <void (*)()>
        struct always_true {
            static constexpr bool value = true;
        };
    }

    template <typename Concept>
    class CONCEPT_CHECKING_FAILED_FOR {
        static void instantiate_destructor() {
            ((Concept*)nullptr)->~Concept();
        }

    public:
        static constexpr bool value = concept_assert_detail::always_true<
            &CONCEPT_CHECKING_FAILED_FOR::instantiate_destructor
        >::value;
    };
} // end namespace react

#define REACT_CONCEPT_ASSERT(... /* concept checking class */)              \
    static_assert(                                                          \
        ::react::CONCEPT_CHECKING_FAILED_FOR<__VA_ARGS__>::value,           \
        "Concept check failed: " #__VA_ARGS__                               \
    )                                                                       \
/**/

#endif // !REACT_CONCEPT_ASSERT_HPP
