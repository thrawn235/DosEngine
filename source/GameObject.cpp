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

	UID = engine->GetUID();

	important 	= false;

	invisible	= false;
	enabled		= true;
	persistent 	= false;
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
bool GameObject::GetImportant()
{
	return important;
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
bool GameObject::GetPersistent()
{
	return persistent;
	//
}
void GameObject::SetUID( unsigned long newUID )
{
	UID = newUID;
	//
}
unsigned long GameObject::GetUID()
{
	return UID;
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
void GameObject::SetPersistent( bool newPersistent )
{
	persistent = newPersistent;
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
void GameObject::OnCreation()
{

}





//========== Actor =============
Actor::Actor( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  	= TYPE_ACTOR; //1

	onFloor 	= false;
	dying 		= false;
	dead 		= false;
}
Actor::~Actor()
{
}
vector<GameObject*> Actor::GetTouchingObjects()
{
	return touchingObjects;
	//
}
bool Actor::IsOnFloor()
{
	return onFloor;
	//
}
bool Actor::IsDying()
{
	return dying;
	//
}
bool Actor::IsDead()
{
	return dead;
	//
}
void Actor::Die()
{
	dying = true;
	//
}
void Actor::AddForce( Vector2D newForce )
{
	direction = direction + newForce;
	//
}
bool Actor::BoxBoxCollision( Vector2D pos1, int width1, int height1, Vector2D pos2, int width2, int height2 )
{
	if( 	pos1.x < pos2.x + width2 && pos1.x + width1 > pos2.x
		&&	pos1.y < pos2.y + height2 && pos1.y + height1 > pos2.y )
	{
		return true;
	}
	
	return false;	
}
vector<GameObject*> Actor::CollisionDetection()
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
bool Actor::RayBoxIntersect( Vector2D origin, Vector2D dir, float *tout, Vector2D boxPos, int boxWidth, int boxHeight )
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
float Actor::FindClosestCollision( vector<GameObject*> objects, GameObject* closestObject )
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
Vector2D Actor::VectorProjection( Vector2D posIn, Vector2D dirIn,  int tmin, GameObject* closestObject )
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
bool Actor::FindCollisionPoint( GameObject* testObject, Vector2D testPoint, Vector2D* out )
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
void Actor::SimpleCollisionResolution( vector<GameObject*> colliders )
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
void Actor::Collision( )
{
	vector<GameObject*> colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );

	float downLeft  = RayDown( centerPos + Vector2D( -3, 0 ), ( height / 2 ), colliders );
	float downRight = RayDown( centerPos + Vector2D(  3, 0 ), ( height / 2 ), colliders );

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
	downLeft  = RayDown( centerPos + Vector2D( -6, 5 ), ( height / 2 ) - 4, colliders );
	downRight = RayDown( centerPos + Vector2D(  6, 5 ), ( height / 2 ) - 4, colliders );

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
float Actor::RayDown( Vector2D origin, float length, vector<GameObject*> testObjects )
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
float Actor::RayRight( Vector2D origin, float length, vector<GameObject*> testObjects )
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
float Actor::RayUp( Vector2D origin, float length, vector<GameObject*> testObjects )
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
float Actor::RayLeft( Vector2D origin, float length, vector<GameObject*> testObjects )
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
void Actor::Move()
{
	pos = pos + direction * engine->time->GetDelta();
	centerPos = pos + centerPosOffset;	
}
void Actor::Friction( float slickness )
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
Player::Player( GameEngine* newEngine ) : Actor( newEngine )
{
	typeID  	= TYPE_PLAYER; //1

	width 		= 16;
	height 		= 24;
	tileIndex 	= 0;
	tileSetID 	= ASSET_KEEN_WALK;
	drawOrder 	= 1;

	levelUID 	= 0;

	important 	= true;


	walkForward.id 					= 0;
	walkForward.tileSetID 			= ASSET_KEEN_WALK;
	walkForward.firstTileIndex 		= 1;
	walkForward.numSprites 			= 3;
	walkForward.currentFrame 		= 0;
	walkForward.speed 				= 7;
	walkForward.currentSpeedStep 	= 0;

	walkBackward.id 				= 0;
	walkBackward.tileSetID 			= ASSET_KEEN_WALK;
	walkBackward.firstTileIndex 	= 5;
	walkBackward.numSprites 		= 3;
	walkBackward.currentFrame 		= 0;
	walkBackward.speed 				= 7;
	walkBackward.currentSpeedStep 	= 0;

	jumpLeft.id 					= 0;
	jumpLeft.tileSetID 				= ASSET_KEEN_WALK;
	jumpLeft.firstTileIndex 		= 14;
	jumpLeft.numSprites				= 6;
	jumpLeft.currentFrame 			= 0;
	jumpLeft.speed 					= 7;
	jumpLeft.currentSpeedStep 		= 0;

	jumpRight.id 					= 0;
	jumpRight.tileSetID 			= ASSET_KEEN_WALK;
	jumpRight.firstTileIndex 		= 8;
	jumpRight.numSprites 			= 6;
	jumpRight.currentFrame 			= 0;
	jumpRight.speed 				= 7;
	jumpRight.currentSpeedStep 		= 0;

	deathAnimation.id 				= 0;
	deathAnimation.tileSetID 		= ASSET_KEEN_WALK;
	deathAnimation.firstTileIndex 	= 22;
	deathAnimation.numSprites 		= 2;
	deathAnimation.currentFrame 	= 0;
	deathAnimation.speed 			= 5;
	deathAnimation.currentSpeedStep = 0;


	jumpCharging 	= false;
	jumpCharge 		= 0;

	spacePressed 	= false;

	centerPosOffset = Vector2D( width / 2, height / 2 );


	score 		= 0;
	extraLife 	= 20000;
	lifes		= 4;
	ammo 		= 0;

	pogo 		= false;
	shipBattery = false;

	blueKey		= false;
	redKey		= false;
	yellowKey	= false;
	greenKey 	= false;

	dyingTime = 0;


	GameManager* manager = new GameManager( engine );
	engine->AddObject( manager );

}
Player::~Player()
{
}
void Player::SetScore( int newScore )
{
	score = newScore;
	//
}
void Player::SetExtraLife( int newExtraLife )
{
	extraLife = newExtraLife;
	//
}
void Player::SetLifes( int newLifes )
{
	lifes = newLifes;
	//
}
void Player::SetAmmo( int newAmmo )
{
	ammo = newAmmo;
	//
}
void Player::SetPogo( bool newPogo )
{
	pogo = newPogo;
	//
}
void Player::SetShipBattery( bool newShipBattery )
{
	shipBattery = newShipBattery;
	//
}
void Player::SetBlueKey( bool newBlueKey )
{
	blueKey = newBlueKey;
	//
}
void Player::SetRedKey( bool newRedKey )
{
	redKey = newRedKey;
	//
}
void Player::SetYellowKey( bool newYellowKey )
{
	yellowKey = newYellowKey;
	//
}
void Player::SetGreenKey( bool newGreenKey )
{
	greenKey = newGreenKey;
	//
}
void Player::SetLevelUID( unsigned long newLevelUID )
{
	levelUID = newLevelUID;
	//
}
int Player::GetScore()
{
	return score;
	//
}
int Player::GetExtraLife()
{
	return extraLife;
	//
}
int Player::GetLifes()
{
	return lifes;
	//
}
int Player::GetAmmo()
{
	return ammo;
	//
}
bool Player::GetPogo()
{
	return pogo;
	//
}
bool Player::GetShipBattery()
{
	return shipBattery;
	//
}
bool Player::GetBlueKey()
{
	return blueKey;
	//
}
bool Player::GetRedKey()
{
	return redKey;
	//
}
bool Player::GetYellowKey()
{
	return yellowKey;
	//
}
bool Player::GetGreenKey()
{
	return greenKey;
	//
}
unsigned long Player::GetLevelUID()
{
	return levelUID;
	//
}
void Player::Die()
{
	if( !dying )
	{
		dyingTime = engine->time->GetCurrentTimeInMS();
		dying = true;
		AddForce( Vector2D( 0.5,-3 ) );
	}
}
void Player::Update()
{
	movement = Vector2D( 0.0f, 0.0f );
	if( engine->input->KeyDown( KEY_LEFT ) )
	{
		movement = movement + Vector2D( -0.4f, 0.0f );	
	}
	if( engine->input->KeyDown( KEY_RIGHT ) )
	{
		movement = movement + Vector2D( 0.4f, 0.0f );
	}
	if( engine->input->KeyDown( SPACE ) && onFloor && !spacePressed)
	{
		//movement = Vector2D( 0.0f, -10.0f );
		spacePressed = true;
		jumpCharging = true;
	}
	
	if( !engine->input->KeyDown( SPACE ) )
	{
		spacePressed = false;
		jumpCharging = false;
	}

	if( !dead && !dying )
	{

		AddForce( movement );

		if( jumpCharging == true)
		{	
			jumpCharge = jumpCharge + engine->time->GetDelta() / 2;
			if( jumpCharge > 23 )
			{
				jumpCharge = 23;
			}
		}
		if( jumpCharging == false && jumpCharge > 0 && onFloor )
		{
			AddForce( Vector2D( 0.0, -jumpCharge/2 ) );
			jumpCharge = 0;
		}
		
		if( !onFloor )
		{
			AddForce( Vector2D( 0, 0.5f ) * engine->time->GetDelta() ); //gravity
			Friction( 0.90f );
		}
		else
		{
			Friction( 0.86f );
		}

		vector<GameObject*> treasures = engine->GetObjectsInArea( pos, width, height, TYPE_TREASURE );
		for( unsigned int i = 0; i < treasures.size(); i++ )
		{
			Treasure* treasure = ( Treasure* )treasures[i];
			treasure->Activate( this );
		}
	}
	if( engine->time->GetCurrentTimeInMS() > dyingTime + 2200 && dying )
	{
		engine->graphics->FadeOut();
	}
	if( engine->time->GetCurrentTimeInMS() > dyingTime + 5000 && dying )
	{
		engine->ClearObjects();
		engine->LoadObjectsFromBank();
		lifes--;
		Player* overWorldPlayer = ( Player* )engine->GetFirstObject( TYPE_PLAYER_TOP_DOWN );
		//overWorldPlayer->SetScore( GetScore() );
		overWorldPlayer->SetLifes( lifes );
		Fader* fader = new Fader( engine );
		engine->AddObject( fader );
		GameManager* manager = ( GameManager* )engine->GetFirstObject( TYPE_GAME_MANAGER );
		if( manager != NULL )
		{
			manager->SetShowLives( true );
		}
	}


	Move();

	if( !dead && !dying )
	{
		Collision();
		
		MoveCamera();
	}
}
void Player::Draw()
{
	/*char str[500];
	sprintf(str, "movement=%f:%f\ndirection=%f:%f\npos=%f:%f\n", movement.x, movement.y, direction.x, direction.y, pos.x, pos.y );
	engine->graphics->DrawText( Vector2D( 0, 9 ) + engine->graphics->GetCamPos() , 1, 0, str );*/
	if( !dead && !dying )
	{
		if( jumpCharging )
		{
			if( direction.x >= 0 )
			{
				engine->graphics->PlayAnimationDelta( &jumpRight, pos, 16, engine->time->GetDelta() );
				if(jumpRight.currentFrame >= 5)
					jumpRight.currentSpeedStep = 0;
			}
			else if( direction.x < 0 )
			{
				engine->graphics->PlayAnimationDelta( &jumpLeft, pos, 16, engine->time->GetDelta() );
				if(jumpLeft.currentFrame >= 5)
					jumpLeft.currentSpeedStep = 0;
			}
		}
		else
		{
			jumpLeft.currentFrame = 0;
			jumpLeft.currentSpeedStep = 0;
			jumpRight.currentSpeedStep = 0;
			jumpRight.currentFrame = 0;

			if( onFloor )
			{
				if( direction.x > 0 )
				{
					engine->graphics->PlayAnimationDelta( &walkForward, pos, 16, engine->time->GetDelta() );
				}
				else if( direction.x < 0 )
				{
					engine->graphics->PlayAnimationDelta( &walkBackward, pos, 16, engine->time->GetDelta() );
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
			else
			{
				if( direction.x >= 0 )
				{
					engine->graphics->DrawSprite( pos, tileSetID, 13, 16 );
				}
				else if( direction.x < 0 )
				{
					engine->graphics->DrawSprite( pos, tileSetID, 19, 16 );
				}
			}
		}
	}
	if( dying )
	{
		engine->graphics->PlayAnimationDelta( &deathAnimation, pos, 16, engine->time->GetDelta() );
	}
}
void  Player::Collision()
{
	vector<GameObject*> colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );

	float downLeft  = RayDown( centerPos + Vector2D( -4, 0 ), 13, colliders );
	float downRight = RayDown( centerPos + Vector2D(  4, 0 ), 13, colliders );

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
	

	float right = RayRight( centerPos + Vector2D( 0, -6 ), 5, colliders );
	float right2 = RayRight( centerPos + Vector2D( 0, 6 ), 5, colliders );
	
	if( right != -1 )
	{
		centerPos.x = centerPos.x - ( 5 - right );
		direction.x = 0;
	}
	else if( right2 != -1 )
	{
		centerPos.x = centerPos.x - ( 5 - right );
		direction.x = 0;
	}

	float left = RayLeft( centerPos + Vector2D( 0, -6 ), 5, colliders );
	float left2 = RayLeft( centerPos + Vector2D( 0, 6 ), 5, colliders );
	
	if( left != -1 )
	{
		centerPos.x = centerPos.x + ( 5 + left );
		direction.x = 0;
	}
	else if ( left2 != -1 )
	{
		centerPos.x = centerPos.x + ( 5 + left );
		direction.x = 0;
	}	
	



	colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID_TOP );
	downLeft  = RayDown( centerPos + Vector2D( -6, 5 ), 8, colliders );
	downRight = RayDown( centerPos + Vector2D(  6, 5 ), 8, colliders );

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
void Player::MoveCamera()
{
	Vector2D formerCamCenter = engine->graphics->GetCamCenter();

	if( pos.x < engine->graphics->GetCamCenter().x - 50 )
	{
		engine->graphics->SetCamCenter( Vector2D( pos.x + 50 , formerCamCenter.y ) );
	}
	if( pos.x > engine->graphics->GetCamCenter().x + 50 )
	{
		engine->graphics->SetCamCenter( Vector2D( pos.x - 50 , formerCamCenter.y ) );
	}

	formerCamCenter = engine->graphics->GetCamCenter();

	if( pos.y < engine->graphics->GetCamCenter().y - 50 )
	{
		engine->graphics->SetCamCenter( Vector2D( formerCamCenter.x , pos.y + 50 ) );
	}
	if( pos.y > engine->graphics->GetCamCenter().y + 50 )
	{
		engine->graphics->SetCamCenter( Vector2D( formerCamCenter.x , pos.y - 50 ) );
	}

	if( engine->graphics->GetCamPos().x < engine->GetLevelBoundaryXMin() )
	{
		engine->graphics->SetCamPos( Vector2D( engine->GetLevelBoundaryXMin(), engine->graphics->GetCamPos().y ) );
	}
	if( engine->graphics->GetCamPos().y < engine->GetLevelBoundaryYMin() )
	{
		engine->graphics->SetCamPos( Vector2D( engine->graphics->GetCamPos().x , engine->GetLevelBoundaryYMin() ) );
	}

	if( engine->graphics->GetCamPos().x + engine->graphics->GetScreenWidth() > engine->GetLevelBoundaryXMax() )
	{
		engine->graphics->SetCamPos( Vector2D( engine->GetLevelBoundaryXMax() - engine->graphics->GetScreenWidth() , engine->graphics->GetCamPos().y ) );
	}
	if( engine->graphics->GetCamPos().y + engine->graphics->GetScreenHeight() > engine->GetLevelBoundaryYMax() )
	{
		engine->graphics->SetCamPos( Vector2D( engine->graphics->GetCamPos().x , engine->GetLevelBoundaryYMax() - engine->graphics->GetScreenHeight() ) );
	}
}
void Player::OnCreation()
{
	engine->graphics->SetCamCenter( pos );
	//
}





//========== PlayerTopDown =============
PlayerTopDown::PlayerTopDown( GameEngine* newEngine ) : Player( newEngine )
{
	typeID  = TYPE_PLAYER_TOP_DOWN; //1

	width = 12;
	height = 16;
	tileIndex = 0;
	tileSetID = ASSET_KEEN_TOP;
	drawOrder = 2;

	centerPosOffset = Vector2D( width / 2, height / 2 );

	important = true;

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
		movement = movement + Vector2D( 0.0f, -0.5f );
	}
	if( engine->input->KeyDown( KEY_LEFT ) )
	{
		movement = movement + Vector2D( -0.5f, 0.0f );	
	}
	if( engine->input->KeyDown( KEY_DOWN ) )
	{
		movement = movement + Vector2D( 0.0f, 0.5f );
	}
	if( engine->input->KeyDown( KEY_RIGHT ) )
	{
		movement = movement + Vector2D( 0.5f, 0.0f );
	}
	if( !engine->input->AnyKeyDown() )
	{
		movement.Zero();
	}


	Vector2D centerPos = pos + Vector2D( width / 2, height / 2 );

	vector<GameObject*> activatables = engine->GetObjectsAtPos( centerPos, TYPE_ACTIVATABLE );
	if( engine->input->KeyDown( ENTER ) && activatables.size() > 0 )
	{
		Activatable* activatable = (Activatable*)activatables[0];
		activatable->Activate( this );
	}

	/*ShipWrek* shipWrek = ( ShipWrek* )engine->GetFirstObjectAtPos( centerPos, TYPE_SHIP_WREK );
	if( engine->input->KeyDown( ENTER ) && shipWrek != NULL )
	{
		shipWrek->Activate( this );
	}*/


	AddForce( movement );
	
	Friction( 0.8f );

	Move();

	Collision();

	
	MoveCamera();
}
void PlayerTopDown::Draw()
{
	

	if( movement.IsUp() )
	{
		engine->graphics->PlayAnimationDelta( &up, pos, 7, engine->time->GetDelta() );
	}
	else if( movement.IsDown() )
	{
		engine->graphics->PlayAnimationDelta( &down, pos, 7, engine->time->GetDelta() );
	}
	else if( movement.IsLeft() )
	{
		engine->graphics->PlayAnimationDelta( &right, pos, 7, true, false, engine->time->GetDelta() );
	}
	else if( movement.IsRight() )
	{
		engine->graphics->PlayAnimationDelta( &right, pos, 7, engine->time->GetDelta() );
	}
	else
	{
		engine->graphics->DrawSprite( pos, tileSetID, tileIndex, 7 );	
	}
}
void  PlayerTopDown::Collision()
{
	vector<GameObject*> colliders = engine->GetObjectsInArea( pos + Vector2D(-10, -10) ,30, 40, TYPE_SOLID );

	float downLeft  = RayDown( centerPos + Vector2D( 0, 0 ), ( height / 2 ), colliders );

	if( downLeft != -1 )
	{
		centerPos.y = centerPos.y - ( ( height / 2 ) - downLeft );
		onFloor = true;
		direction.y = 0;
	}


	float up  = RayUp( centerPos, ( height / 2 ), colliders );

	if( up != -1 )
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
	

	pos = centerPos - centerPosOffset;
}




//========== Activatable =============
Activatable::Activatable( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID  = TYPE_ACTIVATABLE; //1
}
Activatable::~Activatable()
{

}

void Activatable::Update()
{

}
void Activatable::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
}
void Activatable::Activate( Player* in )
{

}





//========== CityOverWorld =============
CityOverWorld::CityOverWorld( GameEngine* newEngine ) : Activatable( newEngine )
{
	typeID  = TYPE_ACTIVATABLE; //1

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
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	engine->graphics->DrawSprite( pos + Vector2D( 16, 0 ), tileSetID, tileIndex + 1 );
	engine->graphics->DrawSprite( pos + Vector2D( 0, 16 ), tileSetID, tileIndex + 13 );
	engine->graphics->DrawSprite( pos + Vector2D( 16, 16 ), tileSetID, tileIndex + 14 );
}
void CityOverWorld::LoadLevel()
{
	Player* overWorldPlayer = ( Player* )engine->GetFirstObject( TYPE_PLAYER_TOP_DOWN );
	engine->SaveObjectsToBank();
	engine->ClearObjects();
	//engine->graphics->BlackOut();
	engine->graphics->ClearScreen( 0 );
	TMXMap testMap = engine->LoadTMXMap( levelPath );		//Load Map
	engine->CreateObjectsFromMap( &testMap );			//crrate Objects
	Player* inLevelPlayer = ( Player* )engine->GetFirstObject( TYPE_PLAYER );
	if( inLevelPlayer != NULL )
	{
		inLevelPlayer->SetLevelUID( overWorldPlayer->GetLevelUID() );
		inLevelPlayer->SetScore( overWorldPlayer->GetScore() );
		inLevelPlayer->SetLifes( overWorldPlayer->GetLifes() );
		inLevelPlayer->SetLevelUID( UID );
	}
	overWorldPlayer->SetLevelUID( UID );
	//Fader* fader = new Fader( engine );
	//engine->AddObject( fader );
}
void CityOverWorld::Activate( Player* in )
{
	LoadLevel();
}





//========== CityOverWorld1x1 =============
CityOverWorld1x1::CityOverWorld1x1( GameEngine* newEngine ) : CityOverWorld( newEngine )
{
	typeID  = TYPE_CITY_OVERWORLD; //1

	width = 16;
	height = 16;
	tileIndex = 0;					//has to be set
	tileSetID = ASSET_K1_TILES;
	drawOrder = 1;
}
CityOverWorld1x1::~CityOverWorld1x1()
{

}
void CityOverWorld1x1::Update()
{

}
void CityOverWorld1x1::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	//
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
	engine->graphics->PlayAnimationDelta( &anim, pos, engine->time->GetDelta() );
	//
}





//========== Banner =============
Banner::Banner( GameEngine* newEngine ) : Actor( newEngine )
{
	typeID = TYPE_BANNER; //6

	drawOrder = 2;

	important = true;

	AddForce( Vector2D( 0, -0.5 ) ); // set initial velocity upwards

	showEverything = false;

	showEverythingTimeStamp = 0;
	black = false;
}
Banner::~Banner()
{
	//free(palette);
	//
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
		//palette = engine->graphics->GetPalette();
	}

	if( showEverything )
	{
		if( showEverythingTimeStamp == 0 )
		{
			showEverythingTimeStamp = engine->time->GetCurrentTimeInMS();
			//palette = engine->graphics->GetPalette();
		}

		if( engine->time->GetCurrentTimeInMS() > showEverythingTimeStamp + 4000)
		{
			//fade to black
			//engine->graphics->ChangePaletteBrightness( -1 );
			
			

			engine->graphics->FadeOut();
		}
		if( engine->time->GetCurrentTimeInMS() > showEverythingTimeStamp + 5500)
		{
			//delete everything
			engine->ClearObjects();
			engine->graphics->ClearScreen( 0 );
			TMXMap testMap = engine->LoadTMXMap("./levels/menubg.tmx");		//Load Map
			engine->CreateObjectsFromMap( &testMap );			//crrate Objects
			//engine->graphics->SetPalette( palette, 255 );
			//engine->graphics->FadeIn();

			GameObject* menu;
			menu = new MainMenu( engine );
			//menu->SetTypeID		( TYPE_MAIN_MENU );
			menu->SetPos 		( Vector2D( 80, 50 ) );
			//menu->SetDimensions ( 16, 16 );
			//menu->SetTileSetID 	( ASSET_K1_TILES );
			//menu->SetTileIndex 	( 468 );
			menu->SetDrawOrder 	( 2 );
			engine->AddObject(menu);


			/*GameObject* manager;
			manager = new GameManager( engine );
			manager->SetPos 		( Vector2D( 80, 50 ) );
			engine->AddObject( manager );*/

			StaticSign* helpSign;
			helpSign = new StaticSign( engine );
			helpSign->SetPos( Vector2D( 95, 180 ) );
			helpSign->SetSpriteID( ASSET_PRESS_F1_HELP );
			engine->AddObject( helpSign );

			HelpWindow* helpWindow;
			helpWindow = new HelpWindow( engine );
			engine->AddObject( helpWindow );

			Fader* fader = new Fader( engine );
			engine->AddObject( fader );
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
		engine->graphics->DrawSprite(pos + Vector2D( 20, +60 ), engine->graphics->GetSprite( ASSET_ID_BLACK ));
		engine->graphics->DrawSprite(pos + Vector2D( 20, +105 ), engine->graphics->GetSprite( ASSET_PRODUCTION ));
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

	keyDown = true;
	menuPos = 1;

	fadeTimeStamp = 0;

	important = true;

	showSelf = false;
	invisible = true;
}
MainMenu::~MainMenu()
{

}
void MainMenu::Update()
{
	if( engine->input->AnyKeyDown() && ! engine->input->KeyDown( KEY_F1 ) && showSelf == false )
	{
		showSelf = true;
		invisible = false;
		keyDown = true;
	}
	if( showSelf )
	{
		if( engine->input->KeyDown( KEY_UP ) && !keyDown && menuPos > 1 )
		{
			blueBallOffsetPos.y = blueBallOffsetPos.y - 8;
			keyDown = true;
			menuPos --;
		}
		if( engine->input->KeyDown( KEY_DOWN ) && !keyDown && menuPos < 8 )
		{
			blueBallOffsetPos.y = blueBallOffsetPos.y + 8;	
			keyDown = true;
			menuPos++;
		}
		if(menuPos == 1 && engine->input->KeyDown( ENTER ) && !keyDown )
		{
			fadeTimeStamp = engine->time->GetCurrentTimeInMS();
			//palette = engine->graphics->GetPalette();
		}
		if(menuPos == 4 && engine->input->KeyDown( ENTER ) && !keyDown )
		{
			fadeTimeStamp = engine->time->GetCurrentTimeInMS();
			//palette = engine->graphics->GetPalette();
		}
		if(menuPos == 3 && engine->input->KeyDown( ENTER ) && !keyDown )
		{
			fadeTimeStamp = engine->time->GetCurrentTimeInMS();
			//palette = engine->graphics->GetPalette();
		}
		if( !engine->input->AnyKeyDown() )
		{
			keyDown = false;
		}

		if( fadeTimeStamp != 0 )
		{
			engine->graphics->FadeOut();

			if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 2000 && menuPos == 1)
			{
				//delete everything
				//GameObject* manager = engine->GetAllObjects( TYPE_GAME_MANAGER )[0];

				engine->ClearObjects();
				engine->graphics->ClearScreen( 0 );
				TMXMap testMap = engine->LoadTMXMap("./levels/mars.tmx");		//Load Map
				engine->CreateObjectsFromMap( &testMap );			//crrate Objects
				//engine->AddObject( manager );

				//HelpWindow* helpWindow = new HelpWindow( engine );
				//engine->AddObject( helpWindow );

				Fader* fader = new Fader( engine );
				engine->AddObject( fader );
				//engine->graphics->SetPalette( palette, 255 );
			}
			if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 2000 && menuPos == 4)
			{
				//delete everything
				engine->ClearObjects();
				engine->graphics->ClearScreen( 0 );
				TMXMap testMap2 = engine->LoadTMXMap("./levels/about1.tmx");		//Load Map
				engine->CreateObjectsFromMap( &testMap2 );			//crrate Objects
				Fader* fader = new Fader( engine );
				engine->AddObject( fader );
				AboutScreen* aboutScreen = new AboutScreen( engine );
				engine->AddObject( aboutScreen );
			}
			if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 2000 && menuPos == 3)
			{
				//delete everything
				engine->ClearObjects();
				engine->graphics->ClearScreen( 0 );
				TMXMap storybg = engine->LoadTMXMap("./levels/storybg.tmx");		//Load Map
				engine->CreateObjectsFromMap( &storybg );			//crrate Objects
				Fader* fader = new Fader( engine );
				engine->AddObject( fader );
				
				StoryScreen* storyScreen = new StoryScreen( engine );
				engine->AddObject( storyScreen );
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
	Player* player = (Player* )engine->GetFirstObjectInArea( pos, width, height, TYPE_PLAYER );
	if( player != NULL )
	{
		player->Die();
	}

}
void Trap::Draw()
{
	engine->graphics->PlayAnimationDelta( &anim, pos, engine->time->GetDelta() );
	//
}
void Trap::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;
	anim.firstTileIndex = newTileIndex;
}
void Trap::SetTileSetID( int newTileSetID )
{
	tileSetID = newTileSetID;
	anim.tileSetID = newTileSetID;
}





//========== GameManager =============
GameManager::GameManager( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_GAME_MANAGER; //11
	drawOrder = 9;

	keyDown = true;

	showHelp = false;
	showStats = false;

	connectedPlayer = NULL;

	important = true;

	constructorTimeStamp = 0;
	showLives = true;

	windowBg = new RawTileMap;
	*windowBg = engine->LoadRawTileMap( "./levels/stats.tmx" );
}
GameManager::~GameManager()
{

}
void GameManager::SetShowLives( bool newShowLives )
{
	constructorTimeStamp = engine->time->GetCurrentTimeInMS();
	showLives = newShowLives;
}
void GameManager::Update()
{
	if(connectedPlayer == NULL)
	{
		connectedPlayer = (Player*)engine->GetFirstObject( TYPE_PLAYER );
		if( connectedPlayer == NULL)
		{
			connectedPlayer = (Player*)engine->GetFirstObject( TYPE_PLAYER_TOP_DOWN );
		}
	}


	if( engine->input->KeyDown( KEY_F2 ) && showStats == false && !keyDown)
	{
		showStats = true;
		keyDown = true;
		engine->DisableAll( this );

	}
	else if( engine->input->KeyDown( KEY_F2 ) && showStats == true  && !keyDown)
	{
		showStats = false;
		keyDown = true;
		engine->EnableAll();
	}
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}

	if( engine->time->GetCurrentTimeInMS() > constructorTimeStamp + 10000 )
	{
		showLives = false;
	}
}
void GameManager::Draw()
{
	if( connectedPlayer != NULL )
	{
		if( showStats )
		{
			DrawStatsWindow();
		}
		if( showLives )
		{
			DrawLivesWindow();
		}
	}
}
void GameManager::DrawLivesWindow()
{
	engine->graphics->DrawWindow( engine->graphics->GetCamPos() + Vector2D( 80, 65 ), 16, 6, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
	char str[20];
	sprintf(str, "     Lives: " );
	engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 88, 73 ), ASSET_TXT_WHITE, 0, str );

	for( unsigned int i = 0; i < connectedPlayer->GetLifes(); i++ )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(90  + ( i*18 ), 85 ), ASSET_KEEN_WALK, 0, 16 );
	}
}
void GameManager::DrawStatsWindow()
{
	engine->graphics->DrawWindow( engine->graphics->GetCamPos() + Vector2D( 40, 35 ), 29, 14, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );

	engine->DrawRawTileMap( engine->graphics->GetCamPos() + Vector2D( 40+8, 35+8 ), windowBg);

	char str[20];
	sprintf(str, "%i", connectedPlayer->GetScore() );
	engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 40+8*7, 35+16 ), ASSET_TXT_WHITE, 0, str );
	sprintf(str, "%i", connectedPlayer->GetExtraLife() );
	engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 56+8*18, 35+16 ), ASSET_TXT_WHITE, 0, str );
	sprintf(str, "%i", connectedPlayer->GetAmmo() );
	engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 56+8*3, 35+8*12 ), ASSET_TXT_WHITE, 0, str );

	for( unsigned int i = 0; i < connectedPlayer->GetLifes(); i++ )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(40+16  + ( i*16 ), 35+8*4 ), ASSET_KEEN_WALK, 0, 16 );
	}

	engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 56+8*2, 35+8*8 ), ASSET_K1_TILES, 414 );

	if( false )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*17, 30+8*5 ), ASSET_K1_TILES, 448, 16 );	//jouystick
	}
	else
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*17, 30+8*5 ), ASSET_K1_TILES, 321, 16 );	//jouystick
	}
	if( connectedPlayer->GetShipBattery() )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*20, 30+8*5 ), ASSET_K1_TILES, 449, 16 );	//jouystick
	}
	else
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*20, 30+8*5 ), ASSET_K1_TILES, 322, 16 );	//battery
	}
	if( false )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*23, 30+8*5 ), ASSET_K1_TILES, 450, 16 );	//jouystick
	}
	else
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*23, 30+8*5 ), ASSET_K1_TILES, 323, 16 );	//vacuumCleaner
	}
	if( false )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*26, 30+8*5 ), ASSET_K1_TILES, 451, 16 );	//jouystick
	}
	else
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*26, 30+8*5 ), ASSET_K1_TILES, 324, 16 );	//boose
	}

	if( connectedPlayer->GetYellowKey() )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*20, 40+8*8 ), ASSET_K1_TILES, 424, 16 );
	}
	if( connectedPlayer->GetRedKey() )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*22, 40+8*8 ), ASSET_K1_TILES, 425, 16 );
	}
	if( connectedPlayer->GetGreenKey() )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*20, 40+8*10 ), ASSET_K1_TILES, 426, 16 );
	}
	if( connectedPlayer->GetBlueKey() )
	{
		engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D( 40+8*22, 40+8*10 ), ASSET_K1_TILES, 427, 16 );
	}
}





