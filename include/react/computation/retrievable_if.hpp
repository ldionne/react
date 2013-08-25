/*!
 * @file
 * This file defines `react::computation::retrievable_if`.
 */

#ifndef REACT_COMPUTATION_RETRIEVABLE_IF_HPP
#define REACT_COMPUTATION_RETRIEVABLE_IF_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace computation {
    /*!
     * Wrapper making a computation retrievable only when a predicate is
     * satisfied.
     *
     * @tparam Predicate
     *         A Boost.MPL `LambdaExpression` returning whether the
     *         computation is retrievable given the `Environment` with
     *         which the retrieval is attempted.
     *
     * @tparam Computation
     *         The wrapped computation to be made conditionally retrievable.
     */
    template <typename Predicate, typename Computation>
    struct retrievable_if : Computation {
        using Computation::Computation;
        using Computation::operator=;

        template <typename Env, typename = typename boost::enable_if<
            typename boost::mpl::apply<Predicate, Env>::type
        >::type>
        static auto retrieve(retrievable_if& self, Env&& env)
        REACT_AUTO_RETURN(
            react::retrieve(
                static_cast<Computation&>(self),
                std::forward<Env>(env)
            )
        )

        template <typename Env, typename = typename boost::enable_if<
            typename boost::mpl::apply<Predicate, Env>::type
        >::type>
        static auto retrieve(retrievable_if const& self, Env&& env)
        REACT_AUTO_RETURN(
            react::retrieve(
                static_cast<Computation const&>(self),
                std::forward<Env>(env)
            )
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_RETRIEVABLE_IF_HPP
