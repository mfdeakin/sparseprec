
#ifndef _SPARSEPREC_HPP_
#define _SPARSEPREC_HPP_

#include <cmath>

#include "genericfp.hpp"

namespace SparsePrec {

template <typename fptype>
class DoublePrec;

template <typename hwfloat>
class CPrec {
 public:
  using single = hwfloat;
  static constexpr int getPrec() {
    return GenericFP::fpconvert<single>::precision;
  }

  CPrec(single val = NAN) { this->val = val; }

  constexpr bool isPlus() const {
    return !std::isnan(val) && (val > 0.0);
  }

  constexpr bool isZero() const {
    return !std::isnan(val) && (val == 0.0);
  }

  constexpr bool isNeg() const {
    return !std::isnan(val) && (val < 0.0);
  }

  template <typename fp>
  constexpr bool operator==(fp rhs) const {
    return val == rhs.val;
  }

  template <typename fp>
  constexpr bool operator<(fp rhs) const {
    return val < rhs.val;
  }

  template <typename fp>
  constexpr bool operator<=(fp rhs) const {
    return val <= rhs.val;
  }

  template <typename fp>
  constexpr bool operator>(fp rhs) const {
    return val > rhs.val;
  }

  template <typename fp>
  constexpr bool operator>=(fp rhs) const {
    return val >= rhs.val;
  }

  template <typename fp>
  constexpr bool operator!=(fp rhs) const {
    return val != rhs.val;
  }

  constexpr explicit operator single() const { return val; }

  constexpr operator DoublePrec<CPrec<single>>() const {
    return DoublePrec<CPrec<single>>(val, val * 0.0);
  }

  constexpr DoublePrec<CPrec<single>> operator+(
      CPrec<single> rhs) const {
    single larger = (val > rhs.val) ? val : rhs.val;
    single smaller = (val < rhs.val) ? val : rhs.val;
    single sum = larger + smaller;
    single err = sum - larger;
    single correct = smaller - err;
    return DoublePrec<CPrec<single>>(
        CPrec<single>(sum), CPrec<single>(correct));
  }

  template <typename>
  friend class CPrec;

 private:
  single val;
};

template <typename single>
class DoublePrec {
 public:
  static constexpr int getPrec() {
    return 2 * single::getPrec();
  }

  DoublePrec(const single &greater, const single &lesser) {
    this->greater = greater;
    this->lesser = lesser;
  }

  constexpr single higherOrder() const { return greater; }
  constexpr single lowerOrder() const { return lesser; }

  constexpr DoublePrec<DoublePrec<single>> operator+(
      single rhs) const {
    /* I'm uncertain whether the cost of branch predicition
     * is better or worse than just doing all of the
     * floating point operations, but this is much cleaner,
     * so keep this
     */
    if(greater < rhs) {
      return DoublePrec<DoublePrec<single>>(
          greater + rhs, DoublePrec<single>(lesser, 0.0));
    } else if(rhs < lesser) {
      // We don't know if rhs is small enough
      DoublePrec<single> sum1(lesser + rhs);
      // We know sum1.lesser is totally safe
      DoublePrec<single> sum2(sum1.greater + greater);
      return DoublePrec<DoublePrec<single>>(
          sum2, DoublePrec<single>(sum1.lesser, 0.0));
    } else {
    }
    single larger((greater > rhs) ? greater : rhs);
    single smaller((greater < rhs) ? greater : rhs);
    single sum(larger + smaller);
    single err = sum - larger;
    single correct = smaller - err;
    return DoublePrec<DoublePrec<single>>(
        DoublePrec<single>(sum),
        DoublePrec<single>(correct));
  }

  constexpr DoublePrec<DoublePrec<single>> operator+(
      DoublePrec rhs) const {
    DoublePrec<DoublePrec<single>> sum;
    
    return sum;
  }

  constexpr bool operator==(DoublePrec<single> rhs) const {
    return (greater == rhs.greater) &&
           (lesser == rhs.lesser);
  }

  constexpr bool operator>(DoublePrec<single> rhs) const {
    return (greater > rhs.greater) ||
           ((greater == rhs.greater) &&
            (lesser > rhs.greater));
  }

  constexpr explicit operator single() const {
    return greater;
  }
  constexpr operator DoublePrec<DoublePrec<single>>()
      const {
    DoublePrec<DoublePrec<single>> ret();
  }

 private:
  single greater;
  single lesser;
};
};
#endif
