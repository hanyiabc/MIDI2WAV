#include "event.hpp"


TempoEvent::TempoEvent(intmax_t t)
{
	tempo = t;
}

intmax_t TempoEvent::getTempo() const
{
	return this->tempo;
}

NoteEvent::NoteEvent(bool on, int8_t notenum, int8_t notevelocity) : on_off(on), number(notenum), velocity(notevelocity) { }

bool NoteEvent::getNoteOn()
{
	return on_off;
}

int8_t NoteEvent::getNoteNumber() const
{
	return number;
}

int8_t NoteEvent::getNoteVelocity() const
{
	return velocity;
}

MIDIEvent::MIDIEvent(TempoEvent e, intmax_t ticktime)
{
	this->eventType = TEMPO_EVENT;
	currentEvent.myTempoEvent = e;
	this->tickTime = ticktime;

}

MIDIEvent::MIDIEvent(EndTrackEvent e, intmax_t ticktime)
{
	this->eventType = END_EVENT;
	currentEvent.myEndTrackEvent = e;
	this->tickTime = ticktime;
}

MIDIEvent::MIDIEvent(NoteEvent e, intmax_t ticktime)
{
	this->eventType = NOTE_EVENT;
	currentEvent.myNoteEvent = e;
	this->tickTime = ticktime;

}

bool MIDIEvent::isTempoEvent() const
{
	return eventType == TypeOfEvent::TEMPO_EVENT;
}

bool MIDIEvent::isEndTrackEvent() const
{

	return eventType == TypeOfEvent::END_EVENT;
}

bool MIDIEvent::isNoteEvent() const
{
	return eventType == TypeOfEvent::NOTE_EVENT;
}

intmax_t MIDIEvent::getTickTime() const
{
	return this->tickTime;
}

TempoEvent MIDIEvent::asTempoEvent() const
{
	return currentEvent.myTempoEvent;
}

NoteEvent MIDIEvent::asNoteEvent() const
{
	return currentEvent.myNoteEvent;
}
