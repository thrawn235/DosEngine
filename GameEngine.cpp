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

}