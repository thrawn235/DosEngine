///////////////////////////////////////////////////////////////////////////////////////
//
//	SoundEngine.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	Memberfunctions of the SoundEngine
//	see SoundEngine.h for details
//
///////////////////////////////////////////////////////////////////////////////////////


#include "SoundEngine.h"




//============= global Variables for the Interrupt Service Routine ========
SoundEngine* globalPSound;
//=========================================================================

//============ global Function that is to be installed as ISR =============
void SoundInterruptRoutine()
{
	MIDISong* song = globalPSound->GetCurrentSong();

	if( !globalPSound->GetMute() && song != NULL )
	{
		long currentTime = globalPSound->GetHeadPosition();

		currentTime++;

		globalPSound->SetHeadPosition( currentTime );

		for( unsigned int i = 0; i < song->tracks.size(); i++ )
		{
			if( song->tracks[i].currentEvent < song->tracks[i].events.size() )
			{
				if( currentTime >= song->tracks[i].events[song->tracks[i].currentEvent].linearTime )
				{
					//CHECK FOR END
					if( currentTime > song->length )
					{
						globalPSound->SetSongEnd( true );
					}

					//handle event
					MIDIEvent* currentEvent = &song->tracks[i].events[song->tracks[i].currentEvent];
					if( currentEvent->command == 0x90 )
					{
						//note on
						if( globalPSound->GetSoundDevice() == DEVICE_PC_SPEAKER )
						{
							globalPSound->PCSpeakerNoteOn( currentEvent->data[0], currentEvent->channel );
						}
						if( globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER || globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER_PRO )
						{
							globalPSound->OPLNoteOn( currentEvent->channel, currentEvent->data[0], currentEvent->data[1] );
						}
					}
					if( currentEvent->command == 0x80 )
					{
						//note off
						if( globalPSound->GetSoundDevice() == DEVICE_PC_SPEAKER )
						{
							globalPSound->PCSpeakerNoteOff( currentEvent->data[0] );
						}
						if( globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER || globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER_PRO )
						{
							globalPSound->OPLNoteOff( currentEvent->channel, currentEvent->data[0] );
						}
					}
					if( currentEvent->command == 0xFF )
					{
						//special command
						if( currentEvent->metaCommand == 0x51 )
						{
							int speedByte = 0;
							speedByte = currentEvent->data[0];
							speedByte = speedByte << 8;
							speedByte = speedByte + currentEvent->data[1];
							speedByte = speedByte << 8;
							speedByte = speedByte + currentEvent->data[2];
							if( globalPSound->GetSoundDevice() == DEVICE_PC_SPEAKER )
							{
								globalPSound->PCSpeakerSetTempo( song->header.ticksPerQuarterNote / ( (float)speedByte / 1000000.0 ) );
							}
							if( globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER || globalPSound->GetSoundDevice() == DEVICE_SOUND_BLASTER_PRO )
							{
								globalPSound->OPLSetTempo( song->header.ticksPerQuarterNote / ( (float)speedByte / 1000000.0 ) );
							}
								
						}
					}

					song->tracks[i].currentEvent++;
				}
			}
		}
		
		if( globalPSound->GetSoundDevice() == DEVICE_PC_SPEAKER )
		{
			globalPSound->PCSpeakerPlayActiveNotes( 40 );
		}
	}
}
//unused dummy functions. meant to calculate the size of TimerInterruptRoutine()
void SoundInterruptRoutineEnd() {} 
//======================================================================







//Connstructor / Destructor ===================================================================================================
SoundEngine::SoundEngine( TimeEngine* newTime )
{
	globalPSound = this;

	time = newTime;

	currentSong = NULL;

	loop = false;
	mute = false;
	songEnd = false;
	playListEnd = false;

	maxVoices = 9; //just an initial guess

	currentSoundDevice = DEVICE_NONE;

	currentTicksToLinger = 0;
	currentActiveNote = 0;
	headPosition = 0;

	InstallSoundInterrupt();

	for( int i = 0; i < 128; i ++ )
	{
		activeVoices[i] = false;
	}

	//debug-------------------------
	#define SONG_DIES_IRAE	0
	#define SONG_EQUINOXE 	1
	#define SONG_SCALE 		3
	#define SONG_BACH 		4
	#define SONG_BEETHOFEN	5

	AddSong( LoadMidiFromFile( "./audio/music/irae.mid" ), SONG_DIES_IRAE );
	AddSong( LoadMidiFromFile( "./audio/music/equinoxe.mid" ), SONG_EQUINOXE );
	AddSong( LoadMidiFromFile( "./audio/music/scale.mid" ), SONG_SCALE );
	AddSong( LoadMidiFromFile( "./audio/music/bach.mid" ), SONG_BACH );
	AddSong( LoadMidiFromFile( "./audio/music/beeth.mid" ), SONG_BEETHOFEN );

	#define PLAYLIST_STANDARD 	1

	CreatePlayList( PLAYLIST_STANDARD );
	AddToPlayList( PLAYLIST_STANDARD, GetSong( SONG_DIES_IRAE ) );
	AddToPlayList( PLAYLIST_STANDARD, GetSong( SONG_EQUINOXE ) );
	AddToPlayList( PLAYLIST_STANDARD, GetSong( SONG_DIES_IRAE ) );

	currentSong = GetSongFromPlayList( PLAYLIST_STANDARD, 0 );
	currentPlayList = GetPlayList( PLAYLIST_STANDARD );

	SetSoundDevice( DEVICE_SOUND_BLASTER_PRO );
	InitPCSpeaker();
	//maxVoices = 3;
	InitSoundBlaster( true );
	
	SetTickRate( currentSong->header.ticksPerQuarterNote * 2 );
	//SetTickRate( 700 );
	//maxVoices = 18;
	//PCSpeakerNoteOn( 50 );
	/*PCSpeakerNoteOn( 70 );
	PCSpeakerNoteOn( 100 );
	PCSpeakerNoteOn( 120 );
	PCSpeakerNoteOn( 80 );
	PCSpeakerNoteOn( 60 );*/

	loop = false;

	masterVolume = 2;

	#define INSTRUMENT_SYNBASS1		1
	#define INSTRUMENT_SYN1 		2
	#define INSTRUMENT_FLUTE2 		3
	#define INSTRUMENT_MOON 		4
	#define INSTRUMENT_STRINGS1		5 
	#define INSTRUMENT_VIOLIN		6
	#define INSTRUMENT_FRENCH_HORN	7

	SoundBlasterInstrument newInst = LoadSBIFromFile( "./audio/inst/SYN1.sbi" );
	AddInstrument( &newInst, INSTRUMENT_SYN1 );
	newInst = LoadSBIFromFile( "./audio/inst/SYNBASS1.sbi" );
	AddInstrument( &newInst, INSTRUMENT_SYNBASS1 );
	newInst = LoadSBIFromFile( "./audio/inst/FLUTE2.sbi" );
	AddInstrument( &newInst, INSTRUMENT_FLUTE2 );
	newInst = LoadSBIFromFile( "./audio/inst/MOON.sbi" );
	AddInstrument( &newInst, INSTRUMENT_MOON );
	newInst = LoadSBIFromFile( "./audio/inst/STRINGS1.sbi" );
	AddInstrument( &newInst, INSTRUMENT_STRINGS1 );
	newInst = LoadSBIFromFile( "./audio/inst/VIOLIN.sbi" );
	AddInstrument( &newInst, INSTRUMENT_VIOLIN );
	newInst = LoadSBIFromFile( "./audio/inst/FRHORN1.sbi" );
	AddInstrument( &newInst, INSTRUMENT_FRENCH_HORN );

	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYNBASS1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYNBASS1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYNBASS1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_FRENCH_HORN ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_VIOLIN ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_VIOLIN ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_MOON ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_STRINGS1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	AddActiveInstrument( GetInstrument( INSTRUMENT_SYN1 ) );
	//------------------------------

}
SoundEngine::~SoundEngine()
{
	RestoreSoundInterrupt();

	if( currentSoundDevice == DEVICE_ADLIB || currentSoundDevice == DEVICE_SOUND_BLASTER || currentSoundDevice == DEVICE_SOUND_BLASTER_PRO )
	{
		ResetSoundBlaster();
	}
	if( currentSoundDevice == DEVICE_PC_SPEAKER )
	{
		ResetPCSpeaker();
	}
	if( currentSoundDevice == DEVICE_GENERAL_MIDI )
	{
		ResetGeneralMidiDevice();
	}
	if( currentSoundDevice == DEVICE_GUS || currentSoundDevice == DEVICE_MT32 )
	{
		ResetGUS();
	}
}
//=============================================================================================================================

