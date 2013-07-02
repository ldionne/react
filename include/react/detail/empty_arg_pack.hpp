/*!
 * @file
 * This file contains the definition of `react::detail::empty_arg_pack`.
 */

#ifndef REACT_DETAIL_EMPTY_ARG_PACK_HPP
#define REACT_DETAIL_EMPTY_ARG_PACK_HPP

#include <boost/parameter/parameters.hpp>


namespace react { namespace detail {
    namespace empty_arg_pack_detail {
        struct dummy;
        typedef boost::parameter::parameters<
            boost::parameter::optional<dummy>
        > ParamSpec;
    }
    static auto const empty_arg_pack = empty_arg_pack_detail::ParamSpec{}();
    using empty_arg_pack_type = decltype(empty_arg_pack);
}}

#endif // !REACT_DETAIL_EMPTY_ARG_PACK_HPP
