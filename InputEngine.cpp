#include "InputEngine.h"

InputEngine::InputEngine()
{
	for(unsigned int i = 0; i < 256; i++)
	{
		keys[i] = false;
	}
}
InputEngine::~InputEngine()
{

}

void InputEngine::CheckKeys()
{
	unsigned char scanCode = 0;
	scanCode = inportb(0x60);
	if(scanCode != 0)
	{
		if(scanCode <= 80 && scanCode)
		{
			keys[scanCode] = true;
		}
		else
		{
			keys[scanCode - 0x80] = false;
		}
		if(scanCode == 0xe0)
		{
			unsigned char scanCode2 = 0;
			scanCode2 = inportb(0x60);
			if(scanCode2 <= 80)
			{
				keys[scanCode2+100] = true;
			}
			else
			{
				keys[scanCode2+100 - 0x80] = false;
			}
		}
	}

	ClearBiosKeyBuffer();
}
bool InputEngine::KeyDown(unsigned char scanCode)
{
	//
	return keys[scanCode];
}
void InputEngine::ClearBiosKeyBuffer()
{
	//set head and tail variables of the Keybuffer in bios to 0
	_farpokeb(_dos_ds, 0x0041a, 0);
	_farpokeb(_dos_ds, 0x0041c, 0);
}