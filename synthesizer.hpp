/** \file Synthesizer Module
 */
#ifndef SYNTHESIZER_HPP
#define SYNTHESIZER_HPP

#include "message_queue.hpp"
#include "track.hpp"
#include <string>
#include "shared_double_buffer.hpp"

using std::string;

/** \fn Synthesize a Track into a Signal.
 * Returned signal is empty on failure.
 * \param track the track to synthesize.
 */
class Synthesizer
{
public:
	Synthesizer();
	Synthesizer(MessageQueue * queuePtr,Track * track, DoubleBuffer * buffer ,int id);
	void operator()() const;

private:
	Track * track;
	DoubleBuffer * buffer;
	int id;
	MessageQueue * mq;
};



#endif
