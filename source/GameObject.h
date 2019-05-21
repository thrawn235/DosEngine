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
#include "HelperFunctions.h"	//has to be last!
//=======================================================


//================== defines: ===========================
#define TYPE_GAME_OBJECT 			0
#define TYPE_PLAYER 				1
#define TYPE_SOLID 					2
#define TYPE_SOLID_TOP 				3
#define TYPE_BACK_GROUND 			4
#define TYPE_BACK_GROUND_ANIMATION	5
#define TYPE_BANNER					6
#define TYPE_MAIN_MENU				7
#define TYPE_PLAYER_TOP_DOWN		8
#define TYPE_CITY_OVERWORLD			9
#define TYPE_TRAP					10
#define TYPE_GAME_MANAGER			11
#define TYPE_STATIC_SIGN			12
#define TYPE_HELP_WINDOW			13
#define TYPE_EXIT					14
#define TYPE_FADER					15
#define TYPE_TREASURE				16
#define TYPE_ACTOR					17
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
	Vector2D	centerPos, centerPosOffset;
	int 		width, height;
	int 		tileSetID, tileIndex;
	int 		drawOrder;

	GameEngine* engine;

	bool 		important;
	
	bool		invisible;
	bool 		enabled;

	unsigned long UID;
	
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
	
	virtual void 				SetTypeID 			( int newTypeID 				);
	virtual void 				SetPos 				( Vector2D newPos 				);
	virtual void 				SetDirection 		( Vector2D newDirection 		);
	virtual void 				SetTileSetID 		( int newTileSetID 				);
	virtual void 				SetTileIndex		( int newTileIndex 				);
	virtual void 				SetDrawOrder 		( int newDrawOrder 				);
	virtual void 				SetDimensions 		( int newWidth, int newHeight 	);
	virtual bool				GetEnabled			();
	virtual bool				GetInvisible		();
	virtual unsigned long		GetUID				();
	virtual void				SetUID				( unsigned long newUID );
	virtual bool				GetImportant 		();

			void 				SetInsivible 		( bool newInvisible );
			void 				Enable 				();
			void 				Disable 			();
	//======================================


	//======= Main Loop Methods: ===========
	virtual void Update ();
	virtual void Draw 	();
	//=====================================
};





class Actor : public GameObject
{
protected:
	bool dying;
	bool dead;

	bool 				onFloor;
	vector<GameObject*> touchingObjects;
public:
	Actor( GameEngine* newEngine );
	~Actor();

	virtual vector<GameObject*> GetTouchingObjects 	();
	virtual bool 				IsOnFloor 			();
	virtual bool 				IsDying 			();
	virtual bool 				IsDead				();
	virtual void				Die 				();

	virtual void 				AddForce 			( Vector2D newForce );	//chang Direction Vector
	virtual bool 				RayBoxIntersect		( Vector2D origin, Vector2D dir, float *tmin, Vector2D boxPos, int boxWidth, int boxHeight );
	virtual bool 				BoxBoxCollision	 	( Vector2D pos1, int width1, int height1, Vector2D pos2, int width2, int height2 );
	virtual float 				FindClosestCollision( vector<GameObject*> objects, GameObject* closestObject );
	virtual vector<GameObject*> CollisionDetection 	();
	virtual Vector2D 			VectorProjection 	( Vector2D posIn, Vector2D dirIn,  int tmin, GameObject* closestObject );
	virtual void 				Collision 			();
	virtual void 				Move 				();					//update position
	virtual void 				Friction 			( float slickness );
	virtual bool 				FindCollisionPoint 	( GameObject* testObject, Vector2D testPoint, Vector2D* out );
	virtual void 				SimpleCollisionResolution 	( vector<GameObject*> colliders );
	virtual float 				RayDown 			( Vector2D origin, float length, vector<GameObject*> testObjects );
	virtual float 				RayRight 			( Vector2D origin, float length, vector<GameObject*> testObjects );
	virtual float 				RayUp 				( Vector2D origin, float length, vector<GameObject*> testObjects );
	virtual float 				RayLeft 			( Vector2D origin, float length, vector<GameObject*> testObjects );
};





class Player : public Actor
{
protected:
	Vector2D movement;

	Animation walkForward;
	Animation walkBackward;
	Animation deathAnimation;

	Animation jumpLeft, jumpRight;
	bool jumpCharging;
	int jumpCharge;

	bool spacePressed;

	unsigned long levelUID;


	int score;
	int extraLife;
	int lifes;
	int ammo;

	bool pogo;
	bool shipBattery;

