#include "synthesizer.hpp"
#include "instrument.hpp"
// TODO

Synthesizer::Synthesizer()
{
}

Synthesizer::Synthesizer(MessageQueue * queuePtr, Track * track, DoubleBuffer * buffer, int id)
{
	this->track = track;
	mq = queuePtr;
	this->buffer = buffer;
	this->id = id;
	
}

void Synthesizer::operator()() const
{
	while (true)
	{
		DefaultInstrument * instrument = nullptr;
		int sampleRate;
		double inc;
		string m = " ";
		mq->wait_and_pop(m);
		if (m[0] == 'S')
		{
			sampleRate = std::atoi(m.substr(1).c_str());
			inc = 1.0 / sampleRate;
			instrument = new DefaultInstrument(*track);
			while (true)
			{
				if (!buffer->isFull())
				{
					double sample;
					if (instrument->halted())
					{
						sample = 0.0;
					}
					else
					{
						sample = instrument->sample(inc);
					}
					buffer->push_back(sample);
				}
				if (mq->try_pop(m))
				{
					if (m[0] == 'E'|| m[0]=='Q')
					{
						//instrument->reset();
						buffer->clear();
						if (instrument != nullptr)
						{
							delete instrument;
						}
						break;
					}
				}

			}


		}
		else if (m[0]=='Q')
		{
			break;
		}
	}
	
}
