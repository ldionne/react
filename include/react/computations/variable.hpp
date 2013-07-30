/*!
 * @file
 * This file defines `react::computations::variable`.
 */

#ifndef REACT_COMPUTATIONS_VARIABLE_HPP
#define REACT_COMPUTATIONS_VARIABLE_HPP

#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <utility>


namespace react { namespace computations {
/*!
 * Computation representing a single variable.
 *
 * When the result of the computation is retrieved, a reference to the
 * variable is returned, with a `const` qualifier matching that of the
 * computation.
 *
 * If a function type is specified, a pointer to the function type is
 * stored instead. Storing array types is not supported; use `std::array`
 * instead.
 */
template <typename Type>
struct variable {
private:
    using AdjustedType = typename boost::mpl::eval_if<
        boost::is_function<Type>,
        boost::add_pointer<Type>,
        boost::mpl::identity<Type>
    >::type;

    AdjustedType var_;

public:
    template <typename ...Args>
    explicit variable(Args&& ...args)
        : var_(std::forward<Args>(args)...)
    { }

    static auto retrieve(variable& self, detail::dont_care)
    REACT_AUTO_RETURN(
        self.var_
    )

    static auto retrieve(variable const& self, detail::dont_care)
    REACT_AUTO_RETURN(
        self.var_
    )
};
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_VARIABLE_HPP
