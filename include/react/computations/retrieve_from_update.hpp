/*!
 * @file
 * This file defines `react::computations::retrieve_from_update`.
 */

#ifndef REACT_COMPUTATIONS_RETRIEVE_FROM_UPDATE_HPP
#define REACT_COMPUTATIONS_RETRIEVE_FROM_UPDATE_HPP

#include <react/computations/named.hpp>
#include <react/computations/temporary.hpp>
#include <react/computations/variable.hpp>
#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react { namespace computations {
    template <typename Computation>
    struct retrieve_from_update : Computation {
    private:
        struct anonymous;
        template <typename T>
        static auto make_temp(T&& t) REACT_AUTO_RETURN(
            named<
                anonymous,
                temporary<variable<typename boost::remove_reference<T>::type>>
            >{std::forward<T>(t)}
        )

    public:
        using Computation::Computation;
        using Computation::operator=;

        template <typename Env>
        auto update(Env const& env) REACT_AUTO_RETURN(
            augment(env, *this, make_temp(Computation::update(env)))
        )

        template <typename Env>
        auto update(Env const& env) const REACT_AUTO_RETURN(
            augment(env, *this, make_temp(Computation::update(env)))
        )

        template <typename Env>
        auto retrieve(Env&& env) const REACT_AUTO_RETURN(
            react::retrieve<anonymous>(std::forward<Env>(env))
        )
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_RETRIEVE_FROM_UPDATE_HPP
