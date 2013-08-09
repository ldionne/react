/*!
 * @file
 * This file contains unit tests for `react::computation::reference`.
 */

#include <react/computation/reference.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <string>


using namespace react;

template <typename T>
using test_concept = Computation<
    computation::reference<T>, dependencies_results<>
>;

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


int main() { }
