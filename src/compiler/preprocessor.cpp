#include "preprocessor.h"
#include <sheet/Track.h>
#include <compiler/metaData.h>
#include "error.hpp"

namespace sheet {
	namespace compiler {

		namespace {
			inline Event::Type resolveRepeatType(const Event &lastNoRepeat, const Event &repeat) 
			{
				if (lastNoRepeat.isRelativeDegree()) {
					return repeat.type == Event::Repeat ? Event::Degree : Event::TiedDegree;
				}
				return repeat.type == Event::Repeat ? Event::Note : Event::TiedNote;
			}
		}

        void Preprocessor::process(Track &track)
        {
            for (auto &voice : track.voices)
			{
				Event lastNoRepeat;
				lastNoRepeat.type = Event::Unknown;
				auto it = voice.events.begin();
				auto end = voice.events.end();
				fm::Ticks lastDuration = VoiceMetaData::DefaultDuration;
				for (; it!=end; ++it)
				{
					auto &ev = *it;
					if (ev.isTimeConsuming()) {
						if (!ev.isRepeat() && ev.type != Event::Rest) {
							lastNoRepeat = ev;
						}	
						if (ev.isRepeat())  {
							if (lastNoRepeat.type == Event::Unknown) {
								FM_THROW(Exception, "no event for shortcut: x");
							}
							ev.pitches = lastNoRepeat.pitches;
							ev.type = resolveRepeatType(lastNoRepeat, ev);
						}
						if (ev.duration == 0) {
							ev.duration = lastDuration;
						} else {
							lastDuration = ev.duration;
						}
					}
				}
			}
        }

    }
}