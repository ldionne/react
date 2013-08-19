/*!
 * @file
 * This file defines `react::detail::computation_of`.
 */

#ifndef REACT_DETAIL_COMPUTATION_OF_HPP
#define REACT_DETAIL_COMPUTATION_OF_HPP

#include <react/intrinsic/default_implementation_of.hpp>

#include <boost/mpl/any_of.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/at_default.hpp>
#include <boost/mpl/deferred_apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>


namespace react { namespace detail {
namespace custom_substitution_until_mpl11 {
    namespace mpl = boost::mpl;

    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(is_placeholder, is_placeholder, false)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template <typename F>
    struct is_placeholder_expression
        : is_placeholder<F>
    { };

    template <template <typename ...> class F, typename ...T>
    struct is_placeholder_expression<F<T...>>
        : mpl::or_<
            is_placeholder<F<T...>>,
            mpl::any_of<
                typename mpl::vector<T...>::type,
                is_placeholder_expression<mpl::_1>
            >
        >
    { };

    template <typename F, typename ...Args>
    struct apply_wrap
        : mpl::deferred_apply_wrap<F, Args...>::type
    { };

    template <template <typename ...> class F>
    struct quote {
        template <typename ...Args>
        struct apply
            : mpl::eval_if<has_type<F<Args...>>,
                F<Args...>,
                mpl::identity<F<Args...>>
            >
        { };
    };

    template <typename SubstitutionMap>
    struct apply_helper {
        template <typename T>
        struct apply_recursive {
            using type = T;
        };

        template <typename F>
        struct apply
            : mpl::eval_if<is_placeholder<F>,
                apply_wrap<F, typename mpl::vector<>::type, SubstitutionMap>,
                apply_recursive<F>
            >
        { };

        template <template <typename ...> class F, typename ...T>
        struct apply_recursive<F<T...>>
            : mpl::eval_if<is_placeholder_expression<F<T...>>,
                apply_wrap<quote<F>, typename apply<T>::type...>,
                mpl::identity<F<T...>>
            >
        { };
    };

    template <typename F, typename SubstitutionMap>
    struct apply
        : apply_helper<SubstitutionMap>::template apply<F>
    { };

    template <typename T>
    using splat = T;
} // end namespace custom_substitution_until_mpl11

namespace computation_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template <typename ...>
    struct always_false {
        static constexpr bool value = false;
    };

    template <typename Name>
    struct helpful_failure {
        BOOST_MPL_ASSERT_MSG(
            always_false<Name>::value,
        MISSING_A_DEFAULT_IMPLEMENTATION_FOR_THE_FOLLOWING_COMPUTATION_NAME,
            (Name)
        );

        static_assert(always_false<Name>::value,
        "Missing a default implementation for some computation name. "
        "The implementation for that computation name must be provided "
        "explicitly or the computation name must define a default "
        "implementation using the `react::default_implementation_of` "
        "intrinsic.");

        struct type;
    };

#if defined(MPL11_IS_READY_WITH_INCREDIBLE_LAMBDA_SUBSTITUTION)
    namespace mpl = boost::mpl;
#else
    namespace mpl = custom_substitution_until_mpl11;
#endif

    template <typename UnboundCustomizedComputations>
    struct given {
        template <typename Name>
        struct unbound_implementation_of
            : boost::mpl::lazy_at_default<
                UnboundCustomizedComputations, Name,
                boost::mpl::eval_if<has_type<default_implementation_of<Name>>,
                    default_implementation_of<Name>,
                    helpful_failure<Name>
                >
            >
        { };

        template <typename Name>
        struct bound_implementation_of
            : mpl::apply<
                typename unbound_implementation_of<Name>::type,
                mpl::splat<UnboundCustomizedComputations>
            >
        { };
    };
} // end namespace computation_of_detail

/*!
 * Retrieve the fully-bound `Computation` associated to a type modeling
 * the `ComputationName` concept.
 *
 * @tparam Name
 *         A model of the `ComputationName` concept representing the name of
 *         the computation that should be generated.
 *
 * @tparam UnboundCustomizedComputations
 *         A Boost.MPL `AssociativeSequence` mapping `ComputationName`s to
 *         models of the `Named` concept. The values in the map may be
 *         `LambdaExpression`s containing special placeholders, in which
 *         case they are substituted accordingly. Whenever the computation
 *         associated to a name that is not in `UnboundCustomizedComputations`
 *         is required, the missing computation is generated using the
 *         `default_implementation_of` intrinsic.
 */
template <typename Name, typename UnboundCustomizedComputations>
struct computation_of
    : computation_of_detail::given<
        UnboundCustomizedComputations
      >::template bound_implementation_of<Name>
{ };
}} // end namespace react::detail

#endif // !REACT_DETAIL_COMPUTATION_OF_HPP
