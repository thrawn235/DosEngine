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
#include "GameEngine.h"

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
	int drawOrder;

	GameEngine* engine;

	bool onFloor;
	vector<GameObject*> touchingObjects;
public:
	GameObject(GameEngine* newEngine);
	~GameObject();

	//Set / Get
	int GetTypeID();
	Vector2D GetPos();
	Vector2D GetDirection();
	int GetTileSetID();
	int GetTileIndex();
	int GetDrawOrder();
	bool IsOnFloor();
	vector<GameObject*> GetTouchingObjects();
	void SetTypeID(int newTypeID);
	void SetPos(Vector2D newPos);
	void SetDirection(Vector2D newDirection);
	void SetTileSetID(int newTileSetID);
	void SetTileIndex(int newTileIndex);
	void SetDrawOrder(int newDrawOrder);

	//Main Loop Methods:
	virtual void Update();
	virtual void Draw();


	/*AddForce //Gravity
	AddForce //Movement
	void AddForce(Vector2D vector); //Wind etc.
	bool OnFloor;	 //Is On Floor (variable)
	void Friction(int strength); //friction (if on floor for example)
	bool Collision(Vector2D* newPos);
	bool Collision(Vector2D* newPos, int typeID); //collision detection and calculation of ne Position sets onFloor variable
	void Move(int newPos); //just setting of the new pos
	vector<GameObject*> GetTouchingObjects();
	vector<GameObject*> GetTouchingObjects(int typeID);*/
};

#endif