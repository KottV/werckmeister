#include "Bend.h"
#include "compiler/AContext.h"
#include <algorithm>
#include <fm/literals.hpp>
#include "sheet/tools.h"

namespace sheet {
    namespace compiler {

        void Bend::perform(AContext *ctx, const Event &ev)
        {
            using namespace fm;
            if (ev.duration == 0) {
                return;
            }
            double b = 0.5;
            double e = value;
            if (mode==From) {
                b = value;
                e = 0.5;
            }
            double c = e - b;
            double d = static_cast<double>(ev.duration);
            auto absPosition = ctx->voiceMetaData()->position;
			for (double t=0; t<ev.duration; t+=static_cast<double>( 1.0_N64 )) {
                double x = c*t/d + b;
                ctx->addPitchbendEvent(x, absPosition + static_cast<fm::Ticks>(t));
            }
            ctx->addPitchbendEvent(0.5, absPosition + ev.duration + 1);
        }

        void Bend::setArguments(const Event::Args &args)
        {
            value = getArgument<double>(args, 2) / 100.;
            auto modestr = getArgument<fm::String>(args, 1);
            if (modestr == FM_STRING("from")) {
                mode = From;
            } else {
                mode = To;
            }
        }

    }
}