//interrupt====================================================================================================================
void SoundEngine::InstallSoundInterrupt()
{
	//_go32_dpmi_lock_data( ( void* )stream, ( long )sizeof( stream ) );
	_go32_dpmi_lock_data( ( void* )globalPSound, ( long )sizeof( SoundEngine ) );

	_go32_dpmi_lock_code( ( void* )SoundInterruptRoutine, 1000 );

 	_go32_dpmi_get_protected_mode_interrupt_vector( 0x08, &OldISR );
	
	NewISR.pm_offset = ( int )SoundInterruptRoutine;
	NewISR.pm_selector = _go32_my_cs();

	_go32_dpmi_chain_protected_mode_interrupt_vector( 0x08, &NewISR );
}
void SoundEngine::RestoreSoundInterrupt()
{
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x08, &OldISR );
	//
}
//=============================================================================================================================

//General =====================================================================================================================
void SoundEngine::SoundOn()
{
	mute = false;
	//
}
void SoundEngine::SoundOff()
{
	mute = true;
	//
}
bool SoundEngine::GetMute()
{
	return mute;
	//
}
bool SoundEngine::GetLoop()
{
	return loop;
	//
}

void SoundEngine::SetSoundDevice( int newCurrentSoundDevice )
{
	//the other devices need to be safely stopped

	currentSoundDevice = newCurrentSoundDevice;
	//
}
int SoundEngine::GetSoundDevice()
{
	return currentSoundDevice;
	//
}

void SoundEngine::SetTickRate( int newTickRate )
{
	time->SetInterruptFrequency	( newTickRate );
	//
}
void SoundEngine::SetTicksPerQNote( int newSpeed )
{
	ticksPerQNote = newSpeed;
	//
}
int SoundEngine::GetTicksPerQNote()
{
	return ticksPerQNote;
	//
}

void SoundEngine::SetMaxVoices( int newVoices )
{
	maxVoices = newVoices;
	//
}
int SoundEngine::GetMaxVoices()
{
	return maxVoices;
	//
}
MIDISong* SoundEngine::GetCurrentSong()
{
	return currentSong;
	//
}
long SoundEngine::GetHeadPosition()
{
	return headPosition;
	//
}
void SoundEngine::SetHeadPosition( long newHeadPosition )
{
	headPosition = newHeadPosition;
	//
}
float SoundEngine::GetMasterVolume()
{
	return masterVolume;
	//
}
void SoundEngine::SetMasterVolume( float newVolume )
{
	if( newVolume < 0 || newVolume > 20 );
	{
		return;
	}
	masterVolume = newVolume;
}
void SoundEngine::SetSongEnd( bool newSongEnd )
{
	songEnd = newSongEnd;
	if( songEnd == true )
	{
		SetHeadPosition( 0 );
		if( loop == true )
		{
			for( int i = 0; i < currentSong->tracks.size(); i++ )
			{
				currentSong->tracks[i].currentEvent = 0;
			}
		}
		else
		{
			songEnd = false;
			currentPlayList->currentTrack++;
			if( currentPlayList->currentTrack < currentPlayList->songs.size() )
			{
				currentSong = currentPlayList->songs[currentPlayList->currentTrack];
				for( int i = 0; i < currentSong->tracks.size(); i++ )
				{
					currentSong->tracks[i].currentEvent = 0;
				}
			}
			else
			{
				SetPlayListEnd( true );
			}
		}
	}
}
void SoundEngine::SetPlayListEnd( bool newPlayListEnd )
{
	playListEnd = newPlayListEnd;
	if( playListEnd == true )
	{
		//loop etc ?
	}
}
//=============================================================================================================================

