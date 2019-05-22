///////////////////////////////////////////////////////////////////////////////////////
//
//	InputEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//	The Class is supposed to handle all input
//	keyboard, mouse, joystick etc
//	its just handling keyboard right now
//	it installs an ISR that does the keybord handling
//	for future reference. polling is no good for games
//	its easy to miss key presses when polling
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef INPUTENGINE
#define INPUTENGINE


//================== includes ==========================
//c++ includes:
#include <vector>

//djgpp includes:
#include <sys/nearptr.h>
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:

//=======================================================

//==================== defines ==========================
#define ESC 		0x01
#define SPACE 		0x39
#define ENTER 		0x1C
#define LCTRL 		0x1D
#define LSHIFT 		0x2A
#define LALT 		0x38
#define TAB			0x0f
#define KEY_W 		0x11
#define KEY_A 		0x1E
#define KEY_S 		0x1f
#define KEY_D 		0x20
#define KEY_Q 		0x10
#define KEY_E 		0x12
#define KEY_R 		0x13
#define KEY_F 		0x21
#define KEY_UP 		0x48
#define KEY_DOWN	0x50
#define KEY_LEFT 	0x4b
#define KEY_RIGHT 	0x4d
#define KEY_F1	 	0x3b
#define KEY_F2	 	0x3c
#define KEY_F3	 	0x3d
#define KEY_F4	 	0x3e
#define KEY_F5	 	0x3f
#define KEY_F6	 	0x40
#define KEY_F7	 	0x41
#define KEY_F8	 	0x42
#define KEY_F9	 	0x43
#define KEY_F10	 	0x44
//=======================================================


using namespace std;

//================= global Variables ====================
//see InputEngine.h for keys[] global variable!
//=======================================================


class InputEngine
{
protected:
	_go32_dpmi_seginfo OldISR, NewISR;

public:
	InputEngine 					();
	~InputEngine 					();

	void InstallKeyboardInterrupt 	();
	void RestoreKeyboardInterrupt 	();

	void CheckKeys 					();
	bool KeyDown 					( unsigned char scanCode );
	bool AnyKeyDown					();

	void ClearBiosKeyBuffer 		();
};

#endif