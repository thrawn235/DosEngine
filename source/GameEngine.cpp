#include "GameEngine.h"

GameEngine::GameEngine()
{
	__djgpp_nearptr_enable();
	graphics = new GraphicsEngine;
	time = new TimeEngine;
	input = new InputEngine;
}
GameEngine::~GameEngine()
{
	//
	//nothing to do yet
	//later add Destroy methods
}

//Set/Get
GraphicsEngine* GameEngine::GetGraphics()
{
	//
	return graphics;
}

//Object Management
void GameEngine::AddObject(GameObject* newObject)
{
	//
	objects.push_back(newObject);
}
void GameEngine::RemoveObject(GameObject* in)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if(objects[i] == in)
		{
			objects.erase(objects.begin() + i);
		}
	}
}
void GameEngine::RemoveObject(int i)
{
	//
	objects.erase(objects.begin() + i);
}
void GameEngine::RemoveObjects(vector<GameObject*> inObjects)
{
	for(unsigned int u = 0; u < inObjects.size(); u++)
	{
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			if(objects[i] == inObjects[u])
			{
				objects.erase(objects.begin() + i);
			}
		}
	}
}
void GameEngine::ClearObjects()
{
	//iterate through all objects and call Destroy first!
	objects.clear();
}

//MainLoopMethods
void GameEngine::UpdateAll()
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}
}
void GameEngine::DrawAll()
{
	vector<GameObject*> drawObjects = objects;
	
	for(int drawOrder = 0; drawOrder < 64; drawOrder++)
	{
		for(unsigned int i = 0; i < drawObjects.size(); i++)
		{
			if(drawOrder == drawObjects[i]->GetDrawOrder())
			{
				drawObjects[i]->Draw();
				drawObjects.erase(drawObjects.begin() + i); //remove Objects from draw list. It has been drawn already
			}
		}
	}
}

//Level
Map* GameEngine::LoadTMXMap()
{

}
void GameEngine::CreateObjectsFromMap(Map* in)
{

}
void GameEngine::CreateObjectsFromMap(Map* in, Vector2D offset)
{

}

//Random
/*void GameEngine::InitRandom()
{

}
unsigned char GameEngine::NotRandom()
{

}*/

//Load/Save

//FindingObjects
vector<GameObject*> GameEngine::GetAllObjects()
{

}
vector<GameObject*> GameEngine::GetAllObjects(int typeID)
{

}
vector<GameObject*> GameEngine::GetObjectsAtPos(Vector2D pos)
{

}
vector<GameObject*> GameEngine::GetObjectsAtPos(Vector2D pos, int typeID)
{

}
GameObject* GameEngine::GetFirstObjectAtPos(Vector2D pos)
{

}
GameObject* GameEngine::GetFirstObjectAtPos(Vector2D pos, int typeID)
{

}
vector<GameObject*> GameEngine::GetObjectsInRadius(Vector2D pos, int radius)
{

}
vector<GameObject*> GameEngine::GetObjectsInRadius(Vector2D pos, int radius, int typeID)
{

}
vector<GameObject*> GameEngine::GetObjectsAlongRay(Vector2D origin, Vector2D direction)
{

}
vector<GameObject*> GameEngine::GetObjectsAlongRay(Vector2D origin, Vector2D direction, int typeID)
{

}
GameObject* GameEngine::GetFirstObjectAlongRay(Vector2D origin, Vector2D direction)
{

}
GameObject* GameEngine::GetFirstObjectAlongRay(Vector2D origin, Vector2D direction, int typeID)
{

}

//math trasitions
float GameEngine::Linear()
{

}
float GameEngine::Clip(float in, float min, float max)
{
	if(in >= max)
	{
		return max;
	}
	if(in <= min)
	{
		return min;
	}
	return in;
}