//Init Device =================================================================================================================
void SoundEngine::InitSoundBlaster( bool isOPL3 )
{
	ResetSoundBlaster();

	if( isOPL3 )
	{
		outportb( OPL3_ADDRESS_PORT + 2, 5 );
		outportb( OPL3_DATA_PORT + 2 , 1 );

		outportb( OPL3_ADDRESS_PORT + 2, 4 );
		outportb( OPL3_DATA_PORT + 2, 0 );

		outportb( OPL3_ADDRESS_PORT + 2, 0xBD );
		outportb( OPL3_DATA_PORT + 2, 0 );

		outportb( OPL3_ADDRESS_PORT, 0xBD );
		outportb( OPL3_DATA_PORT, 0 );

		SetMaxVoices( 18 );
	}
	else
	{
		SetMaxVoices( 9 );
	}
}
void SoundEngine::ResetSoundBlaster()
{
	for( int i = 0; i <= 0xF5; i++ )
	{
		outportb( OPL3_ADDRESS_PORT, i );
		outportb( OPL3_DATA_PORT, 0 );

		outportb( OPL3_ADDRESS_PORT +2, i );
		outportb( OPL3_DATA_PORT+2, 0 );
	}
}

void SoundEngine::InitPCSpeaker()
{
	outportb( 0x43, 182 );
	//
}
void SoundEngine::ResetPCSpeaker()
{
	outportb( PC_SPEAKER_PORT, 0); //turn speaker off
	//
}

void SoundEngine::InitGeneralMidiDevice()
{
	//nothing to do 
	//
}
void SoundEngine::ResetGeneralMidiDevice()
{
	// nothing to do
	//
}

void SoundEngine::InitGUS()
{
	
}
void SoundEngine::ResetGUS()
{
	
}
//=============================================================================================================================

//Midi Commands ===============================================================================================================
void SoundEngine::PCSpeakerNoteOff( int note )
{

	if( note <= 36 )
	{
		note = note + 24;
	}

	//check wheter note is already in list
	for( unsigned int i = 0; i < activeNotes.size(); i++ )
	{
		if( note == activeNotes[i].note )
		{
			activeNotes.erase( activeNotes.begin() + i );
		}
	}

	if( activeNotes.size() <= 0)
	{
		outportb( PC_SPEAKER_PORT, 0); //turn speaker off
	}
}
void SoundEngine::PCSpeakerNoteOn( int note, unsigned char midiChannel )
{
	//already at max Voices?
	if( activeNotes.size() >= maxVoices )
	{
		// kill oldest note
		int oldestTime = 99999;
		int oldestIndex = 0;
		for( unsigned int i = 0; i < activeNotes.size(); i++ )
		{
			if( activeNotes[i].timeStamp < oldestTime )
			{
				oldestTime = activeNotes[i].timeStamp;
				oldestIndex = i;
			}
		}
		PCSpeakerCutNote( activeNotes[oldestIndex].note );
	}

	//check wheter note is already in list
	for( unsigned int i = 0; i < activeNotes.size(); i++ )
	{
		if( note == activeNotes[i].note )
		{
			//nothing more to do
			return;
		}
	}

	//if not put it in List
	if( note <= 36 )
	{
		note = note + 24;
	}

	Note newNote;
	newNote.note = note;
	newNote.timeStamp = time->GetCurrentTimeInMS();
	newNote.midiChannel = midiChannel;
	activeNotes.push_back( newNote );

	if( activeNotes.size() == 1 )
	{
		outportb( PC_SPEAKER_PORT, 3); //turn speaker on
	}	
}
void SoundEngine::PCSpeakerSetTempo( int newTempo )
{
	SetTickRate( newTempo );
	//
}
void SoundEngine::PCSpeakerCutNote( int note )
{
	PCSpeakerNoteOff( note );
	//
}
void SoundEngine::PCSpeakerPlayActiveNotes( int arpeggioRate )
{
	//lets assume arpeggio rate 5
	int ticksToLinger = time->GetInterruptFrequency() / ( arpeggioRate * ( activeNotes.size() + 1 ) );


	if(currentTicksToLinger >= ticksToLinger )
	{
		currentTicksToLinger = 0;

		if( currentActiveNote < activeNotes.size() )
		{
			int frequency = 1193181 / MidiFrequencies[activeNotes[currentActiveNote].note];

			outportb( PC_TIMER_PORT, frequency & 0xFF );
			outportb( PC_TIMER_PORT, ( frequency >> 8 ) & 0xFF );

			currentActiveNote++;
		}
		else
		{
			currentActiveNote = 0;
		}
	}
	currentTicksToLinger++;
}

