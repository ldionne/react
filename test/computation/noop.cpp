/*!
 * @file
 * Contains unit tests for `react::computation::noop`.
 */

#include <react/computation/noop.hpp>

#include <react/concept/assert.hpp>
#include <react/concept/computation.hpp>


REACT_CONCEPT_ASSERT(react::Computation<react::computation::noop>);


int main() { }
