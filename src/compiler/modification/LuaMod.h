#ifndef MODIFICATION_LUA_SCRIPT_HPP
#define MODIFICATION_LUA_SCRIPT_HPP

#include "AModification.h"
#include <fm/lua/ALuaScript.h>
#include <fm/common.hpp>
#include <forward.hpp>

#define LUA_MODIFICATION_FENTRY "perform"

namespace sheet {
    namespace compiler {
        class LuaModification : public AModification, public lua::ALuaScript {
        public:
            typedef VoicingStrategy Base;
            typedef lua::ALuaScript LuaBase;
            LuaModification(const fm::String &path);
            virtual ~LuaModification() = default;
            virtual bool canExecute() const override;
            virtual void assertCanExecute() const override;
            virtual void setArguments(const Event::Args &args) override;
            virtual void perform(AContext *ctx, Events &events) override;
        protected:
            void pushArgs(const Event::Args &args);
            void pushEvents(AContext *ctx, const Events &events);
            Events popEvents();
            void popNoteEvent(Event &event);
            void popPitchBendEvent(Event &event);
        private:
            Event::Args args_;
        };
    }
}

#endif