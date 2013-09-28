/*!
 * @file
 * Defines `REACT_FETCH_NESTED`.
 */

#ifndef REACT_DETAIL_FETCH_NESTED_HPP
#define REACT_DETAIL_FETCH_NESTED_HPP

#define REACT_FETCH_NESTED(metafunction, nested_type, alternative)          \
    template <typename T>                                                   \
    struct metafunction {                                                   \
    private:                                                                \
        template <typename U>                                               \
        static typename U::nested_type pick(int);                           \
                                                                            \
        template <typename U>                                               \
        static alternative pick(...);                                       \
                                                                            \
    public:                                                                 \
        using type = decltype(pick<T>(int{}));                              \
    };                                                                      \
/**/

#endif // !REACT_DETAIL_FETCH_NESTED_HPP
