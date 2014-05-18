#pragma once
#ifndef USRT_LIGAND_HPP
#define USRT_LIGAND_HPP

#include <vector>
#include <istream>
#include "atom.hpp"

//! Represents a ROOT or a BRANCH in PDBQT structure.
class frame
{
public:
	size_t parent; //!< Frame array index pointing to the parent of current frame. For ROOT frame, this field is not used.
	size_t rotorYidx; //!< Index pointing to the current frame atom which forms a rotatable bond with the rotorX atom of parent frame.
	size_t childYidx; //!< The exclusive ending index to the heavy atoms of the current frame.
	vector<size_t> branches; //!< Indexes to child branches.

	//! Constructs an active frame, and relates it to its parent frame.
	explicit frame(const size_t parent, const size_t rotorYidx) : parent(parent), rotorYidx(rotorYidx)
	{
		branches.reserve(4);
	}
};

class ligand
{
public:
	vector<atom> atoms;
	vector<frame> frames;

	//! Load a ligand from an ifstream.
	explicit ligand(istream& is);
};

#endif
