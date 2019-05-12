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

	centerPosOffset = Vector2D( width / 2, height / 2 );
	//centerPos = pos + centerPosOffset;
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

bool GameObject::BoxBoxCollision( Vector2D pos1, int width1, int height1, Vector2D pos2, int width2, int height2 )
{
	if( 	pos1.x < pos2.x + width2 && pos1.x + width1 > pos2.x
		&&	pos1.y < pos2.y + height2 && pos1.y + height1 > pos2.y )
	{
		return true;
	}
	
	return false;	
}

vector<GameObject*> GameObject::CollisionDetection()
{
	vector<GameObject*> objectsInRange = engine->GetObjectsInArea(pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );
	vector<GameObject*> outObjects;


	for( unsigned int i = 0; i < objectsInRange.size(); i++ )
	{	
		if( BoxBoxCollision(pos + direction, width, height, objectsInRange[i]->GetPos(), objectsInRange[i]->GetWidth(), objectsInRange[i]->GetHeight() ) )
		{
			outObjects.push_back( objectsInRange[i] );
		}
	}

	return outObjects;
}

bool GameObject::RayBoxIntersect( Vector2D origin, Vector2D dir, float *tout, Vector2D boxPos, int boxWidth, int boxHeight )
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
 	/*if( tmin <= 0 && tmax <= 0 )
 	{
 		return false;
 	}*/

 	/*if(tmin < 0)
 	{
 		tmin = 100000;
 	}
 	if(tmax < 0)
 	{
 		tmax = 100000;
 	}*/

    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 

    if(tmin < 0)
    {
    	return false;
    }

    *tout = tmin;

    //*hit = origin + ( dir * tmin );
    //*hit2 = origin + ( dir * tmax );
 
    return true; 
}

float GameObject::FindClosestCollision( vector<GameObject*> objects, GameObject* closestObject )
{
	float tmin = 10000;
	closestObject = NULL;

	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		float t = 100000;
		Vector2D origin = pos;
		if( RayBoxIntersect( origin, direction, &t, objects[i]->GetPos(), objects[i]->GetWidth(), objects[i]->GetHeight() ) )
		{
			if( t < tmin)
			{
				tmin = t;
				closestObject = objects[i];
			}
		}
		origin = pos + Vector2D( width, 0 );
		if( RayBoxIntersect( origin, direction, &t, objects[i]->GetPos(), objects[i]->GetWidth(), objects[i]->GetHeight() ) )
		{
			if( t < tmin)
			{
				tmin = t;
				closestObject = objects[i];
			}
		}
		origin = pos + Vector2D( 0, height );
		if( RayBoxIntersect( origin, direction, &t, objects[i]->GetPos(), objects[i]->GetWidth(), objects[i]->GetHeight() ) )
		{
			if( t < tmin)
			{
				tmin = t;
				closestObject = objects[i];
			}
		}
		origin = pos + Vector2D( width, height );
		if( RayBoxIntersect( origin, direction, &t, objects[i]->GetPos(), objects[i]->GetWidth(), objects[i]->GetHeight() ) )
		{
			if( t < tmin)
			{
				tmin = t;
				closestObject = objects[i];
			}
		}
	}

	return tmin;
}

Vector2D GameObject::VectorProjection( Vector2D posIn, Vector2D dirIn,  int tmin, GameObject* closestObject )
{
	Vector2D v1 = dirIn * tmin;
	posIn = posIn + v1;

	Vector2D out;

	if( (int)posIn.x == (int)closestObject->GetPos().x || (int)posIn.x == (int)closestObject->GetPos().x + closestObject->GetWidth() )
	{
		out = Vector2D( v1.x , dirIn.y );
	}
	else
	{	
		out = Vector2D( dirIn.x , v1.y );
	}

	return out;
}

