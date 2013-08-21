/*!
 * @file
 * This file contains unit tests for `react::computation::depends_on`.
 */

#include <react/computation/depends_on.hpp>
#include <react/archetypes.hpp>
#include <react/concept/computation.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/vector.hpp>


using namespace react;

template <int>
struct dependency : feature_archetype<> { };

template <typename ...Dependencies>
struct test {
    using with_variadic = Computation<
        computation::depends_on<Dependencies...>,
        typename fake_result_of<
            Dependencies
        >::template with<
            boost::copy_constructible_archetype<>
        >...
    >;

    using with_mpl_sequence = Computation<
        computation::depends_on<
            typename boost::mpl::vector<Dependencies..., Dependencies>::type...
        >,
        typename fake_result_of<
            Dependencies
        >::template with<
            boost::copy_constructible_archetype<>
        >...
    >;
};

BOOST_CONCEPT_ASSERT((test<>::with_variadic));
BOOST_CONCEPT_ASSERT((test<>::with_mpl_sequence));

BOOST_CONCEPT_ASSERT((test<dependency<1>>::with_variadic));
BOOST_CONCEPT_ASSERT((test<dependency<1>>::with_mpl_sequence));

BOOST_CONCEPT_ASSERT((test<dependency<1>, dependency<2>>::with_variadic));
BOOST_CONCEPT_ASSERT((test<dependency<1>, dependency<2>>::with_mpl_sequence));


int main() { }
