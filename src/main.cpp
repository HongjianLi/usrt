#include <iostream>
#include <iomanip>
#include <cmath>
#include "ligand.hpp"

int main(int argc, char* argv[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(3);
	while (true)
	{
		ligand lig(cin);
		if (lig.atoms.empty()) break;
		bool output = false;
		for (const auto& f : lig.frames)
		{
			const auto& r = lig.atoms[f.rotorYidx].coord; // Reference atom.
			const auto n = f.childYidx - f.rotorYidx; // Number of atoms of the current frame.
			if (n < 2) continue; // No way to compute 2nd and 3rd moments given only one sample.
			vector<double> dists(n);
			for (size_t i = 0; i < n; ++i)
			{
				const auto& a = lig.atoms[f.rotorYidx + i].coord;
				const auto d0 = a[0] - r[0];
				const auto d1 = a[1] - r[1];
				const auto d2 = a[2] - r[2];
				dists[i] = sqrt(d0*d0 + d1*d1 + d2*d2);
			}
			array<double, 3> m{};
			for (size_t i = 0; i < n; ++i)
			{
				const auto d = dists[i];
				m[0] += d;
			}
			m[0] /= n;
			for (size_t i = 0; i < n; ++i)
			{
				const auto d = dists[i] - m[0];
				m[1] += d * d;
			}
			m[1] = sqrt(m[1] / (n - 1));
			for (size_t i = 0; i < n; ++i)
			{
				const auto d = dists[i] - m[0];
				m[2] += d * d * d;
			}
			m[2] = cbrt(m[2] / (n - 1)) / m[1];
			if (output) cout << ',';
			cout << m[0] << ',' << m[1] << ',' << m[2];
			output = true;
		}
		cout << endl;
	}
}