bool GameObject::FindCollisionPoint( GameObject* testObject, Vector2D testPoint, Vector2D* out )
{
	if( 	testPoint.x > testObject->GetPos().x && testPoint.x < testObject->GetPos().x + testObject->GetWidth() 
		&&	testPoint.y > testObject->GetPos().y && testPoint.y < testObject->GetPos().y + testObject->GetHeight() )
	{
		float xDistance = testPoint.x - ( testObject->GetWidth() + testPoint.x );
		float yDistance = testPoint.y - ( testObject->GetHeight() + testPoint.y );


		if( fabs( xDistance ) < fabs( yDistance ) )
		{
			
			if( xDistance < 0 )
			{
				out->SetXY( testObject->GetPos().x, testPoint.y );
			}
			else
			{
				out->SetXY( testObject->GetPos().x + testObject->GetWidth(), testPoint.y );
			}
			
		}
		else
		{
			if( yDistance < 0 )
			{
				out->SetXY( testPoint.x, testObject->GetPos().y );
			}
			else
			{
				out->SetXY( testPoint.x, testObject->GetPos().y + testObject->GetHeight() );
			}
		}

		return true;
	}

	return false;
}
void GameObject::SimpleCollisionResolution( vector<GameObject*> colliders )
{
	Vector2D nextPos = pos + direction;

	for( unsigned int i = 0; i < colliders.size(); i++ )
	{
		Vector2D topLeft 		= nextPos;
		
		
		Vector2D testPos;
		if( FindCollisionPoint( colliders[i], topLeft, &testPos ) )
		{
			nextPos = testPos;
		}

		Vector2D topRight 		= nextPos + Vector2D( width, 0 );
		if( FindCollisionPoint( colliders[i], topRight, &testPos ) )
		{
			nextPos = testPos;
			nextPos = nextPos - Vector2D( width, 0 );
		}

		Vector2D bottomLeft 	= nextPos + Vector2D( 0, height );
		if( FindCollisionPoint( colliders[i], bottomLeft, &testPos ) )
		{
			nextPos = testPos;
			nextPos = nextPos - Vector2D( 0, height );
		}

		Vector2D bottomRight 	= nextPos + Vector2D( width, height );
		if( FindCollisionPoint( colliders[i], bottomRight, &testPos ) )
		{
			nextPos = testPos;
			nextPos = nextPos - Vector2D( width, height );
		}
	}

	direction = nextPos - pos;
}

void GameObject::Collision( )
{
	vector<GameObject*> colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );

	float downLeft  = RayDown( centerPos + Vector2D( -6, 0 ), ( height / 2 ), colliders );
	float downRight = RayDown( centerPos + Vector2D(  6, 0 ), ( height / 2 ), colliders );

	if( downLeft != -1 )
	{
		centerPos.y = centerPos.y - ( ( height / 2 ) - downLeft );
		onFloor = true;
		direction.y = 0;
	}
	else if( downRight != -1 )
	{
		centerPos.y = centerPos.y - ( ( height / 2 ) - downRight );
		onFloor = true;
		direction.y = 0;
	}
	else
	{
		onFloor = false;
	}

	float up  = RayUp( centerPos, ( height / 2 ), colliders );

	if( up != -1 )
	{
		centerPos.y = centerPos.y + ( ( height / 2 ) + up );
		direction.y = 0;
	}
	else if( up != -1 )
	{
		centerPos.y = centerPos.y + ( ( height / 2 ) + up );
		direction.y = 0;
	}
	

	float right = RayRight( centerPos, ( width / 2 ), colliders );
	
	if( right != -1 )
	{
		centerPos.x = centerPos.x - ( ( width / 2 ) - right );
		
	}

	float left = RayLeft( centerPos, ( width / 2 ), colliders );
	
	if( left != -1 )
	{
		centerPos.x = centerPos.x + ( ( width / 2 ) + left );
		
	}
	



	colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID_TOP );
	downLeft  = RayDown( centerPos + Vector2D( -6, 5 ), ( height / 2 ) - 5, colliders );
	downRight = RayDown( centerPos + Vector2D(  6, 5 ), ( height / 2 ) - 5, colliders );

	if( downLeft != -1 )
	{
		centerPos.y = centerPos.y - ( ( ( height / 2 ) - 5 ) - downLeft );
		onFloor = true;
	}
	else if( downRight != -1 )
	{
		centerPos.y = centerPos.y - ( ( ( height / 2 ) - 5 ) - downRight );
		onFloor = true;
	}


	pos = centerPos - centerPosOffset;
}

