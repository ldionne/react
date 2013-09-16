/*!
 * @file
 * Contains unit tests for `react::computation::thread_local_`.
 */

#include <react/computation/thread_local.hpp>

#include <react/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept_archetype.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::thread_local_<
        computation_archetype<boost::default_constructible_archetype<>>
    >
>);


int main() { }
