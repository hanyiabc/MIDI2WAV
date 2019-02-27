#ifndef TRACK_HPP
#define TRACK_HPP

#include <list>
#include "event.hpp"

/** A Track represents a sequence of Events ordered by thier MIDI tock time.
* Events can be added to the Track based on type of event. Events can be accessed
* using a constant iterator in order, first to last.
*/


/** Typdef for iterator type. This object should support the operators
* ++, *, and -> (i.e. it is a forward iterator).
*/
typedef std::list<MIDIEvent>::const_iterator ConstIteratorType;

class Track {

public:
	/** Typdef for iterator type. This object should support the operators
	* ++, *, and -> (i.e. it is a forward iterator).
	*/
	//typedef std::list<MIDIEvent>::const_iterator ConstIteratorType;

public:

	/** Construct Track with ticks per beat in microsec.
	* default is 480 MIDI ticks per beat
	*/
	Track(intmax_t ticks_per_beat = 480);

	/** Get ticks per beat in microsec. */
	intmax_t getTicksPerBeat() const;

	/** Get iterator to first event. */
	ConstIteratorType begin() const;

	/** Get iterator to end of event sequence. */
	ConstIteratorType end() const;

	/** Add an EndEvent to the track sequence.
	* \param tick_time, the MIDI tick time of the event.
	*/
	void addEndEvent(intmax_t tick_time);

	/** Add a TempoEvent to the track sequence.
	* \param tempo the new tempo of the track after this tick
	* \param tick_time the MIDI tick time of the event.
	*/
	void addTempoEvent(intmax_t tempo, intmax_t tick_time);

	/** Add a NoteEvent to the track sequence.
	* \param code the event code (8 = On, 9 = Off unless data2 = 0)
	* \param data1 the first data byte (holding the note number)
	* \param data2 the second data byte (holding the velocity)
	* \param tick_time, the MIDI tick time of the event.
	*/
	void addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time);


private:
	intmax_t TPB;
	std::list<MIDIEvent> eventList;
};

/** A list of tracks. */
typedef std::list<Track> TrackListType;

#endif
