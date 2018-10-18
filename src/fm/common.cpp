#include "common.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace {
	union {
		unsigned short shortVar;    // bin�re Ganzzahl der L�nge 16 Bits
		unsigned char  charVar[2];  // 2 bin�re Ganzzahlen, jede 8 Bits
	} test_endianness;
}

namespace fm {
	// https://de.wikipedia.org/wiki/Byte-Reihenfolge
	bool isLittleEndian() {
		test_endianness.shortVar = 0x8000; // das Most Significant Bit innerhalb von 16
		if (test_endianness.charVar[0] != 0) {
			// Das Programm l�uft auf einer Big-Endian-Maschine.
			return false;
		}
		else {
			// Das Programm l�uft auf einer Little-Endian-Maschine.
			return true;
		}
	}

	UId generateUid()
	{
        boost::uuids::uuid u = boost::uuids::random_generator()();
        return boost::uuids::to_string(u);
	}
}