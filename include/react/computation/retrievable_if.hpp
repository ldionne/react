/*!
 * @file
 * Defines `react::computation::retrievable_if`.
 */

#ifndef REACT_COMPUTATION_RETRIEVABLE_IF_HPP
#define REACT_COMPUTATION_RETRIEVABLE_IF_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/mpl/apply.hpp>
#include <boost/type_traits.hpp>
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
    private:
        // Avoid `using Computation::Computation`, which makes us unable to
        // use `Computation` to refer to the template parameter with Clang.
        using Computation_ = Computation;

    public:
        using Computation_::Computation_;
        using Computation::operator=;

        template <typename Env, typename = typename
        std::enable_if<
            boost::mpl::apply<Predicate, Env>::type::value
        >::type>
        static auto retrieve(retrievable_if& self, Env&& env)
        REACT_AUTO_RETURN(
            react::retrieve(
                static_cast<Computation&>(self),
                std::forward<Env>(env)
            )
        )

        template <typename Env, typename = typename
        std::enable_if<
            boost::mpl::apply<Predicate, Env>::type::value
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
