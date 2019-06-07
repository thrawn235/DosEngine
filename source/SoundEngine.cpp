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
int* stream;
int streamLength = 255;
int streamPos = 0;
bool soundOn = true;
bool repeat = false;
int noteLength = 1;
int notePos = 0;
int BPM = 160;
int timerInterruptFrequency = 0;
//=========================================================================

//============ global Function that is to be installed as ISR =============
void SoundInterruptRoutine()
{
	if( soundOn )
	{
		if( streamPos < streamLength )
		{
			outportb( 0x43, 182 );

			int frequency = 1193181 / 100;

			if( stream[streamPos] != -1 ) //keep same frequency on -1
			{
				outportb( 0x42, stream[streamPos] & 0xFF );
				outportb( 0x42, ( stream[streamPos] >> 8 ) & 0xFF );
			}
			if( stream[streamPos] == 0 )
			{
				outportb( 0x61, 0); //turn speaker off
			}
			else
			{
				outportb( 0x61, 3); //turn speaker on
			}

			notePos++;
			if( notePos >= ((noteLength * timerInterruptFrequency)*60) / BPM )
			{
				notePos = 0;
				streamPos++;
			}
		}
		if( streamPos >= streamLength )
		{
			outportb( 0x61, 0); //turn speaker off

			if( repeat )
			{
				streamPos = 0;
			}
		}
	}
}
//unused dummy functions. meant to calculate the size of TimerInterruptRoutine()
void SoundInterruptRoutineEnd() {} 
//======================================================================



