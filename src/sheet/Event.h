#ifndef SHEET_EVENT_H
#define SHEET_EVENT_H

#include <fm/common.hpp>
#include <fm/units.hpp>
#include <vector>

namespace sheet {

	struct PitchDef {
		typedef int Pitch;
		typedef int Octave;
		enum {
			NoPitch = -1,
			DefaultOctave = 0,
		};
		Pitch pitch = NoPitch;
		Octave octave = DefaultOctave;
	};

	struct Event {
		enum {
			NoDuration = 0,
		};
		enum Type { 
			Unknown,
			Rest,
			Degree, 
			Absolute, 
			EOB, // End of Bar aka. Bar Line
			Meta
		};
		typedef fm::Ticks Duration;
		PitchDef pitch;
		Type type = Unknown;
		Duration duration = NoDuration;
		fm::String metaCommand;
		fm::String metaArgs;
	};
}

#endif