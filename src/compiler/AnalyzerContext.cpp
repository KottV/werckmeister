#include "AnalyzerContext.h"
#include <compiler/metaCommands.h>

namespace sheet {
    namespace compiler {
        bool AnalyzerContext::isCurrentTrackRelevant() const
        {
            auto trackData = this->trackMetaData(this->track());
            return 
                   trackData->type == SHEET_META__TRACK_META_KEY_TRACK_TYPE_DEFAULT
                || trackData->type == SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP;
        }
        void AnalyzerContext::newBar(const Event &newBarEvent)
        {
            auto voiceMeta = voiceMetaData();
            int barCount = voiceMeta->barCount;
            fm::Ticks barlength = voiceMeta->barPosition; // bar position means the actual written ticks per bar
                                                         // voiceMeta->barLength means the length per definiton
            Base::newBar(newBarEvent);
            if (this->analyzerData == nullptr) {
                return;
            }
            if (!this->isCurrentTrackRelevant()) {
                return;
            }
            BarEvent barEvent(newBarEvent);
            barEvent.position = voiceMeta->position;
            barEvent.barCount = barCount;
            barEvent.barLength = barlength;
            this->analyzerData->barEvents.emplace_back(barEvent);
        }

        void AnalyzerContext::setChord(const Event &chordEvent) 
        {
            auto voiceMeta = voiceMetaData();
            auto position = voiceMeta->position;
            Base::setChord(chordEvent);
            if (!this->isCurrentTrackRelevant()) {
                return;
            }
            if (this->analyzerData == nullptr) {
                return;
            }
            AnalyzerEvent analyzerEvent(chordEvent);
            analyzerEvent.position = position;
            this->analyzerData->analyzerEvents.emplace_back(analyzerEvent);
        }
        void AnalyzerContext::setNote(const Event &noteEvent) 
        {
            auto voiceMeta = voiceMetaData();
            auto position = voiceMeta->position;
            Base::setNote(noteEvent);
            if (!this->isCurrentTrackRelevant()) {
                return;
            }
            if (this->analyzerData == nullptr) {
                return;
            }
            AnalyzerEvent analyzerEvent(noteEvent);
            analyzerEvent.position = position;
            analyzerEvent.duration = noteEvent.duration;
            this->analyzerData->analyzerEvents.emplace_back(analyzerEvent);
        }
    }
}