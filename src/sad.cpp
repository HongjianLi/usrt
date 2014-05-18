#include <iostream>
#include <array>
#include <immintrin.h>
using namespace std;

int main(int argc, char* argv[])
{
	const size_t n = 12;
	const array<double, n> dq = { 4.44, 2.98, 1.04, 4.55, 4.70, 0.23, 8.30, 16.69, -22.97, 7.37, 15.64, 0.51 };
	const array< float, n> sq = { 4.44, 2.98, 1.04, 4.55, 4.70, 0.23, 8.30, 16.69, -22.97, 7.37, 15.64, 0.51 };
	const array<double, n> dl = { 4.39, 3.11, 1.36, 4.50, 4.44, 0.09, 8.34, 16.78, -23.20, 7.15, 16.52, 0.13 };
	const array< float, n> sl = { 4.39, 3.11, 1.36, 4.50, 4.44, 0.09, 8.34, 16.78, -23.20, 7.15, 16.52, 0.13 };
	//                          { 0.05, 0.13, 0.32, 0.05, 0.26, 0.14, 0.04,  0.09,   0.23, 0.22,  0.88, 0.38 }, sum = 2.79, 1 / (1 + sum / n) = 0.811359
	const auto m256ds = _mm256_set1_pd(-0. ); // -0.  = 1 << 63
	const auto m256ss = _mm256_set1_ps(-0.f); // -0.f = 1 << 31
	const __m256i m256im = _mm256_set_epi32(0, 0, 0, 0, 1 << 31, 1 << 31, 1 << 31, 1 << 31);

	// __m256d, C++ style
/*	double sum = 0;
	array<double, 4> da;
	for (size_t k = 0; k < 12; k += 4)
	{
		auto m256dq = _mm256_load_pd(&dq[k]);
		auto m256dl = _mm256_load_pd(&dl[k]);
		auto m256db = _mm256_sub_pd(m256dq, m256dl);
		auto m256da = _mm256_andnot_pd(m256ds, m256db);
		_mm256_stream_pd(da.data(), m256da);
		for (size_t i = 0; i < 4; ++i)
		{
			sum += da[i];
		}
	}
	cout << sum << endl;*/

	// __m256, C++ style
	float sum = 0;
	array<float, 8> sa;
	auto m256sq = _mm256_load_ps(&sq[0]);
	auto m256sl = _mm256_load_ps(&sl[0]);
	auto m256sb = _mm256_sub_ps(m256sq, m256sl);
	auto m256sa = _mm256_andnot_ps(m256ss, m256sb);
	_mm256_store_ps(sa.data(), m256sa);
	for (size_t i = 0; i < 8; ++i)
	{
		sum += sa[i];
	}
	m256sq = _mm256_maskload_ps(&sq[8], m256im);
	m256sl = _mm256_maskload_ps(&sl[8], m256im);
	m256sb = _mm256_sub_ps(m256sq, m256sl);
	m256sa = _mm256_andnot_ps(m256ss, m256sb);
	_mm256_maskstore_ps(sa.data(), m256im, m256sa);
	for (size_t i = 0; i < 4; ++i)
	{
		sum += sa[i];
	}
	cout << sum << endl;

	// __m256d, C style
/*	auto *m256dq = (__m256d*)dq.data();
	auto *m256dl = (__m256d*)dl.data();
	double sum = 0;
	for (size_t k = 0; k < 3; ++k)
	{
		auto m256db = _mm256_sub_pd(m256dq[k], m256dl[k]);
		auto m256da = _mm256_andnot_pd(m256ds, m256db);
		double* m256dp = (double*)&m256da;
		for (size_t i = 0; i < 4; ++i)
		{
			sum += m256dp[i];
		}
	}
	cout << sum << endl;*/

	// __m512, C++ style
/*	auto m512sq = _mm512_load_ps(&sq[0]);
	auto m512sl = _mm512_load_ps(&sl[0]);
	auto m512sb = _mm512_sub_ps(m512sq, m512sl);
	auto m512sa = _mm512_abs_ps(m512sb);
	auto sum = _mm512_reduce_add_ps(m512sa);
	cout << sum << endl;*/
}