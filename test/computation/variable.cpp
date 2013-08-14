/*!
 * @file
 * This file contains unit tests for `react::computation::variable`.
 */

#include <react/computation/variable.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>


using namespace react;

template <typename T>
struct test_concept
    : Computation<computation::variable<T>>,
      Computation<computation::variable<T*>>,
      Computation<computation::variable<T* const>>,
      Computation<computation::variable<T&>>
{ };

using T = boost::copy_constructible_archetype<>;
BOOST_CONCEPT_ASSERT((test_concept<T>));
BOOST_CONCEPT_ASSERT((test_concept<T const>));
BOOST_CONCEPT_ASSERT((test_concept<T()>));
BOOST_CONCEPT_ASSERT((test_concept<T(T)>));
BOOST_CONCEPT_ASSERT((test_concept<T[]>));
BOOST_CONCEPT_ASSERT((test_concept<T[2]>));
BOOST_CONCEPT_ASSERT((test_concept<T[2][4]>));
BOOST_CONCEPT_ASSERT((test_concept<T[][2][4]>));


int main() { }
