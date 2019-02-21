#ifndef SHEET_SHEETDEF_H
#define SHEET_SHEETDEF_H

#include <fm/common.hpp>
#include "Track.h"
#include "Event.h"
#include <vector>
#include "DocumentConfig.h"

namespace sheet {

	struct SheetInfo {
		typedef std::vector<fm::String> Args;
		fm::String name;
		Args args;
	};

	struct SheetDef {
		typedef std::vector<Track> Tracks;
		typedef std::vector<SheetInfo> SheetInfos;
		DocumentConfig documentConfig;
		SheetInfos sheetInfos;
		Tracks tracks;
	};

}

#endif