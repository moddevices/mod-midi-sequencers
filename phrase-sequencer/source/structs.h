/*
 * =====================================================================================
 *
 *       Filename:  structs.h
 *
 *    Description:  file containing structs
 *
 *        Version:  1.0
 *        Created:  12/25/2018 02:22:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bram Giesen (), bram@moddevices.com
 *   Organization:  MOD Devices
 *
 * =====================================================================================
 */

#ifndef _H_STRUCTS_
#define _H_STRUCTS_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/time/time.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#ifndef DEBUG
#define DEBUG 0
#endif
#define debug_print(...) \
    ((void)((DEBUG) ? fprintf(stderr, __VA_ARGS__) : 0))

typedef enum PortEnum {
    PORT_ATOM_IN = 0,
    PORT_ATOM_OUT1,
    PORT_METROME_OUT,
    METRO_CONTROL,
    MODE,
    ACTIVATERECORDING,
    PRECOUNT,
    RECORDINGLENGTH,
    DIVISION,
    NOTELENGTH,
    OCTAVESPREAD,
    TRANSPOSE,
    SWING,
    RANDOMIZETIMMING,
    FX_MODE,
    MOMENTARY_FX
} PortEnum;

typedef enum ModeEnum {
    CLEAR_ALL = 0,
    STOP,
    PLAY,
    RECORD_OVERWRITE,
    RECORD_APPEND,
    UNDO_LAST
} ModeEnum;

typedef enum RecordEnum {
    R_IDLE = 0,
    R_PRE_COUNT,
    R_PRE_RECORDING,
    R_RECORDING,
    R_STOP_RECORDING
} RecordEnum;

typedef enum FindNoteEnum {
    FIND_NOTE_ON = 0,
    FIND_NOTE_OFF,
    CALCULATE_NOTE_LENGTH,
    NEXT_INDEX
} FindNoteEnum;

typedef enum AttackReleaseEnum {
    IDLE = 0,
    ATTACK,
    RELEASE
} AttackReleaseEnum;

typedef struct MetroURIs {
    LV2_URID atom_Blank;
    LV2_URID atom_Float;
    LV2_URID atom_Object;
    LV2_URID atom_Path;
    LV2_URID atom_Resource;
    LV2_URID atom_Sequence;
    LV2_URID time_Position;
    LV2_URID time_barBeat;
    LV2_URID time_beatsPerMinute;
    LV2_URID time_beatsPerBar;
    LV2_URID time_speed;
} MetroURIs;

typedef struct EventList {
    //eventList[0] = midiNote
    //eventList[1] = velocity
    //eventList[2] = note On/Off 
    //eventList[3] = recordedPosition
    //eventList[4] = calculated noteLength
   
    uint32_t eventList[248][5];
    size_t   amountOfProps;
    size_t   used;
} EventList;

typedef struct Data {

    int barCounter;
    int periodCounter;
    bool barNotCounted;

	long int pos;
	uint32_t period;
    uint32_t recordingPos;
	uint32_t h_wavelength;
    uint32_t noteOffTimer[16][3];
    long int recordedFrames;

    double  rate;   // Sample rate
    double  frequency;
    double  nyquist;
    double  phase;
    double  sinePhase;
    double  phaseRecord;
    float   *metroOut;
    float   amplitude;
    float   bpm;
    float   previousBpm;
    float   recordingBpm;
    float   previousFactorBpm;
    float   barsize;
    float   beat;
    float   speed; // Transport speed (usually 0=stop, 1=play)
    float   noteLengthTime[2];
    int     activeNotes;
    int     previousSpeed;
    int     modeHandle;
    int     prevMod;
    int     prevLatch;

    uint8_t velocity;
    float   beatInMeasure;
    float   division;
    long int fullRecordingLength;
    size_t  inputIndex;
    size_t  notesPressed;
    size_t  activeNoteIndex; 
    uint8_t prevThrough;
    uint8_t midiThroughInput[16];
    uint8_t recordingStatus;
    uint8_t barCount;
    uint8_t ARstate;
    uint8_t prevRecordTrigger; 
    uint8_t applyMomentaryFx;
    //resetPhase vars:
    float previousDevision;
    bool  recordingLengthSet;
    bool  barCounted;
    bool  recordingTriggered;
    bool  recordingEnabled;
    bool  startPreCount;
    bool  recording;
    bool  previousPlaying;
    bool  resetPhase;
    bool  alreadyPlaying;

    size_t  noteFound;
    size_t  notePlayed;
    size_t  octaveIndex;
    size_t  noteOffIndex;
    size_t  noteOffSendIndex;
    bool    firstRecordedNote;
    bool    through;
    bool    firstBar;
    bool    playingEnabled;
    bool    playing;
    bool    trigger;
    bool    triggerSet;
    bool    preCountTrigger;
    bool    cleared;
    int     transpose;

    EventList recordedEvents;
    EventList mergedEvents;
    EventList storedEvents;
    EventList playEvents;
    AttackReleaseEnum  ARStatus;

    const float* recordTrigger;
    const float* mode;
    const float* preCountLength;
    const float* recordingLength;
    const float* changeDiv;
    const float* noteLengthParam;
    const float* latchTranspose;
    const float* swing;
    const float* randomizeTimming;
    const float* octaveSpread;
    const float* fxMode;
    const float* momentaryFx;

    const float** recordingLengths[2];

    const LV2_Atom_Sequence* port_events_in;
    LV2_Atom_Sequence*       port_events_out1;

    LV2_URID           urid_midiEvent;
    LV2_URID_Map*      map;     // URID map feature
    LV2_Atom_Sequence* control;
    MetroURIs          uris;    // Cache of mapped URIDs

} Data;
#endif //_H_STRUCTS_