//========== StaticSign =============
StaticSign::StaticSign( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_STATIC_SIGN; //12
	drawOrder = 8;
	//
}
StaticSign::~StaticSign()
{

}
void StaticSign::SetSpriteID( int newSpriteID )
{
	spriteID = newSpriteID;
	//
}
void StaticSign::Update()
{

}
void StaticSign::Draw()
{
	engine->graphics->DrawSprite( pos, engine->graphics->GetSprite( spriteID ) );
	//
}





//========== HelpWindow =============
HelpWindow::HelpWindow( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_HELP_WINDOW; //12
	drawOrder = 8;
	show = false;
	keyDown = true;
	important = true;
	persistent = true;

	firstLine = 0;

	helpText = new RawTileMap;
	*helpText = engine->LoadRawTileMap( "./levels/helptxt.tmx" );
}
HelpWindow::~HelpWindow()
{
	delete helpText;
	//
}
void HelpWindow::Update()
{
	if( engine->input->KeyDown( KEY_F1 ) && show == true && !keyDown )
	{
		show = false;
		keyDown = true;
		engine->EnableAll();
	}
	if( engine->input->KeyDown( KEY_F1 ) && !keyDown )
	{
		show = true;
		keyDown = true;
		engine->DisableAll( this );
	}
	if( engine->input->KeyDown( KEY_UP ) && show )
	{
		firstLine --;
		if( firstLine < 0 )
		{
			firstLine = 0;
		}
	}
	if( engine->input->KeyDown( KEY_DOWN ) && show )
	{
		firstLine ++;
		if( firstLine > helpText->height - 20 )
		{
			firstLine = helpText->height + 20;
		}
	}
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}
}
void HelpWindow::Draw()
{
	if( show )
	{
		engine->graphics->DrawWindow( Vector2D( 0, 165 ) + engine->graphics->GetCamPos() , 39, 2, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
		engine->graphics->DrawWindow( Vector2D( 0, 5 ) + engine->graphics->GetCamPos() , 39, 20, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
		char str[50];
		sprintf(str, "     F1 To Exit : Up/Down to Read     " );
		engine->graphics->DrawText( Vector2D( 8, 165+8 ) + engine->graphics->GetCamPos(), ASSET_TXT_GREY, 0, str );
		RawTileMap temp = engine->CropRawTileMap( helpText, 0,firstLine, 36, 19 );
		engine->DrawRawTileMap( engine->graphics->GetCamPos() + Vector2D( 8, 13 ), &temp );
	}
}





//========== Exit =============
Exit::Exit( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_EXIT; //14
	//
}
Exit::~Exit()
{

}
void Exit::Update()
{
	vector<GameObject*> objects = engine->GetObjectsInArea( pos, 32,32 , TYPE_PLAYER );
	if( objects.size() >= 1 )
	{
		BackToOverworld();
	}
}
void Exit::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	engine->graphics->DrawPixel( pos, 15);
	engine->graphics->DrawPixel( Vector2D( pos.x + width-1, pos.y ), 15);
	engine->graphics->DrawPixel( Vector2D( pos.x, pos.y + height-1 ), 15);
	engine->graphics->DrawPixel( Vector2D( pos.x + width-1, pos.y + height-1 ), 15);
}
void Exit::BackToOverworld()
{
	engine->graphics->ClearScreen( 0 );

	Player* inLevelPlayer = ( Player* )engine->GetFirstObject( TYPE_PLAYER );
	engine->ClearObjects();
	engine->LoadObjectsFromBank();
	Player* overWorldPlayer = ( Player* )engine->GetFirstObject( TYPE_PLAYER_TOP_DOWN );
	overWorldPlayer->SetScore( inLevelPlayer->GetScore() );
	overWorldPlayer->SetShipBattery( inLevelPlayer->GetShipBattery() );
	//destroy city
	GameObject* city = engine->GetObjectByUID( inLevelPlayer->GetLevelUID() );
	engine->RemoveObject( city );
}






//========== Fader =============
Fader::Fader( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_FADER; //14
	fadeOut = false;
	important = true;
	persistent = false;
}
Fader::~Fader()
{

}
void Fader::Update()
{
	if( fadeOut )
	{
		if( engine->graphics->FadeOut() )
		{
			engine->RemoveObject( this );
		}
	}
	else
	{
		if( engine->graphics->FadeIn() )
		{
			engine->RemoveObject( this );
		}
	}
}
void Fader::Draw()
{

}






//========== Treasure =============
Treasure::Treasure( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_TREASURE; //14
	score = 10;
}
Treasure::~Treasure()
{

}
void Treasure::Update()
{
	
}
void Treasure::SetScore( int newScore )
{
	score = newScore;
	//
}
int Treasure::GetScore()
{
	engine->RemoveObject( this );

	return score;
}
void Treasure::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;

	if( tileIndex == 202 )
	{
		score = 100;
	}
}
void Treasure::Activate( Player* in )
{
	in->SetScore( in->GetScore() + score );
	engine->RemoveObject( this );
}





