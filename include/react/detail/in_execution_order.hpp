/*!
 * @file
 * Defines `react::detail::in_execution_order`.
 */

#ifndef REACT_DETAIL_IN_EXECUTION_ORDER_HPP
#define REACT_DETAIL_IN_EXECUTION_ORDER_HPP

#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/predecessors_of.hpp>
#include <react/intrinsic/successors_of.hpp>

#include <boost/mpl/any_of.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/make_index_of.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/value_view.hpp>


namespace react {
namespace in_execution_order_detail {
    namespace mpl = boost::mpl;

    template <typename NeighborsOf, typename DescriptorOf, typename VertexOf>
    class exists_path_from_to {
        template <typename Vertex>
        struct neighbors_of
            : mpl::apply<NeighborsOf, Vertex>
        { };

        template <typename Vertex>
        struct descriptor_of
            : mpl::apply<DescriptorOf, Vertex>
        { };

        template <typename VertexDescriptor>
        struct vertex_of
            : mpl::apply<VertexOf, VertexDescriptor>
        { };

    public:
        // Is there a path from `U` to `V`?
        template <typename U, typename V>
        struct apply
            : mpl::or_<
                mpl::has_key<
                    typename neighbors_of<U>::type,
                    typename descriptor_of<V>::type
                >,
                mpl::any_of<
                    typename neighbors_of<U>::type,
                    apply<vertex_of<mpl::_1>, V>
                >
            >
        { };
    };

    // Is `Computation1` a predecessor of `Computation2`?
    template <typename Computation1, typename Computation2, typename FeatureMap>
    struct is_predecessor
        : exists_path_from_to<
            predecessors_of<mpl::_1>,
            feature_of<mpl::_1>,
            mpl::at<FeatureMap, mpl::_1>
        >::template apply<Computation2, Computation1>
    { };

    // Is `Computation1` a successor of `Computation2`?
    template <typename Computation1, typename Computation2, typename FeatureMap>
    struct is_successor
        : exists_path_from_to<
            successors_of<mpl::_1>,
            feature_of<mpl::_1>,
            mpl::at<FeatureMap, mpl::_1>
        >::template apply<Computation2, Computation1>
    { };

    template <typename Computation1, typename Computation2, typename FeatureMap>
    struct is_executed_before
        : mpl::or_<
            is_predecessor<Computation1, Computation2, FeatureMap>,
            is_successor<Computation2, Computation1, FeatureMap>
        >
    { };


    // Is Computation1 a direct predecessor of Computation2
    template <typename Computation1, typename Computation2>
    struct is_direct_predecessor
        : mpl::or_<
            mpl::has_key<
                typename predecessors_of<Computation2>::type,
                typename feature_of<Computation1>::type
            >,
            mpl::has_key<
                typename successors_of<Computation1>::type,
                typename feature_of<Computation2>::type
            >
        >
    { };

    // Is Computation1 a direct successor of Computation2
    template <typename Computation1, typename Computation2>
    struct is_direct_successor
        : mpl::or_<
            mpl::has_key<
                typename successors_of<Computation2>::type,
                typename feature_of<Computation1>::type
            >,
            mpl::has_key<
                typename predecessors_of<Computation1>::type,
                typename feature_of<Computation2>::type
            >
        >
    { };

    template <typename Computation, typename FeatureMap>
    struct wrap_one {
        struct type : Computation {
            using original = Computation;

            using feature = typename feature_of<original>::type;

            // We make sure that all the
            using predecessors = typename mpl::fold<
                mpl::value_view<FeatureMap>,
                typename predecessors_of<original>::type,
                mpl::if_<is_direct_successor<original, mpl::_2>,
                    mpl::insert<mpl::_1, feature_of<mpl::_2>>,
                    mpl::_1
                >
            >::type;

            using successors = typename mpl::fold<
                mpl::value_view<FeatureMap>,
                typename successors_of<original>::type,
                mpl::if_<is_direct_predecessor<original, mpl::_2>,
                    mpl::insert<mpl::_1, feature_of<mpl::_2>>,
                    mpl::_1
                >
            >::type;
        };
    };

    template <typename Wrapped>
    struct unwrap_one {
        using type = typename Wrapped::original;
    };

    template <typename Computations>
    struct sort_in_execution_order
        : mpl::sort<
            Computations,
            is_executed_before<
                mpl::_1, mpl::_2,
                typename mpl::make_index_of<
                    Computations, feature_of<mpl::_1>
                >::type
            >
        >
    { };

    template <typename Computations>
    struct wrap
        : mpl::transform<Computations,
            wrap_one<mpl::_1,
                typename mpl::make_index_of<
                    Computations, feature_of<mpl::_1>
                >::type
            >
        >
    { };

    template <typename Computations>
    struct unwrap
        : mpl::transform<Computations, unwrap_one<mpl::_1>>
    { };
} // end namespace in_execution_order_detail

namespace detail {
    template <typename Computations>
    struct in_execution_order
        : in_execution_order_detail::unwrap<
            typename in_execution_order_detail::sort_in_execution_order<
                typename in_execution_order_detail::wrap<Computations>::type
            >::type
        >
    { };
} // end namespace detail
} // end namespace react

#endif // !REACT_DETAIL_IN_EXECUTION_ORDER_HPP
