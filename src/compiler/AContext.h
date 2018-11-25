#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "sheet/Event.h"
#include <memory>
#include <unordered_map>
#include <fm/units.hpp>
#include <fm/literals.hpp>
#include <map>
#include <unordered_map>
#include "sheet/ChordDef.h"
#include "sheet/StyleDefServer.h"
#include "sheet/VoicingStrategy.h"
#include <fm/common.hpp>
#include "metaCommands.h"
#include <list>

namespace sheet {
    namespace compiler {

        class AContext {
        public:
			template<typename TArg>
			TArg getArgument(const Event &metaEvent, int idx, TArg *defaultValue = nullptr);
			static const fm::Ticks DefaultDuration;
			static const fm::Ticks DefaultBarLength;
			enum { INVALID_TRACK_ID = -1, INVALID_VOICE_ID = -1 };
			typedef int Id;
			typedef Id TrackId;
			typedef Id VoiceId;
			typedef IStyleDefServer* IStyleDefServerPtr;
			typedef std::list<std::string> Warnings;
			struct VoiceMetaData {
				typedef std::map<PitchDef, fm::Ticks> WaitForTieBuffer;
				fm::Ticks position = 0;
				fm::Ticks duration = DefaultDuration;
				fm::Ticks barLength = DefaultBarLength;
				fm::Ticks barPosition = 0;
				WaitForTieBuffer waitForTieBuffer;
				/*
					used for continue style track rendering after chord change
				*/
				int idxLastWrittenEvent = -1;
				/*
					from a aborted style rendering
				*/
				fm::Ticks remainingTime = 0;

				virtual ~VoiceMetaData() = default;
				bool pendingTie() const { return !waitForTieBuffer.empty(); }
			};
			typedef std::shared_ptr<VoiceMetaData> VoiceMetaDataPtr;
			typedef std::unordered_map<VoiceId, VoiceMetaDataPtr> VoiceMetaDataMap;
			virtual void setTrack(TrackId trackId);
			virtual void setVoice(VoiceId voice);
			TrackId track() const;
			VoiceId voice() const;
			inline void setTarget(TrackId trackId, VoiceId voiceId)
			{
				setTrack(trackId);
				setVoice(voiceId);
			}
			virtual void addEvent(const PitchDef &pitch, fm::Ticks absolutePosition, fm::Ticks duration) = 0;
			virtual ~AContext() {};
			virtual TrackId createTrack();
			virtual VoiceId createVoice();
			VoiceMetaDataPtr voiceMetaData(VoiceId voiceid) const;
			template<typename TVoiceMeta>
			std::shared_ptr<TVoiceMeta> voiceMetaData(VoiceId voiceid) const 
			{
				return std::dynamic_pointer_cast<TVoiceMeta>(voiceMetaData(voiceid));
			}
			virtual void throwContextException(const std::string &msg);
			virtual void warn(const std::string &msg);
			IStyleDefServerPtr styleDefServer() const;
			void styleDefServer(IStyleDefServerPtr server);
			virtual IStyleDefServer::ConstChordValueType currentChordDef();
			virtual IStyleDefServer::ConstStyleValueType currentStyle();
			virtual VoicingStrategyPtr currentVoicingStrategy();
			virtual const ChordEvent * currentChord() const { return &currentChord_; }
			/////// meta commands
			virtual void setMeta(const Event &metaEvent);
			/////// actual context stuff
			virtual void addEvent(const PitchDef &pitch, fm::Ticks duration, bool tying = false);
			virtual void seek(fm::Ticks duration);
			virtual void newBar();
			virtual void rest(fm::Ticks duration);
			virtual void setChord(const ChordEvent &ev);
			virtual void setStyle(const fm::String &styleName);
			virtual void renderStyle(fm::Ticks duration);
			virtual void addEvent(const Event &ev);
			virtual fm::Ticks barPos() const;
			Warnings warnings;
		protected:
			PitchDef resolvePitch(const PitchDef &pitch) const;
			virtual TrackId createTrackImpl() = 0;
			virtual VoiceId createVoiceImpl() = 0;
			virtual VoiceMetaDataPtr createVoiceMetaData() = 0;
		private:
			typedef std::unordered_map<const void*, Id> PtrIdMap;
			PtrIdMap ptrIdMap_;
			void setTarget(const Track &track, const Voice &voice);
			ChordEvent currentChord_;
			IStyleDefServer::ConstChordValueType currentChordDef_ = nullptr;
			IStyleDefServer::ConstStyleValueType currentStyleDef_ = nullptr;
			VoicingStrategyPtr currentVoicingStrategy_ = nullptr;
			TrackId trackId_ = INVALID_TRACK_ID;
			VoiceId voiceId_ = INVALID_VOICE_ID;
			VoiceMetaDataMap voiceMetaDataMap_;
			IStyleDefServerPtr styleDefServer_ = nullptr;

        };

		template<typename TArg>
		TArg AContext::getArgument(const Event &metaEvent, int idx, TArg *defaultValue) {
			if (idx >= (int)metaEvent.metaArgs.size()) {
				if (defaultValue) {
					return *defaultValue;
				}
				throw std::runtime_error("missing argument for '" + fm::to_string(metaEvent.metaCommand) + "'");
			}
			TArg result;
			fm::StringStream ss;
			ss << metaEvent.metaArgs[idx];
			ss >> result;
			return result;
		}


    }
}

#endif