//========== Battery =============
Battery::Battery( GameEngine* newEngine ) : Treasure( newEngine )
{
	typeID = TYPE_TREASURE; //14

	anim.id = 0;
	anim.tileSetID = ASSET_K1_TILES;
	anim.firstTileIndex = 237;
	anim.numSprites = 4;
	anim.currentFrame = 0;
	anim.speed = 10;
	anim.currentSpeedStep = 0;
}
Battery::~Battery()
{

}
void Battery::Update()
{
	
}
void Battery::Draw()
{
	engine->graphics->PlayAnimationDelta( &anim, pos, engine->time->GetDelta() );
	//
}
void Battery::Activate( Player* in )
{
	in->SetShipBattery( true );
	engine->RemoveObject( this );
}





//========== ShipWrek =============
ShipWrek::ShipWrek( GameEngine* newEngine ) : Activatable( newEngine )
{
	typeID = TYPE_ACTIVATABLE; //18

	windowVisible		= false;
	keyDown 			= true;
	windowAnimationX	= 0;
	windowAnimationY	= 0;

	windowSizeX 		= 21;
	windowSizeY 		= 10;

	drawOrder = 1;

	anim.id = 0;
	anim.tileSetID = ASSET_K1_TILES;
	anim.firstTileIndex = 83;
	anim.numSprites = 4;
	anim.currentFrame = 0;
	anim.speed = 12;
	anim.currentSpeedStep = 0;

	blueBallAnim.id = 0;
	blueBallAnim.tileSetID = ASSET_8_PIXEL_BORDER_TILES;
	blueBallAnim.firstTileIndex = 9;
	blueBallAnim.numSprites = 6;
	blueBallAnim.currentFrame = 0;
	blueBallAnim.speed = 10;
	blueBallAnim.currentSpeedStep = 0;

	connectedPlayer = NULL;

	rawShip = new RawTileMap;
	*rawShip = engine->LoadRawTileMap( "./levels/rawShip.tmx" );
}
ShipWrek::~ShipWrek()
{
	delete rawShip;
	//
}
void ShipWrek::Activate( Player* in )
{
	windowVisible = true;
	drawOrder = 9;
	if( in != NULL)
	{
		connectedPlayer = in;
	}
	engine->DisableAll( this );
}
void ShipWrek::Update()
{
	if( !keyDown && engine->input->AnyKeyDown() && windowVisible )
	{
		keyDown = true;
		drawOrder = 1;
		windowVisible = false;
		engine->EnableAll();
	}
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}
}
void ShipWrek::Draw()
{
	engine->graphics->PlayAnimationDelta( &anim, pos, engine->time->GetDelta() );

	if( windowVisible )
	{
		DrawWindow();

		if( windowAnimationX < windowSizeX )
		{
			windowAnimationX++;
		}
		if( windowAnimationY < windowSizeY )
		{
			windowAnimationY++;
		}
	}
	else
	{
		windowAnimationY = 0;
		windowAnimationX = 0;
	}
}
void ShipWrek::DrawWindow()
{
	engine->graphics->DrawWindow( engine->graphics->GetCamPos() + Vector2D( 80, 65 ), windowAnimationX, windowAnimationY, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
	
	if( windowAnimationX == windowSizeX && windowAnimationY == windowSizeY )
	{
		char str[50];
		sprintf(str, "Your Ship is missing\nthese Parts: " );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 88, 73 ), ASSET_TXT_WHITE, 0, str );

		sprintf(str, "Go get them\npress a key!: " );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + Vector2D( 88, 125 ), ASSET_TXT_WHITE, 0, str );

		if( connectedPlayer != NULL )
		{
			if( false )
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+0, 100 ), ASSET_K1_TILES, 448, 16 );	//jouystick
			}
			else
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+0, 100 ), ASSET_K1_TILES, 321, 16 );	//jouystick
			}
			if( connectedPlayer -> GetShipBattery() )
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+20, 100 ), ASSET_K1_TILES, 449, 16 );	//jouystick
			}
			else
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+20, 100 ), ASSET_K1_TILES, 322, 16 );	//battery
			}
			if( false )
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+40, 100 ), ASSET_K1_TILES, 450, 16 );	//jouystick
			}
			else
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+40, 100 ), ASSET_K1_TILES, 323, 16 );	//vacuumCleaner
			}
			if( false )
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+60, 100 ), ASSET_K1_TILES, 451, 16 );	//jouystick
			}
			else
			{
				engine->graphics->DrawSprite( engine->graphics->GetCamPos() + Vector2D(100+60, 100 ), ASSET_K1_TILES, 324, 16 );	//boose
			}


		}
		
		engine->graphics->PlayAnimation( &blueBallAnim, engine->graphics->GetCamPos() + Vector2D( 190, 133 ) );
	}
}




