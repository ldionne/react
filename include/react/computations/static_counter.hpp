/*!
 * @file
 * This file defines `react::computations::static_counter`.
 */

#ifndef REACT_COMPUTATIONS_STATIC_COUNTER_HPP
#define REACT_COMPUTATIONS_STATIC_COUNTER_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsics.hpp>

#include <utility>


namespace react { namespace computations {
template <typename Integer, Integer InitialValue = static_cast<Integer>(0)>
class static_counter {
    template <typename DependentType, typename T>
    struct make_dependent_on {
        using type = T;
    };

public:
    template <typename Env>
    auto update(Env&& env) const
    REACT_AUTO_RETURN(
        augment(
            std::forward<Env>(env),
            // We need to make the next counter's type dependent in order to
            // avoid instantiating the next template, which would create
            // infinite recursion.
            static_counter<
                typename make_dependent_on<Env, Integer>::type,
                InitialValue + 1
            >{}
        )
    )

    constexpr Integer retrieve(detail::dont_care) const {
        return InitialValue;
    }
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_STATIC_COUNTER_HPP
