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

	_go32_dpmi_seginfo OldISR, NewISR;

public:
	TimeEngine 					();
	~TimeEngine 				();

	void InstallTimerInterrupt 	();
	void RestoreTimerInterrupt 	();

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
	int GetCurrentTimeInMS		();

	//Conversion
	int TicksToMilliSeconds 	( unsigned long long ticksIn );
	int TicksToSeconds 			( unsigned long long ticksIn );
	int GetFPS 					();

	//delta
	float GetDelta 				();

	//time stamps:
	int AddTimeStamp 			();
	void RemoveTimeStamp 		( int id );
	unsigned long long GetTimeStamp 		( int id );
	unsigned long long GetTimeSinceStamp 	( int id );
	void ClearTimeStamps 		();
};



#endif