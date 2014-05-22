#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
using namespace std;

int main(int argc, char* argv[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	const size_t n = 23129083;
	vector<array<float, 12>> db;
	db.reserve(n);
	string line;
	for (ifstream ifs(argv[1]); getline(ifs, line);)
	{
		array<float, 12> r;
		for (size_t i = 0, b = 9, e; true; b = e + 1)
		{
			e = line.find(',', b + 6);
			if (e == string::npos)
			{
				r[i] = stof(line.substr(b));
				break;
			}
			r[i++] = stof(line.substr(b, e - b));
		}
		db.push_back(r);
	}
	cout << "score" << endl;
	for (size_t k = 0; k < 100; ++k)
	{
		const array<float, 12> q = db[k];
		float m = 0;
		for (const auto& r : db)
		{
			float s = 0;
			for (size_t i = 0; i < 12; ++i)
			{
				s += fabs(q[i] - r[i]);
			}
			if (m < s) m = s;
		}
		cout << m << endl;
	}
}
