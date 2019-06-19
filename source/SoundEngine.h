///////////////////////////////////////////////////////////////////////////////////////
//
//	SoundEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//
//	relies on time Engine!
//
//	The Sound Engine is meant to play PC speaker sounds, using an interrupt
//	The interrupt frequency is set in the timeEngine!
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef SOUNDENGINE
#define SOUNDENGINE


//================== includes ==========================
//c++ includes:
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

//djgpp includes:
#include <sys/nearptr.h>
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:
#include "TimeEngine.h"

//=======================================================

//==================== defines ==========================
#define OPL3_ADDRESS_PORT			0x388
#define OPL3_DATA_PORT 				0x389
#define GENERAL_MIDI_BASE_PORT 		0x300 //or 0x330
#define PC_TIMER_PORT 				0x42
#define PC_SPEAKER_PORT 			0x61

#define DEVICE_NONE					0
#define DEVICE_PC_SPEAKER			1
#define DEVICE_ADLIB 				2
#define DEVICE_SOUND_BLASTER 		3
#define DEVICE_SOUND_BLASTER_PRO 	4
#define DEVICE_MT32 				5
#define DEVICE_GENERAL_MIDI 		6
#define DEVICE_GUS 			 		7
//=======================================================

using namespace std;

struct SoundBlasterInstrumentData
{
	char 			signature[4];
	char 			name[32];
	unsigned char 	modulatorSoundCharacteristic;
	unsigned char 	carrierSoundCharacteristic;
	unsigned char 	modulatorScalingLevel;
	unsigned char 	carrierScalingLevel;
	unsigned char 	modulatorAttackDecay;
	unsigned char 	carrierAttackDecay;
	unsigned char 	modulatorSustainRelease;
	unsigned char 	carrierSustainRelease;
	unsigned char 	modulatorWaveSelect;
	unsigned char 	carrierWaveSelect;
	unsigned char 	feedback;
	unsigned char 	reserved[4];
};

struct SoundBlasterInstrument
{
	int ID;
	SoundBlasterInstrumentData data;
};

struct MIDIEvent
{
	long int 		deltaTime;
	unsigned char 	command;
	unsigned char 	metaCommand;
	unsigned char 	channel;
	vector<char> 	data;
	long 			linearTime = 0;
};

struct MIDITrackHeader
{
	char 			signature[4];
	long int 	 	lengthOfTrack;
};

struct MIDITrack
{
	MIDITrackHeader 	trackHeader;
	vector<MIDIEvent> 	events;
	unsigned int		currentEvent = 0;
	long 				length = 0;
};

struct MIDIHeader
{
	char 			signature[4];
	long int 	 	headerSize;
	unsigned short 	fileFormat;
	unsigned short 	numberOfTracks;
	unsigned short 	ticksPerQuarterNote;
};

struct MIDISong
{
	int 				ID;
	MIDIHeader 			header;
	vector<MIDITrack> 	tracks;
	long 				length = 0;
};

struct PlayList
{
	int 				ID;
	vector<MIDISong*> 	songs;
	int 				currentTrack = 0;
};


struct Note
{
	int note;
	long timeStamp;
	char midiChannel;
	int playingAtVoice;
};


class SoundEngine
{
protected:
	TimeEngine* time;
	_go32_dpmi_seginfo 				OldISR, NewISR;

	int 							timeTicksDivisor;

	char 							channelMapOPL[9] = {0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10,  0x11,  0x12};

	const unsigned short int 		FNr [128] = {	86,91,96,102,108,114,121,128,136,144,
		                                 			153,162,172,182,192,204,108,114,121,
		                                       		128,136,144,153,162,172,182,192,204,
		                                       		216,229,242,257,136,144,153,162,172,
		                                       		182,192,204,216,229,242,257,272,288,
		                                       		306,324,172,182,192,204,216,229,242,
		                                       		257,272,288,306,324,343,363,385,408,
		                                       		216,229,242,257,272,288,306,324,343,
		                                       		363,385,408,432,458,485,514,272,288,
		                                       		306,324,343,363,385,408,432,458,485,
		                                       		514,544,577,611,647,343,363,385,408,
		                                       		432,458,485,514,544,577,611,647,686,
		                                       		726,770,816,432,458,485,514,544,577,
		                                       		611,647,686,726,770,816,864,916,970,1023 };

	//PCSpeaker specific Attributes=================================================================================================
	const float MidiFrequencies[128] = { 	8.18, 8.66, 9.18, 9.72, 10.30, 10.91, 11.56, 12.25, 12.98,
											13.75, 14.57, 15.43, 16.35, 17.32, 18.35, 19.45, 20.60,
											21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87, 32.70, 
											34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 
											58.27, 61.74, 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50,
											98.00, 103.83, 110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 155.56, 
											164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94, 261.63,
											277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 
											466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 
											783.99, 830.61, 880.00, 932.33, 987.77, 1046.50, 1108.73, 1174.66, 1244.51, 
											1396.91, 1479.98, 1567.98, 1661.22, 1760.00, 1864.66, 1975.53, 2093.00, 2217.46, 
											2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 
											3951.07, 4186.01, 4434.92, 4698.64, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 
											6644.88, 7040.00, 7458.62, 7902.13, 8372.02, 8869.84, 9397.27, 9956.06, 10548.08, 
											11175.30, 11839.82, 12543.85, 13289.75 };
	int 								currentTicksToLinger;
	unsigned int 						currentActiveNote;
	unsigned int						maxVoices;
	//===============================================================================================================================

