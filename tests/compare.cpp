// compare two wav files
// - check header similarity
// - cross correlation between two signals
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// allowed percent difference in sample lengths
const double MAX_LENGTH_DIFF = 0.001;

// allowed difference in cross-correlation coefficient
const double XCORR_THRESH = 0.9;

struct WAV
{
  int8_t   ChunkID[4];     //"RIFF"
  int32_t  ChunkSize;      // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
  int8_t   Format[4];      // "WAVE"

  int8_t   Subchunk1ID[4]; // "fmt "
  int32_t  Subchunk1Size;  // 16
  int16_t  AudioFormat;    // 1
  int16_t  NumChannels;    // Mono = 1
  int32_t  SampleRate;     // samples per second, 44100
  int32_t  ByteRate;       // SampleRate * NumChannels * BitsPerSample/8
  int16_t  BlockAlign;     // NumChannels * BitsPerSample/8
  int16_t  BitsPerSample;  // 8 bits = 8, 16 bits = 16

  int8_t   Subchunk2ID[4]; // "data"
  int32_t  Subchunk2Size;  // NumSamples * NumChannels * BitsPerSample/8
  int16_t* Data;           // the Pulse Code Modulated data
};

bool read_wav(std::vector<double> & sound, const std::string &filename)
{
  std::ifstream infile(filename.c_str(), std::ios::binary);
  if(!infile){
    return false;
  }

  // read header
  WAV header;
  infile.read((char *)&header.ChunkID, sizeof(header.ChunkID));
  if((header.ChunkID[0] != 'R') || (header.ChunkID[1] != 'I') ||
     (header.ChunkID[2] != 'F') || (header.ChunkID[3] != 'F')){
    return false;
  }
    
  infile.read((char *)&header.ChunkSize, sizeof(header.ChunkSize));

  infile.read((char *)&header.Format, sizeof(header.Format));
  if((header.Format[0] != 'W') || (header.Format[1] != 'A') ||
     (header.Format[2] != 'V') || (header.Format[3] != 'E')){
    return false;
  }
  
  infile.read((char *)&header.Subchunk1ID, sizeof(header.Subchunk1ID));
  if((header.Subchunk1ID[0] != 'f') || (header.Subchunk1ID[1] != 'm') ||
     (header.Subchunk1ID[2] != 't') || (header.Subchunk1ID[3] != ' ')){
    return false;
  }
  
  infile.read((char *)&header.Subchunk1Size, sizeof(header.Subchunk1Size));
  if(header.Subchunk1Size != 16) return false;
  
  infile.read((char *)&header.AudioFormat, sizeof(header.AudioFormat));
  if(header.AudioFormat != 1) return false;
    
  infile.read((char *)&header.NumChannels, sizeof(header.NumChannels));
  if(header.NumChannels != 1) return false;
    
  infile.read((char *)&header.SampleRate, sizeof(header.SampleRate));
  if(header.SampleRate != 44100) return false;
    
  infile.read((char *)&header.ByteRate, sizeof(header.ByteRate));
  infile.read((char *)&header.BlockAlign, sizeof(header.BlockAlign));
  infile.read((char *)&header.BitsPerSample, sizeof(header.BitsPerSample));
  if(header.BitsPerSample != 16) return false;
  
  infile.read((char *)&header.Subchunk2ID, sizeof(header.Subchunk2ID));
  if((header.Subchunk2ID[0] != 'd') || (header.Subchunk2ID[1] != 'a') ||
     (header.Subchunk2ID[2] != 't') || (header.Subchunk2ID[3] != 'a')){
    return false;
  }
  infile.read((char *)&header.Subchunk2Size, sizeof(header.Subchunk2Size));

  // read the data
  for(intmax_t i = 0; i < header.Subchunk2Size; i+= 2){
    int16_t temp;
    infile.read((char *)&temp, 2);
    double sample = temp;
    sound.push_back(sample);
  }
  
  return true;
}

void stats(std::vector<double> & s, double &mean, double &std){

  std::vector<double>::size_type sz = s.size();
  mean = 0;
  for( auto sample : s){
    mean += sample;
  }
  mean = mean/static_cast<double>(sz);

  double var = 0;
  for( auto sample : s){
    double diff = sample - mean; 
    var += diff*diff;
  }
  var = var/static_cast<double>(sz);

  std = sqrt(var);
}

double xcorr(std::vector<double> & s1, std::vector<double> & s2){

  double mean1, mean2, std1, std2;
  stats(s1, mean1, std1);
  stats(s2, mean2, std2);
    
  std::vector<double>::size_type shortest = std::min(s1.size(), s2.size());

  double coeff = 0;
  for(std::size_t i = 0; i < shortest; ++i){
    double temp1 = s1[i] - mean1;
    double temp2 = s2[i] - mean2;
    coeff += temp1*temp2;
  }
  coeff = coeff/static_cast<double>(shortest);
  coeff = coeff/(std1*std2);
  
  return coeff;
}

	     
int main(int argc, char *argv[])
{
  if(argc != 3){
    std::cerr << "Error: two file arguments expected." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<double> data1;
  if(!read_wav(data1, argv[1])){
    std::cerr << "Error: cannot read file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<double> data2;
  if(!read_wav(data2, argv[2])){
    std::cerr << "Error: cannot read file " << argv[2] << std::endl;
    return EXIT_FAILURE;
  }

  // length of the two signals cannot differ by more than MAX_LENGTH_DIFF percent
  double longest = std::max(data1.size(), data2.size());
  double len_diff = fabs(static_cast<double>(data1.size()) - static_cast<double>(data2.size()))/longest;
  if(len_diff > MAX_LENGTH_DIFF){
    std::cerr << "Failure: file lengths differ by " << len_diff << " percent." << std::endl;
    return EXIT_FAILURE;
  }

  double xc = xcorr(data1, data2);

  if(xc < XCORR_THRESH){
    std::cerr << "Failure: signal cross-correlation " << xc << " < " << XCORR_THRESH << std::endl;
    return EXIT_FAILURE;
  }
  
  std::cout << "Files similar.\n";
  
  return EXIT_SUCCESS;
}
