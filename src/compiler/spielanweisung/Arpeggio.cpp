#include "Arpeggio.h"
#include "compiler/AContext.h"
#include <algorithm>
#include "sheet/tools.h"

namespace sheet {
    namespace compiler {

        void Arpeggio::addEvent(AContext *ctx, const Event &ev)
        {
			// if (pitches.empty()) {
			// 	return;
			// }
			// std::vector<PitchDef> pitchesCopy(pitches.begin(), pitches.end());

			// std::sort(pitchesCopy.begin(), pitchesCopy.end(), [this](const auto &a, const auto &b) { 
			// 	if (direction == Up) {
			// 		return a < b;
			// 	}
			// 	return a > b;
			// });

			// auto arpduration = duration / pitches.size();
			// for (const auto &pitch : pitchesCopy)
			// {
            //     NoteEvent note = ctx->createNote(pitch, duration);
            //     ctx->addNote(note);
			// 	ctx->seek(arpduration);
			// }
			// auto roundingError = duration - (arpduration * pitches.size());
			// if (roundingError != 0) {
			// 	//ctx->seek(roundingError);
			// }
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