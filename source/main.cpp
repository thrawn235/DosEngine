#include <iostream>
#include <conio.h>

#include "GameEngine.h"
#include "GameObject.h"

using namespace std;

int main()
{
	//TimeEngine time;
	GameEngine* engine = new GameEngine;

	TMXMap testMap = engine->LoadTMXMap("./levels/tst2.tmx");

	engine->CreateObjectsFromMap(&testMap, Vector2D(0,0));


	engine->graphics->SetGraphicsMode(0x13);

	

	/*printf("mapData = %i \n", testMap.layers[0].data[99]);
	printf("tileSetID = %i \n", engine->GetTileSetID(&testMap, testMap.layers[0].data[99]));
	printf("firstGID  = %i \n", engine->GetFirstGid(&testMap, 0));
	printf("tileID    = %i \n", engine->GetTileID(&testMap, 384, 0));
	printf("typeID    = %i", engine->GetTypeID(&testMap, 612, 1));*/
	
	
	BMP bmp = engine->graphics->LoadBMP("./gfx/k1tiles.bmp");
	engine->graphics->SetPalette(bmp.colorTable, 16);
	Sprite sprite = engine->graphics->BMPToSprite(&bmp, 0);
	engine->graphics->FreeBMP(&bmp);

	TileSet k1Tiles = engine->graphics->ExtractTileSet(0, &sprite, Vector2D(0,0), 16, 16, 13, 53);
	engine->graphics->AddTileSet(k1Tiles);


	bmp = engine->graphics->LoadBMP("./gfx/k2tiles.bmp");
	sprite = engine->graphics->BMPToSprite(&bmp, 1);
	engine->graphics->FreeBMP(&bmp);

	TileSet k2Tiles = engine->graphics->ExtractTileSet(1, &sprite, Vector2D(0,0), 16, 16, 13, 53);
	engine->graphics->AddTileSet(k2Tiles);


	

	GameObject* testObject;
	testObject = new GameObject(engine);
	testObject->SetTypeID(0);
	//newObject->SetPos(ne);
	testObject->SetDimensions(16,16);
	testObject->SetTileSetID(1);
	testObject->SetTileIndex(1);
	testObject->SetDrawOrder(1);
	engine->AddObject(testObject);


	bool running = true;
	while(running)
	{
		engine->time->FrameStart();
		engine->input->CheckKeys();
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

		engine->graphics->ClearScreen(190);

		engine->DrawAll();



		/*char str[20];
		sprintf(str, "%d(%d)", engine->time->GetFPS(), engine->time->TicksToMilliSeconds(engine->time->GetLastTime()));
		engine->graphics->DrawText(Vector2D(0,0), 200, 0, str);*/
		
		engine->graphics->Flip();
		engine->time->FrameEnd();
	}
	engine->graphics->BackToTextMode();
	engine->graphics->Destroy();


	return 0;
}