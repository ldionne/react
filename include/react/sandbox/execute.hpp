/*!
 * @file
 * Defines `react::execute`.
 */

#ifndef REACT_EXECUTE_HPP
#define REACT_EXECUTE_HPP

namespace react {
    /*!
     * Executes a computation in a given environment.
     *
     * Prior to executing the computation, all of its predecessors are
     * executed. After executing the computation, all of its successors
     * are executed.
     *
     * If any predecessor, successor or requirement of the computation is
     * missing from the environment, a static assertion is triggered.
     */
    static constexpr struct {
        template <typename Computation, typename Env>
        void operator()(Computation&& c, Env&& e) const {

        }
    } execute{};
} // end namespace react

#endif // !REACT_EXECUTE_HPP
