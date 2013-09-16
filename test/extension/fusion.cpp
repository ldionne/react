/*!
 * @file
 * This file contains unit tests for the intrinsics for Boost.Fusion sequences.
 */

#include <react/extension/fusion.hpp>
#include <react/computation/implementing.hpp>
#include <react/concepts.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/fusion/include/vector.hpp>
#include <type_traits>


using namespace react;

template <int i>
struct Comp : computation::implementing<Comp<i>> {
    static constexpr int retrieve(detail::dont_care, detail::dont_care) {
        return i;
    }

    template <typename Other>
    friend constexpr bool operator==(Comp const&, Other const&) {
        return std::is_same<Comp, Other>::value;
    }
};

BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<>,
    available_features<>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<Comp<0>>,
    available_features<Comp<0>>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<Comp<0>, Comp<1>>,
    available_features<Comp<0>, Comp<1>>
>));


int main() {
    // augment
    boost::fusion::vector<Comp<1>> env1;
    auto env2 = augment(env1, Comp<2>{});
    auto env3_4 = augment(env2, Comp<3>{}, Comp<4>{});


    // retrieve
    BOOST_ASSERT(retrieve<Comp<1>>(env1) == 1);

    BOOST_ASSERT(retrieve<Comp<1>>(env2) == 1);
    BOOST_ASSERT(retrieve<Comp<2>>(env2) == 2);

    BOOST_ASSERT(retrieve<Comp<1>>(env3_4) == 1);
    BOOST_ASSERT(retrieve<Comp<2>>(env3_4) == 2);
    BOOST_ASSERT(retrieve<Comp<3>>(env3_4) == 3);
    BOOST_ASSERT(retrieve<Comp<4>>(env3_4) == 4);


    // execute
    execute(env1);
    execute(env2);
    execute(env3_4);
}