	vector<Note> 						activeNotes;
	bool 								activeVoices[64];

	//Midi Playback specific=========================================================================================================
	long headPosition;
	//===============================================================================================================================

	float 								masterVolume;

	MIDISong* 							currentSong;
	PlayList* 							currentPlayList;

	vector<SoundBlasterInstrument*> 	activeInstruments;
	vector<SoundBlasterInstrument> 		instruments;
	vector<MIDISong>					songs;
	vector<PlayList> 					playLists;

	bool 								playing;
	bool 								loop;
	bool 								mute;
	bool 								songEnd;
	bool 								playListEnd;

	

	int 								currentSoundDevice;

	int 								ticksPerQNote;

public:
	//Connstructor / Destructor ===================================================================================================
							SoundEngine 					( TimeEngine* newTime );
							~SoundEngine 					();
	//=============================================================================================================================

	//interrupt====================================================================================================================
	void 					InstallSoundInterrupt 			();
	void 					RestoreSoundInterrupt 			();
	//=============================================================================================================================

	//General =====================================================================================================================
	void 					SoundOn 						();
	void 					SoundOff 						();

	bool 					GetMute 						();
	bool 					GetLoop 						();

	void 					SetSoundDevice 					( int newCurrentSoundDevice );
	int 					GetSoundDevice 					();

	void 					SetTickRate 					( int newTickRate );
	void 					SetTicksPerQNote 				( int newSpeed );
	int 					GetTicksPerQNote 				();

	void 					SetMaxVoices 					( int newVoices );
	int 					GetMaxVoices					();

	MIDISong* 				GetCurrentSong 					();
	long 					GetHeadPosition 				();
	void 					SetHeadPosition 				( long newHeadPosition );

	float 					GetMasterVolume 				();
	void 					SetMasterVolume 				( float newVolume );

	void 					SetSongEnd 						( bool newSongEnd );
	void 					SetPlayListEnd 					( bool newPlayListEnd );
	bool 					GetPlaying 						();
	//=============================================================================================================================

	//Init Device =================================================================================================================
	void 					InitSoundBlaster 				( bool isOPL3 );
	void 					ResetSoundBlaster 				();

	void 					InitPCSpeaker 					();
	void 					ResetPCSpeaker 					();

	void 					InitGeneralMidiDevice 			();
	void 					ResetGeneralMidiDevice 			();

	void 					InitGUS 			 			();
	void 					ResetGUS 			 			();
	//=============================================================================================================================

	//Midi Commands ===============================================================================================================
	void 					PCSpeakerNoteOff 				( int note );
	void 					PCSpeakerNoteOn 				( int note, unsigned char midiChannel );
	void 					PCSpeakerSetTempo 				( int newTempo );
	void 					PCSpeakerCutNote 				( int note );
	void 					PCSpeakerPlayActiveNotes 		( int arpeggioRate );

	void 					OPLNoteOff 						( int midiChannel, unsigned char note );
	void					OPLNoteOn 						( int midiChannel, unsigned char note, unsigned char velocity );
	void 					OPLControllerEvent 				();
	void 					OPLProgramChange 				();
	void 					OPLSetTempo 					( int newTempo );
	void 					OPLCutNote 						( int midiChannel, unsigned char note );
	void 					OPLCutAllNotes					();

	void 					GeneralMidiNoteOff 				();
	void 					GeneralMidiNoteOn 				();
	void 					GeneralMidiNoteAfterTouch 		();
	void 					GeneralMidiControllerEvent 		();
	void 					GeneralMidiProgramChange 		();
	void 					GeneralMidiChannelAfterTouch 	();
	void 					GeneralMidiPitchBend 			();
	void 					GeneralMidiSetTempo 			();
	void 					GeneralMidiCutNote 				();

	void 					GUSNoteOff 						();
	void 					GUSNoteOn 						();
	void 					GUSNoteAfterTouch 				();
	void 					GUSControllerEvent 				();
	void 					GUSProgramChange 				();
	void 					GUSChannelAfterTouch 			();
	void 					GUSPitchBend 					();
	void 					GUSSetTempo 					();
	void 					GUSCutNote 						();
	//=============================================================================================================================

