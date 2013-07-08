/*!
 * @file
 * This file defines `react::feature_sets::from_argument_pack`.
 */

#ifndef REACT_FEATURE_SETS_FROM_ARGUMENT_PACK_HPP
#define REACT_FEATURE_SETS_FROM_ARGUMENT_PACK_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <utility>


namespace react { namespace feature_sets {
    template <typename ArgumentPack>
    class from_argument_pack {
        ArgumentPack& args_;

    public:
        explicit from_argument_pack(ArgumentPack& args)
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
    from_argument_pack<ArgumentPack>
    make_from_argument_pack(ArgumentPack& args) {
        return {args};
    }
}} // end namespace react::feature_sets

#endif // !REACT_FEATURE_SETS_FROM_ARGUMENT_PACK_HPP
