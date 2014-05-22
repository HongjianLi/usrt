#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <numeric>
#include <algorithm>
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
		for (size_t j = 0; j < n; ++j)
		{
			const auto& r = features[j];
			double s = 0;
			if (r.size() == qn)
			{
				for (size_t i = 0; i < qn; ++i)
				{
					s += fabs(q[i] - r[i]);
				}
				s = 1 / (1 + s * qv);
			}
			scores[j] = s;
		}
		vector<size_t> scase(n);
		iota(scase.begin(), scase.end(), 0);
		sort(scase.begin(), scase.end(), [&scores](size_t val1, size_t val2)
		{
			return scores[val1] > scores[val2];
		});
		for (size_t j = 0; j < n; ++j)
		{
			cout << j << '\t' << scase[j] << '\t' << headers[scase[j]] << '\t' << scores[scase[j]] << endl;
		}
	}
}
