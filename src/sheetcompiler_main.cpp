#include <fm/werckmeister.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "compiler/MidiContext.h"
#include "fmapp/json.hpp"
#include <fstream>
#include <fm/common.hpp>
#include "sheet/Document.h"
#include "sheet.h"

#define ARG_HELP "help"
#define ARG_INPUT "input"
#define ARG_OUTPUT "output"
#define ARG_MODE "mode"
#define ARG_NOMETA "nometa"

struct Settings {
	typedef boost::program_options::variables_map Variables;
	typedef boost::program_options::options_description OptionsDescription;
	OptionsDescription optionsDescription;
	Variables variables;

	Settings(int argc, const char** argv)
		: optionsDescription("Allowed options")
	{
		namespace po = boost::program_options;
		optionsDescription.add_options()
			(ARG_HELP, "produce help message")
			(ARG_INPUT, po::value<std::string>(), "input file")
			(ARG_OUTPUT, po::value<std::string>(), "output file")
			(ARG_MODE, po::value<std::string>(), "mode: normal or json; in JSON mode the input and output will be implemented using JSON strings. The JSON has to be Base64 encoded.")
			(ARG_NOMETA, "dosen't render midi meta events like track name or tempo")
			;
		po::positional_options_description p;
		p.add(ARG_INPUT, -1);
		po::store(po::command_line_parser(argc, argv).
			options(optionsDescription).positional(p).run(), variables);
		po::notify(variables);
	}


	bool help() const {
		return !!variables.count(ARG_HELP);
	}

	bool input() const {
		return !!variables.count(ARG_INPUT);
	}

	auto getInput() const {
		return variables[ARG_INPUT].as<std::string>();
	}

	bool output() const {
		return !!variables.count(ARG_OUTPUT);
	}

	bool noMeta() const {
		return !!variables.count(ARG_NOMETA);
	}

	auto getOutput() const {
		return variables[ARG_OUTPUT].as<std::string>();
	}

	bool isJsonMode() const {
		if (variables.count(ARG_MODE) == 0) {
			return false;
		}
		return variables[ARG_MODE].as<std::string>() == "json";
	}

};

void saveMidi(fm::midi::MidiPtr midi, const std::string &filename)
{
	std::ofstream fstream(filename, std::ios::binary);
	midi->write(fstream);
	fstream.flush();
}

void toJSONOutput(sheet::DocumentPtr doc, fm::midi::MidiPtr midi, const sheet::Warnings &warnings)
{
	fmapp::JsonWriter jsonWriter;
	std::cout << jsonWriter.midiToJSON(*(doc.get()), midi, warnings);
}

void printWarnings(const sheet::Warnings &warnings)
{
	if (warnings.empty()) {
		return;
	}
	for (const auto &warning : warnings) {
		std::cout << warning << std::endl;
	}
}

fm::Path prepareJSONMode(const std::string &jsonData) {
	auto &wm = fm::getWerckmeister();
	fmapp::JsonReader jsonReader;
	auto vfiles = jsonReader.readVirtualFS(jsonData);
	fm::Path sheetPath;
	for(const auto &vfile : vfiles) {
		if (wm.fileIsSheet(vfile.path)) {
			sheetPath = vfile.path;
		}
		wm.createVirtualFile(vfile.path, vfile.data);
	}
	return sheetPath;
}

int main(int argc, const char** argv)
{
	bool jsonMode = false;
	try {
		Settings settings(argc, argv);
		jsonMode = settings.isJsonMode();
		std::string infile;

		if (settings.help()) {
			std::cout << settings.optionsDescription << "\n";
			return 1;
		}

		if (jsonMode && settings.input()) {
			auto json = settings.getInput();
			json = fmapp::base64Decode(json);
			infile = prepareJSONMode(json);
		}
		else if (!settings.input()) {
			throw std::runtime_error("missing input file");
		}

		if (infile.empty()) {
			infile = settings.getInput();
		}

		sheet::Warnings warnings;
		fm::midi::MidiConfig midiConfig;
		midiConfig.skipMetaEvents = settings.noMeta();
		auto document = sheet::createDocument(infile);
		auto midi = sheet::processFile(document, warnings, &midiConfig);

		if (!jsonMode) {
			printWarnings(warnings);
		}

		std::string outfile = boost::filesystem::path(infile).filename().string() + ".mid";
		if (settings.output()) {
			outfile = settings.getOutput();
		}
		if (jsonMode) {
			toJSONOutput(document, midi, warnings);
		} else {
			saveMidi(midi, outfile);
		}
		return 0;
	}
	catch (const fm::Exception &ex)
	{
		if (jsonMode) {
			fmapp::JsonWriter json;
			std::cout << json.exceptionToJSON(ex);

		} else {
			sheet::onCompilerError(ex);
		}
	}
	catch (const std::exception &ex)
	{
		if (jsonMode) {
			fmapp::JsonWriter json;
			std::cout << json.exceptionToJSON(ex);

		} else {
			sheet::onCompilerError(ex);
		}
	}
	catch (...)
	{
		if (jsonMode) {
			fmapp::JsonWriter json;
			auto ex = std::runtime_error("unkown error");
			std::cout << json.exceptionToJSON(ex);

		} else {
			sheet::onCompilerError();
		}
	}
	return -1;
}