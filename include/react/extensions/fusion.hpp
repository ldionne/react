/*!
 * @file
 * This file defines intrinsic operations for Boost.Fusion sequences.
 */

#ifndef REACT_EXTENSIONS_FUSION_HPP
#define REACT_EXTENSIONS_FUSION_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/fusion_fold.hpp>
#include <react/detail/topological_indexing.hpp>
#include <react/detail/update_computation.hpp>
#include <react/intrinsics.hpp>
#include <react/traits.hpp>

#include <boost/fusion/include/clear.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/include/nview.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace extensions {
template <typename T>
struct update<
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
    static auto call(Computations const& computations)
    REACT_AUTO_RETURN(
        detail::fusion_fold(
            in_visitation_order(computations),
            boost::fusion::clear(computations),
            detail::update_computation
        )
    )
};

template <typename T>
struct augment<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Env, typename Computation, typename ...Rest>
    static constexpr auto call(Env&& env, Computation&& c, Rest&& ...rest)
    REACT_AUTO_RETURN(
        react::augment(
            // We use push_front instead of push_back because using push_back
            // creates a type of view causing problems with retrieve. This is
            // due to a bug in Fusion. Using push_front does not change
            // anything else.
            boost::fusion::push_front(
                std::forward<Env>(env), std::forward<Computation>(c)
            ),
            std::forward<Rest>(rest)...
        )
    )

    template <typename Env>
    static constexpr auto call(Env&& env)
    REACT_AUTO_RETURN(
        std::forward<Env>(env)
    )
};

template <typename T>
struct retrieve<
    T, typename boost::enable_if<boost::fusion::traits::is_sequence<T>>::type
> {
    template <typename Name, typename Computations>
    static auto call(Computations const& computations)
    REACT_AUTO_RETURN(
        boost::fusion::deref(
            boost::fusion::find_if<
                boost::is_same<Name, name_of<boost::mpl::_1>>
            >(computations)
        ).retrieve(computations)
    )
};
}} // end namespace react::extensions

#endif // !REACT_EXTENSIONS_FUSION_HPP
