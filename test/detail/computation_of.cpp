/*!
 * @file
 * This file contains unit tests for `react::detail::computation_of`.
 */

#include <react/detail/computation_of.hpp>
#include <react/computation/implements.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/placeholder_for.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/type_traits/is_same.hpp>


using namespace react;
using boost::mpl::pair;

template <typename ...Pairs>
using map = typename boost::mpl::map<Pairs...>::type;

namespace without_substitution {
    template <int i>
    struct comp : computation::implements<comp<i>> { };

    template <typename Computation, typename ...Others>
    using test = boost::is_same<
        typename detail::computation_of<
            Computation,
            map<pair<Computation, Computation>, pair<Others, Others>...>
        >::type,
        Computation
    >;

    static_assert(test<comp<0>>::value, "");
    static_assert(test<comp<0>, comp<1>>::value, "");
    static_assert(test<comp<0>, comp<1>, comp<2>>::value, "");
    static_assert(test<comp<0>, comp<1>, comp<2>, comp<3>>::value, "");
} // end namespace without_substitution

namespace with_cycle_free_substitution {
    template <typename Feature, typename Map, typename Result>
    using test = boost::is_same<
        typename detail::computation_of<Feature, Map>::type, Result
    >;

    template <typename Feature, typename ...PossiblyPlaceholders>
    struct comp : computation::implements<Feature> { };

    struct c1; struct c2; struct c3; struct c4;

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, placeholder_for<c2>>>,
            pair<c2, comp<c2>>
        >,
        comp<c1, comp<c2>>
    >::value, "");

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, placeholder_for<c2>>>,
            pair<c2, comp<c2, placeholder_for<c3>>>,
            pair<c3, comp<c3>>
        >,
        comp<c1, comp<c2, comp<c3>>>
    >::value, "");

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, placeholder_for<c2>>>,
            pair<c2, comp<c2, placeholder_for<c3>, placeholder_for<c4>>>,
            pair<c3, comp<c3>>,
            pair<c4, comp<c4>>
        >,
        comp<c1, comp<c2, comp<c3>, comp<c4>>>
    >::value, "");
} // end namespace with_cycle_free_substitution

namespace with_default_implementation {
    using namespace with_cycle_free_substitution;

    template <typename Computation>
    struct defaults_to {
        using default_implementation = Computation;
    };

    static_assert(test<
        defaults_to<comp<c1>>,
        map<>,
        comp<c1>
    >::value, "");

    static_assert(test<
        defaults_to<comp<c1>>,
        map<pair<c2, comp<c2>>>,
        comp<c1>
    >::value, "");

    static_assert(test<
        defaults_to<comp<c1>>,
        map<pair<defaults_to<comp<c1>>, comp<c1, struct custom>>>,
        comp<c1, struct custom>
    >::value, "");

    static_assert(test<
        defaults_to<comp<c1, placeholder_for<c2>>>,
        map<pair<c2, comp<c2>>>,
        comp<c1, comp<c2>>
    >::value, "");

    static_assert(test<
        defaults_to<comp<c1, placeholder_for<c2>>>,
        map<
            pair<c2, comp<c2, placeholder_for<c3>>>,
            pair<c3, comp<c3>>
        >,
        comp<c1, comp<c2, comp<c3>>>
    >::value, "");

    static_assert(test<
        defaults_to<comp<c1, placeholder_for<defaults_to<comp<c2>>>>>,
        map<>,
        comp<c1, comp<c2>>
    >::value, "");
} // end namespace with_default_implementation

namespace with_additional_stuff_to_evaluate_around_the_placeholder {
    using namespace with_cycle_free_substitution;

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, boost::mpl::identity<placeholder_for<c2>>>>,
            pair<c2, comp<c2>>
        >,
        comp<c1, comp<c2>>
    >::value, "");

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, feature_of<placeholder_for<c2>>>>,
            pair<c2, comp<c2>>
        >,
        comp<c1, c2>
    >::value, "");

    struct dummy;
    static_assert(test<c1,
        map<
            pair<c1, comp<c1, boost::is_same<placeholder_for<c2>, placeholder_for<c3>>>>,
            pair<c2, comp<dummy>>,
            pair<c3, comp<dummy>>
        >,
        comp<c1, boost::is_same<comp<dummy>, comp<dummy>>::type>
    >::value, "");

    // Try with an expression that is invalid until the placeholder is
    // replaced. This way, we make sure that we don't instantiate
    // `some_nested_type` in any way before the placeholder is replaced
    // by the computation itself.
    template <typename T>
    struct some_nested_type {
        using type = typename T::some_nested_type;
    };

    struct nested;
    struct nested_impl : computation::implements<nested> {
        struct some_nested_type;
    };

    static_assert(test<c1,
        map<
            pair<c1, comp<c1, some_nested_type<placeholder_for<nested>>>>,
            pair<nested, nested_impl>
        >,
        comp<c1, some_nested_type<nested_impl>::type>
    >::value, "");
} // end namespace with_additional_stuff_to_evaluate_around_the_placeholder


int main() { }
