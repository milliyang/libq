/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

namespace std {
    /// @brief computes square root for fixed-point by long-algorithm: to enable
    /// sqrt computation in case if no built-in integral type can represent
    /// (2*f) bits
    /// @ref http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> sqrt(utils::number<T, n, f, op, up> const val)
    {
        typedef utils::number<T, n, f, op, up> fp;
        typedef utils::cordic::lut<f, fp> lut;

        assert(("sqrt parameter is negative", val > fp(0)));
        if (val < fp(0)) {
            throw std::exception("sqrt: arg is negative");
        }

        if (val == fp(0.0)) {
            return fp(0.0);
        }
        else if (val == fp(1.0)) {
            return fp(1.0);
        }

        // Chosen fixed-point format must have several bits to represent
        // lut. Also format must enable argument translating to interval [1.0, 2.0].
        // So format must reserve two bits at least for integer part.
        typedef utils::number<boost::int_t<1u+n+2u>::least, n+2u, f, op, up> work_type;

        // reduces argument to interval [1.0, 2.0]
        int power(0);
        work_type arg(val);
        while (arg >= work_type(2.0)) {
            as_native(arg) >>= 1u;
            power--;
        }
        while (arg < work_type(1.0)) {
            as_native(arg) <<= 1u;
            power++;
        }

        // CORDIC vectoring mode:
        lut const angles = lut::build_arctanh_lut();
        fp const norm(1.0 / lut::compute_hyperbolic_scale(n));
        work_type x(work_type(arg) + 0.25), y(work_type(arg) - 0.25), z(arg);
        {
            size_t repeated(4u);
            size_t num(0);
            for(size_t i(1); i < f + 1u; ++i)
            {
                int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
                work_type::value_type const store(x.value());
                x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                y = y - work_type::wrap(sign * (store >> (num + 1u)));
                z = (sign > 0) ? z + angles[num] : z - angles[num];

                // repeat for convergence
                if (i == repeated && i != n - 1) {
                    int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
                    work_type::value_type const store(x.value());
                    x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                    y = y - work_type::wrap(sign * (store >> (num + 1u)));
                    z = (sign > 0) ? z + angles[num] : z - angles[num];

                    i += 1u;
                    repeated = 3u * repeated  + 1u;
                }

                num += 1u;
            }
        }

        work_type result(norm * x); // interval [1.0, 2.0]
        if (power > 0) {
            as_native(result) >>= (power >> 1u);
            if (power & 1u) {
                result = result * work_type::CONST_SQRT1_2;
            }
        }
        else {
            size_t const p(-power);
            as_native(result) <<= (p >> 1u);
            if (p & 1u) {
                result = result * work_type::CONST_SQRT2;
            }
        }

        return fp(result);
    }
}