	//Transport Controls ==========================================================================================================
	void 					Play 							();
	void 					Pause 							();
	void 					JumpToStart 					();
	void 					JumpToTimeCode 					();
	void 					JumpRelative 					();
	void 					Loop 							( bool newLoop );
	void 					SetSpeed 						();
	void 					ChangeSpeed 					();
	long 					GetCurrentTimeStamp 			();
	void 					SkipToSong 						( int newSong );
	void 					SkipToSong 						( int newPlayList, int newSong );
	void 					SkipToPlayList 					( int newPlayList );
	//=============================================================================================================================

	//File Management =============================================================================================================
	MIDISong 				LoadMidiFromFile( const char* filePath );
	void 					SaveMidiToFile();
	void 					LoadCMFFromFile();
	void 					SaveCMFToFile();
	void 					LoadIFFFromFile();
	void 					SaveIFFToFile();
	void 					LoadRIFFFromFile();
	void 					SaveRIFFToFile();

	SoundBlasterInstrument 	LoadSBIFromFile 				( const char* filePath );
	void 					SaveSBIToFile 					( SoundBlasterInstrument* in, const char* filePath );

	SoundBlasterInstrument 	LoadIBKFromFile 				();
	void 					SaveIBKToFile 					();

	void 					LoadPlayListFromFile();
	void 					SavePlayListToFile();
	//=============================================================================================================================

	//Song Controls ===============================================================================================================
	void 					CreateSong();
	void 					DestroySong();
	void 					AddSong 						( MIDISong in, int newID );
	MIDISong* 				GetSong 						( int ID );
	void 					RemoveSong 						( int ID );
	//=============================================================================================================================

	//PlayList ====================================================================================================================
	void 					CreatePlayList 					(int newID);
	void 					AddPlayList 					( PlayList* newPlayList );
	PlayList* 				GetPlayList 					( int ID );
	void 					RemovePlayList 					( int ID );
	void 					AddToPlayList 					( int playListID, MIDISong* in );
	void 					RemoveFromPlayList 				( int playListID, int index );
	MIDISong* 				GetSongFromPlayList 			( int playListID, int index );
	MIDISong* 				GetFirstSongFromPlayListByID 	( int playListID, int ID );
	//=============================================================================================================================

	//Instrument Controls =========================================================================================================
	//Drums ?
	SoundBlasterInstrument 	CreateNewInstrument 			(const char* name);
	SoundBlasterInstrument 	CreateNewInstrument 			(const 	char* name, char modulatorSoundCharacteristic, char carrierSoundCharacteristic,
																	char modulatorScalingLevel, char carrierScalingLevel,
																	char modulatorAttackDecay, char carrierAttackDecay,
																	char modulatorSustainRelease, char carrierSustainRelease,
																	char modulatorWaveSelect, char carrierWaveSelect,
																	char feedback);
	void 					AddInstrument 					( SoundBlasterInstrument* in, int ID );
	SoundBlasterInstrument* GetInstrument 					( int ID );
	void 					RemoveInstrument 				( int ID );

	void 					AddActiveInstrument 			( SoundBlasterInstrument* in );
	SoundBlasterInstrument* GetActiveInstrument 			( unsigned int index );
	void 					RemoveActiveInstrument 			( int index );
	void 					ReplaceActiveInstrument			( SoundBlasterInstrument* in, int index );

	void 					ApplyInstrument 				( SoundBlasterInstrument* in, int voice, bool left, bool right, unsigned char velocity );

	SoundBlasterInstrument 	SetInstrumentLevel 				( SoundBlasterInstrument in, bool op, unsigned char newScalingLevel, unsigned char newLevel );
	SoundBlasterInstrument 	SetInstruemtADSREnvelope 		( SoundBlasterInstrument in, bool op, unsigned char attack, unsigned char decay, unsigned char sustain, unsigned char release );
	SoundBlasterInstrument 	SetInstrumentSoundCharacteristic( SoundBlasterInstrument in, bool op, bool amplitudeModulation, bool vibrato, bool rythm, bool bassDrum, bool snareDrum, bool tomTom, bool cymbal, bool hiHat );
	SoundBlasterInstrument 	SetInstrumentWaveForm			( SoundBlasterInstrument in, bool op, char newWaveForm );
	SoundBlasterInstrument 	SetInstrumentFeedBack			( SoundBlasterInstrument in, unsigned char newFeedback, bool newAlgorithm );

	SoundBlasterInstrument 	SetInstrumentLevel 				( SoundBlasterInstrument in, bool op, unsigned char newLevelByte );
	SoundBlasterInstrument 	SetInstruemtADSREnvelope 		( SoundBlasterInstrument in, bool op, unsigned char newAttackByte, unsigned char newSustainByte );
	SoundBlasterInstrument 	SetInstrumentSoundCharacteristic( SoundBlasterInstrument in, bool op, unsigned char newCharacteristicByte );
	SoundBlasterInstrument 	SetInstrumentWaveForm			( SoundBlasterInstrument in, bool op, unsigned char newWaveFormByte );
	SoundBlasterInstrument 	SetInstrumentFeedBack			( SoundBlasterInstrument in, unsigned char newFeedbackByte );
	//=============================================================================================================================

	
};



#endif