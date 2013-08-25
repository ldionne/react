/*!
 * @file
 * This file defines intrinsic operations for Boost.Fusion sequences.
 */

#ifndef REACT_EXTENSION_FUSION_HPP
#define REACT_EXTENSION_FUSION_HPP

#include <react/computation/implementing.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/topological_indexing.hpp>
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
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <functional>
#include <utility>


namespace react { namespace extension {
template <typename T>
struct execute_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
private:
    template <typename Sequence>
    struct as_mpl_vector
        : boost::mpl::copy<
            Sequence,
            boost::mpl::back_inserter<typename boost::mpl::vector<>::type>
        >
    { };

    // Wrap the real `topological_indexing` to interact with Boost.Fusion.
    template <typename Computations>
    using topological_indexing = as_mpl_vector<
        typename detail::topological_indexing<
            typename as_mpl_vector<Computations>::type
        >::type
    >;

    template <typename Computations>
    static auto in_visitation_order(Computations& computations)
    REACT_AUTO_RETURN(
        boost::fusion::nview<
            Computations,
            typename topological_indexing<Computations>::type
        >{computations}
    )

    template <typename Computations>
    static void call_impl(Computations& computations) {
        boost::fusion::for_each(
            in_visitation_order(computations),
            std::bind(execute, std::placeholders::_1, std::ref(computations))
        );
    }

public:
    template <typename Computations>
    static void call(Computations const& computations) {
        call_impl(computations);
    }

    template <typename Computations>
    static void call(Computations& computations) {
        call_impl(computations);
    }
};

template <typename T>
struct augment_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
private:
    template <typename Feature, typename Computation, typename =
        typename boost::enable_if<
            boost::is_same<typename feature_of<Computation>::type, Feature>
        >::type
    >
    static auto implement(Computation&& c, int)
    REACT_AUTO_RETURN(
        std::forward<Computation>(c)
    )

    template <typename Feature, typename Computation>
    static auto implement(Computation&& c, ...)
    REACT_AUTO_RETURN(
        computation::implementing<
            Feature, typename boost::remove_reference<Computation>::type
        >(std::forward<Computation>(c))
    )

    // We use our own `filter` to create a view to a non-`const` sequence
    // when the underlying sequence is a non-`const` lvalue.
    template <typename Pred, typename Sequence>
    static auto filter(Sequence& seq) REACT_AUTO_RETURN(
        boost::fusion::filter_view<Sequence, Pred>{seq}
    )

    template <typename Pred, typename Sequence>
    static auto filter(Sequence const& seq) REACT_AUTO_RETURN(
        boost::fusion::filter_view<Sequence const, Pred>{seq}
    )

    template <typename Feature, typename Env, typename Computation>
    static auto call_impl(Env&& env, Computation&& c)
    REACT_AUTO_RETURN(
        // We use push_front instead of push_back because using push_back
        // creates a type of view causing problems with retrieve. This is
        // due to a bug in Fusion. Using push_front does not change
        // anything else.
        boost::fusion::push_front(
            filter<
                boost::mpl::not_<
                    boost::is_same<feature_of<boost::mpl::_1>, Feature>
                >
            >(std::forward<Env>(env)),
            implement<Feature>(std::forward<Computation>(c), 0)
        )
    )

public:
    template <typename Env, typename Head, typename ...Tail>
    static auto call(Env&& env, Head&& head, Tail&& ...tail)
    REACT_AUTO_RETURN(
        augment(
            call_impl<typename feature_of<Head>::type>(
                std::forward<Env>(env), std::forward<Head>(head)
            ),
            std::forward<Tail>(tail)...
        )
    )

    template <typename Env>
    static auto call(Env&& env)
    REACT_AUTO_RETURN(
        std::forward<Env>(env)
    )
};

template <typename T>
struct retrieve_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Feature, typename Env>
    static auto call(Env&& env)
    REACT_AUTO_RETURN(
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
