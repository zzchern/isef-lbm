/***
    This file is part of 'isef-lbm'. Program for simulation of acoustic
    waves using Lattice Boltzmann Method.
    
    Copyright (C) 2013  Arkadiy Prigojin, Sergey Dukanov

    'isef-lbm' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/ 

#include <stdio.h>
#include "wav_codec.h"
#include <math.h>

unsigned long Codec :: num_of_samples(const char* filename)
{
	size_t result;
	WAV_Header header;
	FILE *file;
	unsigned long samples;
	file = fopen(filename, "rb");
	result = fread(&header, sizeof(WAV_Header), 1, file);
	fclose(file);
	samples =  header.subchunk2Size / sizeof(unsigned short);
	return samples;
}

void Codec :: wav_to_array(float* array, const char* wav)
{
	FILE *file;
	size_t result;
	WAV_Header header;
	unsigned short* data;
	unsigned long samples;
	file = fopen(wav, "rb");
	result = fread(&header, sizeof(WAV_Header), 1, file);
	samples = header.subchunk2Size / sizeof(unsigned short);
	data = new unsigned short[samples];
	result = fread(data, header.subchunk2Size, 1, file);
	fclose(file);
	copy(array, data, samples);
	delete data;
}

void Codec :: array_to_wav(float* array, const char* wav, 
			   unsigned long samples, unsigned long sample_rate)
{
	FILE *file;
	size_t result;
	WAV_Header header;
	unsigned short* data;
	file = fopen(wav, "wb");
	data = new unsigned short[samples];
	get_header(header, sample_rate, samples);
	copy(data, array, samples);
	result = fwrite(&header, sizeof(WAV_Header), 1, file);
	result = fwrite(data, header.subchunk2Size , 1, file);
	delete data;
	fclose(file);
}

void Codec :: copy(unsigned short* A, float* B, unsigned long samples)
{
	unsigned long i;
	float max_number;
	max_number = pow(2, 8*sizeof(unsigned short)) - 1;
	max_number /= 4;
	max_number *= 3;
	// pick up
	float min = 1.0;
	for(i = 0; i < samples; ++i)
		if(B[i] < min)
			min = B[i];
	for(i = 0; i < samples; ++i)
		B[i] -= min;
	// copy
	for(i = 0; i < samples; ++i)
		A[i] = (unsigned short)(max_number * B[i]) - max_number/2;
}

void Codec :: copy(float* A, unsigned short* B, unsigned long samples)
{
	unsigned long i;
	// copy
	for(i = 0; i < samples; ++i)
		A[i] = (float)B[i];
	// normalize
	float max = 0.0;
	for(i = 0; i < samples; ++i)
		if(A[i] > max)
			max = A[i];
	for(i = 0; i < samples; ++i)
		A[i] /= max;
}
		

void Codec :: get_header(WAV_Header &wh, unsigned long sample_rate, 
			 unsigned long samples)
{
	unsigned long numSamples = samples;
	wh.chunkId[0] = 'R';
	wh.chunkId[1] = 'I';
	wh.chunkId[2] = 'F';
	wh.chunkId[3] = 'F';
	wh.format[0] = 'W';
	wh.format[1] = 'A';
	wh.format[2] = 'V';
	wh.format[3] = 'E';
	wh.subchunk1Id[0] = 'f';
	wh.subchunk1Id[1] = 'm';
	wh.subchunk1Id[2] = 't';
	wh.subchunk1Id[3] = ' ';
	wh.audioFormat = 1;
	wh.numChannels = 1;
	wh.sampleRate = sample_rate;
	wh.bitsPerSample = 16;	
	wh.byteRate = wh.sampleRate * wh.numChannels * wh.bitsPerSample / 8;	   
	wh.subchunk2Size = numSamples * wh.numChannels * wh.bitsPerSample / 8;
	wh.subchunk1Size = 16;
	wh.blockAlign = wh.numChannels * wh.bitsPerSample / 8;
	wh.chunkSize = 36 + wh.subchunk2Size;
	wh.subchunk2Id[0] = 'd';
	wh.subchunk2Id[1] = 'a';
	wh.subchunk2Id[2] = 't';
	wh.subchunk2Id[3] = 'a';
}

void Codec :: info(const char* filename)
{
	FILE* file;
	size_t result;
	WAV_Header header;
	int duration;
	file = fopen(filename, "rb");
	result = fread(&header, sizeof(WAV_Header), 1, file);
	printf("----------WAV file information---------\n");
	printf("File           : %s\n", filename); 	    
	printf("Size of header : %d\n", sizeof(header));
	printf("Size of file   : %d\n", int(header.chunkSize));
	printf("Size of data   : %d\n", 
	       int(header.chunkSize - sizeof(header)));
	printf("Sample rate    : %d\n", int(header.sampleRate));
	printf("Channels       : %d\n", int(header.numChannels));
	printf("Bits per sample: %d\n", int(header.bitsPerSample));
	printf("---------------------------------------\n");
	fclose(file);
}

void printHead(WAV_Header header)
{
	printf("ChunkID[4] = %s\n", header.chunkId); 		// "RIFF"
	printf("ChunkSize = %d\n", header.chunkSize);     // size of f
	printf("Format[4] = %s\n", header.format); 		// "WAVE"
	printf("SubChunk1Id[4] = %s\n", header.subchunk1Id); 	// "ftm"
	printf("SubChunk1Size = %d\n", header.subchunk1Size);
	printf("AudioFromat = %d\n", header.audioFormat);	// PCM = 1
	printf("Channels = %d\n", header.numChannels); 	
	printf("SampleRate = %d\n", header.sampleRate);	       
	printf("ByteRate = %d\n", header.byteRate);			
	printf("BlockAlign = %d\n", header.blockAlign);       
	printf("BitsPerSample = %d\n", header.bitsPerSample);	// Depth.
	printf("SubChunk2Id[4] = %s\n", header.subchunk2Id);	// "data"
	printf("SubChunk2Size = %d\n", header.subchunk2Size);	// size of data
}
