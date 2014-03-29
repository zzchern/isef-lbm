#pragma once
#include <inttypes.h>

struct WAV_Header
{
    char chunkId[4]; 		// "RIFF"
	uint32_t chunkSize;     // size of file
    char format[4]; 		// "WAVE"
    char subchunk1Id[4]; 	// "ftm"
    uint32_t subchunk1Size;
    uint16_t audioFormat;	// PCM = 1
    uint16_t numChannels; 	// Number of channels (mono, stereo, etc.)
    uint32_t sampleRate;		// 8000 Hz, 44100 Hz, etc.
    uint32_t byteRate;			
    uint16_t blockAlign;	// Bytes for sample (all chanells)
    uint16_t bitsPerSample;	// Depth.
    char subchunk2Id[4];	// "data"
    uint32_t subchunk2Size;	// size of data
};

class Codec
{
public:
		unsigned long  num_of_samples(const char* file);
		void wav_to_array(float* array, const char* file);
		void array_to_wav(float* array, const char* file, 
						  unsigned long samples, unsigned long sample_rate);
		void info(const char* file);
private:
		void copy(unsigned short* A, float* B, unsigned long samples);
		void copy(float* A, unsigned short* B, unsigned long samples);
		void get_header(WAV_Header &header, unsigned long sample_rate, 
						unsigned long samples);
};

