/*!
 * @file
 * This file defines the traits used by the library.
 */

#ifndef REACT_TRAITS_HPP
#define REACT_TRAITS_HPP

#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/set.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>


namespace react {
namespace traits_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(dependencies)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(name)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(react_tag)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(default_computation)

    template <typename Computation>
    struct nested_dependencies {
        using type = typename Computation::dependencies;
    };

    template <typename Computation>
    struct nested_name {
        using type = typename Computation::name;
    };

    template <typename ComputationName>
    struct nested_default_computation {
        using type = typename ComputationName::default_computation;
    };

    template <typename Computation>
    struct anonymous_name {
        struct type;
    };

    template <typename T>
    struct nested_react_tag {
        using type = typename T::react_tag;
    };

    template <typename T>
    struct tag_of_impl
        : boost::mpl::eval_if<
            has_react_tag<T>,
            nested_react_tag<T>,
            boost::mpl::identity<T>
        >
    { };

    template <typename ComputationName, bool always_false = false>
    struct trigger_failure {
        BOOST_MPL_ASSERT_MSG(always_false,
            missing_a_default_computation_for_some_computation_name,
            (ComputationName)
        );

        static_assert(always_false,
            "missing a default computation for some computation name");

        struct type;
    };
} // end namespace traits_detail

template <typename T>
struct tag_of
    : traits_detail::tag_of_impl<
        typename boost::remove_cv<
            typename boost::remove_reference<T>::type
        >::type
    >
{ };

template <typename Computation>
struct dependencies_of
    : boost::mpl::eval_if<
        traits_detail::has_dependencies<Computation>,
        traits_detail::nested_dependencies<Computation>,
        boost::mpl::set<>
    >
{ };

template <typename Computation>
struct name_of
    : boost::mpl::eval_if<
        traits_detail::has_name<Computation>,
        traits_detail::nested_name<Computation>,
        traits_detail::anonymous_name<Computation>
    >
{ };

template <typename ComputationName>
struct default_computation
    : boost::mpl::eval_if<
        traits_detail::has_default_computation<ComputationName>,
        traits_detail::nested_default_computation<ComputationName>,
        traits_detail::trigger_failure<ComputationName>
    >
{ };
} // end namespace react

#endif // !REACT_TRAITS_HPP
