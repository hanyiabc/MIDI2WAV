// DO NOT EDIT
#include "midi.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <bitset>

MIDIReader::MIDIReader(): status(true){

}

bool MIDIReader::parse(std::istream & ins){

  parseHeader(ins);

  if(status){
    for(int i = 0; i < head.n; ++i){
      parseNextTrack(ins);
      if(!status) break;
    }
  }

  // TODO strictly, signature must be present in the file
  
  return status;
}

TrackListType MIDIReader::getAllTracks(){
  return std::move(tracklist);
}

void MIDIReader::parseHeader(std::istream & instream){

  if(instream.good()){
    // read header
    char magic[4];
    instream.read(reinterpret_cast<std::istream::char_type *>(&magic[0]), 4);
    status = status && instream.good();
    status = status && ((magic[0] == 'M') && (magic[1] == 'T') &&
			(magic[2] == 'h') && (magic[3] == 'd'));
    head.length = read_big32(instream);
    head.format = read_big16(instream);
    head.n = read_big16(instream);
    head.division = read_big16(instream);

    // format 2 files unsupported
    status = status && (head.format < 2);

    // Tempo in SMPTE format unsupported
    std::bitset<16> b(head.division);
    status = status && !b.test(15);
  }
  else{
    status = false;
  }

}

void MIDIReader::parseNextTrack(std::istream & instream){

  // allocate a track in the list
  tracklist.emplace_back(head.division);
  Track& trk = tracklist.back();

  bool track_end = false;
  
  char magic[4];
  instream.read(reinterpret_cast<std::istream::char_type *>(&magic[0]), 4);
  status = status && instream.good();
  status = status && ((magic[0] == 'M') && (magic[1] == 'T') &&
		      (magic[2] == 'r') && (magic[3] == 'k'));

  
  // use this to verify the number of bytes read is correct
  int32_t track_length = read_big32(instream);

  // save stream position
  std::istream::pos_type startpos = instream.tellg();
  
  // ticks tracks the number of MIDI ticks accumulated since the
  // track started
  intmax_t ticks = 0;

  // flag used for running status;
  uint8_t notestatus = false;
    
  do{
      
    ticks += read_variable_length(instream);
	
    uint8_t eventcode = read_byte(instream);
    
    uint8_t data1;
    uint8_t data2;
	
    track_end = false;
	
    if(eventcode == 0xFF){ // META event
      uint8_t code = read_byte(instream);

      // if at track end, add event and stop parsing
      if(code == 0x2F){
	track_end = true;
	trk.addEndEvent(ticks);
      }
      
      intmax_t length = read_variable_length(instream);
      
      if(code == 0x51){ // tempo
	intmax_t tempo = read_tempo(instream);
	trk.addTempoEvent(tempo, ticks);
      }
      else if(code == 0x58){ // time signature
	read_signature(instream);
	// TODO: For now we assume 4/4 time
      }
      else{
	// TODO: handle others like instrument name, etc.,
	//       for now just skip them
	instream.seekg(length, std::ios_base::cur);
      }
    }
    else if(eventcode == 0xF0){ // sysex events not supported.
      status = false;
      break;
    }
    else if(eventcode >= 0x80){ // MIDI event

      notestatus = eventcode >> 4;
      
      data1 = read_byte(instream);
      
      if((notestatus != 0xC) && (notestatus != 0xD)){
	data2 = read_byte(instream);
	trk.addNoteEvent(notestatus, data1, data2, ticks);
      }
    }
    else{ // running status
      
      data1 = eventcode;
      
      if((notestatus != 0xC) && (notestatus != 0xD)){
	data2 = read_byte(instream);
	trk.addNoteEvent(notestatus, data1, data2, ticks);      
      }

    }
    
  } while(!track_end && instream.good());

  // check stream pos - startpos is equal to expected track length
  std::istream::pos_type endpos = instream.tellg();
  status = status && ((endpos - startpos) == track_length);

}

// read bigendian bytes as long as most-signifigant bit is 1,
// using lower 7 bits
intmax_t MIDIReader::read_variable_length(std::istream & instream){

  auto startpos = instream.tellg();
  
  intmax_t value;
  uint8_t temp;
  
  instream.read(reinterpret_cast<std::istream::char_type *>(&temp), 1);
  status = status && instream.good();
  
  value = temp;

  if(value & 0x80){
    value &= 0x7f;
    do{
      instream.read(reinterpret_cast<std::istream::char_type *>(&temp), 1);
      value = (value << 7) + (temp & 0x7f);
    }while(temp & 0x80);
  }

  // check we did not read more bytes than is in intmax_t
  auto endpos = instream.tellg();
  status = status && ((endpos - startpos) <
                      static_cast<std::istream::pos_type>(sizeof(intmax_t)));

  return value;
}


int32_t MIDIReader::read_big32(std::istream &instream) {
  std::istream::char_type raw[4];
  instream.read(reinterpret_cast<std::istream::char_type *>(&raw[0]), 4);
  status = status && instream.good();
  uint8_t data0 = raw[0];
  uint8_t data1 = raw[1];
  uint8_t data2 = raw[2];
  uint8_t data3 = raw[3];
  int32_t temp = (data0 << 24) | (data1 << 16) | (data2 << 8) | (data3 << 0);
  return temp;
}

int16_t MIDIReader::read_big16(std::istream &instream) {
  std::istream::char_type raw[2];
  instream.read(reinterpret_cast<std::istream::char_type *>(&raw[0]), 2);
  status = status && instream.good();
  uint8_t data0 = raw[0];
  uint8_t data1 = raw[1];
  int16_t temp = (data0 << 8) | (data1 << 0);
  return temp;
}

uint8_t MIDIReader::read_byte(std::istream & instream){
  uint8_t temp;
  instream.read(reinterpret_cast<std::istream::char_type *>(&temp), 1);
  status = status && instream.good();
  return temp;
}


// tempo is 3 bytes
intmax_t MIDIReader::read_tempo(std::istream & instream){
  std::istream::char_type raw[3];
  instream.read(reinterpret_cast<std::istream::char_type *>(&raw[0]), 3);
  status = status && instream.good();
  uint8_t data0 = raw[0];
  uint8_t data1 = raw[1];
  uint8_t data2 = raw[2];
  int32_t temp = (data0 << 16) | (data1 << 8) | (data2 << 0);
  
  return temp;
}

// signature is 4 bytes
void MIDIReader::read_signature(std::istream & instream){
  std::istream::char_type raw[4];
  instream.read(reinterpret_cast<std::istream::char_type *>(&raw[0]), 4);
  status = status && instream.good();
}


TrackListType readMIDIFromFile(const std::string & infilename){
  
  std::ifstream ifs(infilename, std::ios::binary);

  MIDIReader reader;

  if(reader.parse(ifs)){
    std::cout << "Parsing MIDI file succeeded.\n";
  }
  else{
    std::cout << "Parsing MIDI file failed.\n";
  }

  return reader.getAllTracks();
}

