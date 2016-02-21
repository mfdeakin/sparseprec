
#include "cdtprec.hpp"
#include <gtest/gtest.h>

#include <iostream>

using spfloat = CDTPrec::CPrec<float>;
using dpfloat = CDTPrec::CPrec<double>;
using lpfloat = CDTPrec::CPrec<long double>;

using spdouble = CDTPrec::DoublePrec<spfloat>;
using dpdouble = CDTPrec::DoublePrec<dpfloat>;
using lpdouble = CDTPrec::DoublePrec<lpfloat>;

TEST(TestCPrec, Precision) {
  EXPECT_EQ(spfloat::getPrec(), 24);
  EXPECT_EQ(dpfloat::getPrec(), 53);
  EXPECT_EQ(lpfloat::getPrec(), 65);

	EXPECT_EQ(spdouble::getPrec(), 48);
	EXPECT_EQ(dpdouble::getPrec(), 106);
	EXPECT_EQ(lpdouble::getPrec(), 130);
}

TEST(TestCPrec, CompareFloat) {
  spfloat spDef;
  EXPECT_EQ(spfloat(0.0), spfloat(0.0));
  EXPECT_NE(spDef, spfloat(NAN));
  EXPECT_NE(spDef, spfloat(0.0));
  EXPECT_NE(spDef, spfloat(1.0));
  for(float val = 0.0; val != (float)(val + 1.0f);
      val += 8.0f) {
    EXPECT_EQ((float)spfloat(val), val);
    EXPECT_EQ((double)dpfloat(val), val);
    EXPECT_EQ((long double)lpfloat(val), val);

    EXPECT_EQ(spfloat(val), spfloat(val));

    EXPECT_EQ(dpfloat(val), spfloat(val));
    EXPECT_EQ(dpfloat(val), dpfloat(val));
    EXPECT_EQ(dpfloat(val), lpfloat(val));

    EXPECT_EQ(lpfloat(val), spfloat(val));
    EXPECT_EQ(lpfloat(val), dpfloat(val));
    EXPECT_EQ(lpfloat(val), lpfloat(val));

    EXPECT_EQ((float)spfloat(val), val);

    EXPECT_EQ((spdouble)spfloat(val), spdouble(val, 0.0));

    EXPECT_GT(spfloat(val), spfloat(val - 1.0f));
    EXPECT_GT(dpfloat(val), dpfloat(val - 1.0f));
    EXPECT_GT(lpfloat(val), lpfloat(val - 1.0f));

    EXPECT_GE(spfloat(val), spfloat(val - 1.0f));
    EXPECT_GE(dpfloat(val), dpfloat(val - 1.0f));
    EXPECT_GE(lpfloat(val), lpfloat(val - 1.0f));

    EXPECT_LT(spfloat(val), spfloat(val + 1.0f));
    EXPECT_LT(dpfloat(val), dpfloat(val + 1.0f));
    EXPECT_LT(lpfloat(val), lpfloat(val + 1.0f));

    EXPECT_LE(spfloat(val), spfloat(val + 1.0f));
    EXPECT_LE(dpfloat(val), dpfloat(val + 1.0f));
    EXPECT_LE(lpfloat(val), lpfloat(val + 1.0f));
  }
}

TEST(TestCPrec, Addition) {
  for(float lhs = 0.0; lhs != (float)lhs + 1.0f;
      lhs += 4096.0f) {
    for(float rhs = 0.0; rhs != (float)rhs + 1.0f;
        rhs += 4096.0f) {
      spdouble sum(spfloat(lhs) + spfloat(rhs));
      EXPECT_EQ(sum.higherOrder(), spfloat(lhs + rhs));
			float sRounded = lhs + rhs;
			float truncated = sRounded - ((lhs > rhs) ? lhs : rhs);
			float err = ((lhs > rhs) ? rhs : lhs) - truncated;
			EXPECT_EQ(sum.lowerOrder(), spfloat(err));
    }
  }
}
