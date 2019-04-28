#ifndef GAMEENGINE
#define GAMEENGINE

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

using namespace std;


class GameObject;


class GameEngine
{
protected:

public:
	GraphicsEngine* graphics;
	TimeEngine* time;
	InputEngine *input;

	vector<GameObject*> objects;
public:
	GameEngine();
	~GameEngine();

	//Set/Get
	/*GraphicsEngine* GetGraphics();

	//Object Management
	void AddObject();
	void RemoveObject();
	void ClearObjects();

	//MainLoopMethods
	void UpdateAll();
	void DrawAll();

	//Level
	void LoadTMXMap();
	void CreateObjectsFromMap();

	//Random
	//void InitRandom();
	//unsigned char NotRandom();

	//Load/Save

	//FindingObjects
	void GetAllObjects();
	void GetAllObjects(int typeID);
	void GetObjectsAtPos(Vector2D pos);
	void GetObjectsAtPos(Vector2D pos, int typeID);
	void GetFirstObjectAtPos(Vector2D pos);
	void GetFirstObjectAtPos(Vector2D pos, int typeID);
	void GetObjectsInRadius(Vector2D pos, int radius);
	void GetObjectsInRadius(Vector2D pos, int radius, int typeID);
	void GetObjectsAlongRay(Vector2D origin, Vector2D direction);
	void GetObjectsAlongRay(Vector2D origin, Vector2D direction, int typeID);
	void GetFirstObjectAlongRay(Vector2D origin, Vector2D direction);
	void GetFirstObjectAlongRay(Vector2D origin, Vector2D direction, int typeID);*/

	//math trasitions
	//float Linear();
};

#endif