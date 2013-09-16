/*!
 * @file
 * Defines the `REACT_AUTO_RETURN` macro.
 */

#ifndef REACT_DETAIL_AUTO_RETURN_HPP
#define REACT_DETAIL_AUTO_RETURN_HPP

#define REACT_AUTO_RETURN(...) \
    -> decltype(__VA_ARGS__) { return __VA_ARGS__; }

#endif // !REACT_DETAIL_AUTO_RETURN_HPP