float GameObject::RayDown( Vector2D origin, float length, vector<GameObject*> testObjects )
{	
	float minDistance = 9999;
	float distance = 0;
	Vector2D down( 0, 1 );
	Vector2D tip = origin + ( down * length );

	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		if( 	tip.x > testObjects[i]->GetPos().x && tip.x < testObjects[i]->GetPos().x + testObjects[i]->GetWidth()
			&& 	tip.y > testObjects[i]->GetPos().y && tip.y < testObjects[i]->GetPos().y + testObjects[i]->GetHeight() )
		{
			distance = testObjects[i]->GetPos().y - origin.y;
			if( distance < minDistance )
			{
				minDistance = distance;
			}
		}
	}

	if( minDistance != 9999 )
	{
		return minDistance;
	}

	return -1;
}

float GameObject::RayRight( Vector2D origin, float length, vector<GameObject*> testObjects )
{	
	float minDistance = 9999;
	float distance = 0;
	Vector2D right( 1, 0 );
	Vector2D tip = origin + ( right * length );

	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		if( 	tip.x > testObjects[i]->GetPos().x && tip.x < testObjects[i]->GetPos().x + testObjects[i]->GetWidth()
			&& 	tip.y > testObjects[i]->GetPos().y && tip.y < testObjects[i]->GetPos().y + testObjects[i]->GetHeight() )
		{
			distance = testObjects[i]->GetPos().x - origin.x;
			if( distance < minDistance )
			{
				minDistance = distance;
			}
		}
	}

	if( minDistance != 9999 )
	{
		return minDistance;
	}

	return -1;
}
float GameObject::RayUp( Vector2D origin, float length, vector<GameObject*> testObjects )
{	
	float minDistance = 9999;
	float distance = 0;
	Vector2D up( 0, -1 );
	Vector2D tip = origin + ( up * length );

	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		if( 	tip.x > testObjects[i]->GetPos().x && tip.x < testObjects[i]->GetPos().x + testObjects[i]->GetWidth()
			&& 	tip.y > testObjects[i]->GetPos().y && tip.y < testObjects[i]->GetPos().y + testObjects[i]->GetHeight() )
		{
			distance = ( testObjects[i]->GetPos().y + testObjects[i]->GetHeight() ) - origin.y;
			if( distance < minDistance )
			{
				minDistance = distance;
			}
		}
	}

	if( minDistance != 9999 )
	{
		return minDistance;
	}

	return -1;
}

float GameObject::RayLeft( Vector2D origin, float length, vector<GameObject*> testObjects )
{	
	float minDistance = 9999;
	float distance = 0;
	Vector2D left( -1, 0 );
	Vector2D tip = origin + ( left * length );

	for( unsigned int i = 0; i < testObjects.size(); i++ )
	{
		if( 	tip.x > testObjects[i]->GetPos().x && tip.x < testObjects[i]->GetPos().x + testObjects[i]->GetWidth()
			&& 	tip.y > testObjects[i]->GetPos().y && tip.y < testObjects[i]->GetPos().y + testObjects[i]->GetHeight() )
		{
			distance = ( testObjects[i]->GetPos().x + testObjects[i]->GetWidth() ) - origin.x;
			if( distance < minDistance )
			{
				minDistance = distance;
			}
		}
	}

	if( minDistance != 9999 )
	{
		return minDistance;
	}

	return -1;
}

void GameObject::Move()
{
	pos = pos + direction * engine->time->GetDelta();
	centerPos = pos + centerPosOffset;	
}
void GameObject::Friction( float slickness )
{
	if(direction.x != 0)
	{
		direction.x = direction.x * slickness;
		if(direction.x <= 0.25f && direction.x >= -0.25f)
		{
			direction.x = 0;
		}
	}
	
	if(direction.y != 0)
	{
		direction.y = direction.y * slickness;
	}
	if(direction.y <= 0.25f && direction.y >= -0.25f)
	{
		direction.y = 0;
	}
}






