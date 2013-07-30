/*!
 * @file
 * This file contains unit tests for `react::computations::static_counter`.
 */

#include <react/computations/static_counter.hpp>
#include <react/computations/named.hpp>
#include <react/concepts.hpp>
#include <react/extensions/fusion.hpp>
#include <react/intrinsics.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/fusion/include/vector.hpp>


using namespace react;

BOOST_CONCEPT_ASSERT((Computation<
    computations::static_counter<unsigned, 0>,
    dependency_results<>
>));


struct number_of_updates;

int main() {
    boost::fusion::vector<
        computations::named<
            number_of_updates,
            computations::static_counter<unsigned, 0>
        >
    > env0{};
    auto env1 = execute(env0);
    auto env2 = execute(env1);
    auto env3 = execute(env2);
    auto env4 = execute(env3);


    BOOST_ASSERT(retrieve<number_of_updates>(env0) == 0);
    BOOST_ASSERT(retrieve<number_of_updates>(env1) == 1);
    BOOST_ASSERT(retrieve<number_of_updates>(env2) == 2);
    BOOST_ASSERT(retrieve<number_of_updates>(env3) == 3);
    BOOST_ASSERT(retrieve<number_of_updates>(env4) == 4);
}
