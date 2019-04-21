/*
 * =====================================================================================
 *
 *       Filename:  sequencer_utils.c
 *
 *    Description:  util functions for MIDI sequencer
 *
 *        Version:  1.0
 *        Created:  12/24/2018 03:29:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bram Giesen (), bram@moddevices.com
 *   Organization:  MOD Devices
 *
 * =====================================================================================
 */


#include "sequencer_utils.h"


float calculateFrequency(uint8_t bpm, float division)
{
  float rateValues[11] = {7.5,10,15,20,30,40,60,80,120,160.0000000001,240};
  float frequency = (bpm / rateValues[(int)division]) * 0.5;

  return frequency;
}


//simple linear attack release envelope, this is used for the clicktrack
void attackRelease(Data *self)
{
  switch(self->ARStatus)
  {
    case IDLE:
      break;
    case ATTACK:
      self->amplitude += 0.001;
      if (self->amplitude >= 1.0) {
        self->ARStatus = RELEASE;
      } 
      break;
    case RELEASE:
      if (self->amplitude >= 0.0) {
        self->amplitude -= 0.001;
      } else {
        self->ARStatus = IDLE;
      }
      break;
  }
}



void precount(Data *self)
{
  static size_t amountOfClicks = 0;
  if (self->beat < 0.5 && !self->preCountTrigger) {
    self->ARStatus = ATTACK;
    amountOfClicks++;
    debug_print("amountOfClicks = %li\n", amountOfClicks);
    self->preCountTrigger = true;
  } else if (self->beat > 0.5 && self->preCountTrigger) {
    self->preCountTrigger = false;
  }
}



int barCounter(Data *self, uint8_t recordingLength)
{
  //return three when the amount of bars has been reached
  if (self->barCount > recordingLength) {
    self->barCount = 0;
    debug_print("self->barCount %i\n", self->barCount);
    debug_print("recordingLength =  %i\n", recordingLength);
    debug_print("BARS ARE DONE\n");
    return 2;
  } else {
    if (self->beatInMeasure < 0.5 && !self->barCounted) {
      self->barCount += 1;
//      debug_print("self->barCount = %i\n", self->barCount);
//      debug_print("self->beatInMeasure = %f\n", self->beatInMeasure);
      self->barCounted = true;
    } else if (self->beatInMeasure > 0.5 && self->barCounted) {
      self->barCounted = false;
    }
    return 1;
  }
}


/*function that handles the process of starting the pre-count at the beginning of next bar,
pre-count length and recording length.*/
void handleBarSyncRecording(Data *self)
{
  static int notePrinted = 1;
  switch(self->recordingStatus)
  {
    case 0: //start pre-counting at next bar
      if (self->beatInMeasure < 0.01 && self->startPreCount) {
        debug_print("self->beatInMeasure while waiting = %f\n", self->beatInMeasure);
        self->startPreCount = false;
        self->recordingStatus = 1;
      }
//      debug_print("WAITING FOR FIRST BAR\n"); 
      break;
    case 1: //count bars while pre-counting
      if (notePrinted == 1) 
        debug_print("self->beatInMeasure while precounting = %f\n", self->beatInMeasure);
      notePrinted = 0;
      precount(self);
      self->recordingStatus = barCounter(self, 1);
//      debug_print("PRE-COUNTING\n"); 
      break;
    case 2: //record
      self->recording = true;
      self->startPreCount = false;
      self->recordingStatus = (barCounter(self, 4)) + 1;
//      debug_print("RECORDING\n"); 
      break;
    case 3: //stop recording 
//      debug_print("STOP RECORDING\n"); 
      self->recording = false;
      copyEvents(self->writeEvents, self->playEvents);
      self->recordingStatus = 0;
      break;
  }
}



void recordNotes(Data *self, uint8_t midiNote)
{
  static int   snappedIndex = 0;
  static int   recIndex     = 0;
  
  if(midiNote > 0) {
    snappedIndex = (int)roundf(self->phaseRecord);
    self->writeEvents->eventList[recIndex++ % 4][snappedIndex][0] = midiNote;
  }

}


//make copy of events from eventList A to eventList B
void copyEvents(Array* eventListA, Array* eventListB)
{
  eventListB->used = eventListA->used;
  for (size_t voices = 0; voices < 4; voices++) {
    for (size_t noteIndex = 0; noteIndex < eventListA->used; noteIndex++) {
      for (size_t noteMeta = 0; noteMeta < 2; noteMeta++) {
        eventListB->eventList[voices][noteIndex][noteMeta] = eventListA->eventList[voices][noteIndex][noteMeta];
      }
    }
  }   
}



void resetPhase(Data *self)
{
  float velInitVal      = 0.000000009;

  if (self->beatInMeasure < 0.5 && self->resetPhase) {

    //TODO move elsewhere
    if (self->playing != self->previousPlaying) {
      if (*self->mode > 1) {
        self->velPhase = velInitVal;
        self->firstBar = true;
      }  
      self->previousPlaying = self->playing;
    }

    if (*self->division != self->previousDevision) {
      self->phase        = 0.0;
      self->velPhase     = velInitVal;
      self->divisionRate = *self->division;  
      self->previousDevision   = *self->division; 
    }
    if (self->phase > 0.989 || self->phase < 0.01) {
      self->phase = 0.0;
    }

    self->resetPhase  = false;

  } else {
    if (self->beatInMeasure > 0.5) {
      self->resetPhase = true;
    } 
  }
}



void clearSequence(Array *arr)
{
  arr->used = 0;
}
