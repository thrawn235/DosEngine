# include "GameObject.h"

GameObject::GameObject(GameEngine* newEngine)
{
	typeID = TYPEGAMEOBJECT; //0
	width = 16; //just a guess reset later
	height = 16; //just a guess reset later
	tileSetID = 0; //default
	tileIndex = 0; //default

	engine = newEngine;
}
GameObject::~GameObject()
{

}

void GameObject::Update()
{

}
void GameObject::Draw()
{
	
}