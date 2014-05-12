#pragma once
#ifndef USRT_MOMENT_HPP
#define USRT_MOMENT_HPP

#include <array>
#include <vector>
using namespace std;

double dist(const array<double, 3>& p0, const array<double, 3>& p1);
array<double, 3> moments(const vector<double>& dists, const size_t n, const double v);

#endif
