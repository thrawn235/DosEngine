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
//=========================================================================

//============ global Function that is to be installed as ISR =============
void SoundInterruptRoutine()
{
	
}
//unused dummy functions. meant to calculate the size of TimerInterruptRoutine()
void SoundInterruptRoutineEnd() {} 
//======================================================================



void SoundEngine::InstallSoundInterrupt()
{
	_go32_dpmi_lock_code( ( void* )SoundInterruptRoutine, 100 );
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


SoundEngine::SoundEngine()
{
	InstallSoundInterrupt();
}
SoundEngine::~SoundEngine()
{
	RestoreSoundInterrupt();
}


void SoundEngine::PlaySound()
{

}