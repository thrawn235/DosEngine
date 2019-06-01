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
void TimerInterruptRoutine()
{
	
}
//unused dummy functions. meant to calculate the size of TimerInterruptRoutine()
void TimerInterruptRoutineEnd() {} 
//======================================================================








SoundEngine::SoundEngine()
{
	//sound(320);
	InstallTimerInterrupt();
}
SoundEngine::~SoundEngine()
{
	RestoreTimerInterrupt();
}


void SoundEngine::InstallTimerInterrupt()
{
	//_go32_dpmi_lock_data( ( void* )keys, ( long )sizeof( keys ) );
	_go32_dpmi_lock_code( ( void* )TimerInterruptRoutine, 1000 );
 	_go32_dpmi_get_protected_mode_interrupt_vector( 0x08, &OldISR );
	
	NewISR.pm_offset = ( int )TimerInterruptRoutine;
	NewISR.pm_selector = _go32_my_cs();

	_go32_dpmi_chain_protected_mode_interrupt_vector( 0x08, &NewISR );
}
void SoundEngine::RestoreTimerInterrupt()
{
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x08, &OldISR );
	//
}

void SoundEngine::PlaySound()
{

}