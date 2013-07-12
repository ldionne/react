/*!
 * @file
 * This file contains unit tests for `react::computations::function`.
 */

#include <react/computations/function.hpp>
#include <react/archetypes.hpp>
#include <react/concepts.hpp>

#include <boost/concept/assert.hpp>
#include <boost/concept_archetype.hpp>
#include <boost/mpl/pair.hpp>


namespace mpl = boost::mpl;
using namespace react;

template <typename Return>
struct any_function {
    template <typename ...Args>
    Return operator()(Args&& ...) const;
};

struct implemented_feature : feature_archetype<> { };
struct function_feature : feature_archetype<> { };

template <typename ...ArgFeaturesAndTypes>
using test = IncrementalComputation<
    computations::function<
        implemented_feature,
        function_feature,
        typename mpl::first<ArgFeaturesAndTypes>::type...
    >,
    default_construct,
    semantic_tags<>,
    dependency_results<
        mpl::pair<
            function_feature,
            any_function<boost::null_archetype<>&>
        >,
        ArgFeaturesAndTypes...
    >
>;

struct a0 : feature_archetype<> { };
struct a1 : feature_archetype<> { };

BOOST_CONCEPT_ASSERT((test<>));
BOOST_CONCEPT_ASSERT((test<
    mpl::pair<a0, boost::null_archetype<>>
>));
BOOST_CONCEPT_ASSERT((test<
    mpl::pair<a0, boost::null_archetype<>>,
    mpl::pair<a1, boost::null_archetype<>>
>));


int main() { }
