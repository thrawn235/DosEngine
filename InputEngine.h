#ifndef INPUTENGINE
#define INPUTENGINE

//c++ includes:
#include <vector>

//djgpp includes:
#include <sys/nearptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:

//defines
#define ESC 	0x01
#define SPACE 	0x39
#define ENTER 	0x1C
#define LCTRL 	0x1D
#define LSHIFT 	0x2A
#define LALT 	0x38
#define TAB		0x0f
#define KEY_W 	0x11
#define KEY_A 	0x1E
#define KEY_S 	0x1f
#define KEY_D 	0x20
#define KEY_Q 	0x10
#define KEY_E 	0x12
#define KEY_R 	0x13
#define KEY_F 	0x21

using namespace std;

class InputEngine
{
protected:
	bool keys[256];

public:
	InputEngine();
	~InputEngine();

	void CheckKeys();
	bool KeyDown(unsigned char scanCode);

	//clear Bios KeyBuffer
};

#endif