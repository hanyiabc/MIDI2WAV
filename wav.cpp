#include "wav.hpp"
// TODO

bool write_wav(Signal & sound, const std::string & filename)
{
	ofstream out(filename, std::ios::binary);
	if (out.fail())
	{
		return false;
	}
	int8_t	ChunkID[4];        // "RIFF"
	int32_t	ChunkSize;         // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	int8_t	Format[4];         // "WAVE"
	int8_t	Subchunk1ID[4];   // "fmt "
	int32_t	Subchunk1Size;    // 16
	int16_t	AudioFormat;      // 1
	int16_t	NumChannels;      // Mono = 1
	int32_t	SampleRate;       // samples per second, 44100
	int32_t	ByteRate;         // SampleRate * NumChannels * BitsPerSample/8
	int16_t	BlockAlign;       // NumChannels * BitsPerSample/8
	int16_t	BitsPerSample;    // 8 bits = 8, 16 bits = 16
	int8_t	Subchunk2ID[4];   // "data"
	int32_t	Subchunk2Size;    // NumSamples * NumChannels * BitsPerSample/8

	ChunkID[0] = 'R';
	ChunkID[1] = 'I';
	ChunkID[2] = 'F';
	ChunkID[3] = 'F';

	Format[0] = 'W';
	Format[1] = 'A';
	Format[2] = 'V';
	Format[3] = 'E';

	Subchunk1ID[0] = 'f';
	Subchunk1ID[1] = 'm';
	Subchunk1ID[2] = 't';
	Subchunk1ID[3] = ' ';

	Subchunk1Size = 16;

	AudioFormat = 1;

	NumChannels = 1;

	SampleRate = 44100;

	BitsPerSample = 16;

	Subchunk2ID[0] = 'd';
	Subchunk2ID[1] = 'a';
	Subchunk2ID[2] = 't';
	Subchunk2ID[3] = 'a';

	auto NumSamples = sound.size();

	ByteRate = (SampleRate * NumChannels *BitsPerSample) / 8;

	BlockAlign = (NumChannels * BitsPerSample) / 8;

	Subchunk2Size = (NumSamples * NumChannels * BitsPerSample) / 8;

	ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size);
	////uint32_t value = 0;
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkID), sizeof ChunkID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ChunkSize), sizeof ChunkSize);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Format), sizeof Format);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1ID), sizeof Subchunk1ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1Size), sizeof Subchunk1Size);
	out.write(reinterpret_cast<std::fstream::char_type*>(&AudioFormat), sizeof AudioFormat);
	out.write(reinterpret_cast<std::fstream::char_type*>(&NumChannels), sizeof NumChannels);
	out.write(reinterpret_cast<std::fstream::char_type*>(&SampleRate), sizeof SampleRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&ByteRate), sizeof ByteRate);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BlockAlign), sizeof BlockAlign);
	out.write(reinterpret_cast<std::fstream::char_type*>(&BitsPerSample), sizeof BitsPerSample);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2ID), sizeof Subchunk2ID);
	out.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2Size), sizeof Subchunk2Size);
	sound.normalize();
	for (size_t i = 0; i < sound.size(); i++)
	{
		int16_t temp = (int16_t)sound[i];
		out.write(reinterpret_cast<std::fstream::char_type*>(&temp), sizeof temp);
	}
	cout << "Finished writing wave file!\n";
	return true;
}
