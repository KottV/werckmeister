#include "Document.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <sheet/tools.h>
#include <compiler/metaCommands.h>
#include <compiler/error.hpp>

namespace sheet {
	namespace {
		template<typename TContainer>
		auto _findByName(const fm::String &name, const TContainer &container)
		{
			typename TContainer::const_iterator it;
			if (name == FM_STRING("?")) {
				it = container.begin();
			} else {
				it = container.find(name);
			}
			return it;			
		}
	}

	fm::String Document::getAbsolutePath(const fm::String &path)
	{
		auto a = boost::filesystem::path(this->path).parent_path();
		auto b = boost::filesystem::path(path);
		auto x = boost::filesystem::absolute(b, a);
		return boost::filesystem::system_complete(x).wstring();
	}

	Document::Parts * Document::findParts(const fm::String &styleName)
	{
		Styles &styles = *styles_;
		Styles::iterator it = styles.find(styleName);
		if (it == styles.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	Document::Tracks * Document::findTracks(const fm::String &partName, Parts &parts)
	{
		Parts::iterator it = parts.find(partName);
		if (it == parts.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	void Document::createStylesMap()
	{
		styles_ = std::make_unique<Styles>();
		Styles &styles = *styles_;
		for(const auto &track : this->sheetDef.tracks) {
			fm::String type = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_TYPE, track.trackInfos);
			if (type != SHEET_META__TRACK_META_VALUE_TYPE_STYLE) {
				continue;
			}
			fm::String styleName = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_NAME, track.trackInfos);
			if (styleName.empty()) {
				FM_THROW(compiler::Exception, "missing 'name' for style track");
			}		
			fm::String partName = getFirstMetaValueBy(SHEET_META__TRACK_META_KEY_PART, track.trackInfos);
			if (partName.empty()) {
				FM_THROW(compiler::Exception, "missing 'part' name for style track");
			}				
			Parts *parts = findParts(styleName);
			if (parts == nullptr) {
				styles[styleName] = Parts();
				parts = &styles[styleName];
			}
			auto tracks = findTracks(partName, *parts);
			if (tracks == nullptr) {
				(*parts)[partName] = Document::Tracks();
				tracks = &(*parts)[partName];
			}
			tracks->push_back(track);
		}
	}

	const Document::Styles & Document::styles()
	{
		if (!styles_) {
			createStylesMap();
		}
		return *styles_;
	}

	IStyleDefServer::ConstStyleValueType Document::getStyle(const fm::String &name, const fm::String &part)
	{
		const Styles &styles = this->styles();
		// find style by name
		Styles::const_iterator it = _findByName(name, styles);
		if (it == styles.end()) {
			return nullptr;
		}
		// find track by name
		const auto& partContainer = it->second;
		Parts::const_iterator partIt = _findByName(part, partContainer);
		if (partIt == partContainer.end()) {
			return nullptr;
		}
		return &(partIt->second);

	}
	IStyleDefServer::ConstChordValueType Document::getChord(const fm::String &name)
	{
		ChordDefs::const_iterator it;
		if (name == FM_STRING("?")) {
			it = chordDefs.begin();
		}
		else {
			it = chordDefs.find(name);
		}
		if (it == chordDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	IStyleDefServer::ConstPitchDefValueType Document::getAlias(fm::String alias)
	{
		PitchmapDefs::const_iterator it;
		it = pitchmapDefs.find(alias);
		
		if (it == pitchmapDefs.end()) {
			return nullptr;
		}
		return &(it->second);
	}
}