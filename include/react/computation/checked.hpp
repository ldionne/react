/*!
 * @file
 * This file defines `react::computation::checked`.
 */

#ifndef REACT_COMPUTATION_CHECKED_HPP
#define REACT_COMPUTATION_CHECKED_HPP

#include <react/detail/auto_return.hpp>
#include <react/intrinsic/dependencies_of.hpp>
#include <react/intrinsic/execute.hpp>
#include <react/intrinsic/retrieve.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <utility>


namespace react { namespace computation {
    template <typename Computation>
    struct checked : Computation {
    private:
        using ptr_to_deps = boost::mpl::transform_view<
            typename dependencies_of<Computation>::type,
            boost::add_pointer<boost::mpl::_1>
        >;

        template <typename Env>
        struct retrieve_from {
            template <typename Dependency, bool always_false = false>
            void operator()(Dependency*) const {
                // Make sure that
                // 1. the code gets instantiated
                // 2. we don't get a warning for unreachable code
                // 3. the code is never executed since it is useless to
                //    actually perform the retrieval
                if (always_false) {
                    typename boost::remove_reference<Env>::type* env = 0;
                    react::retrieve<Dependency>(static_cast<Env>(*env));
                }
            }
        };

    public:
        using Computation::Computation;
        using Computation::operator=;

        template <typename Env>
        static auto execute(checked const& self, Env&& env)
        REACT_AUTO_RETURN(
            boost::mpl::for_each<ptr_to_deps>(retrieve_from<Env&&>{}),
            react::execute(
                static_cast<Computation const&>(self),
                std::forward<Env>(env)
            )
        )

        template <typename Env>
        static auto execute(checked& self, Env&& env)
        REACT_AUTO_RETURN(
            boost::mpl::for_each<ptr_to_deps>(retrieve_from<Env&&>{}),
            react::execute(
                static_cast<Computation&>(self),
                std::forward<Env>(env)
            )
        )
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_CHECKED_HPP
