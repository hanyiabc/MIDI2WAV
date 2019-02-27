#include "interactive_instrument.hpp"
#include <iostream>
#include <cmath>

using std::cout;
using std::vector;

const double T0 = 0.0;
const double S = 0.8;
const double F = 349.63;

InteractiveInstrument::InteractiveInstrument(const Track & track)
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
	cout << "deltaTick = " << deltaTicks << "\n";

}

bool InteractiveInstrument::halted()
{
	if (ended)
	{
		cout << "End of Track\n";
	}
	return ended;
}

void InteractiveInstrument::enableFirstHarmonic(bool value)
{
	enabled1 = value;
}

void InteractiveInstrument::enableSecondHarmonic(bool value)
{
	enabled2 = value;
}

void InteractiveInstrument::setVelocity(double value)
{
	Velocity = value;
}

void InteractiveInstrument::setAttackTime(double value)
{
	Ta = value;
}

void InteractiveInstrument::setDecayTime(double value)
{
	Td = value;
}

void InteractiveInstrument::setReleaseTime(double value)
{
	Tr = value;
}

void InteractiveInstrument::reset()
{
	this->currentNotes.clear();
	this->ended = false;
	this->it = myTrack.begin();
	this->t = 0.0;
	this->numTimes = 0;
}

double InteractiveInstrument::sample(double deltaT)
{
	while (((t - deltaT) <= (deltaTicks * (double)it->getTickTime())) && ((deltaTicks * (double)it->getTickTime()) <= t))//if tick time is inbetween t and last sample t
																														 //process event
	{

		cout << "TickTime = " << it->getTickTime() << "\t";
		cout << "Ideal Time =" << it->getTickTime()*deltaTicks << "\t";

		if (it->isEndTrackEvent())
		{
			cout << "at t= " << this->t << "\t";
			cout << "EndTrackEvent\n";
			this->ended = true;
		}
		else if (it->isNoteEvent())
		{
			if (it->asNoteEvent().getNoteOn())
			{
				cout << "at t= " << this->t << "\t";
				cout << "NoteOn, V = " << (int)it->asNoteEvent().getNoteVelocity() << "\t# = " << (int)it->asNoteEvent().getNoteNumber() << "\n";
				//add note to container
				currentNotes.push_back(NoteStates(it->asNoteEvent(), it->getTickTime()));

			}
			else
			{
				cout << "at t= " << this->t << "\t";
				cout << "NoteOff, V = " << (int)it->asNoteEvent().getNoteVelocity() << "\t# = " << (int)it->asNoteEvent().getNoteNumber() << "\n";
				for (size_t i = 0; i < currentNotes.size(); i++)
				{
					if (currentNotes[i].note.getNoteNumber()==it->asNoteEvent().getNoteNumber())
					{
						currentNotes[i].endT = deltaTicks * ( (double)it->getTickTime() - currentNotes[i].startTick);
						break;
					}
				}	
				//set the note off time

			}
			//this->getFreq(it->asNoteEvent().getNoteNumber());
		}
		else if (it->isTempoEvent())
		{
			cout << "at t= " << this->t << "\t";
			cout << "TempoEvent Tempo=" << it->asTempoEvent().getTempo() << "\n";
			it->asTempoEvent().getTempo();
			this->tempo = it->asTempoEvent().getTempo();
			//calculate deltaTicks
			double temp = (double)tempo / 1000000.0;
			this->deltaTicks = temp / (double) this->TPB;
			cout << "deltaTick = " << deltaTicks << "\n";
		}
		it++;
		if (it == myTrack.end())
		{
			this->ended = true;
			break;
		}
	}
	double result = 0.0;

	//use envelope func and sample func to get sample
	for (auto itNotes = currentNotes.begin(); itNotes != currentNotes.end(); itNotes++)
	{
		double temp;
		if (enabled1 && enabled2)
		{
			temp = (1.0/3.0) * (std::sin(this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) + std::sin(2.0 * this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) + std::sin(3.0 * this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) );
		}
		else if (!enabled1 && !enabled2)
		{
			temp = std::sin(this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t);
		}
		else if (enabled1 && !enabled2)
		{
			temp = (1.0/2.0) * (std::sin(this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) + std::sin(2.0 * this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) );
		}
		else if(!enabled1 && enabled2)
		{
			temp = (1.0 / 2.0) *( std::sin(this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) + std::sin(3.0 * this->getFreq(itNotes->note.getNoteNumber()) * 2.0 * M_PI * itNotes->t) );
		}
		
		result += ( getEnvelope(*itNotes)  * Velocity * temp);
		itNotes->numT++;
		itNotes->t = deltaT * itNotes->numT;
	}
	numTimes++;
	t = deltaT * numTimes;
	//USE dealtaT * number of called time
	return result;
}

double InteractiveInstrument::getFreq(int8_t code)
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

double InteractiveInstrument::getEnvelope(NoteStates states)
{
	double result;
	if (states.t >= T0 && Ta >= states.t)
	{
		result = (1.0 / Ta) * states.t;
	}
	else if (states.t > Ta && states.t <= (Ta+Td))
	{
		result = 1.0 - (states.t - Ta);
	}
	else if (states.t > (Ta + Td) && states.t <= states.endT)
	{
		result = S;
	}
	else if (states.t > states.endT && states.t <= (states.endT + Tr) )
	{
		result = S - ((S / ((states.endT + Tr) - states.endT)) * (states.t - states.endT));
	}
	else
	{
		result = 0;
	}
	return result;
}
