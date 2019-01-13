#include "Vorschlag.h"
#include <fm/literals.hpp>
#include <compiler/AContext.h>

namespace sheet {
    namespace compiler {
        using namespace fm;
        const fm::Ticks Vorschlag::defaultDuration = 1.0_N64;
        void Vorschlag::addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration, bool tying)
        {
            auto vorschlagDuration = vorschlagNote.duration != Event::NoDuration ? vorschlagNote.duration : defaultDuration;
            ctx->seek(-vorschlagDuration);
            Base::addEvent(ctx, vorschlagNote.pitches, vorschlagDuration, false);
            Base::addEvent(ctx, pitches, duration, tying);
        }
    }
}