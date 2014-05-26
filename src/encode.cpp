#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include "ligand.hpp"
#include "moment.hpp"

int main(int argc, char* argv[])
{
	const bool usr = argc > 1;
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (true)
	{
		ligand lig(cin);
		if (lig.atoms.empty()) break;
		bool output = false;
		if (usr)
		{
			const auto n = lig.atoms.size();
			const auto v = 1.0 / n;

			// Find the reference point ctd.
			array<double, 3> ctd{};
			for (size_t i = 0; i < 3; ++i)
			{
				for (const auto& a : lig.atoms)
				{
					ctd[i] += a.coord[i];
				}
				ctd[i] *= v;
			}

			// Find the reference points cst and fct.
			array<double, 3> cst{};
			array<double, 3> fct{};
			double cst_dist = numeric_limits<double>::max();
			double fct_dist = numeric_limits<double>::lowest();
			for (const auto& a : lig.atoms)
			{
				const auto this_dist = dist2(a.coord, ctd);
				if (this_dist < cst_dist)
				{
					cst = a.coord;
					cst_dist = this_dist;
				}
				if (this_dist > fct_dist)
				{
					fct = a.coord;
					fct_dist = this_dist;
				}
			}

			// Find the reference point ftf.
			array<double, 3> ftf{};
			double ftf_dist = numeric_limits<double>::lowest();
			for (const auto& a : lig.atoms)
			{
				const auto this_dist = dist2(a.coord, fct);
				if (this_dist > ftf_dist)
				{
					ftf = a.coord;
					ftf_dist = this_dist;
				}
			}

			// Compute the distances to the reference points and their moments.
			for (const auto& rpt : { ctd, cst, fct, ftf })
			{
				vector<double> dists(n);
				for (size_t i = 0; i < n; ++i)
				{
					dists[i] = sqrt(dist2(lig.atoms[i].coord, rpt));
				}
				const auto m = moments(dists, n, v);
				if (output) cout << ',';
				cout << m[0] << ',' << m[1] << ',' << m[2];
				output = true;
			}
		}
		else
		{
			for (const auto& f : lig.frames)
			{
				// Find the number of atoms of the current frame.
				const auto n = f.childYidx - f.rotorYidx + f.branches.size();

				// Use rotorY as the only reference point.
				const auto& r = lig.atoms[f.rotorYidx].coord;

				// Compute the distances to the reference point and their moments.
				vector<double> dists(n);
				size_t o = 0;
				for (size_t i = f.rotorYidx; i < f.childYidx; ++i)
				{
					dists[o++] = sqrt(dist2(r, lig.atoms[i].coord));
				}
				for (const auto b : f.branches)
				{
					dists[o++] = sqrt(dist2(r, lig.atoms[lig.frames[b].rotorYidx].coord));
				}
				const auto m = moments(dists, n, 1.0 / n);

				if (output) cout << ',';
				cout << m[0] << ',' << m[1] << ',' << m[2];
				output = true;
			}
		}
		cout << endl;
	}
}
