///////////////////////////////////////////////////////////////////////////////////////
//
//	SoundEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//	The Sound Engine is meant to play PC speaker sounds, using an interrupt
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef SOUNDENGINE
#define SOUNDENGINE


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

using namespace std;


class SoundEngine
{
protected:

public:
	SoundEngine 				();
	~SoundEngine 				();

	void InstallTimerInterrupt 	();
	void RestoreTimerInterrupt 	();

	void PlaySound 				();
};



#endif