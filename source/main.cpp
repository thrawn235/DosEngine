///////////////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	main function. entry point of the Program
//	it will possibly contain the mainloop, file loading etc
//	its also possible all that will move to a GameEngine mothod ad one time
//
///////////////////////////////////////////////////////////////////////////////////////


//================== includes ==========================
//c++ includes:
#include <iostream>
#include <conio.h>

//own includes:
#include "GameEngine.h"
#include "GameObject.h"
//=======================================================


using namespace std;


int main()
{
	//======================= Init ===============================
	GameEngine* engine = new GameEngine;							//create GameEngine

	
	printf( "Welcome to thrawns Keen!\n" );

	printf( "getting into graphics mode...\n" );
	engine->graphics->SetGraphicsMode( 0x13 );						//enter Grphics Mode

	engine->LoadAssets();

	engine->graphics->BlackOut();

	TMXMap testMap = engine->LoadTMXMap( "./levels/startup.tmx" );		//Load Map

	engine->CreateObjectsFromMap( &testMap );			//crrate Objects

	GameObject* banner;
	banner = new Banner		( engine );
	banner->SetTypeID		( 6 );
	banner->SetPos 			( Vector2D( 100, 210 ) );
	banner->SetDimensions 	( 16, 16 );
	banner->SetTileSetID 	( ASSET_K1_TILES );
	banner->SetTileIndex 	( 468 );
	banner->SetDrawOrder 	( 2 );

	engine->AddObject 		(banner);

	Fader* fader = new Fader 	( engine );
	engine->AddObject 			( fader );


	//========================== Main Loop ======================================
	float filterTimeStamp, updateTimeStamp, drawTimeStamp;
	float filterTime, updateTime, drawTime;

	bool running = true;
	while(running)
	{
		engine->time->FrameStart();		//init Frametiming


		//------------------------ input -----------------------------
		if(engine->input->KeyDown(ESC))
			running = false;
		//-----------------------------------------------------------

		//engine->graphics->ClearScreen(190);

		filterTimeStamp = engine->time->GetCurrentTimeInMS();
		engine->FilterObjectsByImportance();
		filterTime = engine->time->GetCurrentTimeInMS() - filterTimeStamp;

		updateTimeStamp = engine->time->GetCurrentTimeInMS();
		engine->UpdateAll();
		updateTime = engine->time->GetCurrentTimeInMS() - updateTimeStamp;

		drawTimeStamp = engine->time->GetCurrentTimeInMS();
		engine->DrawAll();	//draw all Objects (that are handled by the GameEngine)
		drawTime = engine->time->GetCurrentTimeInMS() - drawTimeStamp;


		//----------------- print Framerate -----------------------
		char str[50];
		sprintf(str, "%.2f(%.2f)%.2f\nfilter=%.2f, update=%.2f, draw=%.2f", engine->time->GetFPS(), engine->time->TicksToMilliSeconds(engine->time->GetLastTime()), engine->time->GetDelta(), filterTime, updateTime, drawTime );
		engine->graphics->DrawText( Vector2D( 0, 0 ) + engine->graphics->GetCamPos() , ASSET_TXT_WHITE, 0, str );
		//--------------------------------------------------------


		engine->graphics->Flip();	//copy Backbuffer to Screen

		//engine->time->WaitForTicked();

		engine->time->FrameEnd();	//get Frametime
	}
	//======================================================================


	//======================= cleanup and shutdown =========================
	delete engine;
	//======================================================================

	return 0;
}