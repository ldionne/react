/*!
 * @file
 * This file defines `react::argument_pack_as_feature_set`.
 */

#ifndef REACT_ARGUMENT_PACK_AS_FEATURE_SET_HPP
#define REACT_ARGUMENT_PACK_AS_FEATURE_SET_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <utility>


namespace react {
    template <typename ArgumentPack>
    class argument_pack_as_feature_set {
        ArgumentPack& args_;

    public:
        explicit argument_pack_as_feature_set(ArgumentPack& args)
            : args_(args)
        { }

        template <typename Key>
        auto operator[](Key&& key) REACT_AUTO_RETURN(
            args_[std::forward<Key>(key)]
        )

        void operator()(detail::dont_care) const { }
        void operator()(detail::dont_care, detail::dont_care) const { }
    };

    template <typename ArgumentPack>
    argument_pack_as_feature_set<ArgumentPack>
    make_feature_set_from_argument_pack(ArgumentPack& args) {
        return {args};
    }
} // end namespace react

#endif // !REACT_ARGUMENT_PACK_AS_FEATURE_SET_HPP
