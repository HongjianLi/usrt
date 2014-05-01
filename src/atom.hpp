#pragma once
#ifndef USRT_ATOM_HPP
#define USRT_ATOM_HPP

#include <string>
#include <array>
using namespace std;

class atom
{
private:
	static const size_t n = 30;
	static const array<string, n> ad_strings;
public:
	size_t serial;
	array<double, 3> coord;
	size_t ad;

	//! Constructs an atom from an ATOM/HETATM line in PDBQT format.
	explicit atom(const string& line);

	//! Returns true if the AutoDock4 atom type is not supported.
	bool ad_unsupported() const;

	//! Returns true if the atom is a hydrogen atom.
	bool is_hydrogen() const;
};

#endif
