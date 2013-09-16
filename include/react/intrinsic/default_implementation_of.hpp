/*!
 * @file
 * This file defines the `react::default_implementation_of` intrinsic.
 */

#ifndef REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP
#define REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>


namespace react {
namespace default_implementation_of_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(default_implementation)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template <typename Feature>
    struct nested_default_implementation {
        using type = typename Feature::default_implementation;
    };

    template <typename Feature>
    struct try_nested
        : boost::lazy_enable_if<
            has_default_implementation<Feature>,
            nested_default_implementation<Feature>
        >
    { };
} // end namespace default_implementation_of_detail

namespace extension {
    template <typename RawFeature, typename Enable = void>
    struct default_implementation_of_impl {
        template <typename>
        struct apply
            : default_implementation_of_detail::try_nested<RawFeature>
        { };
    };
} // end namespace extension

/*!
 * @ingroup intrinsics
 *
 * The default implementation of the computation represented by `Feature`.
 */
template <typename Feature>
struct default_implementation_of
    : extension::default_implementation_of_impl<
        typename detail::strip<Feature>::type
    >::template apply<Feature>
{ };

template <typename T>
struct has_default_implementation
    : default_implementation_of_detail::has_type<
        default_implementation_of<T>
    >
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP
