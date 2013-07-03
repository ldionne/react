/*!
 * @file
 * This file defines `react::detail::feature_set_by_ref`.
 */

#ifndef REACT_DETAIL_FEATURE_SET_BY_REF_HPP
#define REACT_DETAIL_FEATURE_SET_BY_REF_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <utility>


namespace react { namespace detail {
template <typename FeatureSet>
class feature_set_by_ref {
    FeatureSet& fset_;

public:
    using computations = typename computations_of<FeatureSet>::type;

    explicit feature_set_by_ref(FeatureSet& fset)
        : fset_(fset)
    { }

    template <typename ...Args>
    auto operator()(Args&& ...args) REACT_AUTO_RETURN(
        fset_(std::forward<Args>(args)...)
    )

    template <typename Arg>
    auto operator[](Arg&& arg) REACT_AUTO_RETURN(
        fset_[std::forward<Arg>(arg)]
    )
};
}} // end namespace react::detail

#endif // !REACT_DETAIL_FEATURE_SET_BY_REF_HPP
