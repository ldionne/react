/*!
 * @file
 * This file defines `react::feature_sets::empty`.
 */

#ifndef REACT_FEATURE_SETS_EMPTY_HPP
#define REACT_FEATURE_SETS_EMPTY_HPP

namespace react { namespace feature_sets {
    struct empty {
        template <typename Feature>
        void operator[](Feature&&) const { }

        template <typename SemanticTag>
        void operator()(SemanticTag&&) const { }

        template <typename SemanticTag, typename FeatureSet>
        void operator()(SemanticTag&&, FeatureSet&&) const { }
    };
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_EMPTY_HPP
