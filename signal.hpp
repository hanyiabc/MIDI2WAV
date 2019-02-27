#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <cstdint>
#include <cstddef>
#include <vector>

/** \brief A Signal holds equally spaceed samples of type double.
*/
class Signal
{
public:

	/** Construct a Signal given a sampling rate is samples-per-second (Hz). The default rate is
	* 44.1 kHz.
	* \param sampleRate the sampling rate in Hz
	*/
	Signal(unsigned int sampleRate = 44100);

	/** return the sampling rate in Hz.*/
	unsigned int getSampleRate();

	/** Normalize the Signal to fit into a 16 bit integer
	* when quantized.
	*/
	void normalize();

	/** Add a sample to the signal.
	* \param value the double sample value to add.
	*/
	void push_back(double value);

	/** Return the number of samples in the signal. */
	std::size_t size();

	/** Return the sample at given index.
	* \param index the index of the sample to return.
	*/
	double operator[](std::size_t index);


private:
	std::vector<double> data;
	unsigned int sampleRate;
};


#endif
