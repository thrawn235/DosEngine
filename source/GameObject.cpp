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
	invisible	= false;
	enabled		= true;
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
bool GameObject::GetEnabled()
{
	return enabled;
	//
}
bool GameObject::GetInvisible()
{
	return invisible;
	//
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
	//engine->graphics->DrawLine( pos, Vector2D( pos.x + width, pos.y + height ), 8 );
	//engine->graphics->DrawLine( Vector2D( pos.x , pos.y +height ), Vector2D( pos.x + width, pos.y ), 8 );
}
//============================================

void GameObject::SetInsivible( bool newInvisible )
{
	invisible = newInvisible;
	//
}
void GameObject::Enable()
{
	enabled = true;
	//
}
void GameObject::Disable()
{
	enabled = false;
	//
}

void GameObject::AddForce( Vector2D newForce )
{
	direction = direction + newForce;
	//
}
bool GameObject::RayBoxIntersect( Vector2D origin, Vector2D dir, Vector2D* hit, Vector2D boxPos, int boxWidth, int boxHeight )
{
	Vector2D boxTopLeft 	= boxPos;
	Vector2D boxBottomRight = boxPos + Vector2D( boxWidth, boxHeight );

	float tmin = ( boxBottomRight.x - origin.x ) / dir.x; 
    float tmax = ( boxTopLeft.x - origin.x )  	/ dir.x; 
 
    if( tmin > tmax )
    {
    	float temp;
    	temp = tmin;
    	tmin = tmax;
    	tmax = temp;
    } 
 
    float tymin = ( boxBottomRight.y - origin.y ) 	/ dir.y; 
    float tymax = ( boxTopLeft.y - origin.y )  		/ dir.y; 
 
    if( tymin > tymax )
	{
		float temp;
		temp = tymin;
		tymin = tymax;
		tymax = temp;
	} 
 
    if ((tmin > tymax) || (tymin > tmax))
    {
        return false; 
    }
 	if( tmin < 0 && tmax < 0 )
 	{
 		return false;
 	}

 	if(tmin < 0)
 	{
 		tmin = 100000;
 	}
 	if(tmax < 0)
 	{
 		tmax = 100000;
 	}

    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 

    *hit = origin + ( dir * tmin );
    //*hit2 = origin + ( dir * tmax );
 
    return true; 
}
bool GameObject::Collision()
{
	//Get cardinal direction of dorection Vector
	Vector2D rayOrigin1 = pos;
	/*Vector2D rayOrigin2 = pos + Vector2D( pos.x + width , pos.y );
	Vector2D rayOrigin3 = pos + Vector2D( pos.x, pos.y + height );
	Vector2D rayOrigin4 = pos + Vector2D( pos.x + width , pos.y +height );*/

	//only test tiles that ate in a logical radius of the ray origin
	vector<GameObject*>testObjects = engine->GetObjectsInRadius(rayOrigin1, 30, TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		Vector2D hit;
		if( RayBoxIntersect( rayOrigin1, direction, &hit, testObjects[i]->GetPos(), testObjects[i]->GetWidth(), testObjects[i]->GetHeight() ) )
		{
			pos = hit;
			return true;
		}
	}

	/*testObjects = engine->GetObjectsInRadius(rayOrigin2, 30, TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		Vector2D hit;
		if( RayBoxIntersect( rayOrigin2, direction, &hit, testObjects[i]->GetPos(), testObjects[i]->GetWidth(), testObjects[i]->GetHeight() ) )
		{
			pos = hit - Vector2D( pos.x + width , pos.y );
			return true;
		}
	}

	testObjects = engine->GetObjectsInRadius(rayOrigin3, 30, TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		Vector2D hit;
		if( RayBoxIntersect( rayOrigin3, direction, &hit, testObjects[i]->GetPos(), testObjects[i]->GetWidth(), testObjects[i]->GetHeight() ) )
		{
			pos = hit - Vector2D( pos.x, pos.y + height );
			return true;
		}
	}

	testObjects = engine->GetObjectsInRadius(rayOrigin4, 30, TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		Vector2D hit;
		if( RayBoxIntersect( rayOrigin4, direction, &hit, testObjects[i]->GetPos(), testObjects[i]->GetWidth(), testObjects[i]->GetHeight() ) )
		{
			pos = hit - Vector2D( pos.x + width , pos.y +height );
			return true;
		}
	}*/

	return false;
}
void GameObject::Move()
{
	pos = pos + direction * engine->time->GetDelta();
	//
}
void GameObject::Friction( float slickness )
{
	//printf( "direction= %f:%f \n", direction.x, direction.y);
	if(direction.x != 0)
	{
		direction.x = direction.x * slickness;
	}
	
	if(direction.y != 0)
	{
		direction.y = direction.y * slickness;
	}
	//printf( "direction= %f:%f \n", direction.x, direction.y);
	//getch();
	//
}






