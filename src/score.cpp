#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <immintrin.h>
using namespace std;

vector<double> parse(const string& line)
{
	vector<double> r;
	if (line.size())
	{
		r.reserve(12);
		for (size_t b = 0, e; true; b = e + 1)
		{
			if ((e = line.find(',', b + 6)) == string::npos)
			{
				r.push_back(stof(line.substr(b)));
				break;
			}
			r.push_back(stof(line.substr(b, e - b)));
		}
	}
	return r;
}

int main(int argc, char* argv[])
{
	const auto m256s = _mm256_set1_pd(-0. ); // -0.  = 1 << 63

	// Read the feature file.
	string line;
	vector<vector<double>> features;
	features.reserve(23129083);
	for (ifstream ifs(argv[1]); getline(ifs, line); features.push_back(parse(line)));
	const size_t n = features.size();

	// Read the header file.
	vector<string> headers;
	headers.reserve(n);
	for (ifstream ifs(argv[2]); getline(ifs, line); headers.push_back(move(line)));

	// Search a query.
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (getline(cin, line))
	{
		const auto& q = parse(line);
		const size_t qn = q.size();
		const double qv = 1.0 / qn;
		vector<double> scores(n);
		for (size_t k = 0; k < n; ++k)
		{
			const auto& l = features[k];
			double s = 0;
			if (l.size() == qn)
			{
				const size_t m = qn / 4;
				const size_t t = 4 * m;
				size_t i = 0;
				array<double, 4> a;
				for (; i < t; i += 4)
				{
					_mm256_stream_pd(a.data(), _mm256_andnot_pd(m256s, _mm256_sub_pd(_mm256_load_pd(&q[i]), _mm256_load_pd(&l[i]))));
					s += a[0] + a[1] + a[2] + a[3];
				}
				for (; i < qn; ++i)
				{
					s += fabs(q[i] - l[i]);
				}
				s = 1 / (1 + s * qv);
			}
			scores[k] = s;
		}
		vector<size_t> scase(n);
		iota(scase.begin(), scase.end(), 0);
		sort(scase.begin(), scase.end(), [&scores](size_t val1, size_t val2)
		{
			return scores[val1] > scores[val2];
		});
		for (size_t k = 0; k < n; ++k)
		{
			cout << k << '\t' << scase[k] << '\t' << headers[scase[k]] << '\t' << scores[scase[k]] << endl;
		}
	}
}
