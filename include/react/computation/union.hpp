/*!
 * @file
 * This file defines `react::computation::union_`.
 */

#ifndef REACT_COMPUTATION_UNION_HPP
#define REACT_COMPUTATION_UNION_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/mpl/set_union.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>
#include <utility>


namespace react { namespace computation {
    namespace union_detail {
        using namespace boost;

        template <typename X, typename Y>
        class union_of_2 {
            X x;
            Y y;

        public:
            union_of_2() = default;

            template <typename ...Args>
            explicit union_of_2(Args const& ...args, typename enable_if_c<
                std::is_constructible<X, Args const&...>::value &&
                std::is_constructible<Y, Args const&...>::value
            >::type* = nullptr)
                : x{args...}, y{args...}
            { }

            template <typename ...Args>
            explicit union_of_2(Args&& ...args, typename enable_if_c<
                std::is_constructible<X, Args&&...>::value &&
                !std::is_constructible<Y, Args&&...>::value
            >::type* = nullptr)
                : x{std::forward<Args>(args)...}, y{}
            { }

            template <typename ...Args>
            explicit union_of_2(Args&& ...args, typename enable_if_c<
                !std::is_constructible<X, Args&&...>::value &&
                std::is_constructible<Y, Args&&...>::value
            >::type* = nullptr)
                : x{}, y{std::forward<Args>(args)...}
            { }

            using dependencies = typename mpl::set_union<
                typename dependencies_of<X>::type,
                typename dependencies_of<Y>::type
            >::type;

#       define REACT_I_INSTANTIATE_UNION(FUNC, XY)                          \
            template <typename Env>                                         \
            static auto FUNC(union_of_2& self, Env&& env)                   \
            REACT_AUTO_RETURN(                                              \
                react::FUNC(self.XY, std::forward<Env>(env))                \
            )                                                               \
                                                                            \
            template <typename Env>                                         \
            static auto FUNC(union_of_2 const& self, Env&& env)             \
            REACT_AUTO_RETURN(                                              \
                react::FUNC(self.XY, std::forward<Env>(env))                \
            )                                                               \
                                                                            \
            template <typename Env>                                         \
            static auto FUNC(union_of_2&& self, Env&& env)                  \
            REACT_AUTO_RETURN(                                              \
                react::FUNC(std::move(self.XY), std::forward<Env>(env))     \
            )                                                               \
        /**/
            REACT_I_INSTANTIATE_UNION(execute, x)
            REACT_I_INSTANTIATE_UNION(execute, y)
            REACT_I_INSTANTIATE_UNION(retrieve, x)
            REACT_I_INSTANTIATE_UNION(retrieve, y)
#       undef REACT_I_INSTANTIATE_UNION
        };

        template <typename X, typename Y, typename ...Z>
        struct make_union
            : make_union<X, typename make_union<Y, Z...>::type>
        { };

        template <typename X, typename Y>
        struct make_union<X, Y> {
            using type = union_of_2<X, Y>;
        };
    } // end namespace union_detail

    /*!
     * Computation implemented as the union of two or more computations.
     *
     * Whenever the `union_` is executed, all of the computations in it are
     * polled for execution. Whenever the result of the `union_` is retrieved,
     * all of the computations in it are polled for their result.
     *
     * Whenever two or more computations in the `union_` answer positively
     * to a poll, the call is ambiguous.
     *
     * @tparam Computations...
     *         A sequence of two or more computations to bundle together into
     *         a single computation.
     */
    template <typename ...Computations>
    using union_ = typename union_detail::make_union<Computations...>::type;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_UNION_HPP
