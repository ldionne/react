/*!
 * @file
 * This file defines the `react::instantiate` intrinsic.
 */

#ifndef REACT_INTRINSIC_INSTANTIATE_HPP
#define REACT_INTRINSIC_INSTANTIATE_HPP

#include <react/detail/strip.hpp>

#include <boost/mpl/eapply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>


namespace react {
namespace instantiate_detail {
    using namespace boost::mpl;
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template <typename Implementation, typename ComputationMap>
    struct instantiate_safe
        : eval_if<has_type<eapply<Implementation, ComputationMap>>,
            eapply<Implementation, ComputationMap>,
            identity<Implementation>
        >
    { };
} // end namespace instantiate_detail

namespace extension {
    template <typename RawImplementation, typename Enable = void>
    struct instantiate_impl {
        template <typename, typename ComputationMap>
        struct apply
            : instantiate_detail::instantiate_safe<
                RawImplementation, ComputationMap
            >
        { };
    };
} // end namespace extension

template <typename Implementation, typename ComputationMap>
struct instantiate
    : extension::instantiate_impl<
        typename detail::strip<Implementation>::type
    >::template apply<Implementation, ComputationMap>
{ };
} // end namespace react

#endif // !REACT_INTRINSIC_INSTANTIATE_HPP
