/*!
 * @file
 * This file defines `react::computation::depends_on`.
 */

#ifndef REACT_COMPUTATION_DEPENDS_ON_HPP
#define REACT_COMPUTATION_DEPENDS_ON_HPP

#include <react/computation/noop.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>


namespace react { namespace computation {
    /*!
     * Computation implemented as a `noop` depending on other computations.
     *
     * @tparam Dependencies...
     *         A sequence of `ComputationName`s on which this computation
     *         depends. If there is a single Boost.MPL `Sequence` provided
     *         as a template argument, the content of that sequence is
     *         interpreted as being the sequence of `ComputationName`s.
     */
    template <typename ...Dependencies>
    struct depends_on : noop {
        using dependencies = typename boost::mpl::set<Dependencies...>::type;
    };

    template <typename Dependencies>
    struct depends_on<Dependencies> : noop {
        using dependencies = typename boost::mpl::eval_if<
            boost::mpl::is_sequence<Dependencies>,
            boost::mpl::set_insert_range<boost::mpl::set0<>, Dependencies>,
            boost::mpl::set<Dependencies>
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_DEPENDS_ON_HPP
