#ifndef MODIFICTAION_BEND_HPP
#define MODIFICTAION_BEND_HPP

#include "AModification.h"

namespace sheet {
    namespace compiler {
        class Bend : public AModification {
        public:
            Bend() = default;
            virtual ~Bend() = default;
            virtual void addModificationEvents(AContext *ctx, fm::Ticks absPosition, fm::Ticks duration) override;
            virtual void setArguments(const Event::Args &args) override;
            double value = 0.5;
            enum BendMode { To, From };
            BendMode mode = To;
        };
    }
}

#endif