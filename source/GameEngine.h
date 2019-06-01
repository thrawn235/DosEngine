///////////////////////////////////////////////////////////////////////////////////////
//
//	GameEngine.h
//
//	by Sebastian Gurlin
//
//	Description:
//	GameEngine is the collection of all Sub-Engines (Graphics, Input, Sound etc)
//	It also contains functions to query all Game Objects (find them by Pos for example)
//	All GameObjects are managed here
//	GameObjects have a Pointer to this, to ask for other GameObjects 
//
///////////////////////////////////////////////////////////////////////////////////////

#ifndef GAMEENGINE
#define GAMEENGINE

//================== includes ==========================
//c++ includes:
#include <vector>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//djgpp includes:
#include <sys/nearptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:
#include "Vector2D.h"
#include "GraphicsEngine.h"
#include "TimeEngine.h"
#include "InputEngine.h"
#include "GameObject.h"
#include "TMXStructs.h"	//just the Datastructures to hold a tmx data file
#include "AssetIDs.h"
#include "SoundEngine.h"
//=======================================================


using namespace std;


//============== forward declarations ==================
class GameObject;
//======================================================

struct RawTileMap
{
	int ID;
	int tileSetID;
	int width, height;
	int tileWidth, tileHeight;
	vector<int> tiles;
};


class GameEngine
{
protected:

public:
	GraphicsEngine*		graphics;
	TimeEngine* 		time;
	InputEngine*		input;
	SoundEngine*		sound;

	short randomIndex;
	unsigned char randomTable[256] = {137, 145, 237, 217, 135, 130, 84, 46, 109, 181, 226, 127, 89, 26, 195, 204, 112, 72, 159, 202, 98, 211, 147, 100, 166, 69, 18, 144, 133, 249, 113, 80, 176, 219, 2, 170, 107, 229, 185, 82, 149, 157, 244, 7, 218, 61, 22, 255, 47, 233, 0, 140, 205, 29, 6, 178, 220, 183, 9, 23, 65, 32, 150, 85, 240, 246, 228, 59, 208, 134, 227, 57, 196, 39, 182, 242, 101, 55, 77, 33, 119, 99, 194, 138, 234, 108, 151, 64, 216, 169, 19, 207, 162, 241, 206, 58, 25, 104, 230, 214, 200, 116, 201, 74, 70, 197, 231, 213, 48, 153, 115, 252, 120, 209, 28, 42, 103, 186, 141, 168, 63, 189, 251, 192, 31, 180, 66, 111, 215, 160, 95, 152, 8, 161, 87, 1, 156, 90, 198, 37, 132, 247, 171, 193, 96, 67, 190, 123, 131, 164, 129, 191, 50, 16, 53, 121, 81, 20, 177, 126, 17, 35, 40, 60, 71, 122, 88, 43, 172, 97, 136, 225, 187, 44, 11, 83, 94, 36, 52, 49, 92, 143, 154, 139, 236, 76, 13, 173, 105, 30, 27, 155, 75, 232, 106, 45, 38, 14, 62, 110, 184, 102, 93, 78, 118, 250, 15, 5, 56, 221, 224, 238, 142, 114, 124, 73, 235, 254, 188, 41, 91, 4, 12, 128, 253, 21, 179, 3, 243, 199, 51, 24, 68, 148, 210, 146, 245, 248, 54, 222, 79, 163, 174, 212, 239, 167, 175, 223, 117, 86, 203, 125, 10, 158, 165};

	unsigned long 		lastUID;

	int levelBoundaryXMin;
	int levelBoundaryYMin;
	int levelBoundaryXMax;
	int levelBoundaryYMax;

private:
	vector<GameObject*> objects;

	vector<GameObject*> drawObjects;

	vector<GameObject*> unimportantObjects;

	vector<GameObject*> saveBank;	//to save the overworld

public:
	GameEngine();
	~GameEngine();

	//================ Set/Get =====================
	GraphicsEngine* GetGraphics					();
	TimeEngine*  	GetTime 					();
	InputEngine* 	GetInput 					();

	int 			GetLevelBoundaryXMin 		();
	int 			GetLevelBoundaryYMin 		();
	int 			GetLevelBoundaryXMax 		();
	int 			GetLevelBoundaryYMax 		();
	//=============================================


