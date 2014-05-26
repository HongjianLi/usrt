#include <cmath>
#include "moment.hpp"

double dist2(const array<double, 3>& p0, const array<double, 3>& p1)
{
	const auto d0 = p0[0] - p1[0];
	const auto d1 = p0[1] - p1[1];
	const auto d2 = p0[2] - p1[2];
	return d0 * d0 + d1 * d1 + d2 * d2;
}

array<double, 3> moments(const vector<double>& dists, const size_t n, const double v)
{
	array<double, 3> m{};
	for (size_t i = 0; i < n; ++i)
	{
		const auto d = dists[i];
		m[0] += d;
	}
	m[0] *= v;
	for (size_t i = 0; i < n; ++i)
	{
		const auto d = dists[i] - m[0];
		m[1] += d * d;
	}
	m[1] = sqrt(m[1] * v);
	for (size_t i = 0; i < n; ++i)
	{
		const auto d = dists[i] - m[0];
		m[2] += d * d * d;
	}
	m[2] = cbrt(m[2] * v);
	return m;
}
