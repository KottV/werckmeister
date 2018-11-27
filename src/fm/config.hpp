#ifndef FM_CONFIG_HPP
#define FM_CONFIG_HPP

#include "units.hpp"

#define SHEET_VERSION "26935eb"
#define CHORD_DEF_EXTENSION  ".chords"
#define STYLE_DEF_EXTENSION ".style"
#define PITCHMAP_DEF_EXTENSION ".pitchmap"

#define FM_CHARSET "ISO-8859-1"

namespace fm {
    const Ticks PPQ = 500;
	const int NotesPerOctave = 12;
}

#endif
