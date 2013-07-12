/*!
 * @file
 * This file defines `react::detail::feature_dependency_graph`.
 */

#ifndef REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP
#define REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP

#include <react/traits.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/reachable_set.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/transform_view.hpp>


namespace react { namespace detail {
namespace feature_dependency_graph_detail {
    namespace mpl = boost::mpl;

    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template <typename T>
    struct is_composite_feature
        : has_type<subfeatures_of<T>>
    { };

    template <typename ...Computations>
    class make_feature_dependency_graph {
        using PredefinedComputations = mpl::map<
            mpl::pair<typename feature_of<Computations>::type, Computations>...
        >;

        template <typename Feature>
        struct computation_of
            : mpl::eval_if<
                mpl::has_key<PredefinedComputations, Feature>,
                mpl::at<PredefinedComputations, Feature>,
                mpl::apply<Feature>
            >
        { };

        template <typename X> struct wrap_feature_or_composite;

        template <typename CompositeFeature>
        class graph : public CompositeFeature {
            struct fake_root_vertex {
                using open_neighborhood = mpl::transform_view<
                    typename subfeatures_of<CompositeFeature>::type,
                    wrap_feature_or_composite<mpl::_1>
                >;
            };

        public:
            using vertices = typename mpl::erase_key<
                typename mpl::reachable_set<fake_root_vertex>::type,
                fake_root_vertex
            >::type;
        };

        template <typename Feature>
        struct vertex : Feature {
            using computation = typename computation_of<Feature>::type;

            using open_neighborhood = mpl::transform_view<
                typename dependencies_of<computation>::type,
                wrap_feature_or_composite<mpl::_1>
            >;
        };

        template <typename X>
        struct wrap_feature_or_composite {
            using type = vertex<
                typename mpl::if_<is_composite_feature<X>, graph<X>, X>::type
            >;
        };

        struct root_composite_feature {
            using subfeatures = mpl::set<
                typename feature_of<Computations>::type...
            >;
        };

    public:
        using type = graph<root_composite_feature>;
    };
} // end namespace feature_dependency_graph_detail

template <typename ...Computations>
using feature_dependency_graph = typename feature_dependency_graph_detail::
                        make_feature_dependency_graph<Computations...>::type;
}} // end namespace react::detail

#endif // !REACT_DETAIL_FEATURE_DEPENDENCY_GRAPH_HPP