void SoundEngine::InstallSoundInterrupt()
{
	//_go32_dpmi_lock_data( ( void* )stream, ( long )sizeof( stream ) );
	_go32_dpmi_lock_data( ( void* )timerInterruptFrequency, ( long )sizeof( timerInterruptFrequency ) );
	_go32_dpmi_lock_data( ( void* )BPM, ( long )sizeof( BPM ) );
	_go32_dpmi_lock_data( ( void* )noteLength, ( long )sizeof( noteLength ) );
	_go32_dpmi_lock_data( ( void* )notePos, ( long )sizeof( notePos ) );
	_go32_dpmi_lock_data( ( void* )repeat, ( long )sizeof( repeat ) );
	_go32_dpmi_lock_data( ( void* )streamLength, ( long )sizeof( streamLength ) );
	_go32_dpmi_lock_data( ( void* )soundOn, ( long )sizeof( soundOn ) );
	_go32_dpmi_lock_data( ( void* )streamPos, ( long )sizeof( streamPos ) );

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

void SoundEngine::SetADSREnvelope( int channel, bool op, char attack, char decay, char sustain, char release )
{
	outportb( OPL3AddressPort, 0x60 + channelMap[channel] + 3*op );
	
	attack = (attack << 4) & 0xF0;
	attack = attack + (decay & 0x0F);
	outportb( OPL3DataPort, attack );

	outportb( OPL3AddressPort, 0x80 + channelMap[channel] + 3*op);
	
	decay = (decay << 4) & 0xF0;
	decay = decay + (sustain & 0x0F);
	outportb( OPL3DataPort, sustain );
}

void SoundEngine::SetLevel( int channel, bool op, unsigned char newLevel )
{
	outportb( OPL3AddressPort, 0x40 + channelMap[channel] + 3*op );
	outportb( OPL3DataPort, newLevel );
}

void SoundEngine::ResetSoundBlaster()
{
	for( int i = 0; i < 0xF5; i++ )
	{
		outportb( OPL3AddressPort, i );
		outportb( OPL3DataPort, 0 );
	}
}

void SoundEngine::NoteOff( int channel )
{
	outportb( OPL3AddressPort, 0xB0 + channel );
	outportb( OPL3DataPort, 0 );
}

void SoundEngine::PlayNote( int channel, int newFrequency, char octave )
{
	NoteOff( channel );

	outportb( OPL3AddressPort, 0xA0 + channel );
	outportb( OPL3DataPort, newFrequency & 0xFF );

	outportb( OPL3AddressPort, 0xB0 + channel );

	char out = 0;
	out = ( newFrequency >> 8 ) & 0b00000011;
	octave = octave << 2;
	octave = octave & 0b00011100;
	out = out | 0b00100000;
	out = out | octave;

	outportb( OPL3DataPort, out );
}

void SoundEngine::SetSoundCharacteristic( int channel, bool op, bool amplitudeModulation, bool vibrato, bool sustain, char harmonics )
{
	outportb( OPL3AddressPort, 0x20 + channelMap[channel] + op*3 );
	
	harmonics = harmonics & 0x0F;
	if( amplitudeModulation )
	{
		harmonics = harmonics | 0b10000000;
	}
	if( vibrato )
	{
		harmonics = harmonics | 0b01000000;
	}
	if( sustain )
	{
		harmonics = harmonics | 0b00100000;
	}

	outportb( OPL3DataPort, harmonics );
}


void SoundEngine::ApplyInstrument(SoundBlasterInstrument* in, char channel )
{
	outportb( OPL3AddressPort, 0x20 + channelMap[channel] );
	outportb( OPL3DataPort, in->carrierSoundCharacteristic );

	outportb( OPL3AddressPort, 0x20 + channelMap[channel] + 3);
	outportb( OPL3DataPort, in->modulatorSoundCharacteristic );

	outportb( OPL3AddressPort, 0x40 + channelMap[channel] );
	outportb( OPL3DataPort, in->carrierScalingLevel );

	outportb( OPL3AddressPort, 0x40 + channelMap[channel] + 3 );
	outportb( OPL3DataPort, in->modulatorScalingLevel );

	outportb( OPL3AddressPort, 0x60 + channelMap[channel] );
	outportb( OPL3DataPort, in->carrierAttackDelay );

	outportb( OPL3AddressPort, 0x60 + channelMap[channel] + 3 );
	outportb( OPL3DataPort, in->modulatorAttackDelay );

	outportb( OPL3AddressPort, 0x80 + channelMap[channel] );
	outportb( OPL3DataPort, in->carrierSustainRelease );

	outportb( OPL3AddressPort, 0x80 + channelMap[channel] + 3 );
	outportb( OPL3DataPort, in->modulatorSustainRelease );

	outportb( OPL3AddressPort, 0x80 + channelMap[channel] );
	outportb( OPL3DataPort, in->carrierSustainRelease );

	outportb( OPL3AddressPort, 0x80 + channelMap[channel] + 3 );
	outportb( OPL3DataPort, in->modulatorSustainRelease );

	outportb( OPL3AddressPort, 0xE0 + channelMap[channel] );
	outportb( OPL3DataPort, in->modulatorWaveSelect );

	outportb( OPL3AddressPort, 0xE0 + channelMap[channel] + 3 );
	outportb( OPL3DataPort, in->carrierWaveSelect );

	outportb( OPL3AddressPort, 0xC0 + channel );
	outportb( OPL3DataPort, in->feedback );
}

SoundBlasterInstrument SoundEngine::CreateNewInstrument( const char* name )
{
	SoundBlasterInstrument out;
	strcpy( out.signature, "SBI " );
	out.signature[3] = 0x1A;
	strcpy( out.name, name );
	return out;
}

SoundBlasterInstrument SoundEngine::CreateNewInstrument( const char* name, char modulatorSoundCharacteristic, char carrierSoundCharacteristic,
														char modulatorScalingLevel, char carrierScalingLevel,
														char modulatorAttackDelay, char carrierAttackDelay,
														char modulatorSustainRelease, char carrierSustainRelease,
														char modulatorWaveSelect, char carrierWaveSelect,
														char feedback )
{
	SoundBlasterInstrument out;
	strcpy( out.signature, "SBI " );
	out.signature[3] = 0x1A;
	strcpy( out.name, name );
	out.modulatorSoundCharacteristic = modulatorSoundCharacteristic;
	out.carrierSoundCharacteristic = carrierSoundCharacteristic;
	out.modulatorScalingLevel = modulatorScalingLevel;
	out.carrierScalingLevel = carrierScalingLevel;
	out.modulatorAttackDelay = modulatorAttackDelay;
	out.carrierAttackDelay = carrierAttackDelay;
	out.modulatorSustainRelease = modulatorSustainRelease;
	out.carrierSustainRelease = carrierSustainRelease;
	out.modulatorWaveSelect = modulatorWaveSelect;
	out.carrierWaveSelect = carrierWaveSelect;
	out.feedback = feedback;
	return out;
}

void SoundEngine::SaveInstrumentToFile( SoundBlasterInstrument* in, const char* filePath )
{
	FILE *file = fopen( filePath, "wb" );
	
	fwrite (in , sizeof(SoundBlasterInstrument), 1, file);
  	fclose (file);
}

SoundBlasterInstrument SoundEngine::LoadInstrumentFromFile( const char* filePath )
{
	FILE *file = fopen( filePath, "rb" );
	if( file == NULL )
	{
		printf( "Error Loading File!\n"  );
	}
	else
	{
		SoundBlasterInstrument in;
		fread(&in, sizeof(SoundBlasterInstrument), 1, file );
		if( in.signature[0] == 'S' && in.signature[1] == 'B' &&  in.signature[2] == 'I' )
		{
			return in;
		}
		else
		{
			printf( "couldnt find signature" );
		}
	}
	fclose (file);
}

void SoundEngine::AddInstrument( SoundBlasterInstrument* in )
{
	instruments.push_back(*in);
}
SoundBlasterInstrument* SoundEngine::GetInstrument( int index )
{
	return &instruments[index];
}
void SoundEngine::DeleteInstrument( int index )
{
	instruments.erase(instruments.begin() + index );
}
void SoundEngine::ReplaceInstrument( SoundBlasterInstrument* in, int index )
{
	instruments[index] = *in;
}

SoundEngine::SoundEngine( TimeEngine* newTime )
{
	stream = noise;
	time = newTime;
	timerInterruptFrequency = time->GetInterruptFrequency();
	InstallSoundInterrupt();
	SoundOff();

	ResetSoundBlaster();

	outportb( OPL3AddressPort + 1, 4 );
	outportb( OPL3DataPort, 0 );

	outportb( OPL3AddressPort + 3, 4 );
	outportb( OPL3DataPort, 0 );

	outportb( OPL3AddressPort + 3, 5 );
	outportb( OPL3DataPort, 0 );

	//soun blaster test:
	//outportb( OPL3AddressPort, 0x20 );
	//outportb( OPL3DataPort, 16 );

	/*unsigned char chan = 8;

	outportb( OPL3AddressPort, 0x20 + chan );
	outportb( OPL3DataPort, 0x01 );

	outportb( OPL3AddressPort, 0x40 + chan);
	outportb( OPL3DataPort, 0x10 );

	outportb( OPL3AddressPort, 0x60 + chan);
	outportb( OPL3DataPort, 0xF0 );

	outportb( OPL3AddressPort, 0x80 + chan);
	outportb( OPL3DataPort, 0x77 );

	outportb( OPL3AddressPort, 0xA0 + 3);
	outportb( OPL3DataPort, 0x98 );

	outportb( OPL3AddressPort, 0x23 + chan);
	outportb( OPL3DataPort, 0x01 );

	outportb( OPL3AddressPort, 0x43 + chan);
	outportb( OPL3DataPort, 0x00 );

	outportb( OPL3AddressPort, 0x63 + chan);
	outportb( OPL3DataPort, 0xF0 );

	outportb( OPL3AddressPort, 0x83 + chan);
	outportb( OPL3DataPort, 0x77 );

	outportb( OPL3AddressPort, 0xB0 + 3);
	outportb( OPL3DataPort, 0x31 );*/

	//SoundBlasterInstrument inst = CreateNewInstrument( "test", 0,0, 0,0, 0x55, 0x66, 0x44, 0x55, 0,0 ,0);
	//SaveInstrumentToFile(&inst, "testinst.sbi");

	LoadMIDIFile( "./audio/music/equinoxe.mid" );
}
SoundEngine::~SoundEngine()
{
	RestoreSoundInterrupt();

	ResetSoundBlaster();
	outportb( 0x61, 0); //turn speaker off

	NoteOff( 0 );
}


void SoundEngine::PlaySound( bool newRepeat )
{
	streamPos = 0;
	repeat = newRepeat;
}


void SoundEngine::SoundOn()
{
	soundOn = true;
	outportb( 0x61, 3); //turn speaker on
}
void SoundEngine::SoundOff()
{	
	soundOn = false;
	outportb( 0x61, 0); //turn speaker off
}

MIDISong* SoundEngine::LoadMIDIFile( const char* filePath )
{
	bool debug = false;

	MIDISong* song = NULL;

	FILE *file = fopen( filePath, "rb" );
	if( file == NULL )
	{
		printf( "Error Loading File!\n"  );
		return NULL;
	}
	else
	{
		if( debug )
		{
			printf("file opened...\n");
			getch();
		}
		song = new MIDISong;

		//read MIDI Header:
		fread(&song->header, 14, 1, file );
		if( song->header.signature[0] == 'M' && song->header.signature[1] == 'T' &&  song->header.signature[2] == 'h' &&  song->header.signature[3] == 'd')
		{
			//swap endians...
			song->header.headerSize = __builtin_bswap32( song->header.headerSize );
			song->header.fileFormat = __builtin_bswap16( song->header.fileFormat );
			song->header.numberOfTracks = __builtin_bswap16( song->header.numberOfTracks );
			song->header.ticksPerQuerterNote = __builtin_bswap16( song->header.ticksPerQuerterNote );

			if( debug )
			{
				printf("%c%c%c%c\n", song->header.signature[0], song->header.signature[1], song->header.signature[2], song->header.signature[3]);
				printf("MIDI Header signatur correct \n");
				printf("header size = %li \n", song->header.headerSize );
				printf("file format = %hi \n", song->header.fileFormat );
				printf("found %hi tracks\n", song->header.numberOfTracks );
				printf("ticks per QNote = %hi \n", song->header.ticksPerQuerterNote );				
				getch();
			}


			for( int trackNum = 1; trackNum < song->header.numberOfTracks; trackNum++ )
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
					fread( &timeByte, 1, 1, file );
					if( debug )
					{
						printf("		timeByte: %hhx \n", timeByte );
					}
					while( timeByte & 0b10000000 )
					{
						fread( &timeByte, 1, 1, file );
						timeByte = timeByte | 0b01111111;
						newEvent.deltaTime << 7;
						newEvent.deltaTime = newEvent.deltaTime & 0b1000000;
						newEvent.deltaTime = newEvent.deltaTime + timeByte;
						if( debug )
						{
							printf("			timeByte: %hhx \n", timeByte );
						}
					}
					newEvent.deltaTime = newEvent.deltaTime & 0b1000000;
					newEvent.deltaTime = newEvent.deltaTime + timeByte; 

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
					
					if( typeByte & 0b1000000 != 0 )
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
								for( int i = 0; i < newEvent.data.size(); i++ )
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
							for( int i = 0; i < newEvent.data.size(); i++ )
							{
								printf("%02hhx ", newEvent.data[i]);
							}
							printf("\n");
						}
		
						newTrack.events.push_back( newEvent );
					}
					if( debug && trackNum >= 14 )
					{
						getch();
					}

				}
				song->tracks.push_back( newTrack );
			}

		}
		else
		{
			printf( "couldnt find signature" );
			return NULL;
		}

		return song;
	}

	return NULL;
};