//========== Player =============
Player::Player( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  = TYPE_PLAYER; //1

	width = 16;
	height = 24;
	tileIndex = 0;
	tileSetID = ASSET_KEEN_WALK;
	drawOrder = 2;

	walkForward.id = 0;
	walkForward.tileSetID = ASSET_KEEN_WALK;
	walkForward.firstTileIndex = 1;
	walkForward.numSprites = 3;
	walkForward.currentFrame = 0;
	walkForward.speed = 7;
	walkForward.currentSpeedStep = 0;

	walkBackward.id = 0;
	walkBackward.tileSetID = ASSET_KEEN_WALK;
	walkBackward.firstTileIndex = 4;
	walkBackward.numSprites = 3;
	walkBackward.currentFrame = 0;
	walkBackward.speed = 7;
	walkBackward.currentSpeedStep = 0;

	spacePressed = false;

	centerPosOffset = Vector2D( width / 2, height / 2 );
}
Player::~Player()
{

}
void Player::Update()
{
	
	movement = Vector2D( 0.0f, 0.0f );
	if( engine->input->KeyDown( KEY_UP ) )
	{
		movement = Vector2D( 0.0f, -0.5f );
	}
	else if( engine->input->KeyDown( KEY_LEFT ) )
	{
		movement = Vector2D( -0.5f, 0.0f );	
	}
	else if( engine->input->KeyDown( KEY_DOWN ) )
	{
		movement = Vector2D( 0.0f, 0.5f );
	}
	else if( engine->input->KeyDown( KEY_RIGHT ) )
	{
		movement = Vector2D( 0.5f, 0.0f );
	}
	else if( engine->input->KeyDown( SPACE ) && onFloor && !spacePressed)
	{
		movement = Vector2D( 0.0f, -10.0f );
		spacePressed = true;
	}
	else
	{
		movement = Vector2D( 0.0f, 0.0f );
	}
	
	if( !engine->input->KeyDown( SPACE ) )
	{
		spacePressed = false;
	}


	AddForce( movement );
	
	if( !onFloor )
	{
		AddForce( Vector2D( 0, 0.5 ) ); //gravity
		Friction( 0.95f );
	}
	else
	{
		Friction( 0.9f );
	}

	Move();

	Collision();
	
	engine->graphics->SetCamCenter( centerPos );
}
void Player::Draw()
{
	/*char str[500];
	sprintf(str, "movement=%f:%f\ndirection=%f:%f\npos=%f:%f\n", movement.x, movement.y, direction.x, direction.y, pos.x, pos.y );
	engine->graphics->DrawText( Vector2D( 0, 9 ) + engine->graphics->GetCamPos() , 1, 0, str );*/
	
	if( direction.x > 0 )
	{
		engine->graphics->PlayAnimation( &walkForward, pos, 16 );
	}
	else if( direction.x < 0 )
	{
		engine->graphics->PlayAnimation( &walkBackward, pos, 16 );
	}
	else
	{
		walkForward.currentFrame = 0;
		walkForward.currentSpeedStep = 0;

		walkBackward.currentFrame = 0;
		walkBackward.currentSpeedStep = 0;

		engine->graphics->DrawSprite( pos, tileSetID, tileIndex, 16 );
	}	
}
void  Player::Collision()
{
	vector<GameObject*> colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );

	float downLeft  = RayDown( centerPos + Vector2D( -4, 0 ), 12, colliders );
	float downRight = RayDown( centerPos + Vector2D(  4, 0 ), 12, colliders );

	if( downLeft != -1 )
	{
		centerPos.y = centerPos.y - ( 12 - downLeft );
		onFloor = true;
		direction.y = 0;
	}
	else if( downRight != -1 )
	{
		centerPos.y = centerPos.y - ( 12 - downRight );
		onFloor = true;
		direction.y = 0;
	}
	else
	{
		onFloor = false;
	}

	float upLeft  = RayUp( centerPos + Vector2D( -3, 0 ), 12, colliders );
	float upRight = RayUp( centerPos + Vector2D(  3, 0 ), 12, colliders );

	if( upLeft != -1 )
	{
		centerPos.y = centerPos.y + ( 12 + upLeft );
		direction.y = 0;
	}
	else if( upRight != -1 )
	{
		centerPos.y = centerPos.y + ( 12 + upRight );
		direction.y = 0;
	}
	

	float right = RayRight( centerPos, 5, colliders );
	
	if( right != -1 )
	{
		centerPos.x = centerPos.x - ( 5 - right );
		direction.x = 0;
	}

	float left = RayLeft( centerPos, 5, colliders );
	
	if( left != -1 )
	{
		centerPos.x = centerPos.x + ( 5 + left );
		direction.x = 0;
	}
	



	colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID_TOP );
	downLeft  = RayDown( centerPos + Vector2D( -6, 5 ), 7, colliders );
	downRight = RayDown( centerPos + Vector2D(  6, 5 ), 7, colliders );

	if( downLeft != -1 )
	{
		centerPos.y = centerPos.y - ( 7 - downLeft );
		onFloor = true;
	}
	else if( downRight != -1 )
	{
		centerPos.y = centerPos.y - ( 7 - downRight );
		onFloor = true;
	}


	pos = centerPos - centerPosOffset;
}










