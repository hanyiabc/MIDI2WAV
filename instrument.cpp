#include "instrument.hpp"
#include <cmath>
#include <algorithm>

using std::vector;

const double T0 = 0.0;
const double T1 = 0.25 * 0.25;
const double T2 = 0.45 * 0.25;
const double T3 = 0.8 * 0.25;
const double T4 = 1 * 0.25;
const double S = 0.8;
const double F = 349.63;
//ofstream out("log.txt");
DefaultInstrument::DefaultInstrument(const Track & track)
{

	myTrack = track;
	this->ended = false;
	this->numTimes = 0;
	it = myTrack.begin();
	this->t = 0;
	this->TPB = myTrack.getTicksPerBeat();
	this->tempo = 500000;
	//calculate deltaTicks
	this->deltaTicks = (double)tempo / 1000000.0 / (double)this->TPB;
}

bool DefaultInstrument::halted()
{
	return ended;
}

double DefaultInstrument::sample(double deltaT)
{

	while (((t - deltaT) <= (deltaTicks * (double)it->getTickTime())) && ((deltaTicks * (double)it->getTickTime()) <= t))//if tick time is inbetween t and last sample t
																														 //process event
	{



		if (it->isEndTrackEvent())
		{

			this->ended = true;
		}
		else if (it->isNoteEvent())
		{
			if (it->asNoteEvent().getNoteOn())
			{

				//add note to container
				currentNotes.push_back(NoteType(Time(), it->asNoteEvent()));

			}
			else
			{

				//delete or turn off note from container

			}
			//this->getFreq(it->asNoteEvent().getNoteNumber());
		}
		else if (it->isTempoEvent())
		{

			it->asTempoEvent().getTempo();
			this->tempo = it->asTempoEvent().getTempo();
			//calculate deltaTicks
			double temp = (double)tempo / 1000000.0;
			this->deltaTicks = temp / this->TPB;

		}
		it++;
		if (it == myTrack.end())
		{
			this->ended = true;
			break;
		}
	}

	currentNotes.erase(std::remove_if(currentNotes.begin(), currentNotes.end(), pred),currentNotes.end());

	double result = 0.0;
	//use envelope func and sample func to get sample
	for (auto itNotes = currentNotes.begin(); itNotes != currentNotes.end(); itNotes++)
	{
			result += getEnvelope(*itNotes) * 200 * itNotes->second.getNoteVelocity() * std::sin(2.0 * this->getFreq(itNotes->second.getNoteNumber()) * M_PI * itNotes->first.t);
			itNotes->first.numT++;
			itNotes->first.t = deltaT * itNotes->first.numT;
	}
	numTimes++;
	t = deltaT * numTimes;
	//USE dealtaT * number of called time
	return result;
}


double DefaultInstrument::getFreq(int8_t code)
{
	switch (code)
	{
	case 5:
		return F / 32.0;
		break;
	case 17:
		return F / 16.0;
		break;
	case 29:
		return F / 8.0;
		break;
	case 41:
		return F / 4.0;
		break;
	case 53:
		return F / 2.0;
		break;
	case 65:
		return F;
		break;
	case 77:
		return F * 2.0;
		break;
	case 89:
		return F * 4.0;
		break;
	case 101:
		return F * 8.0;
		break;
	case 113:
		return F * 16.0;
		break;
	default:
		break;
	}
	double f = 440.0 * pow(2.0, (code - 69) / 12.0);
	return f;
}

double DefaultInstrument::getEnvelope(NoteType note)
{
	double result;
	if (note.first.t >= T0 && T1 >= note.first.t)
	{
		result = (1.0 / T1) * note.first.t;
	}
	else if (note.first.t > T1 && note.first.t <= T2)
	{
		result = 1.0 - (note.first.t - T1);
	}
	else if (note.first.t > T2 && note.first.t <= T3)
	{
		result = S;
	}
	else if (note.first.t > T3 && note.first.t <= T4)
	{
		result = S - ((S / (T4 - T3)) * (note.first.t - T3));
	}
	else
	{
		result = 0;
	}

	return result;
}

bool DefaultInstrument::pred(NoteType note)
{
	return( note.first.t > T4);
}
