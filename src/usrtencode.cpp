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
			// Find the number of atoms of the current frame.
			const auto n = f.childYidx - f.rotorYidx + f.branches.size();

			// Initialize moment values.
			array<double, 3> m{};

			// Skip frames that have only one atom.
			if (n >= 2)
			{

			// Use rotorY as the only reference point.
			const auto& r = lig.atoms[f.rotorYidx].coord;

			// Compute the distances to the reference point and their moments.
			const auto v = 1.0 / n;
			vector<double> dists(n);
			size_t o = 0;
			for (size_t i = f.rotorYidx; i < f.childYidx; ++i)
			{
				dists[o++] = dist(r, lig.atoms[i].coord);
			}
			for (const auto b : f.branches)
			{
				dists[o++] = dist(r, lig.atoms[lig.frames[b].rotorYidx].coord);
			}
			m = moments(dists, n, v);
			}
			if (output) cout << ',';
			cout << m[0] << ',' << m[1] << ',' << m[2];
			output = true;
		}
		cout << endl;
	}
}
