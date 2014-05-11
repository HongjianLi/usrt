#include <iostream>
#include <iomanip>
#include <cmath>
#include "ligand.hpp"

double dist(const array<double, 3>& p0, const array<double, 3>& p1)
{
	const auto d0 = p0[0] - p1[0];
	const auto d1 = p0[1] - p1[1];
	const auto d2 = p0[2] - p1[2];
	return sqrt(d0*d0 + d1*d1 + d2*d2);
}

int main(int argc, char* argv[])
{
	cout.setf(ios::fixed, ios::floatfield);
	cout << setprecision(4);
	while (true)
	{
		ligand lig(cin);
		if (lig.atoms.empty()) break;
		const auto n = lig.atoms.size();

		// Find the reference point ctd.
		array<double, 3> ctd{};
		for (size_t i = 0; i < 3; ++i)
		{
			for (const auto& a : lig.atoms)
			{
				ctd[i] += a.coord[i];
			}
			ctd[i] /= n;
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

		// Compute the distributions and moments.
		bool output = false;
		for (const auto& rpt : rps)
		{
			vector<double> dists(n);
			for (size_t i = 0; i < n; ++i)
			{
				dists[i] = dist(lig.atoms[i].coord, rpt);
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
