#include <boost/test/unit_test.hpp>
#include <iostream>
#include "compiler/parser.h"
#include "compiler/error.hpp"
#include <fm/literals.hpp>

BOOST_AUTO_TEST_CASE(test_chordDefparser)
{
	fm::String str(FM_STRING("--here goes comment 1\n\
  \t@import 'old.chdef';\r\n\
@import 'old2.chdef'; --here goes comment 2\n\
@import 'old3.chdef'; \n\
Xmaj: 1 5 8\n\
X7: 1 5 8 10\n\
\n\
\r\n\
   \n\
  \t\r\n\
Xmaj7 : 1 5 8 11\n\
x7+ : 1 5 8 11\n\
X/V: -6 1 5 8 --quinte im bass\
"));

	sheet::compiler::ChordDefParser defParser;

	auto chordDefs = defParser.parse(str);
	BOOST_CHECK(chordDefs.size() == 5);
	auto &def = chordDefs[0];
	BOOST_CHECK((chordDefs[0].name == FM_STRING("Xmaj")));
	auto interval = def.intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval == chordDefs[0].intervals.end()));


	BOOST_CHECK((chordDefs[1].name == FM_STRING("X7")));
	interval = chordDefs[1].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 10);
	BOOST_CHECK((interval == chordDefs[1].intervals.end()));


	BOOST_CHECK((chordDefs[2].name == FM_STRING("Xmaj7")));
	interval = chordDefs[2].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 11);
	BOOST_CHECK((interval == chordDefs[2].intervals.end()));

	BOOST_CHECK((chordDefs[3].name == FM_STRING("x7+")));
	interval = chordDefs[3].intervals.begin();
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval++)->value == 11);
	BOOST_CHECK((interval == chordDefs[3].intervals.end()));

	BOOST_CHECK((chordDefs[4].name == FM_STRING("X/V")));
	interval = chordDefs[4].intervals.begin();
	BOOST_CHECK((interval++)->value == -6);
	BOOST_CHECK((interval++)->value == 1);
	BOOST_CHECK((interval++)->value == 5);
	BOOST_CHECK((interval++)->value == 8);
	BOOST_CHECK((interval == chordDefs[4].intervals.end()));
}

namespace {
	bool checkNote(const sheet::Event &ev,
		sheet::Event::Type type,
		sheet::PitchDef::Pitch pitch = sheet::PitchDef::NoPitch,
		sheet::PitchDef::Octave octave = sheet::PitchDef::DefaultOctave,
		sheet::Event::Duration duration = sheet::Event::NoDuration)
	{
		if (ev.pitches.empty() && pitch != sheet::PitchDef::NoPitch) {
			return false;
		}
		if (pitch != sheet::PitchDef::NoPitch) {
			auto first = ev.pitches.begin();
			if (first->pitch != pitch || first->octave != octave) {
				return false;
			}
		}
		return ev.type == type && ev.duration == duration;
	}


	bool checkNote(const sheet::Event &ev,
		sheet::Event::Type type,
		const sheet::Event::Pitches &pitches,
		sheet::Event::Duration duration = sheet::Event::NoDuration)
	{
		bool pre = ev.type == type && ev.duration == duration;
		if (!pre) {
			return false;
		}
		if (ev.pitches.size() != pitches.size()) {
			return false;
		}
		auto it1 = ev.pitches.begin();
		auto it2 = pitches.begin();
		while (it1 != ev.pitches.end())
		{
			if (it1->pitch != it2->pitch || it1->octave != it2->octave) {
				return false;
			}
			++it1;
			++it2;
		}
		return true;
	}

	bool checkChord(const sheet::ChordEvent &ev, sheet::PitchDef::Pitch root, const fm::String &options)
	{
		return ev.type == sheet::Event::Chord && ev.root == root && ev.options == options;
	}

	bool checkMetaEvent(const sheet::Event &ev, const fm::String &command, const sheet::Event::Args &args)
	{
		bool pre = ev.type == sheet::Event::Meta && ev.metaCommand == command;
		if (!pre) {
			return false;
		}
		if (ev.metaArgs.size() != args.size()) {
			return false;
		}
		auto it1 = ev.metaArgs.begin();
		auto it2 = args.begin();
		while (it1 != ev.metaArgs.end())
		{
			if (*it1 != *it2) {
				return false;
			}
			++it1;
			++it2;
		}
		return true;
	}
}

BOOST_AUTO_TEST_CASE(test_styleDefparser)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		@use anything; \n\
		@or not; \n\
	--some useless comment\n\
		section intro--begin a section\n\