//========== AboutScreen =============
AboutScreen::AboutScreen( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_ABOUT_SCREEN; //18

	drawOrder = 5;

	aboutText = new RawTileMap;
	*aboutText = engine->LoadRawTileMap( "./levels/rawabout.tmx" );

	keyDown = true;

	fadeTimeStamp = 0;
}
AboutScreen::~AboutScreen()
{
	delete aboutText;
	//
}

void AboutScreen::Update()
{
	if( engine->input->AnyKeyDown() && !keyDown && fadeTimeStamp == 0 )
	{
		keyDown = true;
		fadeTimeStamp = engine->time->GetCurrentTimeInMS();
	}
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}

	if( fadeTimeStamp > 0 )
	{
		engine->graphics->FadeOut();

		if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 3500 )
		{
			//delete everything
			engine->ClearObjects();
			engine->graphics->ClearScreen( 0 );
			TMXMap testMap = engine->LoadTMXMap("./levels/menubg.tmx");		//Load Map
			engine->CreateObjectsFromMap( &testMap );			//crrate Objects

			GameObject* menu;
			menu = new MainMenu( engine );
			menu->SetPos 		( Vector2D( 80, 50 ) );
			menu->SetDrawOrder 	( 2 );
			engine->AddObject(menu);

			StaticSign* helpSign;
			helpSign = new StaticSign( engine );
			helpSign->SetPos( Vector2D( 95, 180 ) );
			helpSign->SetSpriteID( ASSET_PRESS_F1_HELP );
			engine->AddObject( helpSign );

			//HelpWindow* helpWindow;
			//helpWindow = new HelpWindow( engine );
			//engine->AddObject( helpWindow );

			Fader* fader = new Fader( engine );
			engine->AddObject( fader );
		}
	}
}
void AboutScreen::Draw()
{
	engine->DrawRawTileMap( engine->graphics->GetCamPos() + Vector2D( 16, 58 ), aboutText );
	engine->graphics->DrawSprite(pos + Vector2D( 100, 20 ), engine->graphics->GetSprite( ASSET_ID_BANNER ));
}





