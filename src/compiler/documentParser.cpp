#include "parser.h"
#include "sheet/Document.h"
#include <boost/filesystem.hpp>
#include "fm/config.hpp"
#include <unordered_map>
#include <functional>
#include "fm/common.hpp"
#include "fm/werckmeister.hpp"

namespace sheet {
	namespace compiler {

		namespace {
			fm::String getAbsolutePath(DocumentPtr doc, const fm::String &path)
			{
				return doc->getAbsolutePath(path);
			}
			void useChordDef(DocumentPtr doc, const fm::String &path)
			{
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				ChordDefParser chordDefParser;
				auto chords = chordDefParser.parse(documentText);
				for (const auto &x : chords) {
					doc->chordDefs[x.name] = x;
				}
			}
			void usePitchmapDef(DocumentPtr doc, const fm::String &path)
			{
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				PitchmapParser pitchmapParser;
				auto pitchmaps = pitchmapParser.parse(documentText);
				for (const auto &x : pitchmaps) {
					doc->pitchmapDefs[x.name] = x.pitch;
				}
			}
			void useLuaScript(DocumentPtr doc, const fm::String &path)
			{
				auto &wm = fm::getWerckmeister();
				wm.registerLuaScript(path);
			}			
			void useStyleDef(DocumentPtr doc, const fm::String &path)
			{
				auto apath = getAbsolutePath(doc, path);
				auto filestream = fm::getWerckmeister().openResource(apath);
				fm::StreamBuffIterator begin(*filestream);
				fm::StreamBuffIterator end;
				fm::String documentText(begin, end);
				StyleDefParser styleDefParser;
				auto name = boost::filesystem::path(path).stem().wstring();
				doc->styleDefs[name] = styleDefParser.parse(documentText);
			}
			typedef std::function<void(DocumentPtr, const fm::String&)> ExtHandler;
			std::unordered_map <std::string, ExtHandler> exthandlers({
				{ CHORD_DEF_EXTENSION , &useChordDef },
				{ STYLE_DEF_EXTENSION , &useStyleDef },
				{ PITCHMAP_DEF_EXTENSION , &usePitchmapDef },
				{ LUA_DEF_EXTENSION , &useLuaScript }
			});

			void processUsings(DocumentPtr doc)
			{
				for (const auto &x : doc->documentConfig.usings)
				{
					auto path = boost::filesystem::path(x);
					auto ext = path.extension().string();
					auto it = exthandlers.find(ext);
					if (it == exthandlers.end()) {
						throw std::runtime_error("unsupported file type: " + fm::to_string(x));
					}
					it->second(doc, x);
				}
			}
		}

		DocumentPtr DocumentParser::parse(const fm::String path)
		{
			auto filestream = fm::getWerckmeister().openResource(path);
			fm::StreamBuffIterator begin(*filestream);
			fm::StreamBuffIterator end;
			fm::String documentText(begin, end);
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			auto res = std::make_shared<Document>();
			res->path = boost::filesystem::system_complete(path).wstring();

			{
				DocumentConfigParser configParser;
				res->documentConfig = configParser.parse(first, last);
			}
			{
				SheetDefParser sheetParser;
				res->sheetDef = sheetParser.parse(first, last);
			}

			processUsings(res);
			return res;
		}

		DocumentPtr DocumentParser::parseString(const fm::String &sheetText, const Usings &optUsings)
		{
			fm::StringStream ss;
			ss << "-- BUG LINE" << std::endl;
			for (const auto &using_ : optUsings) {
				ss << "@load \""<< using_ << "\";" << std::endl;
			}
			for (int i=0; i<10; ++i) {
				ss << sheetText << "|" << std::endl;
			}
			fm::String documentText(ss.str());
			const fm::String::value_type *first = documentText.c_str();
			const fm::String::value_type *last = first + documentText.length();

			auto res = std::make_shared<Document>();
			res->path = boost::filesystem::system_complete(".").wstring();

			{
				DocumentConfigParser configParser;
				res->documentConfig = configParser.parse(first, last);
			}
			{
				SheetDefParser sheetParser;
				res->sheetDef = sheetParser.parse(first, last);
			}

			processUsings(res);
			return res;
		}
		DocumentPtr DocumentParser::parseString(const fm::String &sheetText)
		{
			return parseString(sheetText, Usings());
		}
	}
}