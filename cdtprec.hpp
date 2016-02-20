
#ifndef _CDTPREC_HPP_
#define _CDTPREC_HPP_

#include <cmath>

#include "genericfp.hpp"

namespace CDTPrec {

template <typename fptype>
class DoublePrec;

template <typename fptype>
class TriplePrec;

template <typename hwfptype>
class CPrec {
 public:
  static constexpr int getPrec() {
    return GenericFP::fpconvert<hwfptype>::precision;
  }

  CPrec(hwfptype val = NAN) { this->val = val; }

  constexpr bool operator==(float rhs) const {
    return val == rhs;
  }

  constexpr bool operator==(double rhs) const {
    return val == rhs;
  }

  constexpr bool operator==(long double rhs) const {
    return val == rhs;
  }

  constexpr bool operator!=(float rhs) const {
    return val != rhs;
  }

  constexpr bool operator!=(double rhs) const {
    return val != rhs;
  }

  constexpr bool operator!=(long double rhs) const {
    return val != rhs;
  }

 private:
  hwfptype val;
};
};
#endif
