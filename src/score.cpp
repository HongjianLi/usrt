#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <immintrin.h>
#include "aligned_allocator.hpp"
using namespace std;

vector<double, aligned_allocator<double, sizeof(__m256d)>> parse(const string& line)
{
	vector<double, aligned_allocator<double, sizeof(__m256d)>> r;
	if (line.size())
	{
		r.reserve(12);
		for (size_t b = 0, e; true; b = e + 1)
		{
			if ((e = line.find(',', b + 6)) == string::npos)
			{
				r.push_back(stod(line.substr(b)));
				break;
			}
			r.push_back(stod(line.substr(b, e - b)));
		}
	}
	return r;
}

int main(int argc, char* argv[])
{
	const auto m256s = _mm256_set1_pd(-0. ); // -0.  = 1 << 63

	// Read the feature file.
	string line;
	vector<vector<double, aligned_allocator<double, sizeof(__m256d)>>> features;
	features.reserve(23129083);
	for (ifstream ifs(argv[1]); getline(ifs, line); features.push_back(parse(line)));
	const size_t n = features.size();

	// Read the header file.
	vector<string> headers;
	headers.reserve(n);
	for (ifstream ifs(argv[2]); getline(ifs, line); headers.push_back(move(line)));

	// Search the features for records similar to each of the queries.
	vector<double> scores(n);
	vector<size_t> scase(n);
	array<double, 4> a;
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (getline(cin, line))
	{
		const auto& q = parse(line);
		const size_t qn = q.size();
		const size_t qt = qn >> 2 << 2;
		const double qv = 1.0 / qn;
		for (size_t k = 0; k < n; ++k)
		{
			const auto& l = features[k];
			double s = 0;
			if (l.size() == qn)
			{
				size_t i = 0;
				for (; i < qt; i += 4)
				{
					const auto m256a = _mm256_andnot_pd(m256s, _mm256_sub_pd(_mm256_load_pd(&q[i]), _mm256_load_pd(&l[i])));
					_mm256_stream_pd(a.data(), _mm256_hadd_pd(m256a, m256a));
					s += a[0] + a[2];
				}
				for (; i < qn; ++i)
				{
					s += fabs(q[i] - l[i]);
				}
				s = 1 / (1 + s * qv);
			}
			scores[k] = s;
		}
		iota(scase.begin(), scase.end(), 0);
		sort(scase.begin(), scase.end(), [&scores](const size_t val1, const size_t val2)
		{
			return scores[val1] > scores[val2];
		});
		for (const size_t c : scase)
		{
			cout << c << '\t' << headers[c] << '\t' << scores[c] << endl;
		}
	}
}
