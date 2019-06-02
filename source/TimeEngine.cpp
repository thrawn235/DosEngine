
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


//============= global Variables for the Interrupt Service Routine ========
unsigned long long timeCount = 0;
int globalTicksPerSecond = 0;
int globalInterruptFrequency = 0;
//=========================================================================

//============ global Function that is to be installed as ISR =============
void TimerInterruptRoutine()
{
	timeCount = timeCount + globalTicksPerSecond / globalInterruptFrequency;
}
//unused dummy functions. meant to calculate the size of TimerInterruptRoutine()
void TimerInterruptRoutineEnd() {} 
//======================================================================




TimeEngine::TimeEngine()
{
	frameStart 	= 0;
	frameEnd  	= 0;
	frameTime 	= 0;

	interruptFrequency = 140;
	globalInterruptFrequency = interruptFrequency;
	ticksPerSecond = interruptFrequency;
	globalTicksPerSecond = ticksPerSecond;


	//start timer:
	//set programmable interrupt timer to specific frequency
	int freq = 1193182 / interruptFrequency;
	outportb( 0x43, 0x34 );
	outportb( 0x40, freq & 0xFF );
	outportb( 0x40, ( freq >> 8 ) & 0xFF );
	InstallTimerInterrupt();
	//uclock();

	highestTimeStampID = 0;
}
TimeEngine::~TimeEngine()
{
	RestoreTimerInterrupt();
	//
}


void TimeEngine::InstallTimerInterrupt()
{
	_go32_dpmi_lock_data( ( void* )timeCount, ( long )sizeof( timeCount ) );
	_go32_dpmi_lock_data( ( void* )globalTicksPerSecond, ( long )sizeof( globalTicksPerSecond ) );
	_go32_dpmi_lock_code( ( void* )TimerInterruptRoutine, 100 );
 	_go32_dpmi_get_protected_mode_interrupt_vector( 0x08, &OldISR );
	
	NewISR.pm_offset = ( int )TimerInterruptRoutine;
	NewISR.pm_selector = _go32_my_cs();

	_go32_dpmi_chain_protected_mode_interrupt_vector( 0x08, &NewISR );
}
void TimeEngine::RestoreTimerInterrupt()
{
	_go32_dpmi_set_protected_mode_interrupt_vector( 0x08, &OldISR );
	//
}

//FrameTiming
void TimeEngine::FrameStart()
{
	//frameStart = uclock();
	frameStart = timeCount;
	//
}
void TimeEngine::FrameEnd()
{
	//frameEnd  = uclock();
	frameEnd = timeCount;
	frameTime = frameEnd - frameStart;
}
int TimeEngine::GetLastTime()
{
	return frameTime;
	//
}
int TimeEngine::GetCurrentFrameTime()
{
	//return uclock() - frameStart;
	return timeCount - frameStart;
	//
}

//generalTiming
int TimeEngine::GetCurrentTime()
{
	//
	//return uclock();
	return timeCount;
}
int TimeEngine::GetCurrentTimeInMS()
{
	//
	//return uclock() / ( UCLOCKS_PER_SEC / 1000 );
	return timeCount / ( ticksPerSecond / 1000 );
}

//Conversion
int TimeEngine::TicksToMilliSeconds( unsigned long long ticksIn )
{
	//
	//return ticksIn / ( UCLOCKS_PER_SEC / 1000 );
	return ticksIn / ( ticksPerSecond / 1000 );
}
int TimeEngine::TicksToSeconds( unsigned long long ticksIn )
{
	//
	//return ticksIn / UCLOCKS_PER_SEC;
	return ticksIn / ticksPerSecond;
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
	float delta = TicksToMilliSeconds( frameTime ) / 14;
	if( delta >= 4)
	{
		return 4;
	}

	return delta;
}

//time stamps:
int TimeEngine::AddTimeStamp()
{
	TimeStamp newTimeStamp;

	newTimeStamp.id = highestTimeStampID + 1;

	highestTimeStampID++;

	//newTimeStamp.timeStamp = uclock();
	newTimeStamp.timeStamp = timeCount;

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
unsigned long long TimeEngine::GetTimeStamp( int id )
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
unsigned long long TimeEngine::GetTimeSinceStamp( int id )
{
	for( unsigned int i = 0; i < timeStamps.size(); i++ )
	{
		if( timeStamps[i].id == id )
		{
			//return uclock() - timeStamps[i].timeStamp;
			return timeCount - timeStamps[i].timeStamp;
		}
	}
	return 0;
}
void TimeEngine::ClearTimeStamps()
{
	timeStamps.clear();
	//
}