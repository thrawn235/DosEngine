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
#include "SoundEngine.h"
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
#define TYPE_SHIP_WREK				18
#define TYPE_CITY_OVERWORLD1X1		19
#define TYPE_BATTERY				20
#define TYPE_ABOUT_SCREEN			21
#define TYPE_STORY_SCREEN			22
#define TYPE_DOOR					23
#define TYPE_KEY					24
#define TYPE_ACTIVATABLE 			25
#define TYPE_SYNTH_STATION 			26
//=======================================================


using namespace std;


//============== forward declarations ==================
class GameEngine;
class RawTileMap;
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
	bool 		persistent;

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
	virtual bool 				GetPersistent 		();

			void 				SetInsivible 		( bool newInvisible );
			void 				SetPersistent 		( bool newPersistent );
			void 				Enable 				();
			void 				Disable 			();
	virtual void 				OnCreation			();
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

	bool rememberLeftRight;
	bool rememberUpDown;

	Animation walkForward;
	Animation walkBackward;
	Animation deathAnimation;

	Animation 	jumpLeft, jumpRight;
	bool 		jumpCharging;
	float		jumpCharge;

	bool 		spacePressed;

	unsigned long levelUID;


	int  			score;
	int 			extraLife;
	unsigned int 	lifes;
	unsigned int 	ammo;

	bool 	pogo;
	bool 	shipBattery;

	bool 	blueKey;
	bool 	redKey;
	bool 	yellowKey;
	bool 	greenKey;
	
	float 	dyingTime;
	
public:
	Player( GameEngine* newEngine );
	~Player();

	virtual void 			SetScore 		( int newScore );
	virtual void 			SetExtraLife 	( int newExtraLife );
	virtual void 			SetLifes 		( unsigned int newLifes );
	virtual void 			SetAmmo 		( unsigned int newAmmo );
	virtual void 			SetPogo 		( bool newPogo );
	virtual void 			SetShipBattery 	( bool newShipBattery );
	virtual void 			SetBlueKey 		( bool newBlueKey );
	virtual void 			SetRedKey 		( bool newRedKey );
	virtual void 			SetYellowKey 	( bool newYellowKey );
	virtual void 			SetGreenKey 	( bool newGreenKey );
	virtual void 			SetLevelUID 	( unsigned long newLevelUID );

	virtual int 			GetScore 		();
	virtual int 			GetExtraLife 	();
	virtual unsigned int 	GetLifes 		();
	virtual unsigned int 	GetAmmo 		();
	virtual bool 			GetPogo			();
	virtual bool 			GetShipBattery 	();
	virtual bool 			GetBlueKey 		();
	virtual bool 			GetRedKey 		();
	virtual bool 			GetYellowKey 	();
	virtual bool 			GetGreenKey 	();
	virtual unsigned long 	GetLevelUID 	();

	virtual void			Die 			();

	virtual void 			Update 			();
	virtual void 			Draw 			();
	virtual void 			Collision 		();

	virtual void 			MoveCamera		();
	virtual void			OnCreation		();
};





class PlayerTopDown : public Player
{
protected:
	Vector2D movement;

	Animation right;
	Animation up;
	Animation down;
	
public:
	PlayerTopDown 				( GameEngine* newEngine );
	~PlayerTopDown 				();

	virtual void Collision 		();

	virtual void Update 		();
	virtual void Draw 			();
};




class Activatable : public GameObject
{
protected:
public:
	Activatable 				( GameEngine* newEngine );
	~Activatable 				();

	virtual void Update 		();
	virtual void Draw 			();
	virtual void Activate 		( Player* in );
};




class CityOverWorld : public Activatable
{
protected:
	char levelPath[30];
public:
	CityOverWorld 				( GameEngine* newEngine );
	~CityOverWorld();

	virtual void SetLevelPath 	( const char* newPath );

	virtual void Update 		();
	virtual void Draw 			();
	virtual void LoadLevel 		();
	virtual void Activate 		( Player* in );
};





class CityOverWorld1x1 : public CityOverWorld
{
protected:
	char levelPath[30];
public:
	CityOverWorld1x1 				( GameEngine* newEngine );
	~CityOverWorld1x1();

	virtual void Update 		();
	virtual void Draw 			();
};





class Solid  : public GameObject
{
protected:
public:
	Solid 						( GameEngine* newEngine );
	~Solid 						();

	virtual void Update 		();
	virtual void Draw 			();
};





class SolidTop : public GameObject
{
protected:
public:
	SolidTop 					( GameEngine* newEngine );
	~SolidTop 					();

	virtual void Update 		();
	virtual void Draw 			();
};






class BackGround  : public GameObject
{
protected:
public:
	BackGround 					( GameEngine* newEngine );
	~BackGround 				();

	virtual void Update 		();
	virtual void Draw 			();
};





class BackGroundAnimation  : public GameObject
{
protected:
	Animation anim;
public:
	BackGroundAnimation 		( GameEngine* newEngine );
	~BackGroundAnimation 		();

	virtual void SetTileIndex	( int newTileIndex 	);

	virtual void Update 		();
	virtual void Draw 			();
};





class Banner  : public Actor
{
protected:
	bool showEverything;
	float showEverythingTimeStamp;
	bool black;
public:
	Banner 						( GameEngine* newEngine );
	~Banner 					();

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
	float fadeTimeStamp;
	bool showSelf;
	bool keyDown2;
public:
	MainMenu 					( GameEngine* newEngine );
	~MainMenu 					();

