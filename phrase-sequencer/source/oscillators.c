/*
 * =====================================================================================
 *
 *       Filename:  oscillators.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07-03-19 00:05:11
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bram Giesen (), bram@moddevices.com
 *   Organization:  MOD Devices
 *
 * =====================================================================================
 */

#include "oscillators.h"

//TODO make division compatible 
float reCalcPhase(int bpm, float beatInMeasure, float sampleRate, float divisions)
{
    float newPhase = fmod((60.0f / bpm) * beatInMeasure, (sampleRate * (60.0f / (bpm * (divisions / 2.0f)))));

    return newPhase;
}


//this is a LFO to use for timing of the beatsync
double sinOsc(float frequency, double* phase, float rate)
{
  *phase += frequency / rate;

  double output = sin(*phase * PI_2);

  if(*phase >= 1){ 
    *phase = *phase - 1;
  }

  return output;
}


//this is a LFO to use for timing of the beatsync
double* phaseOsc(float frequency, double* phase, float rate, float swing)
{
  *phase += frequency / rate;

  if(*phase >= 1){ 
    *phase = *phase - 1;
  }

  return phase;
}


//this is a LFO to use for timing of the beatsync
double* phaseRecord(float frequency, double* phase, float rate)
{
  *phase += frequency / rate;

  return phase;
}
