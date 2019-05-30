/*
 * =====================================================================================
 *
 *       Filename:  sequencer_utils.h
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  12/24/2018 03:39:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bram Giesen (), bram@moddevices.com
 *   Organization:  MOD Devices
 *
 * =====================================================================================
 */
#ifndef _H_SEQ_UTILS_
#define _H_SEQ_UTILS_

#include "structs.h"
#include "oscillators.h"

float     reCalcPhase(int bpm, float beatInMeasure, float sampleRate, float divisions);
float     calculateFrequency(uint8_t bpm, float division);
float*    envelope(Data *self, float *amplitude);
void      precount(Data *self);
int       barCounter(Data *self, uint8_t recordingLength);
void      clearSequence(EventList *arr);
void      recordNotes(Data *self, uint8_t midiNote, uint8_t noteType, float notePos);
EventList quantizeNotes(EventList events);
EventList mergeEvents(EventList eventListA, EventList eventListB);
EventList copyEvents(EventList eventListA, EventList eventListB);
EventList calculateNoteLength(EventList events, float sampleRate);
void      resetPhase(Data* self);

#endif //_H_SEQ_UTILS_
