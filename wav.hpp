/** \file WAV Module
 */

#ifndef WAV_HPP
#define WAV_HPP

#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <math.h>
#include <string>
#include "signal.hpp"

using std::ifstream;
using std::int16_t;
using std::int32_t;
using std::int8_t;
using std::ofstream;
using std::vector;
using std::string;
using std::getline;
using std::stof;
using std::cerr;
using std::cout;

/** \fn Write a signal to a WAV file. Returns true on success, false on failure.
 * The signal should be quantized into a 16 bit integer using rounding to nearest.
 * \param sound the signal to write.
 * \param filename the filename to write to.
 */
bool write_wav(Signal & sound, const std::string &filename);

#endif
