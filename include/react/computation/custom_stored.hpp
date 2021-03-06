/*!
 * @file
 * Defines `react::computation::custom_stored`.
 */

#ifndef REACT_COMPUTATION_CUSTOM_STORED_HPP
#define REACT_COMPUTATION_CUSTOM_STORED_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/feature_of.hpp>
#include <react/intrinsic/predecessors_of.hpp>
#include <react/intrinsic/requirements_of.hpp>
#include <react/intrinsic/retrieve.hpp>
#include <react/intrinsic/successors_of.hpp>

#include <utility>


namespace react { namespace computation {
    /*!
     * Computation forwarding everything to another computation, which is
     * retrieved through a call to a custom function object.
     *
     * @tparam Computation
     *         The computation to which everything is forwarded.
     *
     * @tparam GetInstance
     *         A function object returning the computation to which everything
     *         shall be forwarded for the instance of `custom_stored` holding
     *         the function object. The function object shall be either
     *         nullary or unary, in which case the single argument is
     *         the `Environment` provided to `custom_stored`.
     */
    template <typename Computation, typename GetInstance>
    class custom_stored {
        GetInstance get_instance;

    public:
        using feature = typename feature_of<Computation>::type;
        using requirements = typename requirements_of<Computation>::type;
        using predecessors = typename predecessors_of<Computation>::type;
        using successors = typename successors_of<Computation>::type;

        template <typename Self, typename Env>
        static auto execute(Self&& self, Env&& env) REACT_AUTO_RETURN(
            react::execute(std::forward<Self>(self).get_instance(env), env)
        )

        template <typename Self, typename Env>
        static auto execute(Self&& self, Env&& env) REACT_AUTO_RETURN(
            react::execute(
                std::forward<Self>(self).get_instance(),
                std::forward<Env>(env)
            )
        )

        template <typename Self, typename Env>
        static auto retrieve(Self&& self, Env&& env) REACT_AUTO_RETURN(
            react::retrieve(std::forward<Self>(self).get_instance(env), env)
        )

        template <typename Self, typename Env>
        static auto retrieve(Self&& self, Env&& env) REACT_AUTO_RETURN(
            react::retrieve(
                std::forward<Self>(self).get_instance(),
                std::forward<Env>(env)
            )
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_CUSTOM_STORED_HPP
