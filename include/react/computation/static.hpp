/*!
 * @file
 * Defines `react::computation::static_`.
 */

#ifndef REACT_COMPUTATION_STATIC_HPP
#define REACT_COMPUTATION_STATIC_HPP

#include <react/computation/custom_stored.hpp>


namespace react { namespace computation {
    namespace static_detail {
        template <typename T, typename MakeUnique>
        class get_static {
            static T& static_instance() {
                static T x;
                return x;
            }

        public:
            T& operator()() { return static_instance(); }
            T const& operator()() const { return static_instance(); }
        };
    } // end namespace static_detail

    /*!
     * Computation forwarding everything to another default-constructed
     * computation with `static` storage duration.
     *
     * @tparam Computation
     *         The type of the computation to which everything shall be
     *         forwarded.
     *
     * @tparam MakeUnique
     *         A type used to control how the `Computation` instance is
     *         shared across instantiations of this template. By default,
     *         all instantiations with the same `Computation` type shall
     *         share the same instance.
     */
    template <typename Computation, typename MakeUnique = Computation>
    using static_ = custom_stored<
        Computation,
        static_detail::get_static<Computation, MakeUnique>
    >;
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_STATIC_HPP