//========== Player =============
Player::Player( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  = TYPE_PLAYER; //1
}
Player::~Player()
{

}
void Player::Update()
{
	
	movement = Vector2D( 0.0f, 0.0f );
	if( engine->input->KeyDown( KEY_UP ) )
	{
		movement = Vector2D( 0.0f, -2.0f );
	}
	else if( engine->input->KeyDown( KEY_LEFT ) )
	{
		movement = Vector2D( -2.0f, 0.0f );	
	}
	else if( engine->input->KeyDown( KEY_DOWN ) )
	{
		movement = Vector2D( 0.0f, 2.0f );
	}
	else if( engine->input->KeyDown( KEY_RIGHT ) )
	{
		movement = Vector2D( 2.0f, 0.0f );
	}
	else
	{
		movement = Vector2D( 0.0f, 0.0f );
	}


	AddForce( movement );
	
	//Collision();

	Move();
	
	Friction( 0.8f );

	Vector2D centerPos = pos + Vector2D( width / 2, height / 2 );
	engine->graphics->SetCamCenter( centerPos );

}
void Player::Draw()
{
	/*char str[500];
	sprintf(str, "movement=%f:%f\ndirection=%f:%f\npos=%f:%f\n", movement.x, movement.y, direction.x, direction.y, pos.x, pos.y );
	engine->graphics->DrawText( Vector2D( 0, 9 ) + engine->graphics->GetCamPos() , 1, 0, str );*/

	engine->graphics->DrawSprite( pos, tileSetID, tileIndex, 16 );
	engine->graphics->DrawRect( pos, width-1, height-1, 2);

	Vector2D rayOrigin(0,0);
	Vector2D rayDirection(1,1);
	Vector2D boxPos(40,20);
	int boxWidth = 20;
	int boxHeight = 30;
	Vector2D hit = Vector2D( 0, 0 );
	RayBoxIntersect( pos, (direction*100), &hit, boxPos, boxWidth, boxHeight );
	engine->graphics->DrawRect(boxPos, boxWidth, boxHeight, 2);
	engine->graphics->DrawLine(pos, pos+(direction * 100), 2);
	engine->graphics->DrawPixel(hit, 7);

	/*Vector2D hit1(5,10);
	if( RayBoxIntersect(rayOrigin, rayDirection, &hit1, boxPos, boxWidth, boxHeight) )
	{	
		engine->graphics->DrawRect(boxPos, boxWidth, boxHeight, 8);
		engine->graphics->DrawLine(rayOrigin, hit1, 12);
	}

	vector<GameObject*> testObjects = engine->GetAllObjects();
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 0 ), testObjects[i]->GetWidth(), 1 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 0, 0 ), testObjects[i]->GetHeight(), 1 );
	}

	testObjects = engine->GetAllObjects(TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 1 ), testObjects[i]->GetWidth() - 1, 2 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 1, 0 ), testObjects[i]->GetHeight() - 1, 2 );
	}
	testObjects = engine->GetObjectsAtPos(pos);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 2 ), testObjects[i]->GetWidth() - 2, 3 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 2, 0 ), testObjects[i]->GetHeight() - 2, 3 );
	}
	testObjects = engine->GetObjectsAtPos(pos, TYPE_SOLID);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 3 ), testObjects[i]->GetWidth() - 3, 3 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 3, 0 ), testObjects[i]->GetHeight() - 3, 3 );
	}
	testObjects = engine->GetObjectsInRadius(pos, 50);
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 4 ), testObjects[i]->GetWidth() - 4, 4 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 4, 0 ), testObjects[i]->GetHeight() - 4, 4 );
	}
	testObjects = engine->GetObjectsAlongRay(pos, Vector2D( -1, -1 ) );
	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		engine->graphics->DrawHLine( testObjects[i]->GetPos() + Vector2D( 0, 5 ), testObjects[i]->GetWidth() - 5, 6 );
		engine->graphics->DrawVLine( testObjects[i]->GetPos() + Vector2D( 5, 0 ), testObjects[i]->GetHeight() - 5, 6 );
	}*/
}






