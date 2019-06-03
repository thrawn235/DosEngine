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
//int stream[256] = {4650, 4202, 8996, 4700, 2479, 8703, 10794, 14977, 3091, 3671, 10937, 263, 7399, 1373, 12195, 12689, 12679, 11059, 13154, 1141, 13988, 1636, 9353, 265, 9581, 3023, 11906, 14738, 3656, 9023, 13409, 7518, 2062, 14048, 5803, 11876, 14868, 7458, 3687, 4906, 8083, 7547, 9989, 14650, 11862, 6140, 9803, 6982, 13474, 383, 294, 4056, 13734, 2929, 12273, 12019, 10286, 4060, 8532, 6862, 11328, 532, 9995, 10975, 1419, 5143, 9378, 856, 7993, 259, 1621, 14447, 4197, 4070, 8635, 8238, 7923, 2424, 2245, 5755, 5033, 4839, 8155, 14407, 9762, 10976, 14612, 9677, 371, 12627, 12547, 2120, 3564, 7000, 11818, 7437, 14081, 12365, 907, 9039, 10922, 4558, 7059, 8364, 6550, 3875, 7838, 13243, 10598, 13678, 899, 1510, 4153, 14812, 4727, 12507, 7931, 2185, 10572, 3980, 8933, 5263, 7652, 11242, 13408, 12097, 4281, 9858, 4093, 3546, 7721, 5471, 14084, 11710, 9198, 1506, 5768, 13981, 1553, 11209, 9952, 5787, 13012, 10635, 136, 5786, 11005, 4715, 4269, 12215, 7484, 2455, 2916, 12908, 7093, 5625, 6843, 1765, 3630, 11564, 14671, 10952, 10447, 13378, 14881, 7361, 11738, 8482, 2605, 1428, 4310, 3956, 2825, 7324, 2628, 5604, 2157, 14337, 7488, 5070, 14826, 2050, 9967, 2207, 6901, 10163, 4424, 12770, 3996, 14240, 5078, 7146, 3638, 11322, 8923, 7851, 6797, 14630, 8269, 9903, 6125, 641, 9382, 9689, 13956, 14158, 12383, 10655, 3794, 5183, 12026, 3815, 13841, 3174, 1642, 7315, 12907, 11384, 4933, 2148, 13260, 12265, 14517, 1296, 5709, 1651, 12569, 7824, 10289, 9331, 6435, 4949, 4518, 1385, 6475, 13030, 4205, 3079, 14085, 12307, 9138, 10619, 11731, 8116, 9978, 13258, 5392, 4317, 14495, 14787, 13261, 3003, 9115, 8221, 11590, 6227};
//int stream[256] = { 500,-1,0,-1,100,16,100,16,500,700,1000,1500};
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


SoundEngine::SoundEngine( TimeEngine* newTime )
{
	stream = noise;
	time = newTime;
	timerInterruptFrequency = time->GetInterruptFrequency();
	InstallSoundInterrupt();
}
SoundEngine::~SoundEngine()
{
	RestoreSoundInterrupt();

	outportb( 0x61, 0); //turn speaker off
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