void SoundEngine::OPLNoteOff( int midiChannel, unsigned char note )
{
	//find note in question
	Note noteInQuestion;
	for( unsigned int i = 0; i < activeNotes.size(); i++ )
	{
		//if yes cut the note
		if( activeNotes[i].note == note && activeNotes[i].midiChannel == midiChannel )
		{
			noteInQuestion = activeNotes[i];
			
			//remove note from active notes
			activeNotes.erase( activeNotes.begin() + i );
			break;
		}
	}

	//update activeVoices list:
	activeVoices[noteInQuestion.playingAtVoice] = false;

	//turn off note
	int tempVoice = noteInQuestion.playingAtVoice;
	int tempAddressPort = OPL3_ADDRESS_PORT;
	int tempDataPort = OPL3_DATA_PORT;
	if( tempVoice > 8 )
	{
		tempVoice = tempVoice - 9;
		tempAddressPort = tempAddressPort+ + 2;
		tempDataPort = tempDataPort + 2;
	}

	outportb( tempAddressPort, 0xA0 + tempVoice );
	outportb( tempDataPort, FNr[note] & 0xFF );

	unsigned char Block = note >> 4;
	outportb( tempAddressPort, 0xB0 + tempVoice );
	outportb( tempDataPort, (FNr[note] >> 8) + (Block << 2) );
}
void SoundEngine::OPLNoteOn( int midiChannel, unsigned char note, unsigned char velocity )
{
	//if velocity is 0, its the same as note off command
	if( velocity == 0 )
	{
		OPLNoteOff( midiChannel, note );
		return;
	}

	//find out wether note+channel is already on
	for( unsigned int i = 0; i < activeNotes.size(); i++ )
	{
		//if yes cut the note
		if( activeNotes[i].note == note && activeNotes[i].midiChannel == midiChannel )
		{
			OPLCutNote( midiChannel, note );
			break;
		}
	}

	//are there free voices
	if( activeNotes.size() >= maxVoices )
	{
		//if not cut oldest note
		int oldestTime = 99999;
		int oldestIndex = 0;
		for( unsigned int i = 0; i < activeNotes.size(); i++ )
		{
			if( activeNotes[i].timeStamp < oldestTime )
			{
				oldestTime = activeNotes[i].timeStamp;
				oldestIndex = i;
			}
		}
		OPLCutNote( activeNotes[oldestIndex].midiChannel, activeNotes[oldestIndex].note );
	}

	//find free voice
	int freeVoice = 0;
	for( unsigned int i = 0; i < maxVoices; i++ )
	{
		if( activeVoices[i] == false )
		{
			freeVoice = i;
			break;
		}
	}
	activeVoices[freeVoice] = true;

	//update active notes table
	Note newNote;
	newNote.note = note;
	newNote.midiChannel = midiChannel;
	newNote.timeStamp = time->GetCurrentTimeInMS();
	newNote.playingAtVoice = freeVoice;
	activeNotes.push_back( newNote );

	//set instrument
	if( midiChannel < (int)activeInstruments.size() )
	{
		ApplyInstrument( activeInstruments[midiChannel], freeVoice, true, true, velocity );
	}

	//actually play the note
	int tempAddressPort = OPL3_ADDRESS_PORT;
	int tempDataPort = OPL3_DATA_PORT;
	if( freeVoice > 8 )
	{
		freeVoice = freeVoice - 9;
		tempAddressPort = tempAddressPort+ + 2;
		tempDataPort = tempDataPort + 2;
	}

	outportb( tempAddressPort, 0xA0 + freeVoice );
	outportb( tempDataPort, FNr[note] & 0xFF );

	outportb( tempAddressPort, 0xB0 + freeVoice );

	unsigned char Block = note >> 4;
	outportb( tempDataPort, (FNr[note] >> 8) + (Block << 2) + 32 );
}
void SoundEngine::OPLControllerEvent()
{
	
}
void SoundEngine::OPLProgramChange()
{
	
}
void SoundEngine::OPLSetTempo( int newTempo )
{
	SetTickRate( newTempo );
	//
}
void SoundEngine::OPLCutNote( int midiChannel, unsigned char note )
{
	//find note in question
	Note noteInQuestion;
	for( unsigned int i = 0; i < activeNotes.size(); i++ )
	{
		//if yes cut the note
		if( activeNotes[i].note == note && activeNotes[i].midiChannel == midiChannel )
		{
			noteInQuestion = activeNotes[i];
			
			//remove note from active notes
			activeNotes.erase( activeNotes.begin() + i );
			break;
		}
	}

	//update activeVoices list:
	activeVoices[noteInQuestion.playingAtVoice] = false;

	//actually turn off note
	int tempVoice = noteInQuestion.playingAtVoice;
	int tempAddressPort = OPL3_ADDRESS_PORT;
	int tempDataPort = OPL3_DATA_PORT;
	if( tempVoice > 8 )
	{
		tempVoice = tempVoice - 9;
		tempAddressPort = tempAddressPort+ + 2;
		tempDataPort = tempDataPort + 2;
	}

	outportb( tempAddressPort, 0x80 + tempVoice );
	outportb( tempDataPort, FNr[note] & 0xF );

	outportb( tempAddressPort, 0xA0 + tempVoice );
	outportb( tempDataPort, FNr[note] & 0xFF );

	unsigned char Block = note >> 4;
	outportb( tempAddressPort, 0xB0 + tempVoice );
	outportb( tempDataPort, (FNr[note] >> 8) + (Block << 2) );
}

void SoundEngine::GeneralMidiNoteOff()
{
	
}
void SoundEngine::GeneralMidiNoteOn()
{
	
}
void SoundEngine::GeneralMidiNoteAfterTouch()
{
	
}
void SoundEngine::GeneralMidiControllerEvent()
{
	
}
void SoundEngine::GeneralMidiProgramChange()
{
	
}
void SoundEngine::GeneralMidiChannelAfterTouch()
{
	
}
void SoundEngine::GeneralMidiPitchBend()
{
	
}
void SoundEngine::GeneralMidiSetTempo()
{
	
}
void SoundEngine::GeneralMidiCutNote()
{
	
}

void SoundEngine::GUSNoteOff()
{
	
}
void SoundEngine::GUSNoteOn()
{
	
}
void SoundEngine::GUSNoteAfterTouch()
{
	
}
void SoundEngine::GUSControllerEvent()
{
	
}
void SoundEngine::GUSProgramChange()
{
	
}
void SoundEngine::GUSChannelAfterTouch()
{
	
}
void SoundEngine::GUSPitchBend()
{
	
}
void SoundEngine::GUSSetTempo()
{
	
}
void SoundEngine::GUSCutNote()
{
	
}
//=============================================================================================================================

//Transport Controls ==========================================================================================================
void SoundEngine::Play()
{
	
}
void SoundEngine::Pause()
{
	
}
void SoundEngine::JumpToStart()
{
	
}
void SoundEngine::JumpToTimeCode()
{
	
}
void SoundEngine::JumpRelative()
{
	
}
void SoundEngine::Loop( bool newLoop )
{
	
}
void SoundEngine::SetSpeed()
{
	
}
void SoundEngine::ChangeSpeed()
{
	
}
long SoundEngine::GetCurrentTimeStamp()
{
	
}
void SoundEngine::SkipToSong( int newSong )
{

}
void SoundEngine::SkipToSong( int newPlayList, int newSong )
{

}
void SoundEngine::SkipToPlayList( int newPlayList )
{
	
}
//=============================================================================================================================

