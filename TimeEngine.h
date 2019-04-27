#ifndef TIMEENGINE
#define TIMEENGINE

//c++ includes:
#include <vector>

//djgpp includes:
#include <time.h>

//own includes:

using namespace std;

class TimeEngine
{
protected:
	uclock_t frameStart;
	uclock_t frameEnd;
	uclock_t frameTime;

public:
	TimeEngine();
	~TimeEngine();

	//FrameTiming
	void FrameStart();
	void FrameEnd();
	int GetLastTime();
	int GetCurrentFrameTime();

	//generalTiming
	int GetCurrentTime();

	//Conversion
	int TicksToMilliSeconds(uclock_t ticksIn);
	int TicksToSeconds(uclock_t ticksIn);
	int GetFPS();
};


#endif