//========== Solid =============
Solid::Solid( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_SOLID; //2
}
Solid::~Solid()
{

}
void Solid::Update()
{

}
void Solid::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	engine->graphics->DrawRect( pos, width-1, height-1, 1);
}




//========== SolidTop =============
SolidTop::SolidTop( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_SOLID_TOP; //3
}
SolidTop::~SolidTop()
{

}
void SolidTop::Update()
{

}
void SolidTop::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	engine->graphics->DrawHLine( pos, width, 2);
}




//========== BackGround =============
BackGround::BackGround( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_BACK_GROUND; //4
}
BackGround::~BackGround()
{

}
void BackGround::Update()
{

}
void BackGround::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	engine->graphics->DrawPixel( pos, 15);
	engine->graphics->DrawPixel( Vector2D( pos.x + width-1, pos.y ), 15);
	engine->graphics->DrawPixel( Vector2D( pos.x, pos.y + height-1 ), 15);
	engine->graphics->DrawPixel( Vector2D( pos.x + width-1, pos.y + height-1 ), 15);
}


//========== BackGroundAnimation =============
BackGroundAnimation::BackGroundAnimation( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_BACK_GROUND_ANIMATION; //5

	anim.id = 0;
	anim.tileSetID = 0;
	anim.firstTileIndex = 0;
	anim.numSprites = 0;
	anim.currentFrame = 0;
	anim.speed = 0;
	anim.currentSpeedStep = 0;
}
BackGroundAnimation::~BackGroundAnimation()
{

}
void BackGroundAnimation::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;
	if( tileSetID == ASSET_K1_TILES && tileIndex == 167 ) //exit sign; 2 frames
	{
		anim.tileSetID 		= tileSetID;
		anim.firstTileIndex = tileIndex;
		anim.currentFrame 	= engine->GetRandom() % 2;
		anim.numSprites 	= 2;
		anim.speed 			= 15;
	}
	if( tileSetID == ASSET_K1_TILES && tileIndex == 468 ) //Star; 4 frames
	{
		anim.tileSetID 		= tileSetID;
		anim.firstTileIndex = tileIndex;
		anim.currentFrame 	= engine->GetRandom() % 4;
		anim.numSprites 	= 4;
		anim.speed 			= 15;
	}

	//
}
void BackGroundAnimation::Update()
{

}
void BackGroundAnimation::Draw()
{
	engine->graphics->PlayAnimation( &anim, pos );
}




