
#include "cdtprec.hpp"
#include <gtest/gtest.h>

#include <iostream>

using spfloat = CDTPrec::CPrec<float>;
using dpfloat = CDTPrec::CPrec<double>;
using lpfloat = CDTPrec::CPrec<long double>;

TEST(TestCPrec, Precision) {
  EXPECT_EQ(spfloat::getPrec(), 24);
  EXPECT_EQ(dpfloat::getPrec(), 53);
  EXPECT_EQ(lpfloat::getPrec(), 65);
}

TEST(TestCPrec, Initialize) {
  spfloat spDef;
  EXPECT_EQ(spfloat(0.0), 0.0);
  EXPECT_NE(spDef, NAN);
  EXPECT_NE(spDef, 0.0);
  EXPECT_NE(spDef, 1.0);
  for(float val = 0.0; val != (float)(val + 1.0f); val += 1.0f) {
    EXPECT_EQ(spfloat(val), val);
    EXPECT_EQ(dpfloat(val), val);
    EXPECT_EQ(lpfloat(val), val);
  }
}
