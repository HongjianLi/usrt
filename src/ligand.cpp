#include <fstream>
#include "ligand.hpp"

ligand::ligand(istream& is)
{
	// Initialize necessary variables for constructing a ligand.
	frames.reserve(30); // A ligand typically consists of <= 30 frames.
	frames.emplace_back(0, 0); // ROOT is also treated as a frame. The parent of ROOT frame are dummy.
	atoms.reserve(100); // A ligand typically consists of <= 100 heavy atoms.

	// Initialize helper variables for parsing.
	size_t current = 0; // Index of current frame, initialized to ROOT frame.

	// Parse the ligand line by line.
	for (string line, record; getline(is, line) && (record = line.substr(0, 6)) != "TORSDO";)
	{
		if (record == "ATOM  " || record == "HETATM")
		{
			// Parse the line.
			atom a(line);

			// Skip unsupported atom types or hydrogens.
			if (a.ad_unsupported() || a.is_hydrogen()) continue;

			// Save the heavy atom.
			atoms.push_back(move(a));
		}
		else if (record == "BRANCH")
		{
			// Insert a new frame whose parent is the current frame.
			frames.push_back(frame(current, atoms.size()));

			// Get a pointer to the current frame.
			auto& f = frames[current];

			// Now the current frame is the newly inserted BRANCH frame.
			current = frames.size() - 1;

			// The parent frame has the current frame as one of its branches.
			f.branches.push_back(current);

			// The ending index of atoms of previous frame is the starting index of atoms of current frame.
			frames[current - 1].childYidx = frames[current].rotorYidx;
		}
		else if (record == "ENDBRA")
		{
			// Now the parent of the following frame is the parent of current frame.
			current = frames[current].parent;
		}
	}
	frames.back().childYidx = atoms.size();
}
