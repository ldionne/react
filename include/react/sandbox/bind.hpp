/*!
 * @file
 * Defines `react::bind`.
 */

#ifndef REACT_BIND_HPP
#define REACT_BIND_HPP

#include <react/detail/either.hpp>
#include <react/sandbox/computation_of.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>


namespace react {
    namespace bind_detail {
        namespace mpl = boost::mpl;

        template <typename T>
        struct is_placeholder
            : mpl::false_
        { };

        template <typename Feature>
        struct is_placeholder<computation_of<Feature>>
            : mpl::true_
        { };

        template <typename T>
        struct is_placeholder_expr
            : is_placeholder<T>
        { };

        template <typename ...Ts>
        struct any_is_placeholder_expr
            : mpl::or_<
                is_placeholder_expr<Ts>...,
                // or_ requires at least 2 arguments
                mpl::false_,
                mpl::false_
            >
        { };

        template <template <typename ...> class F, typename ...T>
        struct is_placeholder_expr<F<T...>>
            : mpl::or_<
                is_placeholder<F<T...>>,
                any_is_placeholder_expr<T...>
            >
        { };
    } // end namespace bind_detail

    /*!
     * Creates a fully functional computation from an unbound computation
     * and a feature map.
     *
     * @return A @ref Computation
     *
     * @todo
     * When we start using MPL11, use extended placeholders instead of
     * our own pseudo lambda substitution.
     */
    template <typename UnboundComputation, typename FeatureMap>
    struct bind {
    private:
        template <typename T, bool = bind_detail::is_placeholder<T>::value>
        struct replace;

        template <typename T>
        struct replace<T, false>
            : boost::mpl::identity<T>
        { };

        template <template <typename ...> class F, typename ...T>
        struct replace_rec
            : detail::either<
                F<typename replace<T>::type...>,
                boost::mpl::identity<F<typename replace<T>::type...>>
            >
        { };

        template <template <typename ...> class F, typename ...T>
        struct replace<F<T...>, false>
            : boost::mpl::if_<
                bind_detail::any_is_placeholder_expr<T...>,
                replace_rec<F, T...>,
                boost::mpl::identity<F<T...>>
            >::type
        { };

        template <typename Placeholder>
        struct replace<Placeholder, true>
            : Placeholder::template apply<FeatureMap>
        { };

    public:
        using type = typename replace<UnboundComputation>::type;
    };
} // end namespace react

#endif // !REACT_BIND_HPP
