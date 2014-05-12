#include <iostream>
#include <iomanip>
#include "ligand.hpp"
#include "moment.hpp"

int main(int argc, char* argv[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (true)
	{
		ligand lig(cin);
		if (lig.atoms.empty()) break;
		bool output = false;
		for (const auto& f : lig.frames)
		{
			// Use rotorY as the only reference point.
			const auto& r = lig.atoms[f.rotorYidx].coord;

			// Find the number of atoms of the current frame.
			const auto n = f.childYidx - f.rotorYidx;
			const auto v = 1.0 / n;

			// Skip frames that have only one atom.
			if (n < 2) continue;

			// Compute the distances to the reference point and their moments.
			vector<double> dists(n);
			for (size_t i = 0; i < n; ++i)
			{
				const auto& a = lig.atoms[f.rotorYidx + i].coord;
				dists[i] = distance(a, r);
			}
			const auto m = moments(dists, n, v);
			if (output) cout << ',';
			cout << m[0] << ',' << m[1] << ',' << m[2];
			output = true;
		}
		cout << endl;
	}
}
