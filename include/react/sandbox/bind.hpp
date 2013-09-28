/*!
 * @file
 * Defines `react::bind`.
 */

#ifndef REACT_BIND_HPP
#define REACT_BIND_HPP

#include <react/detail/either.hpp>
#include <react/sandbox/computation_of.hpp>
#include <react/sandbox/detail/placeholder.hpp>

#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>


namespace react {
    /*!
     * Creates a fully functional computation from an unbound computation
     * and a feature map.
     *
     * @return A @ref Computation
     *
     * @todo
     * When we start using MPL11, use extended placeholders instead of
     * our own pseudo lambda substitution, if that does the same job.
     */
    template <typename UnboundComputation, typename ...Requirements>
    struct bind {
    private:
        template <typename T, bool = detail::is_placeholder<T>::value>
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
                detail::is_placeholder_expression<F<T...>>,
                replace_rec<F, T...>,
                boost::mpl::identity<F<T...>>
            >::type
        { };

        template <typename Placeholder>
        struct replace<Placeholder, true>
            : Placeholder::template apply<Requirements...>
        { };

    public:
        using type = typename replace<UnboundComputation>::type;
    };
} // end namespace react

#endif // !REACT_BIND_HPP