	//=========== Object Management ================
	void 			AddObject 					( GameObject* newObject 		);
	void 			RemoveObject 				( GameObject* in 				);
	void 			RemoveObject 				( unsigned int index 			);
	void 			RemoveObjects 				( vector<GameObject*> objects 	);
	void 			ClearObjects 				();
	void 			PurgeObjects 				();
	void 			SaveObjectsToBank 			();
	void 			LoadObjectsFromBank 		();
	void 			DisableAll					();
	void 			DisableAll					( GameObject* exclude );
	void 			DisableAll					( vector<GameObject*> excludes);
	void 			EnableAll					();
	void 			EnableAll					( GameObject* exclude );
	void 			EnableAll					( vector<GameObject*> excludes);

	unsigned long 	GetUID						();

	void 			FilterObjectsByImportance 	();
	void 			RestoreAllUnimportantObjects();
	//==============================================


	//============= MainLoopMethods ================
	void UpdateAll 	();
	void DrawAll 	();
	//==============================================

	//================= Assets =====================
	void LoadAssets ();
	//==============================================

	//================= Level ======================
	TMXMap 			LoadTMXMap					( const char* filePath								);
	TMXProperty		GetProperty					( vector<TMXProperty> properties, const char* name	);
	void 			CreateObjectsFromMap 		( TMXMap* in 										);
	void 			CreateObjectsFromMap 		( TMXMap* in, Vector2D offset 						);
	int 			GetTypeID					( TMXMap* in, int mapValue, int TMXTileSetIndex 	);
	int 			GetTMXTileSetIndex			( TMXMap* in, int mapValue 							);
	int 			GetTileID 					( TMXMap* in, int mapValue, int TMXTileSetIndex		);
	int 			GetTileSetID 				( TMXMap* in, int TMXTileSetIndex 					);
	int 			GetFirstGid 				( TMXMap* in, int tileSetID 						);
	void 			FindLevelBoundaries			();
	//==============================================

	//=============== RawTileMap ===================
	RawTileMap		LoadRawTileMap 				( const char* filePath );
	RawTileMap 		ConvertTMXMapToRaw 			( TMXMap* in );
	void 			DrawRawTileMap 				( Vector2D pos, RawTileMap* in );
	RawTileMap 		CropRawTileMap 				( RawTileMap* in, int startX, int startY, int width, int height );
	//==============================================

	//================ Misc =======================
	void 			Quit 						(const char* message);
	//=============================================


	//================= Random =====================
	unsigned char 	GetRandom 					();
	//==============================================


	//================= Load/Save ==================
	//==============================================


	//================ FindingObjects ==============
	vector<GameObject*> GetAllObjects 			();
	vector<GameObject*> GetAllObjects 			( int typeID 										);
	GameObject*			GetFirstObject 			( int typeID 										);
	vector<GameObject*> GetTouchingObjects		( Vector2D testDirection							);
	vector<GameObject*> GetTouchingObjects		( Vector2D testDirection, int typeID				);
	vector<GameObject*> GetObjectsAtPos 		( Vector2D pos 										);
	vector<GameObject*> GetObjectsAtPos 		( Vector2D pos, int typeID 							);
	vector<GameObject*> GetObjectsInArea 		( Vector2D pos, int width, int height				);
	vector<GameObject*> GetObjectsInArea 		( Vector2D pos, int width, int height, int typeID 	);
	GameObject* 		GetFirstObjectInArea 	( Vector2D pos, int width, int height				);
	GameObject* 		GetFirstObjectInArea 	( Vector2D pos, int width, int height, int typeID 	);
	GameObject* 		GetFirstObjectAtPos 	( Vector2D pos 										);
	GameObject* 		GetFirstObjectAtPos 	( Vector2D pos, int typeID 							);
	vector<GameObject*> GetObjectsInRadius 		( Vector2D pos, int radius 							);
	vector<GameObject*> GetObjectsInRadius 		( Vector2D pos, int radius, int typeID 				);
	vector<GameObject*> GetObjectsAlongRay 		( Vector2D origin, Vector2D direction 				);
	vector<GameObject*> GetObjectsAlongRay 		( Vector2D origin, Vector2D direction, int typeID 	);
	GameObject* 		GetFirstObjectAlongRay 	( Vector2D origin, Vector2D direction 				);
	GameObject* 		GetFirstObjectAlongRay 	( Vector2D origin, Vector2D direction, int typeID 	);
	GameObject* 		GetObjectByUID		 	( unsigned long UID 								);
	//==============================================


	//============= math trasitions ================
	float Linear 								();
	float Clip 									( float in, float min, float max );
	//==============================================
};

#endif