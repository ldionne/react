/*!
 * @file
 * Contains unit tests for `react::feature_map`.
 */

#include <react/sandbox/feature_map.hpp>

#include <react/sandbox/computation_of.hpp>
#include <react/sandbox/defaults_to.hpp>
#include <react/sandbox/feature_of.hpp>
#include <react/sandbox/implements.hpp>
#include <react/sandbox/requires.hpp>

#include <boost/mpl/assoc_equal.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>


namespace mpl = boost::mpl;
using namespace react;
using mpl::pair;

template <typename ...Requirements>
struct requirements {
    template <typename ...Computations>
    struct generates
        : mpl::assoc_equal<
            feature_map<Requirements...>,
            typename mpl::map<Computations...>::type
        >
    { };
};

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__::value, #__VA_ARGS__)


namespace without_substitution {
    namespace with_redundant_dependencies {
        struct c1 : implements<c1>, defaults_to<c1>               { };
        struct c2 : implements<c2>, defaults_to<c2>, requires<c1> { };
        struct c3 : implements<c3>, defaults_to<c3>, requires<c1> { };

        STATIC_ASSERT(
            requirements<c1>::
            generates<pair<c1, c1>>
        );

        STATIC_ASSERT(
            requirements<c1, c2>::
            generates<
                pair<c1, c1>,
                pair<c2, c2>
            >
        );

        STATIC_ASSERT(
            requirements<c1, c2, c3>::
            generates<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        );

        STATIC_ASSERT(
            requirements<c2>::
            generates<
                pair<c1, c1>,
                pair<c2, c2>
            >
        );

        STATIC_ASSERT(
            requirements<c3>::
            generates<
                pair<c1, c1>,
                pair<c3, c3>
            >
        );
    } // end namespace with_redundant_dependencies

    namespace with_linear_dependency_chain {
        struct c1 : implements<c1>, defaults_to<c1>               { };
        struct c2 : implements<c2>, defaults_to<c2>, requires<c1> { };
        struct c3 : implements<c3>, defaults_to<c3>, requires<c2> { };

        STATIC_ASSERT(
            requirements<c3>::
            generates<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        );
    } // end namespace with_linear_dependency_chain

    namespace with_cyclic_dependency {
        struct c1 : implements<c1>, defaults_to<c1>, requires<struct c3> { };
        struct c2 : implements<c2>, defaults_to<c2>, requires<c1> { };
        struct c3 : implements<c3>, defaults_to<c3>, requires<c2> { };

        STATIC_ASSERT(
            requirements<c3>::
            generates<
                pair<c1, c1>,
                pair<c2, c2>,
                pair<c3, c3>
            >
        );
    } // end namespace with_cyclic_dependency
} // end namespace without_substitution

namespace substitution_edge_cases {
    // A requires B. A is a placeholder expression containing computation_of<B>
    namespace scenario1 {
        struct B : implements<B>, defaults_to<B> { };
        template <typename = computation_of<B>>
        struct A_default : implements<struct A> { };
        struct A : implements<A>, requires<B>, defaults_to<A_default<>> { };

        STATIC_ASSERT(
            requirements<A, B>::
            generates<
                pair<A, A_default<B>>,
                pair<B, B>
            >
        );
    }

    // A requires B. B is a placeholder expression containing computation_of<A>
    namespace scenario2 {
        struct A : implements<A>, requires<struct B>, defaults_to<A> { };
        template <typename = computation_of<A>>
        struct B_default : implements<struct B> { };
        struct B : implements<B>, defaults_to<B_default<>> { };

        STATIC_ASSERT(
            requirements<A, B>::
            generates<
                pair<A, A>,
                pair<B, B_default<A>>
            >
        );
    }

    // A requires B _and_ B requires A. A is a placeholder expression
    // containing computation_of<B>
    namespace scenario3 {
        struct B : implements<B>, defaults_to<B>, requires<struct A> { };
        template <typename = computation_of<B>>
        struct A_default : implements<struct A> { };
        struct A : implements<A>, requires<B>, defaults_to<A_default<>> { };

        STATIC_ASSERT(
            requirements<A, B>::
            generates<
                pair<A, A_default<B>>,
                pair<B, B>
            >
        );
    }

    // A requires B _and_ B requires A. B is a placeholder expression
    // containing computation_of<A>
    namespace scenario4 {
        struct A : implements<A>, requires<struct B>, defaults_to<A> { };
        template <typename = computation_of<A>>
        struct B_default : implements<struct B> { };
        struct B : implements<B>, requires<A>, defaults_to<B_default<>> { };

        STATIC_ASSERT(
            requirements<A, B>::
            generates<
                pair<A, A>,
                pair<B, B_default<A>>
            >
        );
    }

    // A requires A
    namespace scenario5 {
        struct A : requires<A>, implements<A>, defaults_to<A> { };
        STATIC_ASSERT(requirements<A>::generates<pair<A, A>>);
    }
} // end namespace substitution_edge_cases

