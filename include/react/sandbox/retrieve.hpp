/*!
 * @file
 * Defines `react::retrieve`.
 */

#ifndef REACT_RETRIEVE_HPP
#define REACT_RETRIEVE_HPP

namespace react {
    /*!
     * Retrieve the result of the computation implementing `Feature`
     * in a given environment.
     *
     * If any requirement of the computation is missing from the environment,
     * a static assertion is triggered.
     */
    template <typename Feature, typename Env>
    void retrieve(Env&& env) {

    }
} // end namespace react

#endif // !REACT_RETRIEVE_HPP
