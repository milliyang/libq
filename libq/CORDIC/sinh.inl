// sinh.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sinh.inl

 Provides CORDIC for sinh function as a sum of exponents

 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures" and
 J.S. Walther, "A Unified Algorithm for Elementary Functions"
*/

#ifndef INC_LIBQ_DETAILS_SINH_INL_
#define INC_LIBQ_DETAILS_SINH_INL_

#include <limits>

namespace libq {
namespace details {
/*!
*/
template<typename T>
class sinh_of {
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class sinh_of<libq::fixed_point<T, n, f, e, op, up> > {
 public:
    using promoted_type = libq::fixed_point<
                                std::intmax_t,
                                std::numeric_limits<std::intmax_t>::digits - f,
                                f,
                                e,
                                op,
                                up>;
};
}  // namespace details
}  // namespace libq

namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::sinh_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    sinh(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using sinh_type = typename libq::details::sinh_of<Q>::promoted_type;

    auto x = static_cast<sinh_type>(std::exp(_val)) -
        static_cast<sinh_type>(std::exp(-_val));
    libq::lift(x) >>= 1u;

    return x;
}
}  // namespace std

#endif  // INC_LIBQ_DETAILS_SINH_INL_