//File Management =============================================================================================================
MIDISong SoundEngine::LoadMidiFromFile( const char* filePath )
{
	bool debug = false;

	MIDISong song;

	FILE *file = fopen( filePath, "rb" );
	if( file == NULL )
	{
		printf( "Error Loading File!\n"  );
		//return NULL;
	}
	else
	{
		if( debug )
		{
			printf("file opened...\n");
			getch();
		}

		//read MIDI Header:
		fread(&song.header, 14, 1, file );
		if( song.header.signature[0] == 'M' && song.header.signature[1] == 'T' &&  song.header.signature[2] == 'h' &&  song.header.signature[3] == 'd')
		{
			//swap endians...
			song.header.headerSize = __builtin_bswap32( song.header.headerSize );
			song.header.fileFormat = __builtin_bswap16( song.header.fileFormat );
			song.header.numberOfTracks = __builtin_bswap16( song.header.numberOfTracks );
			song.header.ticksPerQuarterNote = __builtin_bswap16( song.header.ticksPerQuarterNote );

			if( debug )
			{
				printf("%c%c%c%c\n", song.header.signature[0], song.header.signature[1], song.header.signature[2], song.header.signature[3]);
				printf("MIDI Header signatur correct \n");
				printf("header size = %li \n", song.header.headerSize );
				printf("file format = %hu \n", song.header.fileFormat );
				printf("found %hu tracks\n", song.header.numberOfTracks );
				printf("ticks per QNote = %hu \n", song.header.ticksPerQuarterNote );				
				getch();
			}


			for( int trackNum = 0; trackNum < song.header.numberOfTracks; trackNum++ )
			{
				MIDITrack newTrack;
				//read track header:
				fread(&newTrack.trackHeader, 8, 1, file );

				
				//swap endianess...
				newTrack.trackHeader.lengthOfTrack = __builtin_bswap32( newTrack.trackHeader.lengthOfTrack );

				if( debug )
				{
					printf("	%c%c%c%c\n", newTrack.trackHeader.signature[0], newTrack.trackHeader.signature[1], newTrack.trackHeader.signature[2], newTrack.trackHeader.signature[3]);
					printf("	Track: %i \n", trackNum );
					printf("	lengthOfTrack = %li \n", newTrack.trackHeader.lengthOfTrack );			
					getch();
				}

				bool trackDone = false;
				while( !trackDone )
				{
					//read events:
					MIDIEvent newEvent;

					//read delta time:
					unsigned char timeByte = 0;

					newEvent.deltaTime = 0;



					do
					{
						fread( &timeByte, 1, 1, file );

						newEvent.deltaTime = newEvent.deltaTime << 7;
						newEvent.deltaTime = newEvent.deltaTime + ( timeByte & 0b01111111 );
						if( newTrack.events.size() != 0 )
						{
							newEvent.linearTime = newTrack.events.back().linearTime + newEvent.deltaTime;
							if( newEvent.linearTime > song.length )
							{
								song.length = newEvent.linearTime;
							}
						}
						else
						{
							newEvent.linearTime = 0;
						}
						if( debug )
						{
							printf("		timeByte: %hhx \n", timeByte );
						}
					} while( timeByte & 0b10000000 );
 

					if( debug )
					{
						printf("		newEvent.deltaTime: %i \n", newEvent.deltaTime );
						//getch();
					}

					//read Type of Event:
					unsigned char typeByte = 0;
					fread( &typeByte, 1, 1, file );

					if( debug )
					{
						printf("		typeByte: %hhx \n", typeByte );
						//getch();
					}
					
					if( typeByte >= 128 )
					{
						if( typeByte == 0xFF )
						{
							newEvent.command = typeByte;

							unsigned char metaByte = 0;
							fread( &metaByte, 1, 1, file );
							newEvent.metaCommand = metaByte;
							if( debug )
							{
								printf("			metaByte: %hhx \n", metaByte );
							}
							unsigned char lengthByte = 0;
							fread( &lengthByte, 1, 1, file );
							if( debug )
							{
								printf("			lengthByte: %hhx \n", lengthByte );
							}

							newEvent.data.reserve( lengthByte );
							for( int i = 0; i < lengthByte; i++ )
							{
								char dataByte = 0;
								fread( &dataByte, 1, 1, file );
								newEvent.data.push_back( dataByte );
							}
							if( debug )
							{
								printf("				");
								for( int i = 0; i < lengthByte; i++ )
								{
									printf("%02hhx ", newEvent.data[i]);
								}
								printf("\n");
							}
							if( metaByte == 0x2F )
							{
								trackDone = true;
								if( debug )
								{
									printf("			track Done!" );
								}
							}
						}
						else if( typeByte == 0xF0 || typeByte == 0xF7 )
						{
							newEvent.command = typeByte;

							unsigned char lengthByte = 0;
							fread( &lengthByte, 1, 1, file );
							if( debug )
							{
								printf("			lengthByte: %hhx \n", lengthByte );
							}

							newEvent.data.reserve( lengthByte );
							for( int i = 0; i < lengthByte; i++ )
							{
								char dataByte = 0;
								fread( &dataByte, 1, 1, file );
								newEvent.data.push_back( dataByte );
							}
							if( debug )
							{
								printf("				");
								for( int i = 0; i < lengthByte; i++ )
								{
									printf("%02hhx ", newEvent.data[i]);
								}
								printf("\n");
							}
						}
						else
						{
							//regular byte
							newEvent.command = typeByte & 0xF0;
							newEvent.channel = typeByte & 0x0F;
							if( debug )
							{
								printf( "		regular command: %hhx channel: %hhi\n", newEvent.command, newEvent.channel );
							}
							if( newEvent.command == 0x80 || newEvent.command == 0x90 || newEvent.command == 0xA0 || newEvent.command == 0xB0 || newEvent.command == 0xE0 )
							{
								char dataByte = 0;
								fread( &dataByte, 1, 1, file );
								newEvent.data.push_back( dataByte );
								fread( &dataByte, 1, 1, file );
								newEvent.data.push_back( dataByte );
							}
							if( newEvent.command == 0xC0 || newEvent.command == 0xD0 )
							{
								char dataByte = 0;
								fread( &dataByte, 1, 1, file );
								newEvent.data.push_back( dataByte );
							}
							if( debug )
							{
								printf("				");
								for( unsigned int i = 0; i < newEvent.data.size(); i++ )
								{
									printf("%02hhx ", newEvent.data[i]);
								}
								printf("\n");
							}
						}
						newTrack.events.push_back( newEvent );
					}
					else
					{
						newEvent.command = newTrack.events.back().command;
						if( debug )
						{
							printf( "		running command: %hhx channel: %hhi\n", newEvent.command, newEvent.channel );
						}
						if( newEvent.command == 0x80 || newEvent.command == 0x90 || newEvent.command == 0xA0 || newEvent.command == 0xB0 || newEvent.command == 0xE0 )
						{
							char dataByte = 0;
							newEvent.data.push_back( typeByte );
							fread( &dataByte, 1, 1, file );
							newEvent.data.push_back( dataByte );
						}
						if( newEvent.command == 0xC0 || newEvent.command == 0xD0 )
						{
							newEvent.data.push_back( typeByte );
						}
						if( debug )
						{
							printf("				");
							for( unsigned int i = 0; i < newEvent.data.size(); i++ )
							{
								printf("%02hhx ", newEvent.data[i]);
							}
							printf("\n");
						}
		
						newTrack.events.push_back( newEvent );
						if( debug )
						{
							getch();
						}
					}
					if( debug )
					{
						getch();
					}

				}
				song.tracks.push_back( newTrack );
			}

		}
		else
		{
			printf( "couldnt find signature" );
			//return NULL;
		}
		if( debug )
		{
			printf( "Done Reading File!" );
			getch();
		}
		return song;
	}

	//return NULL;
}
void SoundEngine::SaveMidiToFile()
{
	
}
void SoundEngine::LoadCMFFromFile()
{
	
}
void SaveCMFToFile()
{
	
}
void SoundEngine::LoadIFFFromFile()
{
	
}
void SoundEngine::SaveIFFToFile()
{
	
}
void SoundEngine::LoadRIFFFromFile()
{
	
}
void SoundEngine::SaveRIFFToFile()
{
	
}

