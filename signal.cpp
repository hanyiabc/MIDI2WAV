#include "signal.hpp"
#include <limits>

// TODO

Signal::Signal(unsigned int sampleRate)
{
	this->sampleRate = sampleRate;
}

unsigned int Signal::getSampleRate()
{
	return sampleRate;
}

void Signal::normalize()
{
	auto flag = false;
	double max = 0.0;
	for (size_t i = 0; i < this->data.size(); i++)
	{
		if (data[i] > std::numeric_limits<int16_t>::max())
		{
			flag = true;
		}
		if (data[i]>max)
		{
			max = data[i];
		}
	}
	if (flag)
	{
		double ratio = std::numeric_limits<int16_t>::max() / max;
		for (size_t i = 0; i < data.size(); i++)
		{
			data[i] *= ratio;
		}
	}
}

void Signal::push_back(double value)
{
	this->data.push_back(value);
}

std::size_t Signal::size()
{
	return this->data.size();
}

double Signal::operator[](std::size_t index)
{
	return this->data[index];
}