//========== StoryScreen =============
StoryScreen::StoryScreen( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID = TYPE_STORY_SCREEN; //18

	drawOrder = 5;

	storyText = new RawTileMap;
	*storyText = engine->LoadRawTileMap( "./levels/storyraw.tmx" );

	keyDown = true;

	fadeTimeStamp = 0;
	firstLine = 0;
}
StoryScreen::~StoryScreen()
{
	delete storyText;
	//
}

void StoryScreen::Update()
{
	if( engine->input->KeyDown( KEY_F1 ) && !keyDown )
	{
		keyDown = true;
		fadeTimeStamp = engine->time->GetCurrentTimeInMS();
	}
	if( engine->input->KeyDown( KEY_F1 ) && !keyDown )
	{
		keyDown = true;
	}
	if( engine->input->KeyDown( KEY_UP ) )
	{
		firstLine --;
		if( firstLine < 0 )
		{
			firstLine = 0;
		}
	}
	if( engine->input->KeyDown( KEY_DOWN ) )
	{
		firstLine ++;
		if( firstLine > storyText->height - 20 )
		{
			firstLine = storyText->height + 20;
		}
	}
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}

	if( fadeTimeStamp > 0 )
	{
		engine->graphics->FadeOut();

		if( engine->time->GetCurrentTimeInMS() > fadeTimeStamp + 3500 )
		{
			//delete everything
			engine->ClearObjects();
			engine->graphics->ClearScreen( 0 );
			TMXMap testMap = engine->LoadTMXMap("./levels/menubg.tmx");		//Load Map
			engine->CreateObjectsFromMap( &testMap );			//crrate Objects

			GameObject* menu;
			menu = new MainMenu( engine );
			menu->SetPos 		( Vector2D( 80, 50 ) );
			menu->SetDrawOrder 	( 2 );
			engine->AddObject(menu);

			StaticSign* helpSign;
			helpSign = new StaticSign( engine );
			helpSign->SetPos( Vector2D( 95, 180 ) );
			helpSign->SetSpriteID( ASSET_PRESS_F1_HELP );
			engine->AddObject( helpSign );

			//HelpWindow* helpWindow;
			//helpWindow = new HelpWindow( engine );
			//engine->AddObject( helpWindow );

			Fader* fader = new Fader( engine );
			engine->AddObject( fader );
		}
	}
}
void StoryScreen::Draw()
{
	engine->graphics->DrawWindow( Vector2D( 0, 125 ) + engine->graphics->GetCamPos() , 39, 2, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
	engine->graphics->DrawWindow( Vector2D( 0, 5 ) + engine->graphics->GetCamPos() , 39, 15, ASSET_8_PIXEL_BORDER_TILES, 1, 3, 6, 8, 2, 4, 31 );
	
	char str[50];
	sprintf(str, "     F1 To Exit : Up/Down to Read     " );
	engine->graphics->DrawText( Vector2D( 8, 125+8 ) + engine->graphics->GetCamPos(), ASSET_TXT_GREY, 0, str );

	RawTileMap temp = engine->CropRawTileMap( storyText, 0,firstLine, 38, 14 );
	engine->DrawRawTileMap( engine->graphics->GetCamPos() + Vector2D( 8, 13 ), &temp );

}





//========== Door =============
Door::Door( GameEngine* newEngine ) : GameObject( newEngine )
{
	typeID 		= TYPE_DOOR; 

	isYellow 	= false;
	isRed 		= false;
	isGreen 	= false;
	isBlue 		= false;

	isClosed 	= true;

	height 		= 32;

	opening		= false;
	posOffset 	= 0;
	subOffset 	= 0;
}
Door::~Door()
{

}


void Door::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;

	if( tileIndex == 173 )
	{
		isYellow = true;
	}
	if( tileIndex == 195 )
	{
		isRed = true;
	}
	if( tileIndex == 197 )
	{
		isGreen = true;
	}
	if( tileIndex == 199)
	{
		isBlue = true;
	}
}

