/*!
 * @file
 * This file contains unit tests for `react::computation::noop`.
 */

#include <react/computation/noop.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>


BOOST_CONCEPT_ASSERT((react::Computation<react::computation::noop>));


int main() { }
