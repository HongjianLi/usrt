#include <iostream>
#include <iomanip>
#include "ligand.hpp"
#include "moment.hpp"

int main(int argc, char* argv[])
{
	const bool branch = argc < 2 ? false : stoi(argv[1]);
	const bool zero   = argc < 3 ? false : stoi(argv[2]);
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (true)
	{
		ligand lig(cin);
		if (lig.atoms.empty()) break;
		bool output = false;
		for (const auto& f : lig.frames)
		{
			// Initialize moment values.
			array<double, 3> m{};

			// Find the number of atoms of the current frame.
			const auto n = f.childYidx - f.rotorYidx + (branch ? f.branches.size() : 0);

			// Skip frames that have only one atom.
			if (n < 2)
			{
				if (!zero) continue;
			}
			else
			{
				// Use rotorY as the only reference point.
				const auto& r = lig.atoms[f.rotorYidx].coord;

				// Compute the distances to the reference point and their moments.
				vector<double> dists(n);
				size_t o = 0;
				for (size_t i = f.rotorYidx; i < f.childYidx; ++i)
				{
					dists[o++] = dist(r, lig.atoms[i].coord);
				}
				if (branch)
				{
					for (const auto b : f.branches)
					{
						dists[o++] = dist(r, lig.atoms[lig.frames[b].rotorYidx].coord);
					}
				}
				m = moments(dists, n, 1.0 / n);
			}
			if (output) cout << ',';
			cout << m[0] << ',' << m[1] << ',' << m[2];
			output = true;
		}
		cout << endl;
	}
}
