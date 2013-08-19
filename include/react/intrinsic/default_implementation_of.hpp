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

    template <typename ComputationName>
    struct nested_default_implementation {
        using type = typename ComputationName::default_implementation;
    };

    template <typename ComputationName>
    struct try_nested
        : boost::lazy_enable_if<
            has_default_implementation<ComputationName>,
            nested_default_implementation<ComputationName>
        >
    { };
} // end namespace default_implementation_of_detail

namespace extension {
    template <typename RawComputationName, typename Enable = void>
    struct default_implementation_of_impl {
        template <typename>
        struct apply
            : default_implementation_of_detail::try_nested<RawComputationName>
        { };
    };
} // end namespace extension

template <typename ComputationName>
struct default_implementation_of
    : extension::default_implementation_of_impl<
        typename detail::strip<ComputationName>::type
    >::template apply<ComputationName>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_DEFAULT_IMPLEMENTATION_OF_HPP
