/*!
 * @file
 * This file contains unit tests for `react::factories::typed`.
 */

#include <react/factories/typed.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept/usage.hpp>
#include <boost/concept_archetype.hpp>
#include <string>
#include <type_traits>


using namespace react;

template <typename T>
struct my_computation : incremental_computation_archetype<> {
    using incremental_computation_archetype<>::incremental_computation_archetype;
    T result(detail::dont_care);
};

template <typename ComputationResult>
struct test_concept {
    using TypedComputation = typename factories::typed<
        ComputationResult, my_computation<ComputationResult>
    >::type;

    template <typename ConstructPolicy>
    using check_incremental = IncrementalComputation<
        TypedComputation,
        ConstructPolicy,
        semantic_tags<boost::null_archetype<>>,
        dependency_results<>
    >;

    BOOST_CONCEPT_USAGE(test_concept) {
        // The dummy computation we're wrapping can be default constructed
        // or constructed with any `FeatureSet`.
        BOOST_CONCEPT_ASSERT((check_incremental<default_construct>));
        BOOST_CONCEPT_ASSERT((check_incremental<feature_set_archetype<>>));

        // If we want the
        // `TypedComputation::result_type v = typed_computation.result(env)`
        // expression to be valid, we require the return type of the
        // underlying computation of `typed_computation` to be convertible to
        // `TypedComputation::result_type`.
        static_assert(std::is_convertible<
            ComputationResult, typename TypedComputation::result_type
        >::value, "");
    }
};


// Check with primitive types and user-defined types.
BOOST_CONCEPT_ASSERT((test_concept<int>));
BOOST_CONCEPT_ASSERT((test_concept<std::string>));
BOOST_CONCEPT_ASSERT((test_concept<int*>));

BOOST_CONCEPT_ASSERT((test_concept<int const>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const>));
BOOST_CONCEPT_ASSERT((test_concept<int* const>));


// Check with [const] references.
BOOST_CONCEPT_ASSERT((test_concept<int&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string&>));
BOOST_CONCEPT_ASSERT((test_concept<int* &>));
BOOST_CONCEPT_ASSERT((test_concept<int(&)[2]>));

BOOST_CONCEPT_ASSERT((test_concept<int const&>));
BOOST_CONCEPT_ASSERT((test_concept<std::string const&>));
BOOST_CONCEPT_ASSERT((test_concept<int* const&>));


int main() { }
