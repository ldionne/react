/*!
 * @file
 * This file defines the `react::execute` intrinsic.
 */

#ifndef REACT_INTRINSIC_EXECUTE_HPP
#define REACT_INTRINSIC_EXECUTE_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/augment.hpp>
#include <react/tag_of.hpp>

#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react {
namespace extension {
    template <typename Tag, typename Enable = void>
    struct execute_impl {
    private:
        template <typename RawComp, typename Comp, typename Env>
        static auto impl(Comp&& c, Env&& env, int)
        REACT_AUTO_RETURN(
            typename boost::enable_if<boost::is_void<decltype(
                RawComp::execute(c, env)
            )>>::type(),
            RawComp::execute(c, env),
            augment(std::forward<Env>(env), std::forward<Comp>(c))
        )

        template <typename RawComp, typename Comp, typename Env>
        static auto impl(Comp&& c, Env&& env, int)
        REACT_AUTO_RETURN(
            typename boost::disable_if<boost::is_void<decltype(
                RawComp::execute(std::forward<Comp>(c), std::forward<Env>(env))
            )>>::type(),
            RawComp::execute(std::forward<Comp>(c), std::forward<Env>(env))
        )

        // This overload is picked if both overloads with int are invalid.
        template <typename RawComp, typename Comp, typename Env>
        static auto impl(Comp&& c, Env&& env, void*)
        REACT_AUTO_RETURN(
            augment(std::forward<Env>(env), std::forward<Comp>(c))
        )

    public:
        template <typename Computation, typename Env>
        static auto call(Computation&& c, Env&& env)
        REACT_AUTO_RETURN(
            impl<
                typename boost::remove_reference<Computation>::type
            >(std::forward<Computation>(c), std::forward<Env>(env), (int)0)
        )
    };
} // end namespace extension

static constexpr struct {
    template <typename Computation, typename Env>
    auto operator()(Computation&& c, Env&& env) const
    REACT_AUTO_RETURN(
        extension::execute_impl<
            typename tag_of<Computation>::type
        >::call(std::forward<Computation>(c), std::forward<Env>(env))
    )
} execute{};
} // end namespace react

#endif // !REACT_INTRINSIC_EXECUTE_HPP
