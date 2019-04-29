#ifndef TIMEENGINE
#define TIMEENGINE

//c++ includes:
#include <vector>

//djgpp includes:
#include <time.h>

//own includes:

using namespace std;

struct TimeStamp
{
	int id;
	uclock_t timeStamp;
};


class TimeEngine
{
protected:
	uclock_t frameStart;
	uclock_t frameEnd;
	uclock_t frameTime;

	vector<TimeStamp> timeStamps;
	int highestTimeStampID;

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

	//delta
	float GetDelta();

	//time stamps:
	int AddTimeStamp();
	void RemoveTimeStamp(int id);
	uclock_t GetTimeStamp(int id);
	uclock_t GetTimeSinceStamp(int id);
	void ClearTimeStamps();
};



#endif