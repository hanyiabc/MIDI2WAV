/** \file Unit tests for DefaultInstrument Module
*/
#include "catch.hpp"

#include "track.hpp"
#include "instrument.hpp"

TEST_CASE( "Test instrument constructors", "[instrument]" ) 
{
	Track myTrack;
	myTrack.addTempoEvent(500000, 100);
	DefaultInstrument I(myTrack);
	REQUIRE(!I.halted());
}

TEST_CASE("Test instrument sample", "[instrument]")
{
	Track myTrack(480);
	myTrack.addTempoEvent(500000, 0);
	myTrack.addNoteEvent(9, 5, 20, 0);
	myTrack.addNoteEvent(9, 17, 20, 0);
	myTrack.addNoteEvent(9, 17, 20, 0);
	myTrack.addNoteEvent(9, 29, 20, 0);
	myTrack.addNoteEvent(9, 41, 20, 0);
	myTrack.addNoteEvent(9, 53, 20, 0);
	myTrack.addNoteEvent(9, 65, 20, 0);
	myTrack.addNoteEvent(9, 89, 20, 0);
	myTrack.addNoteEvent(9, 101, 20, 0);
	myTrack.addNoteEvent(9, 113, 20, 0);
	myTrack.addNoteEvent(8, 5, 20, 1);
	myTrack.addNoteEvent(8, 17, 20, 1);
	myTrack.addNoteEvent(8, 17, 20, 1);
	myTrack.addNoteEvent(8, 29, 20, 1);
	myTrack.addNoteEvent(8, 41, 20, 1);
	myTrack.addNoteEvent(8, 53, 20, 1);
	myTrack.addNoteEvent(8, 65, 20, 1);
	myTrack.addNoteEvent(8, 89, 20, 1);
	myTrack.addNoteEvent(8, 101, 20, 1);
	myTrack.addNoteEvent(8, 113, 20, 1);
	myTrack.addEndEvent(300);

	DefaultInstrument I(myTrack);
	REQUIRE(I.sample(1.0/44100.0) == 0);
	while (!I.halted())
	{
		I.sample(1.0 / 44100.0);
	}
}
