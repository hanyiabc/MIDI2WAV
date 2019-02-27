#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

/** A TempoEvent. This event indicates that a change in the tempo should occur
* on the next MIDI tick.
*/


enum TypeOfEvent { TEMPO_EVENT, NOTE_EVENT, END_EVENT, PAUSE_EVENT, STOP_EVENT };

class TempoEvent {
public:

	/** Construct a TempoEvent. The default tempo is 0.5 bpm
	*/
	TempoEvent(intmax_t t = 500000);

	/** Get the tempo in micro-sec per beat.
	*/
	intmax_t getTempo() const;

private:
	intmax_t tempo;
};

/** An EndTrackEvent. This event indicates that the track has ended and the
* synthesizer should halt sampling the instrument.
*/
class EndTrackEvent {};

/** A NoteEvent. This event indicates that a note should be activated
* or deactivated.
*/
class NoteEvent {
public:

	/** You cannot default construct a Note Event. */
	NoteEvent() = delete;

	/** Construct a note event.
	* \param on true if the note should be turned on, false if it should be
	*           turned off.
	* \param notenum the MIDI note number (0-127) to be modified.
	* \param notevelocity the velocity of the note change.
	*/
	NoteEvent(bool on, int8_t notenum, int8_t notevelocity);

	/** Get note change type. true if ON, false if OFF.
	*/
	bool getNoteOn();

	/** Get MIDI note number to change. Takes on values in [0,127]
	*/
	int8_t getNoteNumber() const;

	/** Get the velocity of the note change.
	*/
	int8_t getNoteVelocity() const;

private:
	bool on_off;
	int8_t number;
	int8_t velocity;
};

union EventType
{
	NoteEvent myNoteEvent;
	TempoEvent myTempoEvent;
	EndTrackEvent myEndTrackEvent;
	EventType() : myTempoEvent(TempoEvent()) {}
};

/** A MIDI Event holds one of a TempoEvent, EndTrackEvent, or NoteEvent and an
* associated MIDI tick time in units of ticks-per-beat.
*/
class MIDIEvent {
public:

	/** Construct a MIDI event holding a TempEvent at ticktime.
	*/
	MIDIEvent(TempoEvent e, intmax_t ticktime);

	/** Construct a MIDI event holding an EndTrackEvent at ticktime.
	*/
	MIDIEvent(EndTrackEvent e, intmax_t ticktime);

	/** Construct a MIDI event holding a NoteEvent at ticktime.
	*/
	MIDIEvent(NoteEvent e, intmax_t ticktime);


	/** True if event is a TempoEvent, else false.
	*/
	bool isTempoEvent() const;

	/** True if event is an EndTrackEvent, else false.
	*/
	bool isEndTrackEvent() const;

	/** True if event is a NoteEvent, else false.
	*/
	bool isNoteEvent() const;

	/** Get the MIDI tick time of this event (ticks per beat)
	*/
	intmax_t getTickTime() const;

	/** Cast the MIDI event as a TempoEvent. Only valid if
	* isTempoEvent() returns true.
	*/
	TempoEvent asTempoEvent() const;

	/** Cast the MIDI event as a NoteEvent. Only valid if
	* isNoteEvent() returns true.
	*/
	NoteEvent asNoteEvent() const;

private:
	enum TypeOfEvent eventType;
	EventType currentEvent;
	intmax_t tickTime;

};

#endif
