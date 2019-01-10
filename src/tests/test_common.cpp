#include <boost/test/unit_test.hpp>
#include <fm/common.hpp>
#include <fm/midi.hpp>
#include <fm/werckmeister.hpp>
#include <iterator>


BOOST_AUTO_TEST_CASE(test_endswap)
{
	using namespace fm;
	if (isLittleEndian()) {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_CHECK(bytes[0] == 0xEF);
		BOOST_CHECK(bytes[1] == 0xCD);
		BOOST_CHECK(bytes[2] == 0xAB);
		BOOST_CHECK(bytes[3] == 0xAA);
		endswap(&x);
		BOOST_CHECK(bytes[0] == 0xAA);
		BOOST_CHECK(bytes[1] == 0xAB);
		BOOST_CHECK(bytes[2] == 0xCD);
		BOOST_CHECK(bytes[3] == 0xEF);
	}
	else {
		midi::DWord x = 0xAAABCDEF;
		Byte *bytes = reinterpret_cast<Byte*>(&x);
		BOOST_CHECK(bytes[0] == 0xAA);
		BOOST_CHECK(bytes[1] == 0xAB);
		BOOST_CHECK(bytes[2] == 0xCD);
		BOOST_CHECK(bytes[3] == 0xEF);
		endswap(&x);
		BOOST_CHECK(bytes[0] == 0xEF);
		BOOST_CHECK(bytes[1] == 0xCD);
		BOOST_CHECK(bytes[2] == 0xAB);
		BOOST_CHECK(bytes[3] == 0xAA);
	}
}

BOOST_AUTO_TEST_CASE(test_resource_loader)
{
	auto resource = fm::getWerckmeister().openResource(FM_STRING("../sheets/chords/default.chords"));
	fm::StreamBuffIterator eos;
	fm::StreamBuffIterator it(*resource.get());
	fm::String res(it, eos);
	BOOST_CHECK(res.length() > 0);
}

#if 0
BOOST_AUTO_TEST_CASE(test_sconv)
{
	std::wstring wstr = fm::to_wstring("äöüÄÖÜ?§");
	BOOST_CHECK(L"äöüÄÖÜ?§" == wstr);

	std::string str = fm::to_string(L"äöüÄÖÜ?§");
	BOOST_CHECK("äöüÄÖÜ?§" == str);
}
#endif