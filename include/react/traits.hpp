/*!
 * @file
 * This file defines the traits used by the library.
 */

#ifndef REACT_TRAITS_HPP
#define REACT_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/enable_if.hpp>


namespace react {
    namespace traits_detail {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(dependencies)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(feature)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(subfeatures)

        template <typename Computation>
        struct dependencies_of {
            using type = typename Computation::dependencies;
        };

        template <typename Computation>
        struct feature_of {
            using type = typename Computation::feature;
        };

        template <typename CompositeFeature>
        struct subfeatures_of {
            using type = typename CompositeFeature::subfeatures;
        };
    } // end namespace traits_detail

    template <typename Computation>
    struct dependencies_of
        : boost::lazy_enable_if<
            traits_detail::has_dependencies<Computation>,
            traits_detail::dependencies_of<Computation>
        >
    { };

    template <typename Computation>
    struct feature_of
        : boost::lazy_enable_if<
            traits_detail::has_feature<Computation>,
            traits_detail::feature_of<Computation>
        >
    { };

    template <typename Computation>
    struct subfeatures_of
        : boost::lazy_enable_if<
            traits_detail::has_subfeatures<Computation>,
            traits_detail::subfeatures_of<Computation>
        >
    { };
} // end namespace react

#endif // !REACT_TRAITS_HPP
