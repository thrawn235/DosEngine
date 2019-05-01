#include <iostream>
#include <conio.h>

#include "GameEngine.h"
#include "GameObject.h"

using namespace std;

int main()
{
	//TimeEngine time;
	GameEngine* engine = new GameEngine;

	//engine->graphics->SetGraphicsMode(0x13);

	TMXMap testMap = engine->LoadTMXMap("./levels/tst2.tmx");

	printf("mapData = %i \n", testMap.layers[0].data[60]);
	printf("tileSetID = %i", engine->GetTileSetID(&testMap, testMap.layers[0].data[0]));
	//printf("tileID    = %i", engine->GetTileID(&testMap, int mapValue, int tileSetID));
	//printf("firstGID  = %i", engine->GetFirstGid(&testMap* in, int tileSetID));
	//printf("typeID    = %i", engine->GetTypeID(&testMap* in, int mapValue, int firstGID));
	
	/*BMP temp;
	temp = engine->graphics->LoadBMP("/gfx/idbanner.bmp");
	engine->graphics->SetPalette(temp.colorTable, temp.DIBHeader.numColors);
	
	Sprite temp2;
	temp2 = engine->graphics->BMPToSprite(&temp, 1);
	engine->graphics->FreeBMP(&temp);

	temp = engine->graphics->LoadBMP("/gfx/8pborder.bmp");
	Sprite border = engine->graphics->BMPToSprite(&temp, 100);
	engine->graphics->FreeBMP(&temp);

	TileSet tsBorder = engine->graphics->ExtractTileSet(100, &border, Vector2D(0,0), 8,8, 16, 2);
	engine->graphics->AddTileSet(tsBorder);

	temp = engine->graphics->LoadBMP("/gfx/txtWhite.bmp");
	Sprite whiteTestSprite = engine->graphics->BMPToSprite(&temp, 100);
	engine->graphics->FreeBMP(&temp);

	TileSet tsWhiteText = engine->graphics->ExtractTileSet(200, &whiteTestSprite, Vector2D(0,0), 8,8, 16, 6);
	engine->graphics->AddTileSet(tsWhiteText);

	Sprite temp3;
	temp3 = engine->graphics->CropSprite(&temp2, 2, Vector2D(5,5), 30,30);

	TileSet tileSetTest1;
	tileSetTest1 = engine->graphics->NewEmptyTileSet(0, 30, 30);

	engine->graphics->AddTileSet(tileSetTest1);*/

	//TileSet* ptiles;
	//ptiles = engine->graphics->GetTileSet(0);

	/*engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);
	engine->graphics->AddTile(0, temp3);


	TileSet extract = engine->graphics->ExtractTileSet(1, &temp2, Vector2D(0,0), 8,8, 10, 4);
	engine->graphics->AddTileSet(extract);

	Animation anim;
	anim.tileSetID = 1;
	anim.firstTileIndex = 0;
	anim.numSprites = 0;
	anim.currentFrame = 0;
	anim.speed = 20;
	anim.currentSpeedStep = 0;

	GameObject testObject(engine);
	testObject.SetTileSetID(100);
	testObject.SetTileIndex(20);
	engine->AddObject(&testObject);
	//engine->graphics->FreeBMP(&temp);*/
	


	/*bool running = true;
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
		engine->graphics->DrawPixel(0,0, 30);
		engine->graphics->DrawPixel(1,1, 30);
		engine->graphics->DrawPixel(2,2, 30);
		engine->graphics->DrawPixel(3,3, 30);
		engine->graphics->DrawHLine(Vector2D(10,10), 100, 35);
		engine->graphics->DrawVLine(Vector2D(15,15), 100, 35);
		engine->graphics->DrawRect(Vector2D(20,20), 100, 100, 45);
		engine->graphics->DrawFilledRect(Vector2D(21,21), 64, 64, 145);
		engine->graphics->DrawFilledRect(Vector2D(21,21), 99, 99, 155);
		engine->graphics->DrawLine(Vector2D(0,0), Vector2D(320,200), 67);
		engine->graphics->DrawLine(Vector2D(0,200), Vector2D(320,0), 67);
		engine->graphics->DrawCircle(Vector2D(160, 120), 50, 88);
		engine->graphics->DrawSprite(Vector2D(0,0), &temp2);
		engine->graphics->DrawTileSet(&extract, Vector2D(0,40), 10);
		engine->graphics->DrawSprite(Vector2D(1,1), &extract.tiles[15]);
		engine->graphics->PlayAnimation(&anim, Vector2D(30,30));
		engine->graphics->DrawWindow(Vector2D(50,50), 15, 20, 100, 1,3, 6,8, 2, 4, 0);
		engine->graphics->DrawText(Vector2D(60,60), 200, 0, " !\"#$%&'()*+,-./0123456789:;(=)", 16);
		engine->graphics->DrawText(Vector2D(60,68), 200, 0, " ?@ABCDEFGHIJKLMNO");
		engine->graphics->DrawText(Vector2D(60,76), 200, 0, "Alright, A proper test then!\n with a return even!!");
		char str[20];
		sprintf(str, "%d(%d)", engine->time->GetFPS(), engine->time->TicksToMilliSeconds(engine->time->GetLastTime()));
		engine->graphics->DrawText(Vector2D(0,0), 200, 0, str);
		engine->DrawAll();
		engine->graphics->Flip();
		engine->time->FrameEnd();
		//getch();
	}
	engine->graphics->BackToTextMode();
	engine->graphics->Destroy();*/


	return 0;
}