void Door::Update()
{
	Player* player = ( Player* )engine->GetFirstObjectInArea( pos - Vector2D(5,5), 16+10, 32+5, TYPE_PLAYER );
	if( player != NULL )
	{
		if( isYellow )
		{
			if( player->GetYellowKey() )
			{
				isClosed = false;
				opening = true;
				drawOrder = 1;
			}
		}
		if( isRed )
		{
			if( player->GetRedKey() )
			{
				isClosed = false;
				opening = true;
				drawOrder = 1;
			}
		}
		if( isGreen )
		{
			if( player->GetGreenKey() )
			{
				isClosed = false;
				opening = true;
				drawOrder = 1;
			}
		}
		if( isBlue )
		{
			if( player->GetBlueKey() )
			{
				isClosed = false;
				opening = true;
				drawOrder = 1;
			}
		}
	}

	if( isClosed )
	{
		typeID = TYPE_SOLID;
	}
	else
	{
		typeID = TYPE_DOOR;
	}

	if( opening )
	{
		subOffset = subOffset + engine->time->GetDelta();
		if( subOffset > 5 )
		{
			posOffset++;
			subOffset = 0;	
		}
		if( posOffset >= 32 )
		{
			engine->RemoveObject( this );
		}
	}
}
void Door::Draw()
{
	engine->graphics->DrawSprite( pos + Vector2D( 0,  posOffset ), tileSetID, tileIndex );
	if( posOffset < 16 )
	{
		engine->graphics->DrawSprite( pos + Vector2D( 0, 16 + posOffset ), tileSetID, tileIndex+1 );
	}
}





