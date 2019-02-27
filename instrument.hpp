/** \file The DefaultInstrument Module
*/
#define _USE_MATH_DEFINES

#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP
#include <fstream>

#include <vector>
#include "event.hpp"
#include "track.hpp"
#include <map>
#include <cmath>

using std::ofstream;
using std::map;
using std::vector;


/** The default instrument. A simple fixed length note instrument using a
* a sinusoid weighted by an envelope function.
*/
class DefaultInstrument {
public:
	/** No default constructor. A DefaultInstrument must have an associated
	* track.
	*/
	DefaultInstrument() = delete;

	/** Cannot be copied.
	*/
	DefaultInstrument(const DefaultInstrument &) = delete;

	/** Cannot be assigned.
	*/
	DefaultInstrument & operator=(const DefaultInstrument &) = delete;

	/** Construct a Default Instrument using the specified track.
	* \param track the track to read events from.
	*/
	DefaultInstrument(const Track & track);

	/** Returns true of the DefaultInstrument has stopped processing events.
	* This can occu e.g. if all events have been consumed or a EndTrackEvent
	* has been processed.
	*/
	bool halted();

	/** Return the sample after the (real) time increment. Samples the
	* instrument based on its state after processing all events prior
	* to this time.
	* \param deltaT the time increment in seconds.
	*/
	double sample(double deltaT);
	//MIDI time is represented by a tick int
	//T=microseconds per beat(defined by tempo)
	//M = MIDI clock ticks per beat(defined in midi file)
	//T' = (T/10^6) seconds per beat
	//hensce senconds per MIDI clopck tick= T'/M
	//process any new track events that becamne active since the last sample time
	//Sum the waveform for the currently active notes.
	//must matntain a container of notes active at the current time
	//note number
	//note velocity
	//note duration
	//time turned on
	//use a track iterator to process all events up to the current sample time
	//define a private helper function to 
	//-convert a note number to a frequency
	//-determine the length of a note
	//-evaluate the envelope functions
	//-sum the active notes

	//double pub_getFreq(int8_t code);

private:
	struct Time
	{
		double t = 0.0;
		intmax_t numT = 0;
	};
	typedef std::pair<Time, NoteEvent> NoteType;

	double getFreq(int8_t code);
	double getEnvelope(NoteType note);

	bool ended;
	Track myTrack;
	long double t;
	intmax_t tempo;
	long double deltaTicks;
	intmax_t TPB;
	intmax_t numTimes;
	ConstIteratorType it;
	vector<NoteType> currentNotes;
	static bool pred(NoteType note);
};


#endif
