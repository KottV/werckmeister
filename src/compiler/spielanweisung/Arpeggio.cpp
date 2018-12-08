#include "Arpeggio.h"
#include "compiler/AContext.h"
#include <algorithm>

namespace sheet {
    namespace compiler {

        void Arpeggio::addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying)
        {
			if (pitches.empty()) {
				return;
			}
			std::vector<PitchDef> pitchesCopy(pitches.begin(), pitches.end());

			std::sort(pitchesCopy.begin(), pitchesCopy.end(), [this](const auto &a, const auto &b) { 
				if (direction == Up) {
					return a < b;
				}
				return a > b;
			});

			auto arpduration = duration / pitches.size();
			for (const auto &pitch : pitchesCopy)
			{
				ctx->addEvent(pitch, duration, tying);
				ctx->seek(arpduration);
			}
			auto roundingError = duration - (arpduration * pitches.size());
			if (roundingError != 0) {
				//ctx->seek(roundingError);
			}
        }

		void Arpeggio::setArguments(const Event::Args &args)
		{
			if (args.size() > 1) {
				auto directionStr = getArgument<fm::String>(args, 1);
				if (directionStr == FM_STRING("down")) {
					direction = Down;
				} else {
					direction = Up;
				}

			}
		}

    }
}