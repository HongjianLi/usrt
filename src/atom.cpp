#include <algorithm>
#include "atom.hpp"

//! AutoDock4 atom type names.
const array<string, atom::n> atom::ad_strings =
{
	"H" , //  0
	"HD", //  1
	"C" , //  2
	"A" , //  3
	"N" , //  4
	"NA", //  5
	"OA", //  6
	"S" , //  7
	"SA", //  8
	"Se", //  9
	"P" , // 10
	"F" , // 11
	"Cl", // 12
	"Br", // 13
	"I" , // 14
	"Zn", // 15
	"Fe", // 16
	"Mg", // 17
	"Ca", // 18
	"Mn", // 19
	"Cu", // 20
	"Na", // 21
	"K" , // 22
	"Hg", // 23
	"Ni", // 24
	"Co", // 25
	"Cd", // 26
	"As", // 27
	"Sr", // 28
	"U" , // 29
};

atom::atom(const string& line) :
	serial(stoul(line.substr(6, 5))),
	ad(find(ad_strings.cbegin(), ad_strings.cend(), line.substr(77, isspace(line[78]) ? 1 : 2)) - ad_strings.cbegin())
{
	coord[0] = stof(line.substr(30, 8));
	coord[1] = stof(line.substr(38, 8));
	coord[2] = stof(line.substr(46, 8));
}

//! Returns true if the AutoDock4 atom type is not supported.
bool atom::ad_unsupported() const
{
	return ad == n;
}

//! Returns true if the atom is a hydrogen atom.
bool atom::is_hydrogen() const
{
	return ad <= 1;
}
