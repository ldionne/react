/*!
 * @file
 * Defines intrinsic operations for Boost.Fusion sequences.
 */

#ifndef REACT_EXTENSION_FUSION_HPP
#define REACT_EXTENSION_FUSION_HPP

#include <react/computation/implementing.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/in_execution_order.hpp>
#include <react/detail/index_of.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/filter_view.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/nview.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <functional>
#include <utility>


namespace react {
namespace fusion_detail {
    using namespace boost;

    template <typename Computations>
    auto in_execution_order(Computations& computations) REACT_AUTO_RETURN(
        fusion::nview<
            Computations,
            typename mpl::transform<
                typename detail::in_execution_order<
                    typename fusion::result_of::as_vector<Computations>::type
                >::type,
                detail::index_of<Computations, mpl::_1>
            >::type
        >{computations}
    )

    template <typename Computations>
    void execute_impl(Computations& computations) {
        fusion::for_each(
            in_execution_order(computations),
            std::bind(execute, std::placeholders::_1, std::ref(computations))
        );
    }

    template <typename Feature, typename Computation, typename =
    typename enable_if<
        is_same<typename feature_of<Computation>::type, Feature>
    >::type>
    auto implement(Computation&& c) REACT_AUTO_RETURN(
        std::forward<Computation>(c)
    )

    template <typename Feature, typename Computation, typename =
    typename disable_if<
        is_same<typename feature_of<Computation>::type, Feature>
    >::type>
    auto implement(Computation&& c) REACT_AUTO_RETURN(
        computation::implementing<
            Feature, typename remove_reference<Computation>::type
        >(std::forward<Computation>(c))
    )

    // We use our own `filter` to create a view to a non-`const` sequence
    // when the underlying sequence is a non-`const` lvalue.
    template <typename Pred, typename Sequence>
    auto filter(Sequence& seq) REACT_AUTO_RETURN(
        fusion::filter_view<Sequence, Pred>{seq}
    )

    template <typename Pred, typename Sequence>
    auto filter(Sequence const& seq) REACT_AUTO_RETURN(
        fusion::filter_view<Sequence const, Pred>{seq}
    )

    template <typename Feature, typename Env, typename Computation>
    auto augment_impl(Env&& env, Computation&& c) REACT_AUTO_RETURN(
        // We use push_front instead of push_back because using push_back
        // creates a type of view causing problems with retrieve. This is
        // due to a bug in Fusion. Using push_front does not change
        // anything else.
        fusion::push_front(
            filter<
                mpl::not_<is_same<feature_of<mpl::_1>, Feature>>
            >(std::forward<Env>(env)),
            implement<Feature>(std::forward<Computation>(c))
        )
    )
} // end namespace fusion_detail

namespace extension {
template <typename T>
struct execute_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Computations>
    static void call(Computations const& computations) {
        fusion_detail::execute_impl(computations);
    }

    template <typename Computations>
    static void call(Computations& computations) {
        fusion_detail::execute_impl(computations);
    }
};

template <typename T>
struct augment_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Env, typename Head, typename ...Tail>
    static auto call(Env&& env, Head&& head, Tail&& ...tail) REACT_AUTO_RETURN(
        augment(
            fusion_detail::augment_impl<typename feature_of<Head>::type>(
                std::forward<Env>(env), std::forward<Head>(head)
            ),
            std::forward<Tail>(tail)...
        )
    )

    template <typename Env>
    static auto call(Env&& env) REACT_AUTO_RETURN(
        std::forward<Env>(env)
    )
};

template <typename T>
struct retrieve_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Feature, typename Env>
    static auto call(Env&& env) REACT_AUTO_RETURN(
        retrieve(
            boost::fusion::deref(
                boost::fusion::find_if<
                    boost::is_same<Feature, feature_of<boost::mpl::_1>>
                >(env)
            ),
            env
        )
    )
};
}} // end namespace react::extension

#endif // !REACT_EXTENSION_FUSION_HPP
