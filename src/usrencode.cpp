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
		double cst_dist =  9999;
		double fct_dist = -9999;
		for (const auto& a : lig.atoms)
		{
			const auto this_dist = dist(a.coord, ctd);
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
		double ftf_dist = -9999;
		for (const auto& a : lig.atoms)
		{
			const auto this_dist = dist(a.coord, fct);
			if (this_dist > ftf_dist)
			{
				ftf = a.coord;
				ftf_dist = this_dist;
			}
		}

		// Aggregate the reference points for convenient processing.
		array<array<double, 3>, 4> rps = { ctd, cst, fct, ftf };

		// Compute the distances to the reference points and their moments.
		bool output = false;
		for (const auto& rpt : rps)
		{
			vector<double> dists(n);
			for (size_t i = 0; i < n; ++i)
			{
				dists[i] = dist(lig.atoms[i].coord, rpt);
			}
			const auto m = moments(dists, n, v);
			if (output) cout << ',';
			cout << m[0] << ',' << m[1] << ',' << m[2];
			output = true;
		}
		cout << endl;
	}
}
