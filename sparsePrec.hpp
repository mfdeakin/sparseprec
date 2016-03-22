
#ifndef _SPARSEPREC_HPP_
#define _SPARSEPREC_HPP_

#include <cmath>

#include "genericfp.hpp"

namespace SparsePrec {

template <typename fptype>
class DoublePrec;

template <typename fptype>
class TriplePrec;

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

  constexpr single sign() const {
    return std::copysign(1.0, val);
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

  static single fmaRounded(single a, single b, single c) {
    return std::fma(a, b, c);
  }

  static single fmaRounded(CPrec<single> a, CPrec<single> b,
                           CPrec<single> c) {
    return std::fma(a.val, b.val, c.val);
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

  constexpr DoublePrec<CPrec<single>> operator*(
      CPrec<single> rhs) const {
    single high = val * rhs.val;
    single low = fmaRounded(val, rhs.val, -high);
    return DoublePrec<CPrec<single>>(CPrec<single>(high),
                                     CPrec<single>(low));
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

  // Currently assumes everything is positive
  constexpr DoublePrec<DoublePrec<single>> operator+(
      DoublePrec rhs) const {
    if(rhs < *this) {
      // Swap the orders to make the computation easier
      return (rhs + *this);
    }
    // *this <= rhs
    DoublePrec<DoublePrec<single>> sum;
    DoublePrec<single> highest = greater + rhs.greater;
    sum.greater.greater = highest.greater;
    // Now exactly compute
    // highest.lesser + lesser + rhs.lesser
    single *alpha, *beta, *gamma;
    sort3(highest.lesser, lesser, rhs.lesser, &alpha, &beta,
          &gamma);
    // *alpha > *beta > *gamma
    DoublePrec<single> mid1 = *alpha + *beta;
    sum.greater.lesser = mid1.greater;
    sum.lesser = mid1.lesser + *beta;
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

template <typename fptype>
void sort3(const fptype &a, const fptype &b,
           const fptype &c, fptype *&high, fptype *&mid,
           fptype *&low) {
  if(a > b) {
    sort3Helper(a, b, c, high, mid, low);
  } else {
    sort3Helper(b, a, c, high, mid, low);
  }
}

template <typename fptype>
void sort3Helper(const fptype &a, const fptype &b,
                 const fptype &c, fptype *&high,
                 fptype *&mid, fptype *&low) {
  if(b > c) {
    high = &a;
    mid = &b;
    low = &c;
  } else {
    if(a > c) {
      high = &a;
      mid = &c;
    } else {
      high = &c;
      mid = &a;
    }
    low = &b;
  }
}

#endif
