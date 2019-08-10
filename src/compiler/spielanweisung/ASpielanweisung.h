#ifndef ASPIELANWEISUNG_HPP
#define ASPIELANWEISUNG_HPP

#include <fm/units.hpp>
#include "sheet/Event.h"

namespace sheet {
    namespace compiler {
        class AContext;
        class ASpielanweisung {
        protected:
            ASpielanweisung() = default;
        public:
            virtual ~ASpielanweisung() = default;
            virtual void addEvent(AContext *ctx, const Event::Pitches &pitches, fm::Ticks duration) = 0;
            virtual void setArguments(const Event::Args &args) {}
        };
    }
}

#endif