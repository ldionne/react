/*!
 * @file
 * This file defines `react::implements`.
 */

#ifndef REACT_IMPLEMENTS_HPP
#define REACT_IMPLEMENTS_HPP

namespace react {
    template <typename Feature>
    struct implements {
        using feature = Feature;
    };
}

#endif // !REACT_IMPLEMENTS_HPP
