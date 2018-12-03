#include "werckmeister.hpp"
#include "midi.hpp"
#include "config.hpp"
#include <type_traits>
#include <boost/filesystem.hpp>
#include <exception>
#include <fstream>
#include "compiler/compiler.h"
#include "compiler/MidiContext.h"
#include <memory>
#include "sheet/DirectVoicingStrategy.h"
#include "sheet/SimpleGuitar.h"
#include "compiler/voicingStrategies.h"

namespace fm {
    
	
	Werckmeister & getWerckmeister()
    {
        typedef Loki::SingletonHolder<Werckmeister> Holder;
		return Holder::Instance();
    }

    const char * Werckmeister::version() const
    {
        return SHEET_VERSION;
    }

	midi::MidiPtr Werckmeister::createMidi()
	{
		return std::make_shared<midi::Midi>(PPQ);
	}


	Werckmeister::ResourceStream Werckmeister::openResourceImpl(const fm::String &path)
	{
		auto fpath = boost::filesystem::system_complete(path);
		auto absolute = fpath.string();
		if (!boost::filesystem::exists(path))
		{
			throw std::runtime_error("resource not found: " + fpath.string());
		}
		auto result = std::make_unique<StreamType>(absolute.c_str());
		return result;
	}

	sheet::compiler::CompilerPtr Werckmeister::createCompiler()
	{
		return std::make_shared<sheet::compiler::Compiler>();
	}

	sheet::compiler::AContextPtr Werckmeister::createContext()
	{
		auto midiContext = std::make_shared<sheet::compiler::MidiContext>();
		return midiContext;
	}

	sheet::VoicingStrategyPtr Werckmeister::getDefaultVoicingStrategy()
	{
		return getVoicingStrategy(SHEET_VOICING_STRATEGY_DEFAULT);
	}

	sheet::VoicingStrategyPtr Werckmeister::getVoicingStrategy(const fm::String &name)
	{
		if (name == SHEET_VOICING_STRATEGY_SIMPLE_GUITAR) {
			return std::make_shared<sheet::SimpleGuitar>();
		}
		if (name == SHEET_VOICING_STRATEGY_AS_NOTATED) {
			return std::make_shared<sheet::DirectVoicingStrategy>();
		}
		throw std::runtime_error("voicing strategy not found: " + fm::to_string(name));
	}

	Werckmeister::~Werckmeister() = default;
}
