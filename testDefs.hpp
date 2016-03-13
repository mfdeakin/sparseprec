
#ifndef _TESTDEFS_HPP_
#define _TESTDEFS_HPP_

#include "sparsePrec.hpp"

#include <gtest/gtest.h>

#include <random>
#include <iostream>

using spfloat = SparsePrec::CPrec<float>;
using dpfloat = SparsePrec::CPrec<double>;
using lpfloat = SparsePrec::CPrec<long double>;

using spdouble = SparsePrec::DoublePrec<spfloat>;
using dpdouble = SparsePrec::DoublePrec<dpfloat>;
using lpdouble = SparsePrec::DoublePrec<lpfloat>;

template <typename fptype>
using uniformDist = std::uniform_real_distribution<fptype>;

static std::random_device randDev;
static std::mt19937_64 rngEngine(randDev());

#endif
