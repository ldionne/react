/*!
 * @file
 * This file defines `react::computation::depends_on`.
 */

#ifndef REACT_COMPUTATION_DEPENDS_ON_HPP
#define REACT_COMPUTATION_DEPENDS_ON_HPP

#include <boost/mpl/copy.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/set.hpp>


namespace react { namespace computation {
    namespace depends_on_detail {
        using namespace boost;

        template <typename ForwardSequence>
        struct as_set
            : mpl::copy<
                ForwardSequence,
                mpl::inserter<mpl::set<>, mpl::insert<mpl::_1, mpl::_2>>
            >
        { };
    } // end namespace depends_on_detail

    template <typename ...Dependencies>
    struct depends_on {
        using dependencies = typename boost::mpl::set<Dependencies...>::type;
    };

    template <typename Dependencies>
    struct depends_on<Dependencies> {
        using dependencies = typename boost::mpl::eval_if<
            boost::mpl::is_sequence<Dependencies>,
            depends_on_detail::as_set<Dependencies>,
            boost::mpl::set<Dependencies>
        >::type;
    };
}} // end namespace react::computation

#endif // !REACT_COMPUTATION_DEPENDS_ON_HPP