SoundBlasterInstrument SoundEngine::LoadSBIFromFile( const char* filePath )
{
	bool debug = false;

	FILE *file = fopen( filePath, "rb" );
	if( file == NULL )
	{
		printf( "Error Loading File!\n"  );
		getch();
	}
	else
	{
		SoundBlasterInstrument in;
		fread(&in.data, sizeof(SoundBlasterInstrumentData), 1, file );
		if( in.data.signature[0] == 'S' && in.data.signature[1] == 'B' &&  in.data.signature[2] == 'I' )
		{
			if( debug )
			{
				printf( "signature: %c%c%c\n", in.data.signature[0], in.data.signature[1], in.data.signature[2] );
				printf( "name: %s\n", in.data.name );
				printf( "modulatorSoundCharacteristic = %02hhx\n", in.data.modulatorSoundCharacteristic );
				printf( "carrierSoundCharacteristic   = %02hhx\n", in.data.carrierSoundCharacteristic );
				printf( "modulatorScalingLevel 	      = %02hhx\n", in.data.modulatorScalingLevel );
				printf( "carrierScalingLevel          = %02hhx\n", in.data.carrierScalingLevel );
				printf( "modulatorAttackDecay         = %02hhx\n", in.data.modulatorAttackDecay );
				printf( "carrierAttackDecay           = %02hhx\n", in.data.carrierAttackDecay );
				printf( "modulatorSustainRelease      = %02hhx\n", in.data.modulatorSustainRelease );
				printf( "carrierSustainRelease        = %02hhx\n", in.data.carrierSustainRelease );
				printf( "modulatorWaveSelect          = %02hhx\n", in.data.modulatorWaveSelect );
				printf( "carrierWaveSelect            = %02hhx\n", in.data.carrierWaveSelect );
				printf( "feedback                     = %02hhx\n", in.data.feedback );
				getch();
			}

			return in;
		}
		else
		{
			printf( "couldnt find signature" );
			getch();
		}
	}
	fclose (file);
}
void SoundEngine::SaveSBIToFile( SoundBlasterInstrument* in, const char* filePath )
{
	FILE *file = fopen( filePath, "wb" );
	
	fwrite (&in->data , sizeof(SoundBlasterInstrumentData), 1, file);
  	fclose (file);
}

SoundBlasterInstrument SoundEngine::LoadIBKFromFile()
{
	
}
void SoundEngine::SaveIBKToFile()
{
	
}

void SoundEngine::LoadPlayListFromFile()
{
	
}
void SoundEngine::SavePlayListToFile()
{
	
}
//=============================================================================================================================

//Song Controls ===============================================================================================================
void SoundEngine::CreateSong()
{
	
}
void SoundEngine::DestroySong()
{
	
}
void SoundEngine::AddSong( MIDISong in, int newID )
{
	in.ID = newID;
	songs.push_back( in );
}
MIDISong* SoundEngine::GetSong( int ID )
{
	for( unsigned int i = 0; i < songs.size(); i++ )
	{
		if( songs[i].ID == ID )
		{
			return &songs[i];
		}
	}

	return NULL;
}
void SoundEngine::RemoveSong( int ID )
{
	for( unsigned int i = 0; i < songs.size(); i++ )
	{
		if( songs[i].ID == ID )
		{
			songs.erase( songs.begin() + i );
		}
	}
}
//=============================================================================================================================

//PlayList ====================================================================================================================
void SoundEngine::CreatePlayList( int newID )
{
	PlayList newPlayList;
	newPlayList.ID = newID;
	playLists.push_back( newPlayList );
}
void SoundEngine::AddPlayList( PlayList* newPlayList )
{
	playLists.push_back( *newPlayList );
	//
}
PlayList* SoundEngine::GetPlayList( int ID )
{
	for( unsigned int i = 0; i < playLists.size(); i++ )
	{
		if( playLists[i].ID == ID )
		{
			return &playLists[i];
		}
	}

	return NULL;
}
void SoundEngine::RemovePlayList( int ID )
{
	for( unsigned int i = 0; i < playLists.size(); i++ )
	{
		if( playLists[i].ID == ID )
		{
			playLists.erase( playLists.begin() + i );
		}
	}
}
void SoundEngine::AddToPlayList( int playListID, MIDISong* in )
{
	for( unsigned int i = 0; i < playLists.size(); i++ )
	{
		if( playLists[i].ID == playListID )
		{
			playLists[i].songs.push_back( in );
		}
	}
}
void SoundEngine::RemoveFromPlayList( int playListID, int index )
{
	for( unsigned int i = 0; i < playLists.size(); i++ )
	{
		if( playLists[i].ID == playListID )
		{
			playLists[i].songs.erase( playLists[i].songs.begin() + index );
		}
	}
}
MIDISong* SoundEngine::GetSongFromPlayList( int playListID, int index )
{
	for( unsigned int i = 0; i < playLists.size(); i++ )
	{
		if( playLists[i].ID == playListID )
		{
			return playLists[i].songs[index];
		}
	}
}
MIDISong* SoundEngine::GetFirstSongFromPlayListByID( int playListID, int ID )
{
	
}
//=============================================================================================================================

