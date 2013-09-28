/*!
 * @file
 * Defines `react::implements`.
 */

#ifndef REACT_IMPLEMENTS_HPP
#define REACT_IMPLEMENTS_HPP

namespace react {
    //! Sets the derived computation's feature to `Feature`.
    template <typename Feature>
    struct implements {
        using feature = Feature;
    };
} // end namespace react

#endif // !REACT_IMPLEMENTS_HPP
