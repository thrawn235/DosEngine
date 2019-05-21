///////////////////////////////////////////////////////////////////////////////////////
//
//	InputEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//	Memberfunctions of the InputEngine
//	see InputEngine.h for details
//
///////////////////////////////////////////////////////////////////////////////////////


#include "InputEngine.h"

//============= global Variables for the Interrupt Service Routine ========
bool keys[256];
//=========================================================================

//============ global Function that is to be installed as ISR =============
void KeyboardInterruptRoutine()
{
	unsigned char scanCode = 0;
	scanCode = inportb( 0x60 );
	if( scanCode != 0 )
	{
		if( scanCode <= 80 && scanCode )
		{
			keys[scanCode] = true;
		}
		else
		{
			keys[scanCode - 0x80] = false;
		}
		if( scanCode == 0xe0 )
		{
			unsigned char scanCode2 = 0;
			scanCode2 = inportb( 0x60 );
			if( scanCode2 <= 80 )
			{
				keys[scanCode2+100] = true;
			}
			else
			{
				keys[scanCode2+100 - 0x80] = false;
			}
		}
	}

	char temp;
	outportb( 0x61, ( temp = inportb( 0x61 )) | 0x80 );
	outportb( 0x61, temp );
}
//unused dummy functions. meant to calculate the size of KeyboardInterruptRoutine()
void KeyboardInterruptRoutineEnd() {} 
//======================================================================








InputEngine::InputEngine()
{
	for( unsigned int i = 0; i < 256; i++ )
	{
		keys[i] = false;
	}

	InstallKeyboardInterrupt();
}
InputEngine::~InputEngine()
{
	RestoreKeyboardInterrupt();
	ClearBiosKeyBuffer();
}


void InputEngine::InstallKeyboardInterrupt()
{
	_go32_dpmi_lock_data( ( void* )keys, ( long )sizeof( keys ) );
	_go32_dpmi_lock_code( ( void* )KeyboardInterruptRoutine, 1000 );
 	_go32_dpmi_get_protected_mode_interrupt_vector( 0x09, &OldISR );
	
	NewISR.pm_offset = ( int )KeyboardInterruptRoutine;
	NewISR.pm_selector = _go32_my_cs();

	_go32_dpmi_chain_protected_mode_interrupt_vector( 0x09, &NewISR );
}

void InputEngine::RestoreKeyboardInterrupt()
{
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x09, &OldISR );
	//
}
void InputEngine::CheckKeys()
{
	//unused function,
	//formerly used to poll the keyboard

	unsigned char scanCode = 0;
	scanCode = inportb( 0x60 );
	if( scanCode != 0 )
	{
		if( scanCode <= 80 && scanCode )
		{
			keys[scanCode] = true;
		}
		else
		{
			keys[scanCode - 0x80] = false;
		}
		if( scanCode == 0xe0 )
		{
			unsigned char scanCode2 = 0;
			scanCode2 = inportb( 0x60 );
			if( scanCode2 <= 80 )
			{
				keys[scanCode2+100] = true;
			}
			else
			{
				keys[scanCode2+100 - 0x80] = false;
			}
		}
	}

	char temp;
	outportb( 0x61, ( temp = inportb( 0x61 )) | 0x80 );
	outportb( 0x61, temp );
	
	//ClearBiosKeyBuffer();
}
bool InputEngine::KeyDown( unsigned char scanCode )
{
	return keys[scanCode];
	//
}
bool InputEngine::AnyKeyDown()
{
	for( int i = 0; i < 255; i++ )
	{
		if( keys[i] == true )
		{
			return true;
		}
	}
	return false;
}
void InputEngine::ClearBiosKeyBuffer()
{
	//set head and tail variables of the Keybuffer in bios to 0
	_farpokeb( _dos_ds, 0x0041a, 0 );
	_farpokeb( _dos_ds, 0x0041c, 0 );
}