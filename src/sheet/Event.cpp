#include "Event.h"
#include <exception>
#include <fm/common.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <locale>

namespace sheet {

	namespace {
		const std::unordered_map<wchar_t, int> _name2pitch = {
			 { FM_CHAR('c'), fm::notes::C }
			,{ FM_CHAR('d'), fm::notes::D }
			,{ FM_CHAR('e'), fm::notes::E }
			,{ FM_CHAR('f'), fm::notes::F }
			,{ FM_CHAR('g'), fm::notes::G }
			,{ FM_CHAR('a'), fm::notes::A }
			,{ FM_CHAR('b'), fm::notes::B }
		};
	}

	/*
		failed to get a proper parser to working which can parse
		a chordname and the options in one step. The problem was that it is needed to
		have the two parser elements in one lexeme. But this seems to consume only one property
		of a struct. May be there is a solution with sematic actions etc. But in sake of make process,
		I decided to parse on the fly.
	*/
	ChordEvent::ChordElements ChordEvent::chordElements() const
	{
		PitchDef::Pitch pitch = 0;
		auto nameLower = chordName;
		if (nameLower.length() == 0) {
			throw std::runtime_error("empty chord");
		}
		boost::algorithm::to_lower(nameLower);
		fm::String::const_iterator it = nameLower.begin();
		auto pitchIt = _name2pitch.find(*it);
		if (pitchIt == _name2pitch.end()) {
			throw std::runtime_error("ivalid chord: " + fm::to_string(chordName));
		}
		pitch = pitchIt->second;
		++it;
		// check for is or es
		if (nameLower.length() >= 3) {
			if (*(it) == FM_CHAR('i') && *(it + 1) == FM_CHAR('s')) {
				pitch += 1;
				it += 2;
			}
			else if (*(it) == FM_CHAR('e') && *(it + 1) == FM_CHAR('s')) {
				pitch -= 1;
				it += 2;
			}
		}
		auto idxOptionsStart = it - nameLower.begin();
		return std::make_tuple(pitch, Options(chordName.begin() + idxOptionsStart,  chordName.end()));
	}

	fm::String ChordEvent::chordDefName() const 
	{
		std::locale loc;
		auto elements = chordElements();
		fm::String::const_iterator it = chordName.begin();
		if (std::isupper(*it, loc)) {
			return FM_STRING("X") + std::get<1>(elements);
		}
		else {
			return FM_STRING("x") + std::get<1>(elements);
		}
	}
}