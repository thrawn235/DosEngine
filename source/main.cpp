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


/* 
Bugs:
1. DrawAll and UpdateAll loops are too slow
	vector push_back() and erase() suck and are slow!
2. Program Segfaults at exit. specifically freeing tiles in FreeTilesets()
3. Player Movement wtf is going on ?
4. VESA Modes dont work at all

ToDo:
1. VESA
2. Asset Management
3. Test all functions
4. Meaningful error cheks for stuff that can feasibly go wrong
*/


int main()
{
	//======================= Init ===============================
	GameEngine* engine = new GameEngine;							//create GameEngine

	TMXMap testMap = engine->LoadTMXMap("./levels/k1e1m1.tmx");		//Load Map
	


	engine->graphics->SetGraphicsMode(0x13);						//enter Grphics Mode

		

	BMP bmp;
	Sprite*  sprite;	
	//-------------------- Load Universal Game Assets -----------------------
	bmp = engine->graphics->LoadBMP("./gfx/txtwhite.bmp");
	engine->graphics->SetPalette(bmp.colorTable, 17);
	sprite = engine->graphics->BMPToSprite(&bmp, 3);
	engine->graphics->FreeBMP(&bmp);

	TileSet* txtwhite = engine->graphics->ExtractTileSet(1, sprite, Vector2D(0,0), 8, 8, 16, 6);	//tileSetId = 1
	engine->graphics->AddTileSet(txtwhite);
	//Sprite* cropTest = engine->graphics->CropSprite( sprite, 20, Vector2D( 0, 0 ), 10,10 );
	//engine->graphics->FreeSprite(cropTest);
	engine->graphics->FreeSprite(sprite);


	//------------------------- Load GameSprites ----------------------------
	bmp = engine->graphics->LoadBMP("./gfx/walk.bmp");
	sprite = engine->graphics->BMPToSprite(&bmp, 3);
	engine->graphics->FreeBMP(&bmp);

	TileSet* keenWalk = engine->graphics->ExtractTileSet(10, sprite, Vector2D(0,0), 16, 24, 17, 2);	//tileSetId = 10
	engine->graphics->AddTileSet(keenWalk);
	engine->graphics->FreeSprite(sprite);


	//------------------------- Load Level Tiles ------------------------
	bmp = engine->graphics->LoadBMP("./gfx/k1tiles.bmp");
	sprite = engine->graphics->BMPToSprite(&bmp, 0);
	engine->graphics->FreeBMP(&bmp);

	TileSet* k1Tiles = engine->graphics->ExtractTileSet(100, sprite, Vector2D(0,0), 16, 16, 13, 53); //tileSetId = 100 (hast to mach property in TMXFile)
	engine->graphics->AddTileSet(k1Tiles);
	engine->graphics->FreeSprite(sprite);


	bmp = engine->graphics->LoadBMP("./gfx/k3tiles.bmp");
	sprite = engine->graphics->BMPToSprite(&bmp, 1);
	engine->graphics->FreeBMP(&bmp);

	TileSet* k2Tiles = engine->graphics->ExtractTileSet(101, sprite, Vector2D(0,0), 16, 16, 13, 53); //tileSetId = 101 (hast to mach property in TMXFile)
	engine->graphics->AddTileSet(k2Tiles);
	engine->graphics->FreeSprite(sprite);
	//---------------------------------------------------------------------

	engine->CreateObjectsFromMap( &testMap );			//crrate Objects

	/*printf( "%i\n", engine->GetTileSetID(&testMap, 274) );
	printf( "%i\n", engine->GetTileSetID(&testMap, 276) );
	printf( "%s %i\n", testMap.tileSets[0].source, engine->GetTileSetIDBySource( testMap.tileSets[0].source ) );
	printf( "%s %i\n", testMap.tileSets[1].source, engine->GetTileSetIDBySource( testMap.tileSets[1].source ) );*/
	

	//------------- test Object ------------------
	/*GameObject* testObject;
	testObject = new Player		( engine );
	testObject->SetTypeID		( 0 );
	testObject->SetPos 			( Vector2D( 30,30) );
	testObject->SetDimensions 	( 16,24 );
	testObject->SetTileSetID 	( 10 );
	testObject->SetTileIndex 	( 0 );
	testObject->SetDrawOrder 	( 2 );
	engine->AddObject(testObject);*/
	//--------------------------------------------
	//============================================================


	//========================== Main Loop ======================================
	bool running = true;
	while(running)
	{
		engine->time->FrameStart();		//init Frametiming

		engine->input->CheckKeys();		//check keyboard

		//------------------------ input -----------------------------
		if(engine->input->KeyDown(ESC))
			running = false;
		/*if(engine->input->KeyDown(KEY_W))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x, engine->graphics->GetCamPos().y -2));
		if(engine->input->KeyDown(KEY_A))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x -2, engine->graphics->GetCamPos().y));
		if(engine->input->KeyDown(KEY_S))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x, engine->graphics->GetCamPos().y +2));
		if(engine->input->KeyDown(KEY_D))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x +2, engine->graphics->GetCamPos().y));*/
		if(engine->input->KeyDown(SPACE))
			engine->graphics->ChangePaletteBrightness(-1);
		if(engine->input->KeyDown(LCTRL))
			engine->graphics->ChangePaletteBrightness(+1);
		//-----------------------------------------------------------

		engine->graphics->ClearScreen(190);

		engine->UpdateAll();
		engine->DrawAll();	//draw all Objects (that are handled by the GameEngine)

		//----------------- print Framerate -----------------------
		char str[20];
		sprintf(str, "%d(%d)%f", engine->time->GetFPS(), engine->time->TicksToMilliSeconds(engine->time->GetLastTime()), engine->time->GetDelta() );
		engine->graphics->DrawText( Vector2D( 0, 0 ) + engine->graphics->GetCamPos() , 1, 0, str );
		//--------------------------------------------------------

		engine->graphics->Flip();	//copy Backbuffer to Screen

		engine->time->FrameEnd();	//get Frametime
	}
	//======================================================================


	//======================= cleanup and shutdown =========================
	engine->graphics->BackToTextMode();
	engine->graphics->Destroy();
	//======================================================================

	return 0;
}