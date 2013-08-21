/*!
 * @file
 * This file defines `react::computation::depends_on`.
 */

#ifndef REACT_COMPUTATION_DEPENDS_ON_HPP
#define REACT_COMPUTATION_DEPENDS_ON_HPP

#include <react/computation/noop.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/set_insert_range.hpp>
#include <boost/mpl/vector.hpp>


namespace react { namespace computation {
    namespace depends_on_detail {
        using namespace boost::mpl;

        template <typename ...Dependencies>
        struct gather_dependencies
            : fold<
                typename vector<Dependencies...>::type,
                typename set<>::type,
                if_<is_sequence<_2>,
                    set_insert_range<_1, _2>,
                    insert<_1, _2>
                >
            >
        { };
    } // end namespace depends_on_detail

    /*!
     * Computation implemented as a `noop` depending on other features.
     *
     * @tparam Dependencies...
     *         A sequence of `Feature`s on which this computation depends.
     *         Any Boost.MPL `Sequence` encountered in `Dependencies...` is
     *         flattened as-if its content was part of `Dependencies...`.
     */
    template <typename ...Dependencies>
    struct depends_on : noop {
        using dependencies = typename depends_on_detail::gather_dependencies<
            Dependencies...
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_DEPENDS_ON_HPP
