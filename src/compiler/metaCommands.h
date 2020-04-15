#ifndef COMPILER_CONTEXT_METACOMMANDS_H
#define COMPILER_CONTEXT_METACOMMANDS_H

#include <fm/common.hpp>

// TRACK META INFO
#define SHEET_META__INSTRUMENT FM_STRING("instrument")
#define SHEET_META__TRACK_META_KEY_TYPE FM_STRING("type")
#define SHEET_META__TRACK_META_VALUE_TYPE_SHEET_TEMPLATE FM_STRING("template")
#define SHEET_META__TRACK_META_VALUE_TYPE_ACCOMP FM_STRING("accomp")
#define SHEET_META__TRACK_META_KEY_NAME FM_STRING("name")


// VOICE
#define SHEET_META__SHEET_TEMPLATE_POSITION FM_STRING("templatePosition")
#define SHEET_META__SHEET_TEMPLATE_POSITION_CMD_RESET FM_STRING("reset")
#define SHEET_META__MIDI_INSTRUMENT_DEF FM_STRING("instrumentDef")
#define SHEET_META__MIDI_CHANNEL FM_STRING("channel")
#define SHEET_META__MIDI_SOUNDSELECT FM_STRING("soundSelect")
#define SHEET_META__SET_SHEET_TEMPLATE FM_STRING("template")
#define SHEET_META__SET_VOLUME FM_STRING("volume")
#define SHEET_META__SET_PAN FM_STRING("pan")
#define SHEET_META__SET_INSTRUMENT_CONFIG FM_STRING("instrumentConf")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VOLUME FM_STRING("volume")
#define SHEET_META__SET_INSTRUMENT_CONFIG_PAN FM_STRING("pan")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_P       FM_STRING("p")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_PP      FM_STRING("pp")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_PPP     FM_STRING("ppp")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_PPPP    FM_STRING("pppp")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_PPPPP   FM_STRING("ppppp")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_F       FM_STRING("f")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_FF      FM_STRING("ff")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_FFF     FM_STRING("fff")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_FFFF    FM_STRING("ffff")
#define SHEET_META__SET_INSTRUMENT_CONFIG_VELOCITY_OVERRIDE_FFFFF   FM_STRING("fffff")
#define SHEET_META__SET_SINGLE_EXPRESSION FM_STRING("singleExpression")
#define SHEET_META__SET_EXPRESSION FM_STRING("expression")
#define SHEET_META__SET_TEMPO FM_STRING("tempo")
#define SHEET_META__SET_TEMPO_VALUE_HALF FM_STRING("half")
#define SHEET_META__SET_TEMPO_VALUE_DOUBLE FM_STRING("double")
#define SHEET_META__SET_TEMPO_VALUE_NORMAL FM_STRING("normal")
#define SHEET_META__SET_VOICING_STRATEGY FM_STRING("voicingStrategy")
#define SHEET_META__SET_SPIELANWEISUNG FM_STRING("do")
#define SHEET_META__SET_SPIELANWEISUNG_ONCE FM_STRING("doOnce")
#define SHEET_META__SET_MOD FM_STRING("mod")
#define SHEET_META__SET_MOD_ONCE FM_STRING("modOnce")
#define SHEET_META__SET_SIGNATURE FM_STRING("signature")
#define SHEET_META__SET_DEVICE FM_STRING("device")
#define SHEET_META__SET_VORSCHLAG FM_STRING("vorschlag")

#endif