/*!
 * @file
 * This file defines `react::computation::variable`.
 */

#ifndef REACT_COMPUTATION_VARIABLE_HPP
#define REACT_COMPUTATION_VARIABLE_HPP

#include <react/computation/noop.hpp>
#include <react/detail/auto_return.hpp>
#include <react/detail/dont_care.hpp>

#include <boost/array.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/extent.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_extent.hpp>
#include <utility>


namespace react { namespace computation {
    namespace variable_detail {
        using namespace boost;

        template <typename Array>
        struct make_array {
            using type = array<
                typename remove_extent<Array>::type,
                extent<Array>::value
            >;
        };

        template <typename T>
        struct adjust
            : mpl::eval_if<is_function<T>, add_pointer<T>,
              mpl::eval_if<is_array<T>, make_array<T>,
                           /* else */   mpl::identity<T>
            >>
        { };
    } // end namespace variable_detail

    /*!
     * Computation implemented as a single value.
     *
     * When the result of the computation is retrieved, a reference to the
     * value is returned, with a `const` qualifier matching that of the
     * computation.
     *
     * If a function type is specified, a pointer to the function type is
     * stored instead. If an array type is specified, a `boost::array` is
     * stored instead.
     *
     * @tparam Type
     *         The type of the value to store.
     */
    template <typename Type>
    struct variable : noop {
    private:
        using AdjustedType = typename variable_detail::adjust<Type>::type;
        AdjustedType var_;

    public:
        template <typename ...Args>
        explicit variable(Args&& ...args)
            : var_(std::forward<Args>(args)...)
        { }

        static auto retrieve(variable& self, detail::dont_care)
        REACT_AUTO_RETURN(self.var_)

        static auto retrieve(variable const& self, detail::dont_care)
        REACT_AUTO_RETURN(self.var_)

        static auto retrieve(variable&& self, detail::dont_care)
        REACT_AUTO_RETURN(std::move(self.var_))
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_VARIABLE_HPP
