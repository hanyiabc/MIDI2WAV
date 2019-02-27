// DO NOT EDIT
// Simple MIDI reader
// Limitations: - only 1 channel (0) per track is supported
//              - only format 0 or 1 files supported
//              - sysex events not supported
//              - Tempo in SMPTE format unsupported.
//              - Signature is assumed to be 4/4 time
#ifndef MIDI_HPP
#define MIDI_HPP

#include <cstdint>
#include <string>
#include <istream>

#include "track.hpp"

/** Read MIDI file and parse into a list of tracks. 
 */
TrackListType readMIDIFromFile(const std::string & infilename);

/** 
 * 
 */
class MIDIReader{
public:

  /** Construct a reader object. */
  MIDIReader();

  /** Parse from a stream. Returns true on success, false on failure.
   * After this successfully completes the tracks are available through
   * a call to getAllTracks().
   * \param ins input stream to parse from
   */
  bool parse(std::istream & ins);

  /** Get all the tracks available from a previous call to parse.
   */
  TrackListType getAllTracks();

private:

  // status of file IO and constraints during parsing
  bool status;

  // TODO (clwyatt): Add an error message log?
  
  TrackListType tracklist;
  
  void parseHeader(std::istream & ifs);
  
  void parseNextTrack(std::istream & ifs);
  
  struct Header {
    int32_t length; 
    int16_t format;
    int16_t n;
    int16_t division;
  } head;

  intmax_t read_variable_length(std::istream & ifs);
  int32_t read_big32(std::istream &ifs);
  int16_t read_big16(std::istream &ifs);
  uint8_t read_byte(std::istream & ifs);
  intmax_t read_tempo(std::istream & ifs);
  void read_signature(std::istream & ifs);

};

#endif