[--a track\n\
	{\n\
		I,4 II,,8 III,,,16 IV32 | I,4 I,, I,,, I | r1 | <I' III' V'>4 \n\
		/name: bass/\n\
		/soundselect: 0 0/\n\
		/acommand: arg1 arg2/\n\
	} -- a voice\n\
	{\n\
		IV'4. VII''8. I'''16. II32. | II'4 II'' II''' II | r1 \n\
	}-- further voice \n\
] \n\
end\n\
");
	sheet::compiler::StyleDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.sections.size() == 1);
	BOOST_CHECK(defs.sections[0].name == FM_STRING("intro"));
	BOOST_CHECK(defs.sections[0].tracks.size() == 1);
	BOOST_CHECK(defs.sections[0].tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.sections[0].tracks[0].voices[0].events.size() == 16);
	BOOST_CHECK(defs.sections[0].tracks[0].voices[1].events.size() == 11);
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[0], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[1], sheet::Event::Degree, 2, -2, 1.0_N8));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[2], sheet::Event::Degree, 3, -3, 1.0_N16));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[3], sheet::Event::Degree, 4, 0, 1.0_N32));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[5], sheet::Event::Degree, 1, -1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[6], sheet::Event::Degree, 1, -2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[7], sheet::Event::Degree, 1, -3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[8], sheet::Event::Degree, 1, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[11], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[0].events[12], sheet::Event::Degree, sheet::Event::Pitches({ PitchDef(1, 1), PitchDef(3, 1), PitchDef(5, 1) }), 1.0_N4));

	BOOST_CHECK(checkMetaEvent(defs.sections[0].tracks[0].voices[0].events[13], FM_STRING("name"), sheet::Event::Args({ FM_STRING("bass") })));
	BOOST_CHECK(checkMetaEvent(defs.sections[0].tracks[0].voices[0].events[14], FM_STRING("soundselect"), sheet::Event::Args({ FM_STRING("0"), FM_STRING("0") })));
	BOOST_CHECK(checkMetaEvent(defs.sections[0].tracks[0].voices[0].events[15], FM_STRING("acommand"), sheet::Event::Args({ FM_STRING("arg1"), FM_STRING("arg2") })));

	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[0], sheet::Event::Degree, 4, 1, 1.0_N4p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[1], sheet::Event::Degree, 7, 2, 1.0_N8p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[2], sheet::Event::Degree, 1, 3, 1.0_N16p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[3], sheet::Event::Degree, 2, 0, 1.0_N32p));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[5], sheet::Event::Degree, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[6], sheet::Event::Degree, 2, 2, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[7], sheet::Event::Degree, 2, 3, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[8], sheet::Event::Degree, 2, 0, sheet::Event::NoDuration));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[9], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.sections[0].tracks[0].voices[1].events[10], sheet::Event::Rest, sheet::PitchDef::NoPitch, 0, 1.0_N1));
}


BOOST_AUTO_TEST_CASE(test_styleDefParser_fail)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
		@use anything; \n\
		@or not; \n\
	--some useless comment\n\
		section intro--begin a section\n\
[--a track\n\
	{\n\
		I,4 II,,8 III,,,16 IV32 | I,4 I,, I,,, I | r1 | <I' III' V'>4 \n\
		/name: bass/\n\
		/soundselect: 0 0/\n\
		/acommand: arg1 arg2/\n\
	} -- a voice\n\
	{\n\
		IV'4. VII''8. I'''16. II32. | II'4 II'' II''' IIt --here is the error (t is not valid without a value)\n\
 | r1 \n\
	}-- further voice \n\
] \n\
end\n\
");
	sheet::compiler::StyleDefParser parser;
	BOOST_CHECK_THROW(parser.parse(text), sheet::compiler::Exception);
}