namespace with_acyclic_substitution {
    template <typename Feature, typename ...PossiblyPlaceholders>
    struct comp
        : implements<Feature>,
          defaults_to<comp<Feature, PossiblyPlaceholders...>>
    { };
    struct c1; struct c2; struct c3; struct c4;

    STATIC_ASSERT(
        requirements<
            comp<c1, computation_of<c2>>,
            comp<c2>
        >::generates<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, comp<c2>>>
        >
    );

    STATIC_ASSERT(
        requirements<
            comp<c1, computation_of<c2>>,
            comp<c2, computation_of<c3>>,
            comp<c3>
        >::generates<
            pair<c3, comp<c3>>,
            pair<c2, comp<c2, comp<c3>>>,
            pair<c1, comp<c1, comp<c2, comp<c3>>>>
        >
    );

    STATIC_ASSERT(
        requirements<
            comp<c1, computation_of<c2>>,
            comp<c2, computation_of<c3>, computation_of<c4>>,
            comp<c3>,
            comp<c4>
        >::generates<
            pair<c4, comp<c4>>,
            pair<c3, comp<c3>>,
            pair<c2, comp<c2, comp<c3>, comp<c4>>>,
            pair<c1, comp<c1, comp<c2, comp<c3>, comp<c4>>>>
        >
    );
} // end namespace with_acyclic_substitution

namespace with_additional_stuff_to_evaluate_around_placeholders {
    using namespace with_acyclic_substitution;

    STATIC_ASSERT(
        requirements<
            comp<c1, mpl::identity<computation_of<c2>>>,
            comp<c2>
        >::generates<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, comp<c2>>>
        >
    );

    STATIC_ASSERT(
        requirements<
            comp<c1, feature_of<computation_of<c2>>>,
            comp<c2>
        >::generates<
            pair<c2, comp<c2>>,
            pair<c1, comp<c1, c2>>
        >
    );

    // Try with an expression that is invalid until the placeholder is
    // replaced. This way, we make sure that we don't instantiate
    // `some_nested_type` in any way before the placeholder is replaced
    // by the computation itself.
    template <typename T> struct nested_xyz { using type = typename T::xyz; };
    struct nested;
    struct nested_impl : implements<nested>, defaults_to<nested_impl> {
        struct xyz;
    };

    STATIC_ASSERT(
        requirements<
            comp<c1, nested_xyz<computation_of<nested>>>,
            nested_impl
        >::generates<
            pair<nested, nested_impl>,
            pair<c1, comp<c1, nested_xyz<nested_impl>::type>>
        >
    );
} // end namespace with_additional_stuff_to_evaluate_around_placeholders

namespace a_single_default_implementation_is_required {
    struct default_c2 : implements<struct c2>, defaults_to<default_c2> { };
    struct default_c3 : implements<struct c3>, defaults_to<default_c3> { };
    struct c2 : implements<c2> { };
    struct c3 : implements<c3> { };

    namespace one_requirement_specifies_one_implementation {
        struct c1 : implements<c1>, defaults_to<c1>, requires<default_c2> { };
        STATIC_ASSERT(
            requirements<c1, c2>::
            generates<
                pair<c1, c1>,
                pair<c2, default_c2>
            >
        );
    }

    namespace one_requirement_specifies_two_implementations {
        struct c1
            : implements<c1>, defaults_to<c1>,
              requires<default_c2, default_c3>
        { };
        STATIC_ASSERT(
            requirements<c1, c2, c3>::
            generates<
                pair<c1, c1>,
                pair<c2, default_c2>,
                pair<c3, default_c3>
            >
        );
    }

    namespace default_implementation_provided_in_feature_map {
        struct c1 : implements<c1>, defaults_to<c1>, requires<c2> { };
        STATIC_ASSERT(
            requirements<c1, default_c2>::
            generates<
                pair<c1, c1>,
                pair<c2, default_c2>
            >
        );
    }
} // end namespace a_single_default_implementation_is_required

namespace multiple_identical_default_implementations_are_accepted {
    struct default_c3 : implements<struct c3>, defaults_to<default_c3> { };
    struct c1 : implements<c1>, defaults_to<c1>, requires<default_c3> { };
    struct c2 : implements<c2>, defaults_to<c2>, requires<default_c3> { };
    struct c3 : implements<c3> { };

    STATIC_ASSERT(
        requirements<c1, c2>::
        generates<
            pair<c1, c1>,
            pair<c2, c2>,
            pair<c3, default_c3>
        >
    );

    STATIC_ASSERT(
        requirements<c1, c2, default_c3>::
        generates<
            pair<c1, c1>,
            pair<c2, c2>,
            pair<c3, default_c3>
        >
    );
} // end namespace multiple_identical_default_implementations_are_accepted


int main() { }
