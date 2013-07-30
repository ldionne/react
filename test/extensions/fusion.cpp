/*!
 * @file
 * This file contains unit tests for the intrinsics for Boost.Fusion sequences.
 */

#include <react/extensions/fusion.hpp>
#include <react/concepts.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>

#include <boost/assert.hpp>
#include <boost/concept/assert.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/mpl/set_equal.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>


using namespace react;

template <int i>
struct computation {
    constexpr int retrieve(detail::dont_care) const { return i; }

    template <typename Other>
    constexpr friend bool operator==(computation const&, Other const&) {
        return boost::is_same<computation, Other>::value;
    }
};

BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<>,
    implemented_computations<>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<computation<0>>,
    implemented_computations<computation<0>>
>));
BOOST_CONCEPT_ASSERT((Environment<
    boost::fusion::vector<computation<0>, computation<1>>,
    implemented_computations<computation<0>, computation<1>>
>));


int main() {
    // augment
    boost::fusion::vector<computation<1>> env1;
    auto env2 = augment(env1, computation<2>{});
    auto env3_4 = augment(env2, computation<3>{}, computation<4>{});


    // retrieve
    BOOST_ASSERT(retrieve<computation<1>>(env1) == 1);

    BOOST_ASSERT(retrieve<computation<1>>(env2) == 1);
    BOOST_ASSERT(retrieve<computation<2>>(env2) == 2);

    BOOST_ASSERT(retrieve<computation<1>>(env3_4) == 1);
    BOOST_ASSERT(retrieve<computation<2>>(env3_4) == 2);
    BOOST_ASSERT(retrieve<computation<3>>(env3_4) == 3);
    BOOST_ASSERT(retrieve<computation<4>>(env3_4) == 4);


    // update
#define TYPE_OF(x) boost::remove_reference<decltype(x)>::type
    update(env1);
    static_assert(boost::mpl::set_equal<
        TYPE_OF(env1), TYPE_OF(update(env1))
    >::value, "");

    update(env2);
    static_assert(boost::mpl::set_equal<
        TYPE_OF(env2), TYPE_OF(update(env2))
    >::value, "");

    update(env3_4);
    static_assert(boost::mpl::set_equal<
        TYPE_OF(env3_4), TYPE_OF(update(env3_4))
    >::value, "");
}
