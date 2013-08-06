/*!
 * @file
 * This file contains unit tests for `react::computation::static_counter`.
 */

#include <react/computation/static_counter.hpp>
#include <react/computation/named.hpp>
#include <react/concepts.hpp>
#include <react/extension/fusion.hpp>
#include <react/intrinsics.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/fusion/include/vector.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computation::static_counter<unsigned, 0>,
    dependency_results<>
>));


struct number_of_updates;

int main() {
    boost::fusion::vector<
        computation::named<
            number_of_updates,
            computation::static_counter<unsigned, 0>
        >
    > env0{};
    auto env1 = update(env0);
    auto env2 = update(env1);
    auto env3 = update(env2);
    auto env4 = update(env3);


    BOOST_ASSERT(retrieve<number_of_updates>(env0) == 0);
    BOOST_ASSERT(retrieve<number_of_updates>(env1) == 1);
    BOOST_ASSERT(retrieve<number_of_updates>(env2) == 2);
    BOOST_ASSERT(retrieve<number_of_updates>(env3) == 3);
    BOOST_ASSERT(retrieve<number_of_updates>(env4) == 4);
}
