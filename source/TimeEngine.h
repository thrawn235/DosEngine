///////////////////////////////////////////////////////////////////////////////////////
//
//	TimeEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//	It does three things.
//	calculates frametimes for an FPS counter
//	it calculates the Delta factor for movement correction
//	Game Objects can ask it for timestamps ( cooldowns etc )
//	consider that the sound engine uses the same interrupt!
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef TIMEENGINE
#define TIMEENGINE


//================== includes ==========================
//c++ includes:
#include <vector>

//djgpp includes:
//#include <time.h>
#include <sys/nearptr.h>
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:

//=======================================================

using namespace std;

struct TimeStamp
{
	int 		id;
	unsigned long long 	timeStamp;
};


class TimeEngine
{
protected:
	unsigned long long			frameStart;
	unsigned long long			frameEnd;
	unsigned long long			frameTime;

	vector<TimeStamp> 		timeStamps;
	int 					highestTimeStampID;

	int 	interruptFrequency;
	float 	ticksPerSecond;

	bool 	ticked;

	_go32_dpmi_seginfo OldISR, NewISR;

public:
	TimeEngine 					();
	~TimeEngine 				();

	void InstallTimerInterrupt 	();
	void RestoreTimerInterrupt 	();

	//Set
	void SetInterruptFrequency	( int newInterruptFrequency );
	void SetTicked 				( bool newTicked );

	//Get
	int GetInterruptFrequency 	();
	int GetTicksPerSecond 		();

	//FrameTiming
	void FrameStart 			();
	void FrameEnd 				();
	int GetLastTime				();
	int GetCurrentFrameTime 	();

	//generalTiming
	int GetCurrentTime 			();
	float GetCurrentTimeInMS	();

	//Conversion
	float TicksToMilliSeconds 	( unsigned long long ticksIn );
	float TicksToSeconds 			( unsigned long long ticksIn );
	float GetFPS 					();

	//delta
	float GetDelta 				();

	//time stamps:
	int AddTimeStamp 			();
	void RemoveTimeStamp 		( int id );
	unsigned long long GetTimeStamp 		( int id );
	unsigned long long GetTimeSinceStamp 	( int id );
	void ClearTimeStamps 		();

	void WaitForTicked 			();
};



#endif