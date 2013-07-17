/*!
 * @file
 * This file defines `react::computations::serialize`.
 */

#ifndef REACT_COMPUTATIONS_SERIALIZE_HPP
#define REACT_COMPUTATIONS_SERIALIZE_HPP

#include <react/computations/depends_on.hpp>
#include <react/intrinsics.hpp>


namespace react { namespace computations {
    template <typename Value, typename Archive>
    struct serialize : depends_on<Value, Archive> {
        template <typename Env>
        void update(Env const& env) const {
            retrieve<Archive>(env) & retrieve<Value>(env);
        }
    };
}} // end namespace react::computations

#endif // !REACT_COMPUTATIONS_SERIALIZE_HPP
