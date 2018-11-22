#ifndef MIDI_PROVIDER
#define MIDI_PROVIDER

#include "AMidiProvider.h"
#include <list>
#include <fm/midi.hpp>
#include <map>

namespace fmapp {

	class MidiProvider : public AMidiProvider {
	public:
		enum { NO_TICK = INT_MAX };
		typedef std::list<fm::midi::Event> Events;
		void getEvents(fm::Ticks at, Events &out);
		virtual ~MidiProvider() = default;
		void midi(fm::midi::MidiPtr midi);
		fm::midi::MidiPtr midi() const;
	private:
		void copyEvents(Events &events, fm::Ticks at);
		fm::midi::MidiPtr midi_ = nullptr;
		typedef fm::midi::EventContainer::ConstIterator EventIt;
		typedef std::unordered_map<fm::midi::TrackPtr, EventIt> TrackEventIts;
		TrackEventIts trackEventIts_;
	};

}

#endif