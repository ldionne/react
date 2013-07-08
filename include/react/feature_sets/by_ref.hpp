/*!
 * @file
 * This file defines `react::feature_sets::by_ref`.
 */

#ifndef REACT_FEATURE_SETS_BY_REF_HPP
#define REACT_FEATURE_SETS_BY_REF_HPP

#include <react/detail/auto_return.hpp>

#include <utility>


namespace react { namespace feature_sets {
    template <typename FeatureSet>
    class by_ref {
        FeatureSet& fset_;

    public:
        explicit by_ref(FeatureSet& fset)
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

    template <typename FeatureSet>
    by_ref<FeatureSet> make_by_ref(FeatureSet& fset) {
        return {fset};
    }
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_BY_REF_HPP
