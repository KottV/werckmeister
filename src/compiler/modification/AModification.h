#ifndef AMODIFICTAION_HPP
#define AMODIFICTAION_HPP

#include <fm/units.hpp>
#include "sheet/Event.h"
#include <vector>

namespace sheet {
    namespace compiler {
        class AContext;
        class AModification {
        protected:
            AModification() = default;
        public:
            typedef std::vector<Event> Events;
            virtual ~AModification() = default;
            virtual void perform(AContext *ctx, Events &events) = 0;
            virtual void setArguments(const Event::Args &args) {}
        };
    }
}

#endif