//========== Banner =============
Banner::Banner( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_BANNER; //6

	AddForce( Vector2D( 0, -0.5 ) ); // set initial velocity upwards

	showEverything = false;

	showEverythingTimeStamp = 0;
	palette = NULL;
}
Banner::~Banner()
{
	free(palette);
}
void Banner::Update()
{
	Move();

	if( pos.y <= (engine->graphics->GetScreenHeight() / 2) - 50 )
	{
		direction = Vector2D( 0, 0 );
		showEverything = true;
	}

	if( showEverything )
	{
		if( showEverythingTimeStamp == 0 )
		{
			showEverythingTimeStamp = engine->time->GetCurrentTimeInMS();
			palette = engine->graphics->GetPalette();
		}

		if( engine->time->GetCurrentTimeInMS() > showEverythingTimeStamp + 4000)
		{
			//fade to black
			engine->graphics->ChangePaletteBrightness( -1 );
		}
		if( engine->time->GetCurrentTimeInMS() > showEverythingTimeStamp + 5000)
		{
			//delete everything
			engine->ClearObjects();
			engine->graphics->ClearScreen( 0 );
			TMXMap testMap = engine->LoadTMXMap("./levels/menubg.tmx");		//Load Map
			engine->CreateObjectsFromMap( &testMap );			//crrate Objects
			engine->graphics->SetPalette( palette, 255 );

			GameObject* menu;
			menu = new MainMenu	( engine );
			//menu->SetTypeID		( TYPE_MAIN_MENU );
			menu->SetPos 		( Vector2D( 80, 50 ) );
			//menu->SetDimensions ( 16, 16 );
			//menu->SetTileSetID 	( ASSET_K1_TILES );
			//menu->SetTileIndex 	( 468 );
			menu->SetDrawOrder 	( 2 );
			engine->AddObject(menu);
		}
	}
}
void Banner::Draw()
{
	engine->graphics->DrawSprite( pos, engine->graphics->GetSprite( ASSET_APOGEE ) );

	if( showEverything )
	{
		engine->graphics->DrawSprite(pos + Vector2D( 45, -12 ), engine->graphics->GetSprite( ASSET_AN ));
		engine->graphics->DrawSprite(pos + Vector2D( 15, +30 ), engine->graphics->GetSprite( ASSET_PRESENTS ));
		engine->graphics->DrawSprite(pos + Vector2D( 35, +45 ), engine->graphics->GetSprite( ASSET_OF_AN ));
		engine->graphics->DrawSprite(pos + Vector2D( 25, +60 ), engine->graphics->GetSprite( ASSET_ID_BLACK ));
		engine->graphics->DrawSprite(pos + Vector2D( 25, +105 ), engine->graphics->GetSprite( ASSET_PRODUCTION ));
	}
}





//========== MainMenu =============
MainMenu::MainMenu( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_MAIN_MENU; //6

	blueBallOffsetPos.SetXY( 15, 10 );

	blueBallAnim.id = 0;
	blueBallAnim.tileSetID = ASSET_8_PIXEL_BORDER_TILES;
	blueBallAnim.firstTileIndex = 9;
	blueBallAnim.numSprites = 6;
	blueBallAnim.currentFrame = 0;
	blueBallAnim.speed = 10;
	blueBallAnim.currentSpeedStep = 0;

	bool keyDown = false;
	menuPos = 1;

	palette = NULL;
	fadeTimeStamp = 0;

}
MainMenu::~MainMenu()
{

}
void MainMenu::Update()
{
	if( engine->input->KeyDown( KEY_UP ) && !keyDown && menuPos > 1 )
	{
		blueBallOffsetPos.y = blueBallOffsetPos.y - 8;
		keyDown = true;
		menuPos --;
	}
	else if( engine->input->KeyDown( KEY_DOWN ) && !keyDown && menuPos < 8 )
	{
		blueBallOffsetPos.y = blueBallOffsetPos.y + 8;	
		keyDown = true;
		menuPos++;
	}
	else
	{
		keyDown = false;
	}

	if(menuPos == 1 && engine->input->KeyDown( ENTER ) )
	{
		fadeTimeStamp = engine->time->GetCurrentTimeInMS();
		palette = engine->graphics->GetPalette();
	}

	if( fadeTimeStamp != 0 )
	{
		engine->graphics->ChangePaletteBrightness( -1 );

		if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 1000)
		{
			//delete everything
			engine->ClearObjects();
			engine->graphics->ClearScreen( 0 );
			TMXMap testMap = engine->LoadTMXMap("./levels/k1e1m1.tmx");		//Load Map
			engine->CreateObjectsFromMap( &testMap );			//crrate Objects
			engine->graphics->SetPalette( palette, 255 );
		}
	}

}
void MainMenu::Draw()
{
	engine->graphics->DrawWindow( pos, 20, 11, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
	char str[200];
	sprintf(str, "New Game\nContinue Game\nStory\nAbout thrawn\nHigh Scores\nOrdering Info\nPreviews!\nRestart Demo" );
	engine->graphics->DrawText( pos + Vector2D( 30, 10 ), ASSET_TXT_WHITE, 0, str );

	sprintf(str, "Use the arrows" );
	engine->graphics->DrawText( pos + Vector2D( 10, 80 ), ASSET_TXT_WHITE, 0, str );

	engine->graphics->PlayAnimation( &blueBallAnim, pos + blueBallOffsetPos );
}