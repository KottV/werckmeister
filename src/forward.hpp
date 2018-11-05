#ifndef SHEET_FORWARD_HPP
#define SHEET_FORWARD_HPP

#include <memory>

namespace sheet {
	namespace compiler {
		class Compiler;
		typedef std::shared_ptr<Compiler> CompilerPtr;
		class AContext;
		typedef std::shared_ptr<AContext> AContextPtr;
	}
	class Document;
	typedef std::shared_ptr<Document> DocumentPtr;
}

namespace fm {
	namespace midi {
		class Midi;
		typedef std::shared_ptr<Midi> MidiPtr;
	}
}

#endif