	virtual void Update 		();
	virtual void Draw 			();
};






class Trap : public GameObject
{
protected:
	Animation anim;
public:
	Trap 						( GameEngine* newEngine );
	~Trap 						();

	virtual void Update 		();
	virtual void Draw 			();
	virtual void SetTileIndex	( int newTileIndex );
	virtual void SetTileSetID	( int newTileSetID );
};





class GameManager : public GameObject
{
protected:
	bool showStats, showHelp, showLives;

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

	float constructorTimeStamp;

	Player* connectedPlayer;

	RawTileMap*  windowBg;
public:
	GameManager 				( GameEngine* newEngine );
	~GameManager();

	virtual void SetShowLives 	( bool newShowLives );

	virtual void DrawStatsWindow();
	virtual void DrawLivesWindow();

	virtual void Update 		();
	virtual void Draw 			();
};





class StaticSign : public GameObject
{
protected:
	int spriteID;
public:
	StaticSign 					( GameEngine* newEngine );
	~StaticSign 				();

	virtual void SetSpriteID	( int newSpriteID );
	virtual void Update 		();
	virtual void Draw 			();
};





class HelpWindow : public GameObject
{
protected:
	bool show;
	bool keyDown;
	RawTileMap*  helpText;
	int firstLine;
public:
	HelpWindow 					( GameEngine* newEngine );
	~HelpWindow 				();

	virtual void Update 		();
	virtual void Draw 			();
};





class Exit : public GameObject
{
protected:
public:
	Exit 						( GameEngine* newEngine );
	~Exit 						();

	virtual void Update 		();
	virtual void Draw 			();
	virtual void BackToOverworld();
};





class Fader : public GameObject
{
protected:
	bool fadeOut;
public:
	Fader 						( GameEngine* newEngine );
	Fader 						( GameEngine* newEngine, bool newFadeOut );
	~Fader 						();

	virtual void Update 		();
	virtual void Draw 			();
};





class Treasure : public GameObject
{
protected:
	int score;
public:
	Treasure 						( GameEngine* newEngine );
	~Treasure 						();

	virtual void 	SetScore 		( int newScore);
	virtual int 	GetScore 		();
	virtual void 	SetTileIndex 	( int newTileIndex );

	virtual void	Update 			();
	virtual void 	Activate		( Player* in );
};





class Battery : public Treasure
{
protected:
	int score;

	Animation anim;
public:
	Battery 						( GameEngine* newEngine );
	~Battery 						();

	virtual void	Update 			();
	virtual void 	Activate		( Player* in );
	virtual void 	Draw 			();
};





class ShipWrek : public Activatable
{
protected:
	Animation 	anim;
	Animation 	blueBallAnim;
	bool 		windowVisible;
	bool 		keyDown;
	int 		windowAnimationX;
	int 		windowAnimationY;
	int 		windowSizeX;
	int 		windowSizeY;
	Player*		connectedPlayer;

	RawTileMap*  rawShip;
public:
	ShipWrek 						( GameEngine* newEngine );
	~ShipWrek 						();

	virtual void Activate 		 	( Player* in );

	virtual void Update 			();
	virtual void Draw 				();

	virtual void DrawWindow 		();
};





class AboutScreen : public GameObject
{
protected:
	RawTileMap*  aboutText;
	bool keyDown;
	float fadeTimeStamp;
public:
	AboutScreen 					( GameEngine* newEngine );
	~AboutScreen 					();

	virtual void Update 			();
	virtual void Draw 				();
};




class StoryScreen : public GameObject
{
protected:
	RawTileMap*  storyText;
	bool keyDown;
	float fadeTimeStamp;
	int firstLine;
public:
	StoryScreen 					( GameEngine* newEngine );
	~StoryScreen 					();

	virtual void Update 			();
	virtual void Draw 				();
};





class Door : public GameObject
{
protected:
	bool isYellow;
	bool isRed;
	bool isGreen;
	bool isBlue;

	bool isClosed;

	bool opening;
	int posOffset;
	int subOffset;
public:
	Door 							( GameEngine* newEngine );
	~Door 							();

	virtual void 	SetTileIndex 	( int newTileIndex );

	virtual void Update 			();
	virtual void Draw 				();
};




class Key : public Treasure
{
protected:
	bool isYellow;
	bool isRed;
	bool isGreen;
	bool isBlue;
public:
	Key 							( GameEngine* newEngine );
	~Key 							();

	virtual void SetTileIndex 		( int newTileIndex );

	virtual void Update 			();
	virtual void Draw 				();
	virtual void Activate			( Player* in );
};




class SynthStation : public Activatable
{
protected:
	bool 			windowVisible;
	bool 			keyDown;

	SoundBlasterInstrument 	instrument;

	char octave;
	int channel;

	bool instrumentChanged;

	bool activeKeys[10] = {0,0,0,0,0,0,0,0,0,0};

	Vector2D 		windowPos;

	Animation 		blueBallAnim;

	int selector;

	RawTileMap*  rawSynth;
public:
	SynthStation 					( GameEngine* newEngine );
	~SynthStation 					();

	virtual void Update 			();
	virtual void Draw 				();
	virtual void Activate			( Player* in );
	virtual void SelectInstrument 	( int index );
	virtual void WriteBackInstrument( unsigned char index );
	virtual void HandleKeys 		();
};

#endif