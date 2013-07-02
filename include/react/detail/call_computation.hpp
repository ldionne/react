/*!
 * @file
 * This file defines `react::detail::call_computation`.
 */

#ifndef REACT_DETAIL_CALL_COMPUTATION_HPP
#define REACT_DETAIL_CALL_COMPUTATION_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/convertible_if.hpp>
#include <react/detail/deferred_apply.hpp>
#include <react/detail/dont_care.hpp>
#include <react/detail/matches.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <utility>


namespace react { namespace detail {
namespace call_computation_detail {
    namespace mpl = boost::mpl;

    BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_nested_type, type, false)
    template <typename T, typename B = bool>
    struct has_nested_type_value {
        template <typename U, B = U::type::value>
        static mpl::true_ test(void*);

        template <typename>
        static mpl::false_ test(...);

        typedef decltype(test<T>(static_cast<void*>(0))) type;
        static bool const value = type::value;
    };

    template <typename T, typename ...Args>
    struct is_metafunction_class
        : has_nested_type<deferred_apply<T, Args...> >
    { };

    template <typename Metafunction>
    struct is_boolean_metafunction
        : mpl::or_<
            // look for Metafunction::type::value
            has_nested_type_value<Metafunction>,
            // look for Metafunction::value
            has_nested_type_value<mpl::identity<Metafunction> >
        >
    { };

    template <typename MakeMetafunction, typename Otherwise>
    struct apply_or_matches_helper
        : mpl::eval_if<
            is_boolean_metafunction<typename MakeMetafunction::type>,
            typename MakeMetafunction::type,
            Otherwise
        >
    { };

    template <typename Tag, typename Pattern>
    struct apply_or_matches
        : mpl::eval_if<
            is_metafunction_class<Pattern, Tag>,
            apply_or_matches_helper<
                deferred_apply<Pattern, Tag>, matches<Tag, Pattern>
            >,
            matches<Tag, Pattern>
        >
    { };

    template <typename SemanticTag>
    using conv = convertible_if<apply_or_matches<SemanticTag, mpl::_1>>;
} // end namespace call_computation_detail

/*!
 * Execute a computation.
 *
 * The first parameter of the computation's `operator()` must be a Boost.MPL
 * boolean `LambdaExpression` returning whether that overload should be
 * picked for processing that semantic tag. The semantic tag is passed
 * to the `LambdaExpression` when it is applied. Alternatively, if the
 * first parameter is not a boolean metafunction, the type of the first
 * parameter is pattern-matched against the semantic tag in order to
 * determine whether this overload should be picked. See `matches` for
 * details on pattern matching.
 *
 * The second parameter of the computation is the feature set providing
 * access to features.
 */
template <typename SemanticTag, typename Computation, typename FeatureSet>
auto call_computation(Computation&& c, FeatureSet&& fset)
REACT_AUTO_RETURN(
    std::forward<Computation>(c)(call_computation_detail::conv<SemanticTag>{},
                                 std::forward<FeatureSet>(fset))
)

template <typename SemanticTag>
void call_computation(dont_care, dont_care) { }
}} // end namespace react::detail

#endif // !REACT_DETAIL_CALL_COMPUTATION_HPP
