///////////////////////////////////////////////////////////////////////////////////////
//
//	GameObject.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	Member functions of GameObject
//	see GameObject.h for details
//
///////////////////////////////////////////////////////////////////////////////////////


# include "GameObject.h"


GameObject::GameObject( GameEngine* newEngine )
{
	typeID 		= TYPE_GAME_OBJECT; //0
	width 		= 16; //just a guess reset later
	height 		= 16; //just a guess reset later
	tileSetID 	= 0; //default
	tileIndex 	= 0; //default
	drawOrder 	= 0;

	engine 		= newEngine;

	onFloor 	= false;
}
GameObject::~GameObject()
{
	//nothing to do yet
	//
}

//================ Set / Get ====================
int GameObject::GetTypeID()
{
	return typeID;
	//
}
Vector2D GameObject::GetPos()
{
	return pos;
	//
}
Vector2D GameObject::GetDirection()
{
 	return direction;
 	//
}
int GameObject::GetTileSetID()
{
	return tileSetID;
	//
}
int GameObject::GetTileIndex()
{
	return tileIndex;
	//
}
int GameObject::GetDrawOrder()
{
	return drawOrder;
	//
}
bool GameObject::IsOnFloor()
{
	return onFloor;
	//
}
vector<GameObject*> GameObject::GetTouchingObjects()
{
	return touchingObjects;
	//
}
int GameObject::GetWidth()
{
	return width;
	//
}
int GameObject::GetHeight()
{
	return height;
	//
}
void GameObject::SetTypeID( int newTypeID )
{
	typeID = newTypeID;
	//
}
void GameObject::SetPos( Vector2D newPos )
{
	pos = newPos;
	//
}
void GameObject::SetDirection( Vector2D newDirection )
{
	direction = newDirection;
	//
}
void GameObject::SetTileSetID( int newTileSetID )
{
	//error checking ?
	tileSetID = newTileSetID;
	//
}
void GameObject::SetTileIndex( int newTileIndex )
{
	//error checking ?
	tileIndex = newTileIndex;
	//
}
void GameObject::SetDrawOrder( int newDrawOrder )
{
	if( drawOrder >= 0 )
	{
		drawOrder = newDrawOrder;
	}
	else
	{
		drawOrder = 0;
	}
}
void GameObject::SetDimensions( int newWidth, int newHeight )
{
	if( newWidth > 0 )
	{
		width = newWidth;
	}
	else
	{
		newWidth = 0;
	}

	if( newHeight > 0 )
	{
		height = newHeight;
	}
	else
	{
		height = 0;
	}
}
//============================================


//============ Main Loop Methods: ============
void GameObject::Update()
{

}
void GameObject::Draw()
{
	//no error checking due to performance
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
}
//============================================







//========== Player =============
Player::Player( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID 		= TYPE_PLAYER; //0
}
Player::~Player()
{

}






//========== Solid =============
Solid::Solid( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID 		= TYPE_SOLID; //0
}
Solid::~Solid()
{

}




//========== SolitTop =============
SolidTop::SolidTop( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID 		= TYPE_SOLID_TOP; //0
}
SolidTop::~SolidTop()
{

}




//========== BackGround =============
BackGround::BackGround( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID 		= TYPE_BACK_GROUND; //0
}
BackGround::~BackGround()
{

}



