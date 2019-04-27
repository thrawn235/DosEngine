#include "TimeEngine.h"

TimeEngine::TimeEngine()
{
	frameStart = 0;
	frameEnd = 0;
	frameTime = 0;

	//start timer:
	uclock();
}
TimeEngine::~TimeEngine()
{
	//
	//nothing to do
}

//FrameTiming
void TimeEngine::FrameStart()
{
	//
	frameStart = uclock();
}
void TimeEngine::FrameEnd()
{
	frameEnd = uclock();
	frameTime = frameEnd - frameStart;
}
int TimeEngine::GetLastTime()
{
	//
	return frameTime;
}
int TimeEngine::GetCurrentFrameTime()
{
	//
	return uclock() - frameStart;
}

//generalTiming
int TimeEngine::GetCurrentTime()
{
	//
	return uclock();
}

//Conversion
int TimeEngine::TicksToMilliSeconds(uclock_t ticksIn)
{
	//
	return ticksIn / (UCLOCKS_PER_SEC / 1000);
}
int TimeEngine::TicksToSeconds(uclock_t ticksIn)
{
	//
	return ticksIn / UCLOCKS_PER_SEC;
}
int TimeEngine::GetFPS()
{
	//
	if(TicksToMilliSeconds(frameTime) > 0)
	{
		return 1000 / TicksToMilliSeconds(frameTime) ;
	}
	return 0;
}