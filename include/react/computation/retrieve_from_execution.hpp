/*!
 * @file
 * This file defines `react::computation::retrieve_from_execution`.
 */

#ifndef REACT_COMPUTATION_RETRIEVE_FROM_EXECUTION_HPP
#define REACT_COMPUTATION_RETRIEVE_FROM_EXECUTION_HPP

#include <react/computation/named.hpp>
#include <react/computation/temporary.hpp>
#include <react/computation/variable.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react { namespace computation {
    template <typename Computation>
    struct retrieve_from_execution : Computation {
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

        template <typename Self, typename Env>
        static auto execute(Self&& self, Env&& env) REACT_AUTO_RETURN(
            augment(env, self, make_temp(Computation::execute(self, env)))
        )

        template <typename Env>
        static auto retrieve(detail::dont_care, Env&& env) REACT_AUTO_RETURN(
            react::retrieve<anonymous>(std::forward<Env>(env))
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_RETRIEVE_FROM_EXECUTION_HPP
