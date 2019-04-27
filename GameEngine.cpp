#include "GameEngine.h"

GameEngine::GameEngine()
{
	graphics = new GraphicsEngine;
	time = new TimeEngine;
}
GameEngine::~GameEngine()
{

}