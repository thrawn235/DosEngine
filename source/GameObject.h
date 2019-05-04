///////////////////////////////////////////////////////////////////////////////////////
//
//	GameObject.h
//
//	by Sebastian Gurlin
//
//	Description:
//	GameObject is the Base class, of which all other Objects are derived from
//	players, enemies, walls, etc. are all children of this
//	it contains basic functionlity that can be called by all children
//	methods can also be overwritten, or ignored
//
//	Important functionality:
//	movement and collision
//	basic Drawing of itself
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef GAMEOBJECT
#define GAMEOBJECT


//================== includes ==========================
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
//=======================================================


//================== defines: ===========================
#define TYPE_GAME_OBJECT 	0
#define TYPE_PLAYER 		1
#define TYPE_SOLID 			2
#define TYPE_SOLID_TOP 		3
#define TYPE_BACK_GROUND 	4
//=======================================================


using namespace std;


//============== forward declarations ==================
class GameEngine;
//======================================================



class GameObject
{
protected:
	int 		typeID;
	Vector2D 	pos, direction;
	int 		width, height;
	int 		tileSetID, tileIndex;
	int 		drawOrder;

	GameEngine* engine;

	bool 		onFloor;

	vector<GameObject*> touchingObjects;
public:
	GameObject 	( GameEngine* newEngine );
	~GameObject ();

	//============ Set / Get =================
	int  				GetTypeID 			();
	Vector2D 			GetPos 				();
	Vector2D 			GetDirection 		();
	int 				GetTileSetID		();
	int 				GetTileIndex   		();
	int 				GetDrawOrder 		();
	int 				GetWidth 			();
	int 				GetHeight 			();
	bool 				IsOnFloor 			();
	vector<GameObject*> GetTouchingObjects 	();
	void 				SetTypeID 			( int newTypeID 				);
	void 				SetPos 				( Vector2D newPos 				);
	void 				SetDirection 		( Vector2D newDirection 		);
	void 				SetTileSetID 		( int newTileSetID 				);
	void 				SetTileIndex		( int newTileIndex 				);
	void 				SetDrawOrder 		( int newDrawOrder 				);
	void 				SetDimensions 		( int newWidth, int newHeight 	);
	//======================================


	//======= Main Loop Methods: ===========
	virtual void Update ();
	virtual void Draw 	();
	//=====================================


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

	/*
	enable
	diable
	invisible
	*/

	/*
	Get Damage
	Die
	*/
};


class Player : public GameObject
{
protected:
public:
	Player( GameEngine* newEngine );
	~Player();

	virtual void Update ();
	virtual void Draw 	();
};

class Solid  : public GameObject
{
protected:
public:
	Solid( GameEngine* newEngine );
	~Solid();

	virtual void Update ();
	virtual void Draw 	();
};

class SolidTop : public GameObject
{
protected:
public:
	SolidTop( GameEngine* newEngine );
	~SolidTop();

	virtual void Update ();
	virtual void Draw 	();
};

class BackGround  : public GameObject
{
protected:
public:
	BackGround( GameEngine* newEngine );
	~BackGround();

	virtual void Update ();
	virtual void Draw 	();
};

#endif