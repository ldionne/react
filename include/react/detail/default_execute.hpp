/*!
 * @file
 * This file defines a default implementation for `react::execute`.
 */

#ifndef REACT_DETAIL_DEFAULT_EXECUTE_HPP
#define REACT_DETAIL_DEFAULT_EXECUTE_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsics.hpp>

#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <utility>


namespace react { namespace extensions {
template <typename Tag, typename Enable>
struct execute {
private:
    template <typename RawComp, typename Comp, typename Env>
    static auto impl(Comp&& c, Env&& env, int)
    REACT_AUTO_RETURN(
        typename boost::enable_if<boost::is_void<decltype(
            RawComp::execute(c, env)
        )>>::type(),
        RawComp::execute(c, env),
        react::augment(std::forward<Env>(env), std::forward<Comp>(c))
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
        react::augment(std::forward<Env>(env), std::forward<Comp>(c))
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
}} // end namespace react::extensions

#endif // !REACT_DETAIL_DEFAULT_EXECUTE_HPP
