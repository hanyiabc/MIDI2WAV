/** \file The InteractiveInstrument Module
 */
#define _USE_MATH_DEFINES
#ifndef INTERACTIVE_INSTRUMENT_HPP
#define INTERACTIVE_INSTRUMENT_HPP

#include "event.hpp"
#include "track.hpp"
#include <fstream>
#include <qmessagebox.h>
#include <vector>
#include "event.hpp"
#include "track.hpp"
#include <map>
#include <cmath>

using std::ofstream;
using std::map;
using std::vector;

/** The interactive instrument.
 */
class InteractiveInstrument{
public:

  /** No default constructor. A InteractiveInstrument must have an associated 
   * track. 
   */
  InteractiveInstrument() = delete;

  /** Cannot be copied. 
   */
  InteractiveInstrument(const InteractiveInstrument & ) = delete;

  /** Cannot be assigned.
   */
  InteractiveInstrument & operator=(const InteractiveInstrument & ) = delete;

  /** Construct a Interactive Instrument using the specified track.
   * \param track the track to read events from. 
   */
  InteractiveInstrument(const Track & track);

  /** Returns true of the InteractiveInstrument has stopped processing events.
   * This can occu e.g. if all events have been consumed or a EndTrackEvent
   * has been processed.
   */
  bool halted();

  /** Enables the addition of a first harmonic to the instrument notes 
   * \param value true if enabled, false if not
   */
  void enableFirstHarmonic(bool value);

  /** Enables the addition of a second harmonic to the instrument notes 
   * \param value true if enabled, false if not
   */
  void enableSecondHarmonic(bool value);

  /** Sets the velocity of the notes.
   * \param value the new velocity
   */
  void setVelocity(double value);

  /** Sets the attack time of the envelope function.
   * \param value the new attack time
   */
  void setAttackTime(double value);

  /** Sets the decay time of the envelope function.
   * \param value the new decay time
   */
  void setDecayTime(double value);

  /** Sets the release time of the envelope function.
   * \param value the new release time
   */
  void setReleaseTime(double value);

  /** Resets the instrument to a default state.*/
  void reset();
  
  /** Return the sample after the (real) time increment. Samples the 
   * instrument based on its state after processing all events prior 
   * to this time.
   * \param deltaT the time increment in seconds.
   */
  double sample(double deltaT);

private:
	struct NoteStates
	{
		double t;
		intmax_t numT;
		double endT;
		NoteEvent note;
		const intmax_t startTick;
		/*NoteType(double a, intmax_t b, double c, NoteEvent d) : t(a), numT(b), endT(c), note(d) {}*/
		NoteStates(NoteEvent d, intmax_t start) : note(d), startTick(start)
		{	
			t = 0.0;
			numT = 0;
			endT = 99999999999.0;
		}
	};

	//typedef std::pair<Time, NoteEvent> NoteType;

	double getFreq(int8_t code);
	double getEnvelope(NoteStates note);
	//typedef std::pair<
	bool ended;
	Track myTrack;
	long double t;
	intmax_t tempo;
	long double deltaTicks;
	intmax_t TPB;
	intmax_t numTimes;
	ConstIteratorType it;
	vector<NoteStates> currentNotes;
	//vector<NoteType> states;
	bool enabled1 = false;
	bool enabled2 = false;
	double Ta;
	double Velocity;
	double Td;
	double Tr;

};


#endif
