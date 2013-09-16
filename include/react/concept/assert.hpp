/*!
 * @file
 * Defines `REACT_CONCEPT_ASSERT`.
 */

#ifndef REACT_CONCEPT_ASSERT_HPP
#define REACT_CONCEPT_ASSERT_HPP

namespace react {
    template <typename Concept>
    struct CONCEPT_CHECKING_FAILED_FOR {
        static void xxxxxxxxxxxxxxxxxxxx() {
            ((Concept*)nullptr)->~Concept();
        }
    };
} // end namespace react

#define REACT_CONCEPT_ASSERT(... /* concept checking class */)              \
    static_assert(                                                          \
        &::react::CONCEPT_CHECKING_FAILED_FOR<                              \
            __VA_ARGS__                                                     \
        >::xxxxxxxxxxxxxxxxxxxx,                                            \
        "Concept check failed: " #__VA_ARGS__                               \
    )                                                                       \
/**/

#endif // !REACT_CONCEPT_ASSERT_HPP
