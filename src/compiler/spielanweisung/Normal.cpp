#include "Normal.h"
#include "compiler/AContext.h"

namespace sheet {
    namespace compiler {

        void Normal::addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying)
        {
            auto meta = ctx->voiceMetaData();
			for (const auto &pitch : pitches)
			{
				ctx->renderPitch(pitch, duration, tying);
			}
			ctx->seek(duration);
        }

    }
}