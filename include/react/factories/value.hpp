/*!
 * @file
 * This file defines `react::factories::value`.
 */

#ifndef REACT_FACTORIES_VALUE_HPP
#define REACT_FACTORIES_VALUE_HPP

#include <react/depends_on.hpp>
#include <react/detail/dont_care.hpp>
#include <react/factories/typed.hpp>
#include <react/implements.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_function.hpp>
#include <utility>


namespace react { namespace factories {
namespace value_detail {
    template <typename T, typename Feature>
    struct store_value : implements<Feature>, depends_on<> {
        template <typename Args>
        explicit store_value(Args&& args)
            : value_(std::forward<Args>(args)[
                boost::parameter::keyword<Feature>::get()])
        { }

        T& result(detail::dont_care) { return value_; }
        T const& result(detail::dont_care) const { return value_; }

    private:
        T value_;
    };
} // end namespace value_detail

/*!
 * Metafunction returning a computation implemented as a single value.
 *
 * When creating the computation, its initial value must be provided in
 * the argument pack by using a keyword whose tag is the feature being
 * implemented.
 *
 * A value computation can't be executed. When its result is retrieved,
 * it returns a reference to the value it holds internally, with a `const`
 * qualifier matching that of the computation.
 *
 * If a function type is specified, a pointer to the function type is
 * stored instead. Storing array types is not supported; use `std::array`
 * instead.
 *
 * Computations created with this factory are impliclty `typed` computations.
 */
template <typename T, typename Feature>
class value {
    using AdjustedType = typename boost::mpl::eval_if<
        boost::is_function<T>, boost::add_pointer<T>, boost::mpl::identity<T>
    >::type;

public:
    using type = typename typed<
        AdjustedType, value_detail::store_value<AdjustedType, Feature>
    >::type;
};
}} // end namespace react::factories

#endif // !REACT_FACTORIES_VALUE_HPP
