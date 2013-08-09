/*!
 * @file
 * This file defines intrinsic operations for Boost.Fusion sequences.
 */

#ifndef REACT_EXTENSION_FUSION_HPP
#define REACT_EXTENSION_FUSION_HPP

#include <react/computation/named.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/topological_indexing.hpp>
#include <react/intrinsic/bind.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/name_of.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/nview.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/include/remove_if.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/if.hpp>
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
            Sequence, boost::mpl::back_inserter<boost::mpl::vector<>>
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
    static auto in_visitation_order(Computations const& computations)
    REACT_AUTO_RETURN(
        boost::fusion::nview<
            Computations const,
            typename topological_indexing<Computations>::type
        >{computations}
    )

public:
    template <typename Computations>
    static void call(Computations const& computations) {
        boost::fusion::for_each(
            in_visitation_order(computations),
            std::bind(execute, std::placeholders::_1, std::cref(computations))
        );
    }
};

template <typename T>
struct bind_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Name, typename Computation>
    using named = typename boost::mpl::if_<
        boost::is_same<typename name_of<Computation>::type, Name>,
        Computation,
        computation::named<Name, Computation>
    >::type;

    template <typename Name, typename Env, typename Computation>
    static auto call(Env&& env, Computation&& c)
    REACT_AUTO_RETURN(
        // We use push_front instead of push_back because using push_back
        // creates a type of view causing problems with retrieve. This is
        // due to a bug in Fusion. Using push_front does not change
        // anything else.
        boost::fusion::push_front(
            boost::fusion::remove_if<
                boost::is_same<
                    name_of<boost::mpl::_1>, Name
                >
            >(std::forward<Env>(env)),
            named<
                Name, typename boost::remove_reference<Computation>::type
            >(std::forward<Computation>(c))
        )
    )
};

template <typename T>
struct retrieve_impl<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Name, typename Env>
    static auto call(Env&& env)
    REACT_AUTO_RETURN(
        retrieve(
            boost::fusion::deref(
                boost::fusion::find_if<
                    boost::is_same<Name, name_of<boost::mpl::_1>>
                >(env)
            ),
            env
        )
    )
};
}} // end namespace react::extension

#endif // !REACT_EXTENSION_FUSION_HPP
