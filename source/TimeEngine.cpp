
///////////////////////////////////////////////////////////////////////////////////////
//
//	TimeEngine.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	Memberfunctions of the TimeEngine
//	see TimeEngine.h for details
//
///////////////////////////////////////////////////////////////////////////////////////


#include "TimeEngine.h"


TimeEngine::TimeEngine()
{
	frameStart 	= 0;
	frameEnd  	= 0;
	frameTime 	= 0;

	//start timer:
	uclock();

	highestTimeStampID = 0;
}
TimeEngine::~TimeEngine()
{
	//nothing to do
	//
}

//FrameTiming
void TimeEngine::FrameStart()
{
	frameStart = uclock();
	//
}
void TimeEngine::FrameEnd()
{
	frameEnd  = uclock();
	frameTime = frameEnd - frameStart;
}
int TimeEngine::GetLastTime()
{
	return frameTime;
	//
}
int TimeEngine::GetCurrentFrameTime()
{
	return uclock() - frameStart;
	//
}

//generalTiming
int TimeEngine::GetCurrentTime()
{
	//
	return uclock();
}
int TimeEngine::GetCurrentTimeInMS()
{
	//
	return uclock() / ( UCLOCKS_PER_SEC / 1000 );
}

//Conversion
int TimeEngine::TicksToMilliSeconds( uclock_t ticksIn )
{
	//
	return ticksIn / ( UCLOCKS_PER_SEC / 1000 );
}
int TimeEngine::TicksToSeconds( uclock_t ticksIn )
{
	//
	return ticksIn / UCLOCKS_PER_SEC;
}
int TimeEngine::GetFPS()
{
	if( TicksToMilliSeconds( frameTime ) > 0 )
	{
		return 1000 / TicksToMilliSeconds( frameTime ) ;
	}
	return 0;
}

//delta
float TimeEngine::GetDelta()
{
	return TicksToMilliSeconds( frameTime ) / 14;
	//
}

//time stamps:
int TimeEngine::AddTimeStamp()
{
	TimeStamp newTimeStamp;

	newTimeStamp.id = highestTimeStampID + 1;

	highestTimeStampID++;

	newTimeStamp.timeStamp = uclock();

	timeStamps.push_back( newTimeStamp );
	
	return newTimeStamp.id;
}
void TimeEngine::RemoveTimeStamp( int id )
{
	for( unsigned int i = 0; i < timeStamps.size(); i++ )
	{
		if( timeStamps[i].id == id )
		{
			timeStamps.erase( timeStamps.begin() + i );
		}
	}
}
uclock_t TimeEngine::GetTimeStamp( int id )
{
	for( unsigned int i = 0; i < timeStamps.size(); i++ )
	{
		if( timeStamps[i].id == id )
		{
			return timeStamps[i].timeStamp;
		}
	}
	return 0;
}
uclock_t TimeEngine::GetTimeSinceStamp( int id )
{
	for( unsigned int i = 0; i < timeStamps.size(); i++ )
	{
		if( timeStamps[i].id == id )
		{
			return uclock() - timeStamps[i].timeStamp;
		}
	}
	return 0;
}
void TimeEngine::ClearTimeStamps()
{
	timeStamps.clear();
	//
}