/*!
 * @file
 * This file defines `react::computation::static_counter`.
 */

#ifndef REACT_COMPUTATION_STATIC_COUNTER_HPP
#define REACT_COMPUTATION_STATIC_COUNTER_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>
#include <react/intrinsic/augment.hpp>

#include <boost/mpl/empty_base.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react { namespace computation {
namespace static_counter_detail {
    template <typename Computation, typename Integer, Integer Value>
    struct counter_wrapper : Computation {
        using Computation::Computation;
        using Computation::operator=;

        template <typename Self, typename Env>
        static auto execute(Self&& self, Env&& env)
        REACT_AUTO_RETURN(
            augment(
                std::forward<Env>(env),
                counter_wrapper<
                    typename boost::remove_reference<Self>::type,
                    Integer,
                    Value + 1
                >{}
            )
        )

        static constexpr Integer
        retrieve(detail::dont_care, detail::dont_care) {
            return Value;
        }
    };
} // end namespace static_counter_detail

template <typename Integer, Integer InitialValue = static_cast<Integer>(0)>
using static_counter = static_counter_detail::counter_wrapper<
    boost::mpl::empty_base,
    Integer,
    InitialValue
>;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_STATIC_COUNTER_HPP
