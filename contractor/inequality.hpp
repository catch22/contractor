#ifndef CONTRACTOR_INEQUALIY_HPP
#define CONTRACTOR_INEQUALIY_HPP

#include <array>
#include <iostream>
#include "bitset.hpp"
#include "subsystem.hpp"

namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// inequality -- represents entropy inequality with +1/-1 coefficients
//////////////////////////////////////////////////////////////////////////////
template <size_t N, size_t L, size_t R> struct inequality {
    enum { num_parties = N, num_lhs = L, num_rhs = R };

    std::array<subsystem<N>, L> lhs;
    std::array<subsystem<N>, R> rhs;

    size_t hamming_lhs(const bitset<L> &x, const bitset<L> &y) const {
        return hamming(x, y);
    }
    size_t hamming_rhs(const bitset<R> &x, const bitset<R> &y) const {
        return hamming(x, y);
    }
    size_t hamming_rhs_coeff(size_t) const { return 1; }
};

template <size_t N, size_t L, size_t R>
std::ostream &operator<<(std::ostream &os, const inequality<N, L, R> &ieq) {
    os << ieq.lhs[0];
    for(size_t l = 1; l < L; ++l) {
        os << " + ";
        os << ieq.lhs[l];
    }
    os << " >= ";
    os << ieq.rhs[0];
    for(size_t r = 1; r < R; ++r) {
        os << " + ";
        os << ieq.rhs[r];
    }
    return os;
}

//////////////////////////////////////////////////////////////////////////////
// inequality -- represents entropy inequality with arbitrary coefficients
//////////////////////////////////////////////////////////////////////////////
template <size_t N, size_t L, size_t R> struct weighted_inequality {
    enum { num_parties = N, num_lhs = L, num_rhs = R };

    std::array<subsystem<N>, L> lhs;
    std::array<subsystem<N>, R> rhs;
    std::array<size_t, L> lhs_weights;
    std::array<size_t, R> rhs_weights;

    size_t hamming_lhs(const bitset<L> &x, const bitset<L> &y) const {
        return weighted_hamming(x, y, lhs_weights);
    }
    size_t hamming_rhs(const bitset<R> &x, const bitset<R> &y) const {
        return weighted_hamming(x, y, rhs_weights);
    }
    size_t hamming_rhs_coeff(size_t r) const { return rhs_weights[r]; }
};

template <size_t N, size_t L, size_t R>
std::ostream &operator<<(std::ostream &os,
                         const weighted_inequality<N, L, R> &ieq) {
    os << ieq.lhs_weights[0] << " " << ieq.lhs[0];
    for(size_t l = 1; l < L; ++l)
        os << " + " << ieq.lhs_weights[l] << " " << ieq.lhs[l];
    os << " >= ";
    os << ieq.rhs_weights[0] << " " << ieq.rhs[0];
    for(size_t r = 1; r < R; ++r)
        os << " + " << ieq.rhs_weights[r] << " " << ieq.rhs[r];
    return os;
}
}

//////////////////////////////////////////////////////////////////////////////
// CONTRACTOR_IEQ, CONTRACTOR_WEIGHTED_IEQ -- macros to conveniently create
// (weighted_)inequality instances
//////////////////////////////////////////////////////////////////////////////
#define CONTRACTOR_IEQ(n, lhs, rhs)                                            \
    contractor::inequality<n, CONTRACTOR_IMPL_NUM_ARGS lhs,                    \
                           CONTRACTOR_IMPL_NUM_ARGS rhs>{                      \
        {{CONTRACTOR_IMPL_UNWRAP lhs}}, {{CONTRACTOR_IMPL_UNWRAP rhs}}};
#define CONTRACTOR_WEIGHTED_IEQ(n, lhs, rhs, lhs_weights, rhs_weights)         \
    contractor::weighted_inequality<n, CONTRACTOR_IMPL_NUM_ARGS lhs,           \
                                    CONTRACTOR_IMPL_NUM_ARGS rhs>{             \
        {{CONTRACTOR_IMPL_UNWRAP lhs}},                                        \
        {{CONTRACTOR_IMPL_UNWRAP rhs}},                                        \
        {{CONTRACTOR_IMPL_UNWRAP lhs_weights}},                                \
        {{CONTRACTOR_IMPL_UNWRAP rhs_weights}}};

#define CONTRACTOR_IMPL_UNWRAP(...) __VA_ARGS__
#define CONTRACTOR_IMPL_NUM_ARGS(...)                                          \
    int(sizeof((const char *[]){"", ##__VA_ARGS__}) / sizeof(const char *) - 1)

#endif
