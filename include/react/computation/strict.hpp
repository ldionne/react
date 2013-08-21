/*!
 * @file
 * This file defines `react::computation::strict`.
 */

#ifndef REACT_COMPUTATION_STRICT_HPP
#define REACT_COMPUTATION_STRICT_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <utility>


namespace react { namespace computation {
namespace strict_detail {
    using namespace boost;

    template <typename Sequence>
    using pointers_to = mpl::transform_view<Sequence, add_pointer<mpl::_1>>;

    template <typename Env>
    struct assert_retrievable_from {
        template <typename Dependency, bool always_false = false>
        void operator()(Dependency*) const {
            // Make sure that
            // 1. the code gets instantiated
            // 2. we don't get a warning for unreachable code
            // 3. the code is never executed since it is useless to
            //    actually perform the retrieval
            if (always_false) {
                typename add_pointer<Env>::type env = 0;

                //////////////////////////////////////////////////////////////
                //
                // If compilation fails here, a computation marked as `strict`
                // was executed or its result was retrieved without all of its
                // dependencies being in the environment.
                //
                //////////////////////////////////////////////////////////////
                retrieve<Dependency>(static_cast<Env>(*env));
            }
        }
    };

    static constexpr struct {
        template <typename Computation, typename Env>
        auto operator()(Computation&& c, Env&& env) const
        REACT_AUTO_RETURN(
            mpl::for_each<
                pointers_to<typename dependencies_of<Computation>::type>
            >(assert_retrievable_from<Env&&>{}),
            execute(std::forward<Computation>(c), std::forward<Env>(env))
        )
    } execute_check{};

    static constexpr struct {
        template <typename Computation, typename Env>
        auto operator()(Computation&& c, Env&& env) const
        REACT_AUTO_RETURN(
            mpl::for_each<
                pointers_to<typename dependencies_of<Computation>::type>
            >(assert_retrievable_from<Env&&>{}),
            retrieve(std::forward<Computation>(c), std::forward<Env>(env))
        )
    } retrieve_check{};
} // end namespace strict_detail

/*!
 * Wrapper to make sure that all the dependencies of a computation are
 * available in the environment when it is executed or its result is
 * retrieved.
 */
template <typename Computation>
struct strict : Computation {
    using Computation::Computation;
    using Computation::operator=;

    template <typename Env>
    static auto execute(strict& self, Env&& env)
    REACT_AUTO_RETURN(
        strict_detail::execute_check(
            static_cast<Computation&>(self), std::forward<Env>(env)
        )
    )

    template <typename Env>
    static auto execute(strict const& self, Env&& env)
    REACT_AUTO_RETURN(
        strict_detail::execute_check(
            static_cast<Computation const&>(self), std::forward<Env>(env)
        )
    )

    template <typename Env>
    static auto retrieve(strict& self, Env&& env)
    REACT_AUTO_RETURN(
        strict_detail::retrieve_check(
            static_cast<Computation&>(self), std::forward<Env>(env)
        )
    )

    template <typename Env>
    static auto retrieve(strict const& self, Env&& env)
    REACT_AUTO_RETURN(
        strict_detail::retrieve_check(
            static_cast<Computation const&>(self), std::forward<Env>(env)
        )
    )
};
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_STRICT_HPP