//========== Key =============
Key::Key( GameEngine* newEngine ) : Treasure( newEngine )
{
	typeID 		= TYPE_TREASURE; 

	isYellow 	= false;
	isRed 		= false;
	isGreen 	= false;
	isBlue 		= false;
}
Key::~Key()
{

}


void Key::SetTileIndex( int newTileIndex )
{
	tileIndex = newTileIndex;

	if( tileIndex == 190 )
	{
		isYellow = true;
	}
	if( tileIndex == 191 )
	{
		isRed = true;
	}
	if( tileIndex == 192 )
	{
		isGreen = true;
	}
	if( tileIndex == 193)
	{
		isBlue = true;
	}
}

void Key::Update()
{

}
void Key::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );
	//
}
void Key::Activate( Player* in )
{
	if( isYellow )
	{
		in->SetYellowKey( true );
	}
	if( isRed )
	{
		in->SetRedKey( true );
	}
	if( isGreen )
	{
		in->SetGreenKey( true );
	}
	if( isBlue )
	{
		in->SetBlueKey( true );
	}

	engine->RemoveObject( this );
}




//========== SynthStation =============
SynthStation::SynthStation( GameEngine* newEngine ) : Activatable( newEngine )
{
	typeID = TYPE_ACTIVATABLE; //18

	drawOrder = 1;

	windowVisible		= false;
	keyDown 			= true;

	instrumentChanged 	= false;

	selector 			= 0;

	octave 				= 5;
	channel 			= 0;

	windowPos.x 		= 20;
	windowPos.y 		= 10;


	blueBallAnim.id = 0;
	blueBallAnim.tileSetID = ASSET_8_PIXEL_BORDER_TILES;
	blueBallAnim.firstTileIndex = 9;
	blueBallAnim.numSprites = 6;
	blueBallAnim.currentFrame = 0;
	blueBallAnim.speed = 10;
	blueBallAnim.currentSpeedStep = 0;

	rawSynth = new RawTileMap;
	*rawSynth = engine->LoadRawTileMap( "./levels/synth.tmx" );
}
SynthStation::~SynthStation()
{
	//
}
void SynthStation::Activate( Player* in )
{
	windowVisible = true;
	drawOrder = 9;

	engine->DisableAll( this );
}
void SynthStation::Update()
{
	/*if( !keyDown && engine->input->KeyDown( KEY_F3 ) && windowVisible )
	{
		keyDown = true;
		drawOrder = 1;
		windowVisible = false;
		engine->EnableAll();
	}
	if( !keyDown && engine->input->KeyDown( TAB ) && windowVisible )
	{
		keyDown = true;
		selector++;
		if( selector >= 20 )
		{
			selector = 0;
		}
	}
	if( selector == 0 && !keyDown && engine->input->KeyDown( KEY_RIGHT ) && windowVisible )
	{
		keyDown = true;
		if( channel < 32 )
		{
			channel++;
		}
	}
	if( selector == 0 && !keyDown && engine->input->KeyDown( KEY_LEFT ) && windowVisible )
	{
		keyDown = true;
		if( channel > 0 )
		{
			channel--;
		}
	}
	
	if( !engine->input->AnyKeyDown() )
	{
		keyDown = false;
	}*/


	if( instrumentChanged )
	{
		/*instrument = engine->sound->SetInstrumentLevel 				( instrument, 0, instrument.modulatorScalingLevel );
		instrument = engine->sound->SetInstruemtADSREnvelope 		( instrument, 0, instrument.modulatorAttackDecay, instrument.modulatorSustainRelease );
		instrument = engine->sound->SetInstrumentSoundCharacteristic( instrument, 0, instrument.modulatorSoundCharacteristic );
		instrument = engine->sound->SetInstrumentWaveForm			( instrument, 0, instrument.modulatorWaveSelect );

		instrument = engine->sound->SetInstrumentLevel 				( instrument, 1, instrument.carrierScalingLevel );
		instrument = engine->sound->SetInstruemtADSREnvelope 		( instrument, 1, instrument.carrierAttackDecay, instrument.carrierSustainRelease );
		instrument = engine->sound->SetInstrumentSoundCharacteristic( instrument, 1, instrument.carrierSoundCharacteristic );
		instrument = engine->sound->SetInstrumentWaveForm			( instrument, 1, instrument.carrierWaveSelect );

		instrument = engine->sound->SetInstrumentFeedBack			( instrument, instrument.feedback );

		WriteBackInstrument( 0 );*/
	}
	

	HandleKeys();
	
}
void SynthStation::Draw()
{
	engine->graphics->DrawSprite( pos, tileSetID, tileIndex );

	if( windowVisible )
	{
		engine->DrawRawTileMap( engine->graphics->GetCamPos() + windowPos, rawSynth );
	
		if( selector == 0 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*9, 8*1 ), engine->time->GetDelta() );
		}
		if( selector == 1 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*4 ), engine->time->GetDelta() );
		}
		if( selector == 2 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*5 ), engine->time->GetDelta() );
		}
		if( selector == 3 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*6 ), engine->time->GetDelta() );
		}
		if( selector == 4 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*7 ), engine->time->GetDelta() );
		}
		if( selector == 5 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*9 ), engine->time->GetDelta() );
		}
		if( selector == 6 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*10 ), engine->time->GetDelta() );
		}
		if( selector == 7 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*11 ), engine->time->GetDelta() );
		}
		if( selector == 8 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*12 ), engine->time->GetDelta() );
		}
		if( selector == 9 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*12, 8*14 ), engine->time->GetDelta() );
		}
		if( selector == 10 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*4 ), engine->time->GetDelta() );
		}
		if( selector == 11 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*5 ), engine->time->GetDelta() );
		}
		if( selector == 12 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*6 ), engine->time->GetDelta() );
		}
		if( selector == 13 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*7 ), engine->time->GetDelta() );
		}
		if( selector == 14 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*9 ), engine->time->GetDelta() );
		}
		if( selector == 15 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*10 ), engine->time->GetDelta() );
		}
		if( selector == 16 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*11 ), engine->time->GetDelta() );
		}
		if( selector == 17 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*12 ), engine->time->GetDelta() );
		}
		if( selector == 18 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*28, 8*14 ), engine->time->GetDelta() );
		}
		if( selector == 19 )
		{
			engine->graphics->PlayAnimationDelta( &blueBallAnim, engine->graphics->GetCamPos() + windowPos + Vector2D( 8*9, 8*16 ), engine->time->GetDelta() );
		}

		char str[10];
		sprintf(str, "%i", channel );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*10, 8*1 ), ASSET_TXT_WHITE, 0, str );
		/*sprintf(str, "%i", carrierAttack );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*4 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierDecay );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*5 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierSustain );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*6 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierRelease );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*7 ), ASSET_TXT_WHITE, 0, str );

		sprintf(str, "%i", carrierAmplitudeMod );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*9 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierVibrato );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*10 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierHold );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*11 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", carrierHarmonics );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*12 ), ASSET_TXT_WHITE, 0, str );
		
		sprintf(str, "%i", carrierLevel );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*13, 8*14 ), ASSET_TXT_WHITE, 0, str );


		sprintf(str, "%i", modulatorAttack );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*4 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorDecay );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*5 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorSustain );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*6 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorRelease );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*7 ), ASSET_TXT_WHITE, 0, str );
		
		sprintf(str, "%i", modulatorAmplitudeMod );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*9 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorVibrato );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*10 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorHold );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*11 ), ASSET_TXT_WHITE, 0, str );
		sprintf(str, "%i", modulatorHarmonics );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*12 ), ASSET_TXT_WHITE, 0, str );

		sprintf(str, "%i", modulatorLevel );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*29, 8*14 ), ASSET_TXT_WHITE, 0, str );


		sprintf(str, "%i", octave );
		engine->graphics->DrawText( engine->graphics->GetCamPos() + windowPos + Vector2D( 8*10, 8*16 ), ASSET_TXT_WHITE, 0, str );*/
	}
}
void SynthStation::SelectInstrument( int index )
{
	//SoundBlasterInstrument* instp = engine->sound->GetInstrument( index );
	//instrument = *instp;
	//
}
void SynthStation::WriteBackInstrument( unsigned char index )
{
	//engine->sound->ReplaceInstrument( &instrument, index );
	//
}
void SynthStation::HandleKeys()
{
	/*for( int i = 0; i < 10; i++ )
	{
		if(  engine->input->KeyDown( i+2 ) && windowVisible && activeKeys[i] == false )
		{
			activeKeys[i] = true;
			engine->sound->NoteOn( channel, i + octave * 12, 40 ); //c#
		}
		if( !engine->input->KeyDown( i+2 ) && activeKeys[i] == true )
		{
			activeKeys[i] = false;
			engine->sound->NoteOff( channel, i + octave * 12 ); //c#
		}
	}*/
}