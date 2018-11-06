#include "AContext.h"
#include <exception>
#include <exception>

namespace sheet {
	namespace compiler {
		
		using namespace fm;
		const Ticks AContext::DefaultDuration = 1.0_N4;
		const Ticks AContext::DefaultBarLength = 4 * 1.0_N4;

		AContext::TrackId AContext::track() const 
		{ 
			if (trackId_ == INVALID_TRACK_ID) {
				throw std::runtime_error("no track set");
			}
			return trackId_; 
		}
		AContext::VoiceId AContext::voice() const 
		{ 
			if (voiceId_ == INVALID_VOICE_ID) {
				throw std::runtime_error("no voice set");
			}
			return voiceId_; 
		}

		void AContext::setTrack(TrackId trackId)
		{
			this->trackId_ = trackId;
		}

		void AContext::setVoice(VoiceId voice)
		{
			this->voiceId_ = voice;
		}
	
		AContext::TrackId AContext::createTrack()
		{
			auto tid = createTrackImpl();
			return tid;
		}
		AContext::VoiceId AContext::createVoice()
		{
			VoiceId vid = createVoiceImpl();
			auto meta = createVoiceMetaData();
			voiceMetaDataMap_[vid] = meta;
			return vid;
		}

		AContext::VoiceMetaDataPtr AContext::voiceMetaData(VoiceId voiceid) const
		{
			auto it = voiceMetaDataMap_.find(voiceid);
			if (it == voiceMetaDataMap_.end()) {
				throw std::runtime_error("no meta data found for voiceid: " + std::to_string(voiceid));
			}
			return it->second;
		}
		
		void AContext::addEvent(const PitchDef &pitch, fm::Ticks duration)
		{
			using namespace fm;
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			addEvent(pitch, meta->position, meta->duration);
			
		}

		void AContext::seek(fm::Ticks duration)
		{
			auto meta = voiceMetaData(voice());
			if (duration > 0) {
				meta->duration = duration;
			}
			meta->position += meta->duration;
			meta->barPosition += meta->duration;
		}

		void AContext::newBar()
		{
			auto meta = voiceMetaData(voice());
			if (meta->barPosition != meta->barLength) {
				throw std::runtime_error("bar check error at voice " + std::to_string(voice()) + " bar: " + std::to_string(meta->position / meta->barLength));
			}
			meta->barPosition = 0;
		}

		void AContext::rest(fm::Ticks duration)
		{
			seek(duration);
		}
	}
}