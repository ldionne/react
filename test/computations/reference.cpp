/*!
 * @file
 * This file contains unit tests for `react::computations::reference`.
 */

#include <react/computations/reference.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>
#include <react/feature_sets/from_argument_pack.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/parameter/keyword.hpp>
#include <string>
#include <type_traits>
#include <utility>


using namespace react;

template <typename T>
struct test_concept {
    struct init_ref : feature_archetype<> { };
    using ReferenceComputation = computations::reference<
        feature_archetype<>, T, init_ref
    >;

    BOOST_CONCEPT_USAGE(test_concept) {
        auto keyword = boost::parameter::keyword<init_ref>::get();
        BOOST_CONCEPT_ASSERT((IncrementalComputation<
            ReferenceComputation,
            // We must set the reference when we construct the computation.
            decltype(feature_sets::make_from_argument_pack(
                keyword = std::declval<T&>()
            )),
            semantic_tags<>,
            dependency_results<>
        >));

        // The result of a reference computation should be convertible to `T&`.
        static_assert(std::is_convertible<
            typename ReferenceComputation::result_type, T&
        >::value, "");
    }
};


// Check with primitive types, user-defined types and function/array types.
BOOST_CONCEPT_ASSERT((test_concept<int>));
BOOST_CONCEPT_ASSERT((test_concept<std::string>));
BOOST_CONCEPT_ASSERT((test_concept<int*>));
BOOST_CONCEPT_ASSERT((test_concept<void()>));
BOOST_CONCEPT_ASSERT((test_concept<int[2]>));

BOOST_CONCEPT_ASSERT((test_concept<int const>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const>));
BOOST_CONCEPT_ASSERT((test_concept<int* const>));


// Check with [const] references. It should handle references to references
// gracefully.
BOOST_CONCEPT_ASSERT((test_concept<int&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string&>));
BOOST_CONCEPT_ASSERT((test_concept<int* &>));
BOOST_CONCEPT_ASSERT((test_concept<void(&)()>));
BOOST_CONCEPT_ASSERT((test_concept<int(&)[2]>));

BOOST_CONCEPT_ASSERT((test_concept<int const&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const&>));
BOOST_CONCEPT_ASSERT((test_concept<int* const&>));


// Small runtime test to make sure the reference is taken correctly.
struct init_my_ref : feature_archetype<> { };
using MyReferenceComputation = computations::reference<
    feature_archetype<>, std::string, init_my_ref
>;

struct dont_care { };

int main() {
    std::string original = "abcd";
    MyReferenceComputation ref{feature_sets::make_from_argument_pack(
        boost::parameter::keyword<init_my_ref>::get() = original
    )};

    std::string& retrieved = ref.result(dont_care{});
    BOOST_ASSERT(&retrieved == &original);

    retrieved = "modified";
    BOOST_ASSERT(original == "modified");
}
