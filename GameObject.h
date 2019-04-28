#ifndef GAMEOBJECT
#define GAMEOBJECT

//c++ includes:
#include <vector>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//djgpp includes:


//own includes:
#include "Vector2D.h"
#include "GraphicsEngine.h"
#include "TimeEngine.h"
#include "InputEngine.h"

//defines:
#define TYPEGAMEOBJECT 0

class GameEngine;

using namespace std;

//SuperObject and Base Class
//It can be showm, and it can move. So it has a position, direction sprite etc.
//It can handle collision and movement, and it has dimensions (for the engine tu query)
class GameObject
{
protected:
	int typeID;
	Vector2D pos, direction;
	int width, height;
	int tileSetID, tileIndex;

	GameEngine* engine;

public:
	GameObject(GameEngine* newEngine);
	~GameObject();

	virtual void Update();
	virtual void Draw();
};

#endif