#ifndef CONTRACTOR_BITSET_HPP
#define CONTRACTOR_BITSET_HPP

#include <bitset>
#include <string>
#include <array>

namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// bitset -- small wrapper around std::bitset to avoid errors
//////////////////////////////////////////////////////////////////////////////
template <size_t B> struct bitset {
    bool operator[](size_t idx) const { return impl[idx]; }
    void set(size_t idx, bool value = true) { impl.set(idx, value); }
    size_t count() const { return impl.count(); }

    bitset operator^(const bitset &rhs) const {
        return bitset{impl ^ rhs.impl};
    }
    bool operator==(const bitset &rhs) const { return impl == rhs.impl; }
    bool operator!=(const bitset &rhs) const { return impl != rhs.impl; }

    std::string str_lsb_first() const {
        auto s = impl.to_string();
        return std::string(s.rbegin(), s.rend());
    }

    std::bitset<B> impl;
};

//////////////////////////////////////////////////////////////////////////////
// (weighted) hamming distance
//////////////////////////////////////////////////////////////////////////////
template <size_t B> size_t hamming(const bitset<B> &x, const bitset<B> &y) {
    return (x ^ y).count();
}

template <size_t B>
size_t weighted_hamming(const bitset<B> &x, const bitset<B> &y,
                        const std::array<size_t, B> &weights) {
    const bitset<B> z = x ^ y;
    size_t total = 0;
    for(size_t i = 0; i < B; ++i)
        total += z[i] * weights[i];
    return total;
}

//////////////////////////////////////////////////////////////////////////////
// bitsets -- all bitsets on B bits
//////////////////////////////////////////////////////////////////////////////
template <size_t B> struct bitsets {
    struct iterator;

    iterator begin() const { return iterator(); }
    iterator end() const {
        iterator bit;
        bit.m_done = true;
        return bit;
    }

    struct iterator {
        bitset<B> operator*() const { return m_bs; }

        void operator++() {
            if(m_done)
                throw std::logic_error("operator++() beyond end()");

            // add first bit
            bool carry = m_bs[0];
            m_bs.set(0, !m_bs[0]);

            // keep on adding carry
            for(size_t i = 1; i < B; ++i) {
                const bool sum = m_bs[i] ^ carry;
                carry = m_bs[i] & carry;
                m_bs.set(i, sum);
            }
            m_done = !m_bs.count();
        }

        bool operator!=(const iterator &rhs) {
            return m_bs != rhs.m_bs || m_done != rhs.m_done;
        }

        bitset<B> m_bs;
        bool m_done{false};
    };
};
}

#endif
