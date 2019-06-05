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
	outportb( OPL3AddressPort, 0x60 + channel + 3*op );
	
	attack = (attack << 4) & 0xF0;
	attack = attack + (decay & 0x0F);
	outportb( OPL3DataPort, attack );

	outportb( OPL3AddressPort, 0x80 + channel + 3*op);
	
	decay = (decay << 4) & 0xF0;
	decay = decay + (sustain & 0x0F);
	outportb( OPL3DataPort, sustain );
}

void SoundEngine::SetLevel( int channel, bool op, unsigned char newLevel )
{
	outportb( OPL3AddressPort, 0x40 + channel + 3*op );
	outportb( OPL3DataPort, newLevel );
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



SoundEngine::SoundEngine( TimeEngine* newTime )
{
	stream = noise;
	time = newTime;
	timerInterruptFrequency = time->GetInterruptFrequency();
	InstallSoundInterrupt();
	SoundOff();

	//soun blaster test:
	outportb( OPL3AddressPort, 0x20 );
	outportb( OPL3DataPort, 16 );

	SetLevel( 0,0, 0x00 );

	SetADSREnvelope( 0, 0, 5,5,5,5 );

	outportb( OPL3AddressPort, 0x23 );
	outportb( OPL3DataPort, 0x01 );

	SetLevel( 0, 1, 0 );

	SetADSREnvelope( 0, 1, 5,5,5,5 );

	PlayNote( 0, 1000, 2 );
}
SoundEngine::~SoundEngine()
{
	RestoreSoundInterrupt();

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