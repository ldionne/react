/*!
 * @file
 * This file defines the `react::has_default_implementation` intrinsic.
 */

#ifndef REACT_INTRINSIC_HAS_DEFAULT_IMPLEMENTATION_HPP
#define REACT_INTRINSIC_HAS_DEFAULT_IMPLEMENTATION_HPP

#include <react/detail/strip.hpp>
#include <react/intrinsic/default_implementation_of.hpp>

#include <boost/mpl/has_xxx.hpp>


namespace react {
namespace has_default_implementation_detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
}

namespace extension {
    template <typename RawComputationName, typename Enable = void>
    struct has_default_implementation_impl {
        template <typename ComputationName>
        struct apply
            : has_default_implementation_detail::has_type<
                default_implementation_of<ComputationName>
            >
        { };
    };
} // end namespace extension

template <typename ComputationName>
struct has_default_implementation
    : extension::has_default_implementation_impl<
        typename detail::strip<ComputationName>::type
    >::template apply<ComputationName>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_HAS_DEFAULT_IMPLEMENTATION_HPP
