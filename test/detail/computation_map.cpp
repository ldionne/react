/*!
 * @file
 * Contains unit tests for `react::detail::computation_map`.
 */

#include <react/detail/computation_map.hpp>
#include <react/computation/implementing.hpp>
#include <react/computation/requiring.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/placeholder_for.hpp>

#include <boost/mpl/assoc_equal.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <type_traits>


namespace mpl = boost::mpl;
using namespace react;
using mpl::pair;
using detail::computation_map;
using computation::requiring;
using computation::implementing;

template <typename ...Pairs>
using map = typename mpl::map<Pairs...>::type;

namespace without_substitution {
    namespace with_redundant_dependencies {
        struct c1 : implementing<c1>, requiring<> {
            using default_implementation = c1;
        };

        struct c2 : implementing<c2>, requiring<c1> {
            using default_implementation = c2;
        };

        struct c3 : implementing<c3>, requiring<c1> {
            using default_implementation = c3;
        };

        static_assert(mpl::assoc_equal<
            computation_map<c1>,
            map<pair<c1, c1>>
        >::value, "");

        static_assert(mpl::assoc_equal<
            computation_map<c1, c2>,
            map<
                pair<c1, c1>,
                pair<c2, c2>
            >
        >::value, "");

        static_assert(mpl::assoc_equal<
            computation_map<c1, c2, c3>,
            map<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        >::value, "");

        static_assert(mpl::assoc_equal<
            computation_map<c2>,
            map<
                pair<c1, c1>,
                pair<c2, c2>
            >
        >::value, "");

        static_assert(mpl::assoc_equal<
            computation_map<c3>,
            map<
                pair<c1, c1>,
                pair<c3, c3>
            >
        >::value, "");
    } // end namespace with_redundant_dependencies

    namespace with_linear_dependency_chain {
        struct c1 : implementing<c1>, requiring<> {
            using default_implementation = c1;
        };

        struct c2 : implementing<c2>, requiring<c1> {
            using default_implementation = c2;
        };

        struct c3 : implementing<c3>, requiring<c2> {

        };

        static_assert(mpl::assoc_equal<
            computation_map<c3>,
            map<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        >::value, "");
    } // end namespace with_linear_dependency_chain

    namespace with_cyclic_dependency {
        struct c3;
        struct c1 : implementing<c1>, requiring<c3> {
            using default_implementation = c1;
        };

        struct c2 : implementing<c2>, requiring<c1> {
            using default_implementation = c2;
        };

        struct c3 : implementing<c3>, requiring<c2> {

        };

        static_assert(mpl::assoc_equal<
            computation_map<c3>,
            map<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        >::value, "");
    } // end namespace with_cyclic_dependency
} // end namespace without_substitution


namespace with_acyclic_substitution {
    template <typename Feature, typename ...PossiblyPlaceholders>
    struct comp : implementing<Feature> { };

    struct c1; struct c2; struct c3; struct c4;

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, placeholder_for<c2>>,
            comp<c2>
        >,
        map<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, comp<c2>>>
        >
    >::value, "");

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, placeholder_for<c2>>,
            comp<c2, placeholder_for<c3>>,
            comp<c3>
        >,
        map<
            pair<c3, comp<c3>>,
            pair<c2, comp<c2, comp<c3>>>,
            pair<c1, comp<c1, comp<c2, comp<c3>>>>
        >
    >::value, "");

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, placeholder_for<c2>>,
            comp<c2, placeholder_for<c3>, placeholder_for<c4>>,
            comp<c3>,
            comp<c4>
        >,
        map<
            pair<c4, comp<c4>>,
            pair<c3, comp<c3>>,
            pair<c2, comp<c2, comp<c3>, comp<c4>>>,
            pair<c1, comp<c1, comp<c2, comp<c3>, comp<c4>>>>
        >
    >::value, "");
} // end namespace with_acyclic_substitution

namespace with_additional_stuff_to_evaluate_around_placeholders {
    using namespace with_acyclic_substitution;

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, mpl::identity<placeholder_for<c2>>>,
            comp<c2>
        >,
        map<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, comp<c2>>>
        >
    >::value, "");

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, feature_of<placeholder_for<c2>>>,
            comp<c2>
        >,
        map<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, c2>>
        >
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
    struct nested_impl : implementing<nested> {
        struct some_nested_type;
    };

    static_assert(mpl::assoc_equal<
        computation_map<
            comp<c1, some_nested_type<placeholder_for<nested>>>,
            nested_impl
        >,
        map<
            pair<nested, nested_impl>,
            pair<c1, comp<c1, some_nested_type<nested_impl>::type>>
        >
    >::value, "");
} // end namespace with_additional_stuff_to_evaluate_around_placeholders

namespace fetch_default_implementation {
    using namespace with_acyclic_substitution;

    template <typename Computation>
    struct defaults_to {
        using default_implementation = Computation;
    };

    static_assert(std::is_same<
        mpl::at<
            computation_map<>,
            defaults_to<comp<c1>>
        >::type,
        comp<c1>
    >::value, "");

    static_assert(std::is_same<
        mpl::at<
            computation_map<
                comp<c2>
            >,
            defaults_to<comp<c1>>
        >::type,
        comp<c1>
    >::value, "");

    static_assert(std::is_same<
        mpl::at<
            computation_map<
                implementing<defaults_to<comp<c1>>, comp<c2>>
            >,
            defaults_to<comp<c1>>
        >::type,
        implementing<defaults_to<comp<c1>>, comp<c2>>
    >::value, "");

    static_assert(std::is_same<
        mpl::at<
            computation_map<
                comp<c2>
            >,
            defaults_to<comp<c1, placeholder_for<c2>>>
        >::type,
        comp<c1, comp<c2>>
    >::value, "");

    static_assert(std::is_same<
        mpl::at<
            computation_map<
                comp<c2, placeholder_for<c3>>,
                comp<c3>
            >,
            defaults_to<comp<c1, placeholder_for<c2>>>
        >::type,
        comp<c1, comp<c2, comp<c3>>>
    >::value, "");

    static_assert(std::is_same<
        mpl::at<
            computation_map<>,
            defaults_to<comp<c1, placeholder_for<defaults_to<comp<c2>>>>>
        >::type,
        comp<c1, comp<c2>>
    >::value, "");
} // end namespace fetch_default_implementation


int main() { }
