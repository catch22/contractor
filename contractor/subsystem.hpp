#pragma once
#include "bitset.hpp"
#include <iostream>

namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// subsystem -- represents subsystem on N parties by a bitset
//////////////////////////////////////////////////////////////////////////////
template <size_t N> struct subsystem {
  subsystem() {}

  subsystem(const char *str) {
    // require non-empty string
    if (!*str)
      throw std::logic_error("empty subsystem");

    for (; *str; ++str) {
      char ch = *str;
      if (ch < 'A' || ch > 'Z')
        throw std::logic_error("each party should be in A...Z");
      size_t idx = ch - 'A';
      if (bitset[idx])
        throw std::logic_error("party appears multiple times");
      bitset.set(idx);
    }
  }

  std::string str() const {
    std::string s;
    for (size_t idx = 0; idx < N; ++idx)
      if (bitset[idx])
        s.push_back('A' + idx);
    return s;
  }

  contractor::bitset<N> bitset;
};

template <size_t N>
std::ostream &operator<<(std::ostream &os, const subsystem<N> &sub) {
  return os << sub.str();
}
} // namespace contractor
