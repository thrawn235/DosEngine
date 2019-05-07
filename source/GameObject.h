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
#define TYPE_GAME_OBJECT 			0
#define TYPE_PLAYER 				1
#define TYPE_SOLID 					2
#define TYPE_SOLID_TOP 				3
#define TYPE_BACK_GROUND 			4
#define TYPE_BACK_GROUND_ANIMATION	5
#define TYPE_BANNER					6
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
	bool		invisible;
	bool 		enabled;

	vector<GameObject*> touchingObjects;
public:
	GameObject 	( GameEngine* newEngine );
	~GameObject ();

	//============ Set / Get =================
	virtual int  				GetTypeID 			();
	virtual Vector2D 			GetPos 				();
	virtual Vector2D 			GetDirection 		();
	virtual int 				GetTileSetID		();
	virtual int 				GetTileIndex   		();
	virtual int 				GetDrawOrder 		();
	virtual int 				GetWidth 			();
	virtual int 				GetHeight 			();
	virtual bool 				IsOnFloor 			();
	virtual vector<GameObject*> GetTouchingObjects 	();
	virtual void 				SetTypeID 			( int newTypeID 				);
	virtual void 				SetPos 				( Vector2D newPos 				);
	virtual void 				SetDirection 		( Vector2D newDirection 		);
	virtual void 				SetTileSetID 		( int newTileSetID 				);
	virtual void 				SetTileIndex		( int newTileIndex 				);
	virtual void 				SetDrawOrder 		( int newDrawOrder 				);
	virtual void 				SetDimensions 		( int newWidth, int newHeight 	);
	virtual bool				GetEnabled			();
	virtual bool				GetInvisible		();
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

	virtual void AddForce 			( Vector2D newForce );	//chang Direction Vector
	virtual bool RayBoxIntersect	( Vector2D origin, Vector2D dir, Vector2D* hit1, Vector2D boxPos, int boxWidth, int boxHeight );
	virtual bool Collision 			();
	virtual void Move 				();					//update position
	virtual void Friction 			( float slickness );

	void SetInsivible 		( bool newInvisible );
	void Enable 			();
	void Disable 			();

	/*
	Get Damage
	Die
	*/
};


class Player : public GameObject
{
protected:
	Vector2D movement;
	
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

class BackGroundAnimation  : public GameObject
{
protected:
	Animation anim;
public:
	BackGroundAnimation( GameEngine* newEngine );
	~BackGroundAnimation();

	virtual void SetTileIndex	( int newTileIndex 				);

	virtual void Update 		();
	virtual void Draw 			();
};

class Banner  : public GameObject
{
protected:
	bool showEverything;
	int showEverythingTimeStamp;
public:
	Banner( GameEngine* newEngine );
	~Banner();

	virtual void Update 		();
	virtual void Draw 			();
};

#endif