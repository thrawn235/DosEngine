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

#define OPL3AddressPort 	0x388
#define OPL3DataPort 		0x389
#define NUM_VOICES			9

using namespace std;

struct SoundBlasterInstrument
{
	char 			signature[4];
	char 			name[32];
	unsigned char 	modulatorSoundCharacteristic;
	unsigned char 	carrierSoundCharacteristic;
	unsigned char 	modulatorScalingLevel;
	unsigned char 	carrierScalingLevel;
	unsigned char 	modulatorAttackDelay;
	unsigned char 	carrierAttackDelay;
	unsigned char 	modulatorSustainRelease;
	unsigned char 	carrierSustainRelease;
	unsigned char 	modulatorWaveSelect;
	unsigned char 	carrierWaveSelect;
	unsigned char 	feedback;
	unsigned char 	reserved[4];
};

struct MIDIEvent
{
	long int 		deltaTime;
	unsigned char 	command;
	unsigned char 	metaCommand;
	unsigned char 	channel;
	vector<char> 	data;
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
	int 				currentEvent = 0;
	int 				currentDeltaTime = 0;
};

struct MIDIHeader
{
	char 			signature[4];
	long int 	 	headerSize;
	unsigned short 	fileFormat;
	unsigned short 	numberOfTracks;
	unsigned short 	ticksPerQuerterNote;
};

struct MIDISong
{
	int 				ID;
	MIDIHeader 			header;
	vector<MIDITrack> 	tracks;
	int 				currentTrack = 0;
};


class SoundEngine
{
protected:
	TimeEngine* time;
	_go32_dpmi_seginfo 				OldISR, NewISR;

	int 							noise[256] = {4650, 4202, 8996, 4700, 2479, 8703, 10794, 14977, 3091, 3671, 10937, 263, 7399, 1373, 12195, 12689, 12679, 11059, 13154, 1141, 13988, 1636, 9353, 265, 9581, 3023, 11906, 14738, 3656, 9023, 13409, 7518, 2062, 14048, 5803, 11876, 14868, 7458, 3687, 4906, 8083, 7547, 9989, 14650, 11862, 6140, 9803, 6982, 13474, 383, 294, 4056, 13734, 2929, 12273, 12019, 10286, 4060, 8532, 6862, 11328, 532, 9995, 10975, 1419, 5143, 9378, 856, 7993, 259, 1621, 14447, 4197, 4070, 8635, 8238, 7923, 2424, 2245, 5755, 5033, 4839, 8155, 14407, 9762, 10976, 14612, 9677, 371, 12627, 12547, 2120, 3564, 7000, 11818, 7437, 14081, 12365, 907, 9039, 10922, 4558, 7059, 8364, 6550, 3875, 7838, 13243, 10598, 13678, 899, 1510, 4153, 14812, 4727, 12507, 7931, 2185, 10572, 3980, 8933, 5263, 7652, 11242, 13408, 12097, 4281, 9858, 4093, 3546, 7721, 5471, 14084, 11710, 9198, 1506, 5768, 13981, 1553, 11209, 9952, 5787, 13012, 10635, 136, 5786, 11005, 4715, 4269, 12215, 7484, 2455, 2916, 12908, 7093, 5625, 6843, 1765, 3630, 11564, 14671, 10952, 10447, 13378, 14881, 7361, 11738, 8482, 2605, 1428, 4310, 3956, 2825, 7324, 2628, 5604, 2157, 14337, 7488, 5070, 14826, 2050, 9967, 2207, 6901, 10163, 4424, 12770, 3996, 14240, 5078, 7146, 3638, 11322, 8923, 7851, 6797, 14630, 8269, 9903, 6125, 641, 9382, 9689, 13956, 14158, 12383, 10655, 3794, 5183, 12026, 3815, 13841, 3174, 1642, 7315, 12907, 11384, 4933, 2148, 13260, 12265, 14517, 1296, 5709, 1651, 12569, 7824, 10289, 9331, 6435, 4949, 4518, 1385, 6475, 13030, 4205, 3079, 14085, 12307, 9138, 10619, 11731, 8116, 9978, 13258, 5392, 4317, 14495, 14787, 13261, 3003, 9115, 8221, 11590, 6227};
	char 							channelMap[NUM_VOICES] = {0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10,  0x11,  0x12};

	const unsigned short int 		FNr [128] = {86,91,96,102,108,114,121,128,136,144,
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
		                                       611,647,686,726,770,816,864,916,970,1023};

	bool inUseVoices[NUM_VOICES];
	int inUseMIDIChannel[NUM_VOICES];
	unsigned char inUseNote[NUM_VOICES];

	MIDISong* 							currentSong;

	SoundBlasterInstrument 			activeInstruments[NUM_VOICES];

	vector<SoundBlasterInstrument> 	instruments;
	vector<MIDISong*> 				songs;

public:
							SoundEngine 			( TimeEngine* newTime );
							~SoundEngine 			();

	void 					InstallSoundInterrupt 	();
	void 					RestoreSoundInterrupt 	();

	void 					PlayNote 				( int channel, unsigned char note );
	//void 					NoteOff 				( int channel );
	void 					SetLevel 				( int channel, bool op, unsigned char newLevel );
	void 					SetADSREnvelope 		( int channel, bool op, char attack, char decay, char sustain, char release );
	void 					SetSoundCharacteristic 	( int channel, bool op, bool amplitudeModulation, bool vibrato, bool sustain, char harmonics );
	void 					ResetSoundBlaster		();
	
	void 					ApplyInstrument 		(SoundBlasterInstrument* in, char channel );
	SoundBlasterInstrument 	CreateNewInstrument 	(const char* name);
	SoundBlasterInstrument 	CreateNewInstrument 	(const 	char* name, char modulatorSoundCharacteristic, char carrierSoundCharacteristic,
															char modulatorScalingLevel, char carrierScalingLevel,
															char modulatorAttackDelay, char carrierAttackDelay,
															char modulatorSustainRelease, char carrierSustainRelease,
															char modulatorWaveSelect, char carrierWaveSelect,
															char feedback);
	void 					SaveInstrumentToFile 	( SoundBlasterInstrument* in, const char* filePath );
	SoundBlasterInstrument 	LoadInstrumentFromFile 	( const char* filePath );

	void 					AddInstrument 			( SoundBlasterInstrument* in );
	SoundBlasterInstrument* GetInstrument 			( int index );
	void 					DeleteInstrument 		( int index );
	void 					ReplaceInstrument 		( SoundBlasterInstrument* in, int index );

	MIDISong* 				LoadMIDIFile 			( const char* filePath );
	void 					AddSong 				( MIDISong* in, int newSongID );
	MIDISong*				GetSong 				( int songID );

	void 					PlaySound 				( bool newRepeat );

	void 					SoundOn 				();
	void 					SoundOff 				();

	void					SetActiveInstrument 	( SoundBlasterInstrument* in, int channel );
	SoundBlasterInstrument* GetActiveInstrument 	( int channel );

	void 					NoteOn 					( int MIDIChannel, unsigned char note, unsigned char velocity );
	void 					NoteOff 				( int MIDIChannel, unsigned char note );

	MIDIEvent* 				GetNextMIDIEvent 		(); 
};



#endif