//Instrument Controls =========================================================================================================
SoundBlasterInstrument SoundEngine::CreateNewInstrument(const char* name)
{
	SoundBlasterInstrument out;
	strcpy( out.data.signature, "SBI " );
	out.data.signature[3] = 0x1A;
	strcpy( out.data.name, name );
	return out;
}
SoundBlasterInstrument SoundEngine::CreateNewInstrument(const 	char* name, char modulatorSoundCharacteristic, char carrierSoundCharacteristic,
																char modulatorScalingLevel, char carrierScalingLevel,
																char modulatorAttackDecay, char carrierAttackDecay,
																char modulatorSustainRelease, char carrierSustainRelease,
																char modulatorWaveSelect, char carrierWaveSelect,
																char feedback )
{
	SoundBlasterInstrument out;
	strcpy( out.data.signature, "SBI " );
	out.data.signature[3] = 0x1A;
	strcpy( out.data.name, name );
	out.data.modulatorSoundCharacteristic = modulatorSoundCharacteristic;
	out.data.carrierSoundCharacteristic = carrierSoundCharacteristic;
	out.data.modulatorScalingLevel = modulatorScalingLevel;
	out.data.carrierScalingLevel = carrierScalingLevel;
	out.data.modulatorAttackDecay = modulatorAttackDecay;
	out.data.carrierAttackDecay = carrierAttackDecay;
	out.data.modulatorSustainRelease = modulatorSustainRelease;
	out.data.carrierSustainRelease = carrierSustainRelease;
	out.data.modulatorWaveSelect = modulatorWaveSelect;
	out.data.carrierWaveSelect = carrierWaveSelect;
	out.data.feedback = feedback;
	return out;
}

void SoundEngine::AddInstrument( SoundBlasterInstrument* in , int ID )
{
	in->ID = ID;
	instruments.push_back(*in);
	//
}
SoundBlasterInstrument* SoundEngine::GetInstrument( int ID )
{
	for( unsigned int i = 0; i < instruments.size(); i++ )
	{
		if( instruments[i].ID == ID )
		{
			return &instruments[i];
		}
	}
	return NULL;
}
void SoundEngine::RemoveInstrument( int ID )
{
	
}
void SoundEngine::AddActiveInstrument( SoundBlasterInstrument* in )
{
	activeInstruments.push_back(in);
	//
}
SoundBlasterInstrument* SoundEngine::GetActiveInstrument( unsigned int index )
{
	if( index < activeInstruments.size() );
	{
		return activeInstruments[index];
	}
	return NULL;
}
void SoundEngine::RemoveActiveInstrument( int index )
{
	
}
void SoundEngine::ReplaceActiveInstrument( SoundBlasterInstrument* in, int index )
{
	
}

void SoundEngine::ApplyInstrument( SoundBlasterInstrument* in, int voice, bool left, bool right, unsigned char velocity )
{
	int tempAddressPort = OPL3_ADDRESS_PORT;
	int tempDataPort = OPL3_DATA_PORT;

	if( voice > 8 )
	{
		voice = voice - 9;
		tempAddressPort = tempAddressPort+ + 2;
		tempDataPort = tempDataPort + 2;
	}

	char* channelMap = channelMapOPL;
	/*if( currentSoundDevice == DEVICE_SOUND_BLASTER_PRO )
	{
		channelMap = channelMapOPL3;
	}*/

	//loudness:
	//master volume + velocity + instrument volume
	velocity = velocity >> 1;
	velocity = velocity * masterVolume;
	if( velocity > 63 )
	{
		velocity = 63;
	}

	velocity = ~velocity;
	velocity = velocity & 0b00111111;

	velocity = velocity + ( in->data.carrierSoundCharacteristic & 0b11000000 );


	outportb( tempAddressPort, 0x20 + channelMap[voice] + 3 );
	outportb( tempDataPort, in->data.carrierSoundCharacteristic );

	outportb( tempAddressPort, 0x20 + channelMap[voice] );
	outportb( tempDataPort, in->data.modulatorSoundCharacteristic );

	outportb( tempAddressPort, 0x40 + channelMap[voice] + 3 );
	outportb( tempDataPort, velocity );

	outportb( tempAddressPort, 0x40 + channelMap[voice]  );
	outportb( tempDataPort, in->data.modulatorScalingLevel );

	outportb( tempAddressPort, 0x60 + channelMap[voice] + 3 );
	outportb( tempDataPort, in->data.carrierAttackDecay );

	outportb( tempAddressPort, 0x60 + channelMap[voice]  );
	outportb( tempDataPort, in->data.modulatorAttackDecay );

	outportb( tempAddressPort, 0x80 + channelMap[voice] + 3 );
	outportb( tempDataPort, in->data.carrierSustainRelease );

	outportb( tempAddressPort, 0x80 + channelMap[voice]  );
	outportb( tempDataPort, in->data.modulatorSustainRelease );

	outportb( tempAddressPort, 0x80 + channelMap[voice] + 3 );
	outportb( tempDataPort, in->data.carrierSustainRelease );

	outportb( tempAddressPort, 0x80 + channelMap[voice]  );
	outportb( tempDataPort, in->data.modulatorSustainRelease );

	outportb( tempAddressPort, 0xE0 + channelMap[voice] + 3 );
	outportb( tempDataPort, in->data.modulatorWaveSelect );

	outportb( tempAddressPort, 0xE0 + channelMap[voice]  );
	outportb( tempDataPort, in->data.carrierWaveSelect );

	outportb( tempAddressPort, 0xC0 + voice );
	char feedBackByte = in->data.feedback;
	if( right )
	{
		feedBackByte = feedBackByte | 0b00100000;
	}
	if( left )
	{
		feedBackByte = feedBackByte | 0b00010000;
	}
	outportb( tempDataPort, feedBackByte );
}

