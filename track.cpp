#include "track.hpp"

Track::Track(intmax_t ticks_per_beat) : TPB(ticks_per_beat) {}

intmax_t Track::getTicksPerBeat() const
{
	return TPB;
}

ConstIteratorType Track::begin() const
{
	return eventList.begin();
}

ConstIteratorType Track::end() const
{
	return eventList.end();
}

void Track::addEndEvent(intmax_t tick_time)
{
	eventList.push_back(MIDIEvent(EndTrackEvent(), tick_time));
}

void Track::addTempoEvent(intmax_t tempo, intmax_t tick_time)
{
	eventList.push_back(MIDIEvent(TempoEvent(tempo), tick_time));
}

void Track::addNoteEvent(uint8_t code, uint8_t data1, uint8_t data2, intmax_t tick_time)
{
	bool status = false;
	if ((code == 8))
	{
		status = false;
	}
	else if (code == 9)
	{
		if (data2 == 0)
		{
			status = false;
		}
		else
		{
			status = true;
		}
	}
	eventList.push_back(MIDIEvent(NoteEvent(status, data1, data2), tick_time));
}
