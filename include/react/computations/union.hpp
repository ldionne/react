/*!
 * @file
 * This file defines `react::computations::union_`.
 */

#ifndef REACT_COMPUTATIONS_UNION_HPP
#define REACT_COMPUTATIONS_UNION_HPP

#include <react/detail/auto_return.hpp>
#include <react/traits.hpp>

#include <boost/mpl/set_union.hpp>
#include <boost/utility/enable_if.hpp>
#include <type_traits>
#include <utility>


namespace react { namespace computations {
    template <typename X, typename Y>
    struct union_ : private X, private Y {
        union_() = default;

        template <typename ...Args>
        explicit union_(Args const& ...args, typename boost::enable_if_c<
            std::is_constructible<X, Args const& ...>::value &&
            std::is_constructible<Y, Args const& ...>::value
        >::type* = nullptr)
            : X{args...}, Y{args...}
        { }

        template <typename ...Args>
        explicit union_(Args const& ...args, typename boost::enable_if_c<
            std::is_constructible<X, Args const& ...>::value &&
            !std::is_constructible<Y, Args const& ...>::value
        >::type* = nullptr)
            : X{args...}, Y{}
        { }

        template <typename ...Args>
        explicit union_(Args const& ...args, typename boost::enable_if_c<
            !std::is_constructible<X, Args const& ...>::value &&
            std::is_constructible<Y, Args const& ...>::value
        >::type* = nullptr)
            : X{}, Y{args...}
        { }

        using dependencies = typename boost::mpl::set_union<
            typename dependencies_of<X>::type,
            typename dependencies_of<Y>::type
        >::type;

        template <typename Self, typename Env>
        static auto update(Self&& self, Env&& env) REACT_AUTO_RETURN(
            X::update(std::forward<Self>(self), std::forward<Env>(env))
        )

        template <typename Self, typename Env>
        static auto update(Self&& self, Env&& env) REACT_AUTO_RETURN(
            Y::update(std::forward<Self>(self), std::forward<Env>(env))
        )


        template <typename Env>
        auto retrieve(Env&& env) REACT_AUTO_RETURN(
            X::retrieve(std::forward<Env>(env))
        )

        template <typename Env>
        auto retrieve(Env&& env) REACT_AUTO_RETURN(
            Y::retrieve(std::forward<Env>(env))
        )

        template <typename Env>
        auto retrieve(Env&& env) const REACT_AUTO_RETURN(
            X::retrieve(std::forward<Env>(env))
        )

        template <typename Env>
        auto retrieve(Env&& env) const REACT_AUTO_RETURN(
            Y::retrieve(std::forward<Env>(env))
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_UNION_HPP
