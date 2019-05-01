# include "GameObject.h"

GameObject::GameObject(GameEngine* newEngine)
{
	typeID = TYPEGAMEOBJECT; //0
	width = 16; //just a guess reset later
	height = 16; //just a guess reset later
	tileSetID = 0; //default
	tileIndex = 0; //default
	drawOrder = 0;

	engine = newEngine;

	onFloor = false;
}
GameObject::~GameObject()
{
	//
	//nothing to do yet
}

//Set / Get
int GameObject::GetTypeID()
{
	//
	return typeID;
}
Vector2D GameObject::GetPos()
{
	//
	return pos;
}
Vector2D GameObject::GetDirection()
{
 	//
 	return direction;
}
int GameObject::GetTileSetID()
{
	//
	return tileSetID;
}
int GameObject::GetTileIndex()
{
	//
	return tileIndex;
}
int GameObject::GetDrawOrder()
{
	//
	return drawOrder;
}
bool GameObject::IsOnFloor()
{
	//
	return onFloor;
}
vector<GameObject*> GameObject::GetTouchingObjects()
{
	//
	return touchingObjects;
}
void GameObject::SetTypeID(int newTypeID)
{
	//
	typeID = newTypeID;
}
void GameObject::SetPos(Vector2D newPos)
{
	//
	pos = newPos;
}
void GameObject::SetDirection(Vector2D newDirection)
{
	//
	direction = newDirection;
}
void GameObject::SetTileSetID(int newTileSetID)
{
	//
	tileSetID = newTileSetID;
}
void GameObject::SetTileIndex(int newTileIndex)
{
	//
	tileIndex = newTileIndex;
}
void GameObject::SetDrawOrder(int newDrawOrder)
{
	//
	drawOrder = newDrawOrder;
}
void GameObject::SetDimensions(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

//Main Loop Methods:
void GameObject::Update()
{

}
void GameObject::Draw()
{
	engine->graphics->DrawSprite(pos, tileSetID, tileIndex);
}