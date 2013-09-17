/*!
 * @file
 * Contains unit tests for `react::computation::executable_if`.
 */

#include <react/computation/executable_if.hpp>

#include <react/concept/archetypes.hpp>
#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>

#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>


using namespace react;

REACT_CONCEPT_ASSERT(Computation<
    computation::executable_if<
        boost::mpl::always<boost::mpl::true_>,
        computation_archetype<>
    >
>);


int main() { }