//========== PlayerTopDown =============
PlayerTopDown::PlayerTopDown( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  = TYPE_PLAYER_TOP_DOWN; //1

	width = 12;
	height = 16;
	tileIndex = 0;
	tileSetID = ASSET_KEEN_TOP;
	drawOrder = 2;

	centerPosOffset = Vector2D( width / 2, height / 2 );


	down.id = 0;
	down.tileSetID = ASSET_KEEN_TOP;
	down.firstTileIndex = 0;
	down.numSprites = 4;
	down.currentFrame = 0;
	down.speed = 7;
	down.currentSpeedStep = 0;

	right.id = 4;
	right.tileSetID = ASSET_KEEN_TOP;
	right.firstTileIndex = 4;
	right.numSprites = 4;
	right.currentFrame = 0;
	right.speed = 5;
	right.currentSpeedStep = 0;

	up.id = 8;
	up.tileSetID = ASSET_KEEN_TOP;
	up.firstTileIndex = 8;
	up.numSprites = 4;
	up.currentFrame = 0;
	up.speed = 5;
	up.currentSpeedStep = 0;
}
PlayerTopDown::~PlayerTopDown()
{

}
void PlayerTopDown::Update()
{
	
	movement = Vector2D( 0.0f, 0.0f );
	if( engine->input->KeyDown( KEY_UP ) )
	{
		movement = Vector2D( 0.0f, -0.5f );
	}
	else if( engine->input->KeyDown( KEY_LEFT ) )
	{
		movement = Vector2D( -0.5f, 0.0f );	
	}
	else if( engine->input->KeyDown( KEY_DOWN ) )
	{
		movement = Vector2D( 0.0f, 0.5f );
	}
	else if( engine->input->KeyDown( KEY_RIGHT ) )
	{
		movement = Vector2D( 0.5f, 0.0f );
	}
	else
	{
		movement = Vector2D( 0.0f, 0.0f );
	}


	Vector2D centerPos = pos + Vector2D( width / 2, height / 2 );

	vector<GameObject*> citiesInRange = engine->GetObjectsAtPos( centerPos, TYPE_CITY_OVERWORLD );
	if( engine->input->KeyDown( ENTER ) && citiesInRange.size() > 0 )
	{
		CityOverWorld* city = (CityOverWorld*)citiesInRange[0];
		city->LoadLevel();
	}


	AddForce( movement );
	
	Friction( 0.8f );

	Move();

	Collision();

	
	engine->graphics->SetCamCenter( centerPos );

}
void PlayerTopDown::Draw()
{
	

	if( movement.IsUp() )
	{
		engine->graphics->PlayAnimation( &up, pos, 7 );
	}
	else if( movement.IsDown() )
	{
		engine->graphics->PlayAnimation( &down, pos, 7 );
	}
	else if( movement.IsLeft() )
	{
		engine->graphics->PlayAnimation( &right, pos, 7, true, false );
	}
	else if( movement.IsRight() )
	{
		engine->graphics->PlayAnimation( &right, pos, 7 );
	}
	else
	{
		engine->graphics->DrawSprite( pos, tileSetID, tileIndex, 7 );	
	}
}









//========== CityOverWorld =============
CityOverWorld::CityOverWorld( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  = TYPE_CITY_OVERWORLD; //1

	width = 32;
	height = 32;
	tileIndex = 0;					//has to be set
	tileSetID = ASSET_K1_TILES;
	drawOrder = 1;
}
CityOverWorld::~CityOverWorld()
{

}
void CityOverWorld::SetLevelPath( const char* newPath )
{
	strcpy( levelPath, newPath );
	//
}

