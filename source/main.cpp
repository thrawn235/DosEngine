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

	TMXMap testMap = engine->LoadTMXMap("./levels/tst2.tmx");	//Load Map
	engine->CreateObjectsFromMap(&testMap, Vector2D(0,0));			//crrate Objects


	engine->graphics->SetGraphicsMode(0x13);						//enter Grphics Mode

		
	//-------------------- Loading Sprites and Tiles ------------------------
	BMP bmp = engine->graphics->LoadBMP("./gfx/k1tiles.bmp");
	//engine->graphics->SetPalette(bmp.colorTable, 16);
	Sprite sprite = engine->graphics->BMPToSprite(&bmp, 0);
	engine->graphics->FreeBMP(&bmp);

	TileSet k1Tiles = engine->graphics->ExtractTileSet(0, &sprite, Vector2D(0,0), 16, 16, 13, 53);
	engine->graphics->AddTileSet(k1Tiles);


	bmp = engine->graphics->LoadBMP("./gfx/k2tiles.bmp");
	sprite = engine->graphics->BMPToSprite(&bmp, 1);
	engine->graphics->FreeBMP(&bmp);

	TileSet k2Tiles = engine->graphics->ExtractTileSet(1, &sprite, Vector2D(0,0), 16, 16, 13, 53);
	engine->graphics->AddTileSet(k2Tiles);


	bmp = engine->graphics->LoadBMP("./gfx/txtwhite.bmp");
	engine->graphics->SetPalette(bmp.colorTable, 17);
	sprite = engine->graphics->BMPToSprite(&bmp, 3);
	engine->graphics->FreeBMP(&bmp);

	TileSet txtwhite = engine->graphics->ExtractTileSet(200, &sprite, Vector2D(0,0), 8, 8, 16, 6);
	engine->graphics->AddTileSet(txtwhite);
	//---------------------------------------------------------------------
	

	//------------- test Object ------------------
	/*GameObject* testObject;
	testObject = new GameObject(engine);
	testObject->SetTypeID		(0);
	//newObject->SetPos 		(ne);
	testObject->SetDimensions 	(16,16);
	testObject->SetTileSetID 	(1);
	testObject->SetTileIndex 	(1);
	testObject->SetDrawOrder 	(1);
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
		if(engine->input->KeyDown(KEY_W))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x, engine->graphics->GetCamPos().y -1));
		if(engine->input->KeyDown(KEY_A))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x -1, engine->graphics->GetCamPos().y));
		if(engine->input->KeyDown(KEY_S))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x, engine->graphics->GetCamPos().y +1));
		if(engine->input->KeyDown(KEY_D))
			engine->graphics->SetCamPos(Vector2D(engine->graphics->GetCamPos().x +1, engine->graphics->GetCamPos().y));
		if(engine->input->KeyDown(SPACE))
			engine->graphics->ChangePaletteBrightness(-1);
		if(engine->input->KeyDown(LCTRL))
			engine->graphics->ChangePaletteBrightness(+1);
		//-----------------------------------------------------------

		engine->graphics->ClearScreen(190);


		engine->DrawAll();	//draw all Objects (that are handled by the GameEngine)


		char str[20];
		sprintf(str, "%d(%d)", engine->time->GetFPS(), engine->time->TicksToMilliSeconds(engine->time->GetLastTime()));
		engine->graphics->DrawText(Vector2D(0,0), 200, 0, str);
		
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