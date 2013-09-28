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

#include <boost/mpl/and.hpp>
#include <boost/mpl/assoc_equal.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>


namespace mpl = boost::mpl;
using namespace react;
using mpl::pair;

template <typename ...T>
struct set;

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

template <typename ...Requirements>
struct requirements<set<Requirements...>>
    : requirements<Requirements...>
{ };

template <typename ...RequirementSets>
struct requirement_sets {
    template <typename ...Computations>
    struct generates
        : mpl::and_<
            typename requirements<RequirementSets>::template
            generates<Computations...>...,
            mpl::true_,
            mpl::true_
        >
    { };
};

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__::value, #__VA_ARGS__)

namespace no_sub {
namespace repeated_deps {
    struct A_comp : implements<struct A> { };
    struct B_comp : implements<struct B>, requires<struct A> { };
    struct C_comp : implements<struct C>, requires<struct A> { };

    struct A_req : implements<struct A>, defaults_to<A_comp> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };
    struct C_req : implements<struct C>, defaults_to<C_comp> { };

    STATIC_ASSERT(
        requirements<A_req>::
        generates<pair<struct A, A_comp>>
    );

    STATIC_ASSERT(
        requirement_sets<
            set<B_req>,
            set<B_req, A_req>
        >::generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>
        >
    );

    STATIC_ASSERT(
        requirement_sets<
            set<C_req>,
            set<C_req, A_req>
        >::generates<
            pair<struct A, A_comp>,
            pair<struct C, C_comp>
        >
    );

    STATIC_ASSERT(
        requirement_sets<
            set<A_req, B_req, C_req>,
            set<B_req, C_req>
        >::generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>,
            pair<struct C, C_comp>
        >
    );
} // end namespace repeated_deps

namespace linear_deps {
    template <int>
    struct feature;

    template <int i>
    struct comp
        : implements<feature<i>>,
          requires<feature<i - 1>>
    { };

    template <>
    struct comp<0>
        : implements<feature<0>>
    { };

    template <int i>
    struct req
        : implements<feature<i>>,
          defaults_to<comp<i>>
    { };

    STATIC_ASSERT(
        requirements<req<0>>::
        generates<
            pair<feature<0>, comp<0>>
        >
    );

    STATIC_ASSERT(
        requirement_sets<
            set<req<1>>,
            set<req<1>, req<0>>
        >::generates<
            pair<feature<0>, comp<0>>,
            pair<feature<1>, comp<1>>
        >
    );

    STATIC_ASSERT(
        requirement_sets<
            set<req<2>>,

            set<req<2>, req<1>>,
            set<req<2>, req<0>>,

            set<req<2>, req<1>, req<0>>
        >::generates<
            pair<feature<0>, comp<0>>,
            pair<feature<1>, comp<1>>,
            pair<feature<2>, comp<2>>
        >
    );
} // end namespace linear_deps

namespace cyclic_deps {
    struct A_comp : implements<struct A>, requires<struct C> { };
    struct B_comp : implements<struct B>, requires<struct A> { };
    struct C_comp : implements<struct C>, requires<struct B> { };

    struct A_req : implements<struct A>, defaults_to<A_comp> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };
    struct C_req : implements<struct C>, defaults_to<C_comp> { };

    STATIC_ASSERT(
        requirement_sets<
            set<A_req>,
            set<B_req>,
            set<C_req>,

            set<A_req, B_req>,
            set<A_req, C_req>,
            set<B_req, C_req>

            set<A_req, B_req, C_req>
        >::generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>,
            pair<struct C, C_comp>
        >
    );
} // end namespace cyclic_deps

// A requires A
namespace self_loop {
    struct A_comp : implements<struct A>, requires<struct A> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    STATIC_ASSERT(requirements<A_req>::generates<pair<struct A, A_comp>>);
} // end namespace self_loop
} // end namespace no_sub

namespace sub {
namespace acyclic_sub {
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

    // Try with an expression that is invalid until the placeholder is
    // replaced. This way, we make sure that we don't instantiate
    // `nested_xyz` in any way before the placeholder is replaced
    // by the computation itself.
    template <typename T> struct nested_xyz { using type = typename T::xyz; };
    struct nested_comp : implements<struct nested> { struct xyz; };
    struct nested_req : implements<struct nested>, defaults_to<nested_comp> { };

    STATIC_ASSERT(
        requirements<
            comp<c1, nested_xyz<computation_of<struct nested>>>,
            nested_req
        >::generates<
            pair<struct nested, nested_comp>,
            pair<c1, comp<c1, nested_xyz<nested_comp>::type>>
        >
    );
} // end namespace acyclic_sub

namespace edge_cases {
// A requires B. A is a placeholder expression containing computation_of<B>
namespace sc1 {
    template <typename = computation_of<struct B>>
    struct A_comp : implements<struct A>, requires<struct B> { };
    struct B_comp : implements<struct B> { };

    struct A_req : implements<struct A>, defaults_to<A_comp<>> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req>::
        generates<
            pair<struct A, A_comp<B_comp>>,
            pair<struct B, B_comp>
        >
    );
}

// A requires B. B is a placeholder expression containing computation_of<A>
namespace sc2 {
    struct A_comp : implements<struct A>, requires<struct B> { };
    template <typename = computation_of<struct A>>
    struct B_comp : implements<struct B> { };

