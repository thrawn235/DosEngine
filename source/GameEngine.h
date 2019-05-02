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
//=======================================================


using namespace std;


//============== forward declarations ==================
class GameObject;
//======================================================


class GameEngine
{
protected:

public:
	GraphicsEngine* 	graphics;
	TimeEngine* 		time;
	InputEngine*		input;

private:
	vector<GameObject*> objects;

public:
	GameEngine();
	~GameEngine();

	//================ Set/Get =====================
	GraphicsEngine* GetGraphics();
	//=============================================


	//=========== Object Management ================
	void AddObject 		( GameObject* newObject 		);
	void RemoveObject 	( GameObject* in 				);
	void RemoveObject 	( int index 					);
	void RemoveObjects 	( vector<GameObject*> objects 	);
	void ClearObjects 	();
	//==============================================


	//============= MainLoopMethods ================
	void UpdateAll 	();
	void DrawAll 	();
	//==============================================


	//================= Level ======================
	TMXMap 	LoadTMXMap			( const char* filePath						);
	void 	CreateObjectsFromMap( TMXMap* in 								);
	void 	CreateObjectsFromMap( TMXMap* in, Vector2D offset 				);
	int 	GetTypeID			( TMXMap* in, int mapValue, int tileSetID 	);
	int 	GetTileSetID		( TMXMap* in, int mapValue 					);
	int 	GetTileID 			( TMXMap* in, int mapValue, int tileSetID 	);
	int 	GetFirstGid 		( TMXMap* in, int tileSetID 				);
	//==============================================


	//================= Random =====================
	//void InitRandom();
	//unsigned char NotRandom();
	//==============================================


	//================= Load/Save ==================
	//==============================================


	//================ FindingObjects ==============
	vector<GameObject*> GetAllObjects 			();
	vector<GameObject*> GetAllObjects 			( int typeID 										);
	vector<GameObject*> GetObjectsAtPos 		( Vector2D pos 										);
	vector<GameObject*> GetObjectsAtPos 		( Vector2D pos, int typeID 							);
	GameObject* 		GetFirstObjectAtPos 	( Vector2D pos 										);
	GameObject* 		GetFirstObjectAtPos 	( Vector2D pos, int typeID 							);
	vector<GameObject*> GetObjectsInRadius 		( Vector2D pos, int radius 							);
	vector<GameObject*> GetObjectsInRadius 		( Vector2D pos, int radius, int typeID 				);
	vector<GameObject*> GetObjectsAlongRay 		( Vector2D origin, Vector2D direction 				);
	vector<GameObject*> GetObjectsAlongRay 		( Vector2D origin, Vector2D direction, int typeID 	);
	GameObject* 		GetFirstObjectAlongRay 	( Vector2D origin, Vector2D direction 				);
	GameObject* 		GetFirstObjectAlongRay 	( Vector2D origin, Vector2D direction, int typeID 	);
	//==============================================


	//============= math trasitions ================
	float Linear 	();
	float Clip 		( float in, float min, float max );
	//==============================================
};

#endif