	bool blueKey;
	bool redKey;
	bool yellowKey;
	
public:
	Player( GameEngine* newEngine );
	~Player();

	virtual void 			SetScore 		( int newScore );
	virtual void 			SetExtraLife 	( int newExtraLife );
	virtual void 			SetLifes 		( int newLifes );
	virtual void 			SetAmmo 		( int newAmmo );
	virtual void 			SetPogo 		( bool newPogo );
	virtual void 			SetShipBattery 	( bool newShipBattery );
	virtual void 			SetBlueKey 		( bool newBlueKey );
	virtual void 			SetRedKey 		( bool newRedKey );
	virtual void 			SetYelloweKey 	( bool newYellowKey );
	virtual void 			SetLevelUID 	( unsigned long newLevelUID );

	virtual int 			GetScore 		();
	virtual int 			GetExtraLife 	();
	virtual int 			GetLifes 		();
	virtual int 			GetAmmo 		();
	virtual bool 			GetPogo			();
	virtual bool 			GetShipBattery 	();
	virtual bool 			GetBlueKey 		();
	virtual bool 			GetRedKey 		();
	virtual bool 			GetYelloweKey 	();
	virtual unsigned long 	GetLevelUID 	();

	virtual void 			Update 			();
	virtual void 			Draw 			();
	virtual void 			Collision 		();
};





class PlayerTopDown : public Player
{
protected:
	Vector2D movement;

	Animation right;
	Animation up;
	Animation down;
	
public:
	PlayerTopDown( GameEngine* newEngine );
	~PlayerTopDown();

	virtual void Update ();
	virtual void Draw 	();
};





class CityOverWorld : public GameObject
{
protected:
	char levelPath[30];
public:
	CityOverWorld( GameEngine* newEngine );
	~CityOverWorld();

	virtual void SetLevelPath( const char* newPath );

	virtual void Update 	();
	virtual void Draw 		();
	virtual void LoadLevel 	();
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

	virtual void SetTileIndex	( int newTileIndex 	);

	virtual void Update 		();
	virtual void Draw 			();
};





class Banner  : public Actor
{
protected:
	bool showEverything;
	int showEverythingTimeStamp;
	bool black;
public:
	Banner( GameEngine* newEngine );
	~Banner();

	virtual void Update 		();
	virtual void Draw 			();
};





class MainMenu  : public GameObject
{
protected:
	Animation blueBallAnim;
	Vector2D blueBallOffsetPos;
	bool keyDown;
	int menuPos;
	int fadeTimeStamp;
	bool showSelf;
public:
	MainMenu( GameEngine* newEngine );
	~MainMenu();

	virtual void Update 		();
	virtual void Draw 			();
};






class Trap : public GameObject
{
protected:
	Animation anim;
public:
	Trap( GameEngine* newEngine );
	~Trap();

	virtual void Update 		();
	virtual void Draw 			();
	virtual void SetTileIndex	( int newTileIndex );
};





class GameManager : public GameObject
{
protected:
	bool showStats, showHelp;

	bool keyDown;

	int score;
	int extraLife;
	int lifes;
	int ammo;

	bool pogo;
	bool shipBattery;

	bool blueKey;
	bool redKey;
	bool yellowKey;

	Player* connectedPlayer;
public:
	GameManager( GameEngine* newEngine );
	~GameManager();

	virtual void Update 		();
	virtual void Draw 			();
};





class StaticSign : public GameObject
{
protected:
	int spriteID;
public:
	StaticSign( GameEngine* newEngine );
	~StaticSign();

	virtual void SetSpriteID	( int newSpriteID );
	virtual void Update 		();
	virtual void Draw 			();
};





class HelpWindow : public GameObject
{
protected:
	bool show;
	bool keyDown;
public:
	HelpWindow( GameEngine* newEngine );
	~HelpWindow();

	virtual void Update 		();
	virtual void Draw 			();
};





class Exit : public GameObject
{
protected:
public:
	Exit( GameEngine* newEngine );
	~Exit();

	virtual void Update 		();
	virtual void Draw 			();
	virtual void BackToOverworld();
};





class Fader : public GameObject
{
protected:
	bool fadeOut;
public:
	Fader( GameEngine* newEngine );
	Fader( GameEngine* newEngine, bool newFadeOut );
	~Fader();

	virtual void Update 		();
	virtual void Draw 			();
};





class Treasure : public GameObject
{
protected:
	int score;
public:
	Treasure( GameEngine* newEngine );
	~Treasure();

	virtual void SetScore( int newScore);
	virtual int GetScore();
	virtual void SetTileIndex( int newTileIndex );

	virtual void Update 		();

};

#endif