SoundBlasterInstrument 	SoundEngine::SetInstrumentLevel( SoundBlasterInstrument in, bool op, unsigned char newScalingLevel, unsigned char newLevel )
{
	char newLevelByte = 0;
	newLevelByte = newScalingLevel << 6;
	newLevelByte = newLevelByte & 0b11000000;
	newLevelByte = newLevelByte & ( newLevel & 0b00111111 );

	if( op )
	{
		//modulator
		in.data.modulatorScalingLevel = newLevelByte;
	}
	else
	{
		//carrier
		in.data.carrierScalingLevel = newLevelByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstruemtADSREnvelope( SoundBlasterInstrument in, bool op, unsigned char attack, unsigned char decay, unsigned char sustain, unsigned char release )
{
	char newAttackDecayByte = attack << 4;
	newAttackDecayByte = newAttackDecayByte & 0xF0;
	newAttackDecayByte = newAttackDecayByte & ( decay & 0x0F );
	char newSustainReleaseByte = sustain << 4;
	newSustainReleaseByte = newSustainReleaseByte & 0xF0;
	newSustainReleaseByte = newSustainReleaseByte & ( release & 0x0F );
	if( op )
	{
		//modulator
		in.data.modulatorAttackDecay = newAttackDecayByte;
		in.data.modulatorSustainRelease = newSustainReleaseByte;
	}
	else
	{
		//carrier
		in.data.carrierAttackDecay = newAttackDecayByte;
		in.data.carrierSustainRelease = newSustainReleaseByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentSoundCharacteristic( SoundBlasterInstrument in, bool op, bool amplitudeModulation, bool vibrato, bool rythm, bool bassDrum, bool snareDrum, bool tomTom, bool cymbal, bool hiHat )
{
	char newCharacteristicByte = 0;
	if( amplitudeModulation )
		newCharacteristicByte = newCharacteristicByte | 0b10000000;
	if( vibrato )
		newCharacteristicByte = newCharacteristicByte | 0b01000000;
	if( rythm )
		newCharacteristicByte = newCharacteristicByte | 0b00100000;
	if( bassDrum )
		newCharacteristicByte = newCharacteristicByte | 0b00010000;
	if( snareDrum )
		newCharacteristicByte = newCharacteristicByte | 0b00001000;
	if( tomTom )
		newCharacteristicByte = newCharacteristicByte | 0b00000100;
	if( cymbal )
		newCharacteristicByte = newCharacteristicByte | 0b00000010;
	if( hiHat )
		newCharacteristicByte = newCharacteristicByte | 0b00000001;



	if( op )
	{
		//modulator
		in.data.modulatorSoundCharacteristic = newCharacteristicByte;
	}
	else
	{
		//carrier
		in.data.carrierSoundCharacteristic = newCharacteristicByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentWaveForm( SoundBlasterInstrument in, bool op, char newWaveForm )
{
	newWaveForm = newWaveForm & 0b00000011;

	if( op )
	{
		//modulator
		in.data.modulatorWaveSelect = newWaveForm;
	}
	else
	{
		//carrier
		in.data.carrierWaveSelect = newWaveForm;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentFeedBack( SoundBlasterInstrument in, unsigned char newFeedback, bool newAlgorithm )
{
	char newFeedbackByte = newFeedback << 1;
	newFeedbackByte = newFeedbackByte & 0b00001110;
	newFeedbackByte = newFeedbackByte & ( newAlgorithm & 0x01 );

	in.data.feedback = newFeedbackByte;

	return in;
}


SoundBlasterInstrument 	SoundEngine::SetInstrumentLevel( SoundBlasterInstrument in, bool op, unsigned char newLevelByte )
{
	if( op )
	{
		//modulator
		in.data.modulatorScalingLevel = newLevelByte;
	}
	else
	{
		//carrier
		in.data.carrierScalingLevel = newLevelByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstruemtADSREnvelope( SoundBlasterInstrument in, bool op, unsigned char newAttackByte, unsigned char newSustainByte )
{
	if( op )
	{
		//modulator
		in.data.modulatorAttackDecay = newAttackByte;
		in.data.modulatorSustainRelease = newSustainByte;
	}
	else
	{
		//carrier
		in.data.carrierAttackDecay = newAttackByte;
		in.data.carrierSustainRelease = newSustainByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentSoundCharacteristic( SoundBlasterInstrument in, bool op, unsigned char newCharacteristicByte )
{
	if( op )
	{
		//modulator
		in.data.modulatorSoundCharacteristic = newCharacteristicByte;
	}
	else
	{
		//carrier
		in.data.carrierSoundCharacteristic = newCharacteristicByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentWaveForm( SoundBlasterInstrument in, bool op, unsigned char newWaveFormByte )
{
	if( op )
	{
		//modulator
		in.data.modulatorWaveSelect = newWaveFormByte;
	}
	else
	{
		//carrier
		in.data.carrierWaveSelect = newWaveFormByte;
	}

	return in;
}
SoundBlasterInstrument 	SoundEngine::SetInstrumentFeedBack( SoundBlasterInstrument in, unsigned char newFeedbackByte )
{
	in.data.feedback = newFeedbackByte;

	return in;
}
//=============================================================================================================================




/*void SoundEngine::AddSong( MIDISong* in, int newSongID )
{
	in->ID = newSongID;
	songs.push_back( in );
}
MIDISong* SoundEngine::GetSong( int songID )
{
	for( int i = 0; i < songs.size(); i++ )
	{
		if( songID == songs[i]->ID )
		{
			return songs[i];
		}
	}
	return NULL;
}
void SoundEngine::SetActiveInstrument( SoundBlasterInstrument* in, int channel )
{
	activeInstruments[channel] = *in;
	//
}
SoundBlasterInstrument* SoundEngine::GetActiveInstrument( int channel )
{
	return &activeInstruments[channel];
	//
}

void SoundEngine::ChangeTicksPerQNote( int newSpeed )
{
	globalCurrentSong->header.ticksPerQuarterNote = newSpeed;
}*/