BOOST_AUTO_TEST_CASE(test_sheetDefParser)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
	{\n\
		<c e g>4 f4 f4 f4 | b4 b4 b4 b4 | \n\
	}\n\
]\n\
-- the sheet, no tracks and voices here\n\
/ style : simplePianoStyle Intro / \n\
/ voicingStrategy : asNotated / \n\
Cmaj | Cmaj C7 | G G | F A9\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(defs.tracks[0].voices[1].events.size() == 10);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({ FM_STRING("0"), FM_STRING("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ FM_STRING("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[0], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[1], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[2], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[3], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[5], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[6], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[7], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[8], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[1].events[9], sheet::Event::EOB));

	BOOST_CHECK(defs.chords.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.chords[0], FM_STRING("style"), sheet::Event::Args({ FM_STRING("simplePianoStyle"), FM_STRING("Intro") })));
	BOOST_CHECK(checkMetaEvent(defs.chords[1], FM_STRING("voicingStrategy"), sheet::Event::Args({ FM_STRING("asNotated") })));
	BOOST_CHECK(checkChord(defs.chords[2], 0, FM_STRING("maj")));
	BOOST_CHECK(checkNote(defs.chords[3], sheet::Event::EOB));
	BOOST_CHECK(checkChord(defs.chords[4], 0, FM_STRING("maj")));
	BOOST_CHECK(checkChord(defs.chords[5], 0, FM_STRING("7")));
	BOOST_CHECK(checkNote(defs.chords[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.chords[7], sheet::Event::Rest));
	BOOST_CHECK(checkChord(defs.chords[8], 7, FM_STRING("")));
	BOOST_CHECK(checkNote(defs.chords[9], sheet::Event::EOB));
	BOOST_CHECK(checkChord(defs.chords[10], 6, FM_STRING("")));
	BOOST_CHECK(checkChord(defs.chords[11], 10, FM_STRING("9+")));
}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_02)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({ FM_STRING("0"), FM_STRING("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ FM_STRING("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

}

BOOST_AUTO_TEST_CASE(test_sheetDefParser_03)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		/ soundselect: 0 0 /\n\
		/ channel : 1 /\n\
		c4 d4 e4 f4 | c'4 d'4 e'4 f'4 |\n\
	}\n\
]\n\
[\n\
	{\n\
		<c e g>4 f4 f4 f4 | b4 b4 b4 b4 | \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks.size() == 2);
	BOOST_CHECK(defs.tracks[0].voices.size() == 1);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 12);
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[0], FM_STRING("soundselect"), sheet::Event::Args({ FM_STRING("0"), FM_STRING("0") })));
	BOOST_CHECK(checkMetaEvent(defs.tracks[0].voices[0].events[1], FM_STRING("channel"), sheet::Event::Args({ FM_STRING("1") })));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[3], sheet::Event::Note, 2, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[4], sheet::Event::Note, 4, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[5], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[6], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[7], sheet::Event::Note, 0, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[8], sheet::Event::Note, 2, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[9], sheet::Event::Note, 4, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[10], sheet::Event::Note, 5, 1, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[11], sheet::Event::EOB));

	BOOST_CHECK(defs.tracks[1].voices.size() == 1);
	BOOST_CHECK(defs.tracks[1].voices[0].events.size() == 10);
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[0], sheet::Event::Note, sheet::Event::Pitches({ PitchDef(0, 0), PitchDef(4, 0), PitchDef(7, 0) }), 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[1], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[2], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[3], sheet::Event::Note, 5, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[4], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[5], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[6], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[7], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[8], sheet::Event::Note, 11, 0, 1.0_N4));
	BOOST_CHECK(checkNote(defs.tracks[1].voices[0].events[9], sheet::Event::EOB));

}


BOOST_AUTO_TEST_CASE(test_sheetDefParser_tie)
{
	using namespace fm;
	using sheet::PitchDef;
	fm::String text = FM_STRING("\
[\n\
	{\n\
		c1~ | c1 \n\
	}\n\
]\n\
");
	sheet::compiler::SheetDefParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.tracks[0].voices[0].events.size() == 3);
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[0], sheet::Event::TiedNote, 0, 0, 1.0_N1));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[1], sheet::Event::EOB));
	BOOST_CHECK(checkNote(defs.tracks[0].voices[0].events[2], sheet::Event::Note, 0, 0, 1.0_N1));
}

BOOST_AUTO_TEST_CASE(test_documentConfigParser)
{
	using namespace fm;
	fm::String text = FM_STRING("\
@load \"Chords1.chords\"; \n\
@load \"Chords.chords\"; \n\
@load \"simplePianoStyle.style\"; \n\
@load \"chords/Chords1.chords\"; \n\
@load \"chords/Chords.chords\"; \n\
@load \"styles/simplePianoStyle.style\"; \n\
@load \"C:\\drivers\\Intel Rapid Storage Technology Driver\"; \n\
\n\
	[\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	--the sheet, no voices here\n\
		/ style: simplePianoStyle:intro /\n\
		/ voicingStrategy : asNotated /\n\
		Cmaj | Cmaj C7 |\n\
\n\
");
	sheet::compiler::DocumentConfigParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.usings.size() == 7);
	auto it = defs.usings.begin();
	BOOST_CHECK(*(it++) == FM_STRING("Chords1.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("Chords.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("simplePianoStyle.style"));
	BOOST_CHECK(*(it++) == FM_STRING("chords/Chords1.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("chords/Chords.chords"));
	BOOST_CHECK(*(it++) == FM_STRING("styles/simplePianoStyle.style"));
	BOOST_CHECK(*(it++) == FM_STRING("C:\\drivers\\Intel Rapid Storage Technology Driver"));

}

BOOST_AUTO_TEST_CASE(test_documentConfigParser_empty)
{
	using namespace fm;
	fm::String text = FM_STRING("\
\n\
	[\n\
	{\n\
		/ soundselect: 0 0 /\n\
			/ channel : 1 /\n\
			c4 d4 e4 f4 | c4 d4 e4 f4 |\n\
	}\n\
	{\n\
		f4 f4 f4 f4 | h4 h4 h4 h4 |\n\
	}\n\
	]\n\
	--the sheet, no voices here\n\
		/ style: simplePianoStyle:intro /\n\
		/ voicingStrategy : asNotated /\n\
		Cmaj | Cmaj C7 |\n\
\n\
");
	sheet::compiler::DocumentConfigParser parser;
	auto defs = parser.parse(text);
	BOOST_CHECK(defs.usings.size() == 0);

}