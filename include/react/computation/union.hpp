/*!
 * @file
 * Defines `react::computation::union_`.
 */

#ifndef REACT_COMPUTATION_UNION_HPP
#define REACT_COMPUTATION_UNION_HPP

#include <react/computation/executed_after.hpp>
#include <react/computation/executed_before.hpp>
#include <react/computation/requiring.hpp>
#include <react/detail/auto_return.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/predecessors_of.hpp>
#include <react/intrinsic/requirements_of.hpp>
#include <react/intrinsic/retrieve.hpp>
#include <react/intrinsic/successors_of.hpp>

#include <type_traits>
#include <utility>


namespace react { namespace computation {
    namespace union_detail {
        template <typename X, typename Y>
        struct union_of_2
            : requiring<
                typename requirements_of<X>::type,
                typename requirements_of<Y>::type
            >,
            executed_before<
                typename successors_of<X>::type,
                typename successors_of<Y>::type
            >,
            executed_after<
                typename predecessors_of<X>::type,
                typename predecessors_of<Y>::type
            >
        {
        private:
            X x;
            Y y;

        public:
            union_of_2() = default;

            template <typename ...Args>
            explicit union_of_2(Args const& ...args, typename std::enable_if<
                std::is_constructible<X, Args const&...>::value &&
                std::is_constructible<Y, Args const&...>::value
            >::type* = nullptr)
                : x{args...}, y{args...}
            { }

            template <typename ...Args>
            explicit union_of_2(Args&& ...args, typename std::enable_if<
                std::is_constructible<X, Args&&...>::value &&
                !std::is_constructible<Y, Args&&...>::value
            >::type* = nullptr)
                : x{std::forward<Args>(args)...}, y{}
            { }

            template <typename ...Args>
            explicit union_of_2(Args&& ...args, typename std::enable_if<
                !std::is_constructible<X, Args&&...>::value &&
                std::is_constructible<Y, Args&&...>::value
            >::type* = nullptr)
                : x{}, y{std::forward<Args>(args)...}
            { }

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
