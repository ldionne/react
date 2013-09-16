/*!
 * @file
 * Defines `react::computation::executable_if`.
 */

#ifndef REACT_COMPUTATION_EXECUTABLE_IF_HPP
#define REACT_COMPUTATION_EXECUTABLE_IF_HPP

namespace react { namespace computation {
    template <template <typename ...> class StatelessWrapper, typename WrappedComputation>
    struct stateless_wrapper : WrappedComputation {
        using WrappedComputation::WrappedComputation;
        using WrappedComputation::operator=;

    private:
        using Wrapper = StatelessWrapper<WrappedComputation>;

    public:
        template <typename Self, typename Env>
        static auto execute(Self&& self, Env&& env) REACT_AUTO_RETURN(
            Wrapper::execute(std::forward<Self>(self), std::forward<Env>(env))
        );

        template <typename Self, typename Env>
        static auto retrieve(Self&& self, Env&& env) REACT_AUTO_RETURN(
            Wrapper::retrieve(std::forward<Self>(self), std::forward<Env>(env))
        );
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_EXECUTABLE_IF_HPP