void CityOverWorld::Update()
{

}
void CityOverWorld::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, 54 );
	engine->graphics->DrawSprite( pos + Vector2D( 16, 0 ), tileSetID, 55 );
	engine->graphics->DrawSprite( pos + Vector2D( 0, 16 ), tileSetID, 67 );
	engine->graphics->DrawSprite( pos + Vector2D( 16, 16 ), tileSetID, 68 );
}
void CityOverWorld::LoadLevel()
{
	engine->SaveObjectsToBank();
	engine->ClearObjects();
	engine->graphics->ClearScreen( 0 );
	TMXMap testMap = engine->LoadTMXMap( levelPath );		//Load Map
	engine->CreateObjectsFromMap( &testMap );			//crrate Objects
}









//========== Solid =============
Solid::Solid( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_SOLID; //2

	drawOrder = 1;
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
	//engine->graphics->DrawRect( pos, width-1, height-1, 1);
}








//========== SolidTop =============
SolidTop::SolidTop( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_SOLID_TOP; //3

	drawOrder = 1;
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
	//engine->graphics->DrawHLine( pos, width, 2);
}








//========== BackGround =============
BackGround::BackGround( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_BACK_GROUND; //4

	drawOrder = 1;
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

	drawOrder = 1;

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
	else if( tileSetID == ASSET_K1_TILES && tileIndex == 468 ) //Star; 4 frames
	{
		anim.tileSetID 		= tileSetID;
		anim.firstTileIndex = tileIndex;
		anim.currentFrame 	= engine->GetRandom() % 4;
		anim.numSprites 	= 4;
		anim.speed 			= 15;
	}
	else //everything else with 4 frames
	{
		anim.tileSetID 		= tileSetID;
		anim.firstTileIndex = tileIndex;
		anim.currentFrame 	= engine->GetRandom() % 4;
		anim.numSprites 	= 4;
		anim.speed 			= 10;
	}
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

	drawOrder = 2;

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

	if( engine->input->AnyKeyDown() )
	{
		showEverything = true;

		showEverythingTimeStamp = engine->time->GetCurrentTimeInMS() -4000;
		palette = engine->graphics->GetPalette();
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

	showSelf = false;
	invisible = true;
}
MainMenu::~MainMenu()
{

}
void MainMenu::Update()
{
	if( engine->input->AnyKeyDown() )
	{
		showSelf = true;
		invisible = false;
	}
	if( showSelf )
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
		if(menuPos == 4 && engine->input->KeyDown( ENTER ) )
		{
			fadeTimeStamp = engine->time->GetCurrentTimeInMS();
			palette = engine->graphics->GetPalette();
		}

		if( fadeTimeStamp != 0 )
		{
			engine->graphics->ChangePaletteBrightness( -1 );

			if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 1000 && menuPos == 1)
			{
				//delete everything
				engine->ClearObjects();
				engine->graphics->ClearScreen( 0 );
				TMXMap testMap = engine->LoadTMXMap("./levels/mars.tmx");		//Load Map
				engine->CreateObjectsFromMap( &testMap );			//crrate Objects
				engine->graphics->SetPalette( palette, 255 );
			}
			if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 1000 && menuPos == 4)
			{
				//delete everything
				engine->ClearObjects();
				engine->graphics->ClearScreen( 0 );
				TMXMap testMap2 = engine->LoadTMXMap("./levels/about1.tmx");		//Load Map
				engine->CreateObjectsFromMap( &testMap2 );			//crrate Objects
				engine->graphics->SetPalette( palette, 255 );
			}
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









//========== Trap =============
Trap::Trap( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_TRAP; //6

	drawOrder = 1;

	anim.id = 0;
	anim.tileSetID = ASSET_K1_TILES;
	anim.firstTileIndex = 0;
	anim.numSprites = 4;
	anim.currentFrame = 0;
	anim.speed = 10;
	anim.currentSpeedStep = 0;
}
Trap::~Trap()
{

}
void Trap::Update()
{

}
void Trap::Draw()
{
	engine->graphics->PlayAnimation( &anim, pos );
}
void Trap::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;
	anim.firstTileIndex = newTileIndex;
}