    struct A_req : implements<struct A>, defaults_to<A_comp> { };
    struct B_req : implements<struct B>, defaults_to<B_comp<>> { };

    STATIC_ASSERT(
        requirements<A_req, B_req>::
        generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp<A_comp>>
        >
    );
}

// A requires B _and_ B requires A. A is a placeholder expression
// containing computation_of<B>
namespace sc3 {
    struct B_comp : implements<struct B>, requires<struct A> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };

    template <typename = computation_of<struct B>>
    struct A_comp : implements<struct A>, requires<struct B> { };
    struct A_req : implements<struct A>, defaults_to<A_comp<>> { };

    STATIC_ASSERT(
        requirements<A_req, B_req>::
        generates<
            pair<struct A, A_comp<B_comp>>,
            pair<struct B, B_comp>
        >
    );
}

// A requires B _and_ B requires A. B is a placeholder expression
// containing computation_of<A>
namespace sc4 {
    struct A_comp : implements<struct A>, requires<struct B> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    template <typename = computation_of<struct A>>
    struct B_comp : implements<struct B>, requires<struct A> { };
    struct B_req : implements<struct B>, defaults_to<B_comp<>> { };

    STATIC_ASSERT(
        requirements<A_req, B_req>::
        generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp<A_comp>>
        >
    );
}

// A is a placeholder expression containing computation_of<C>. B requires C
// with a default implementation.
//
// Substituting this is hard because we need to instantiate B while we're
// instantiating A in order to get C's default implementation from B's
// requirements.
namespace sc5 {
    template <typename = computation_of<struct C>>
    struct A_comp : implements<struct A> { };
    struct A_req : implements<struct A>, defaults_to<A_comp<>> { };

    struct C_comp : implements<struct C> { };
    struct C_req : implements<struct C>, defaults_to<C_comp> { };

    struct B_comp : implements<struct B>, requires<C_req> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req>::
        generates<
            pair<struct A, A_comp<C_comp>>,
            pair<struct B, B_comp>,
            pair<struct C, C_comp>
        >
    );
}

// A contains computation_of<D>, B contains computation_of<A> and
// C requires D with default implementation.
//
// This one is hardcore because we can't instantiate B (because it requires A)
// while we're instantiating A. So we need to instantiate in this order:
// - C
// - D via C's requirements
// - A
// - B
namespace sc6 {
    template <typename = computation_of<struct D>>
    struct A_comp : implements<struct A> { };
    struct A_req : implements<struct A>, defaults_to<A_comp<>> { };

    template <typename = computation_of<struct A>>
    struct B_comp : implements<struct B> { };
    struct B_req : implements<struct B>, defaults_to<B_comp<>> { };

    struct C_comp : implements<struct C>, requires<D_req> { };
    struct C_req : implements<struct C>, defaults_to<C_comp> { };

    struct D_comp : implements<struct D> { };
    struct D_req : implements<struct D>, defaults_to<D_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req, C_req>::
        generates<
            pair<struct A, A_comp<D_comp>>,
            pair<struct B, B_comp<A_comp<D_comp>>>,
            pair<struct C, C_comp>,
            pair<struct D, D_comp>
        >
    );
}
} // end namespace edge_cases
} // end namespace sub

namespace defaults {
namespace single_req {
    struct B_comp : implements<struct B> { };
    struct B_req_nodef : implements<struct B> { };
    struct B_req_def : implements<struct B>, defaults_to<B_comp> { };

    struct A_comp : implements<struct A>, requires<B_req_def> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req_nodef>::
        generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>
        >
    );
} // end namespace single_req

namespace multi_req {
    struct C_comp : implements<struct C> { };
    struct C_req_nodef : implements<struct C> { };
    struct C_req_def : implements<struct C>, defaults_to<C_comp> { };

    struct B_comp : implements<struct B> { };
    struct B_req_nodef : implements<struct B> { };
    struct B_req_def : implements<struct B>, defaults_to<B_comp> { };

    struct A_comp : implements<struct A>, requires<B_req_def, C_req_def> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req_nodef, C_req_nodef>::
        generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>,
            pair<struct C, C_comp>
        >
    );
} // end namespace multi_req

namespace single_fmap {
    struct B_comp : implements<struct B> { };
    struct B_req_nodef : implements<struct B> { };
    struct B_req_def : implements<struct B>, defaults_to<B_comp> { };

    struct A_comp : implements<struct A>, requires<B_req_nodef> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    STATIC_ASSERT(
        requirements<A_req, B_req_def>::
        generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>
        >
    );
} // end namespace single_fmap

namespace identical_dupes {
    struct C_comp : implements<struct C> { };
    struct C_req_nodef : implements<struct C> { };
    struct C_req_def : implements<struct C>, defaults_to<C_comp> { };

    struct B_comp : implements<struct B>, requires<C_req_def> { };
    struct B_req : implements<struct B>, defaults_to<B_comp> { };

    struct A_comp : implements<struct A>, requires<C_req_def> { };
    struct A_req : implements<struct A>, defaults_to<A_comp> { };

    STATIC_ASSERT(
        requirement_sets<
            set<A_req, B_req>,
            set<A_req, B_req, C_req_def>
        >::generates<
            pair<struct A, A_comp>,
            pair<struct B, B_comp>,
            pair<struct C, C_comp>
        >
    );
} // end namespace identical_dupes
} // end namespace defaults


int main() { }
