#ifndef MIDI_PROVIDER
#define MIDI_PROVIDER

#include <list>
#include <fm/midi.hpp>
#include <map>
#include <climits>
#include <functional>
#include <fm/config.hpp>
#include <fm/units.hpp>

namespace fmapp {

	class MidiProvider  {
	public:
		enum { NO_TICK = INT_MAX, INVALID_TRACKID = 0 };
		typedef long TrackId;
		typedef long double Millis;
		struct Event {
			TrackId trackId;
			fm::midi::Event event;
		};
		typedef std::unordered_map<TrackId, int> TrackOffsets;
		typedef std::list<Event> Events;
		typedef std::function<bool(fm::Ticks, const Event &)> IterateFunction;
		void getEvents(Millis at, Events &out, const TrackOffsets &offsets);
		void iterate(const IterateFunction &f);
		virtual ~MidiProvider() = default;
		void midi(fm::midi::MidiPtr midi);
		fm::midi::MidiPtr midi() const;
		void reset();
		inline fm::Ticks millisToTicks(Millis millis) const {
			return static_cast<fm::Ticks>( millis / (60 / (bpm() * fm::PPQ) * 1000) );
		}
		inline Millis ticksToMillis(fm::Ticks ticks) const {
			return 0; // TODO;
		}
		inline fm::BPM bpm() const { return std::max(bpm_, 1.0); }
		void bpm(fm::BPM bpm) { bpm_ = bpm; }
	protected:
		void seek(Millis millis, const TrackOffsets &offsets);
	private:
		fm::midi::MidiPtr midi_ = nullptr;
		typedef fm::midi::EventContainer::ConstIterator EventIt;
		typedef std::unordered_map<fm::midi::TrackPtr, EventIt> TrackEventIts;
		TrackEventIts trackEventIts_;
		EventIt* getEventIt(fm::midi::TrackPtr trackPtr);
		fm::BPM bpm_ = 120.0;
	};

}

#endif