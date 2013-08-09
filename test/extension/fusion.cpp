/*!
 * @file
 * This file contains unit tests for the intrinsics for Boost.Fusion sequences.
 */

#include <react/extension/fusion.hpp>
#include <react/computation/named.hpp>
#include <react/concepts.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/bind.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/type_traits/is_same.hpp>


using namespace react;

template <int i>
struct Comp : computation::named<Comp<i>> {
    static constexpr int retrieve(detail::dont_care, detail::dont_care) {
        return i;
    }

    template <typename Other>
    friend constexpr bool operator==(Comp const&, Other const&) {
        return boost::is_same<Comp, Other>::value;
    }
};

BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<>,
    available_names<>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<Comp<0>>,
    available_names<Comp<0>>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<Comp<0>, Comp<1>>,
    available_names<Comp<0>, Comp<1>>
>));


int main() {
    // bind
    boost::fusion::vector<Comp<1>> env1;
    auto env2 = bind(env1, Comp<2>{});
    auto env3_4 = bind(bind(env2, Comp<3>{}), Comp<4>{});


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
