///////////////////////////////////////////////////////////////////////////////////////
//
//	GameEngine.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	Memberfunctions of the GameEngine class
//	see GameEngine.h
//
///////////////////////////////////////////////////////////////////////////////////////


#include "GameEngine.h"



GameEngine::GameEngine()
{
	__djgpp_nearptr_enable();

	randomIndex = 0;

	graphics 	= new GraphicsEngine;
	time 		= new TimeEngine;
	input 		= new InputEngine;
}
GameEngine::~GameEngine()
{
	//nothing to do yet
	//later add Destroy methods
	//
}

//====================== Set/Get ===========================
GraphicsEngine* GameEngine::GetGraphics()
{
	return graphics;
	//
}
TimeEngine* GameEngine::GetTime()
{
	return time;
	//
}
InputEngine* GameEngine::GetInput()
{
	return input;
	//
}
//==========================================================


//================= Object Management=======================
void GameEngine::AddObject( GameObject* newObject )
{
	if( newObject != NULL )
	{
		objects.push_back( newObject );
	}
}
void GameEngine::RemoveObject( GameObject* in )
{
	if( in != NULL )
	{
		for( unsigned int i = 0; i < objects.size(); i++ )
		{
			if( objects[i] == in )
			{
				objects.erase( objects.begin() + i );
			}
		}
	}
}
void GameEngine::RemoveObject( unsigned int i )
{
	if( i >= 0 && i < objects.size() )
	{
		objects.erase( objects.begin() + i );
	}
}
void GameEngine::RemoveObjects( vector<GameObject*> inObjects )
{
	for( unsigned int u = 0; u < inObjects.size(); u++ )
	{
		for( unsigned int i = 0; i < objects.size(); i++ )
		{
			if( objects[i] == inObjects[u] )
			{
				objects.erase( objects.begin() + i );
				i--;
			}
		}
	}
}
void GameEngine::ClearObjects()
{
	//iterate through all objects and call Destroy first!
	objects.clear();
}
//==========================================================


//================ MainLoop Methods ========================
void GameEngine::UpdateAll()
{
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( objects[i]->GetEnabled() )
		{
			objects[i]->Update();
		}
	}
}
void GameEngine::DrawAll()
{
	vector<GameObject*> drawObjects = objects;
	
	/*for( int drawOrder = 0; drawOrder < 64; drawOrder++ )
	{
		for( unsigned int i = 0; i < drawObjects.size(); i++ )
		{
			if( drawOrder == drawObjects[i]->GetDrawOrder() )
			{
				drawObjects[i]->Draw();
				drawObjects.erase( drawObjects.begin() + i ); //remove Objects from draw list. It has been drawn already
				i--; //change i bc remove changes all the indices...
			}
		}
	}*/

	for(unsigned int i = 0; i < drawObjects.size(); i++)
	{
		if( !drawObjects[i]->GetInvisible() )
		{
			drawObjects[i]->Draw();
		}
	}
}
//==========================================================


//===================== Assets =============================
void GameEngine::LoadAssets()
{
	BMP bmp;
	Sprite*  sprite;	
	//-------------------- Load Universal Game Assets -----------------------
	bmp = graphics->LoadBMP("./gfx/txtwhite.bmp");
	graphics->SetPalette(bmp.colorTable, 17);
	sprite = graphics->BMPToSprite(&bmp, 3);
	graphics->FreeBMP(&bmp);

	TileSet* txtwhite = graphics->ExtractTileSet( ASSET_TXT_WHITE, sprite, Vector2D( 0,0 ), 8, 8, 16, 6 );	//tileSetId = 1
	graphics->AddTileSet(txtwhite);
	graphics->FreeSprite(sprite);


	bmp = graphics->LoadBMP("./gfx/8pborder.bmp");
	sprite = graphics->BMPToSprite(&bmp, 3);
	graphics->FreeBMP(&bmp);

	TileSet* border = graphics->ExtractTileSet( ASSET_8_PIXEL_BORDER_TILES, sprite, Vector2D( 0,0 ), 8, 8, 16, 2 );	//tileSetId = 1
	graphics->AddTileSet(border);
	graphics->FreeSprite(sprite);


	bmp = graphics->LoadBMP("./gfx/apogee.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_APOGEE);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	bmp = graphics->LoadBMP("./gfx/an.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_AN);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	bmp = graphics->LoadBMP("./gfx/idblack.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_ID_BLACK);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	bmp = graphics->LoadBMP("./gfx/ofan.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_OF_AN);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	bmp = graphics->LoadBMP("./gfx/present.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_PRESENTS);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	bmp = graphics->LoadBMP("./gfx/product.bmp");
	sprite = graphics->BMPToSprite(&bmp, ASSET_PRODUCTION);
	graphics->AddSprite( sprite );
	graphics->FreeBMP(&bmp);


	//------------------------- Load GameSprites ----------------------------
	bmp = graphics->LoadBMP("./gfx/walk.bmp");
	sprite = graphics->BMPToSprite(&bmp, 3);
	graphics->FreeBMP(&bmp);

	TileSet* keenWalk = graphics->ExtractTileSet( ASSET_KEEN_WALK, sprite, Vector2D( 0,0 ), 16, 24, 17, 2);	//tileSetId = 10
	graphics->AddTileSet(keenWalk);
	graphics->FreeSprite(sprite);


	//------------------------- Load Level Tiles ------------------------
	bmp = graphics->LoadBMP("./gfx/k1tiles.bmp");
	sprite = graphics->BMPToSprite(&bmp, 0);
	graphics->FreeBMP(&bmp);

	TileSet* k1Tiles = graphics->ExtractTileSet( ASSET_K1_TILES, sprite, Vector2D( 0,0 ), 16, 16, 13, 53 ); //tileSetId = 100 (hast to mach property in TMXFile)
	graphics->AddTileSet(k1Tiles);
	graphics->FreeSprite(sprite);


	bmp = graphics->LoadBMP("./gfx/k3tiles.bmp");
	sprite = graphics->BMPToSprite(&bmp, 1);
	graphics->FreeBMP(&bmp);

	TileSet* k2Tiles = graphics->ExtractTileSet( ASSET_K3_TILES, sprite, Vector2D( 0,0 ), 16, 16, 13, 53); //tileSetId = 101 (hast to mach property in TMXFile)
	graphics->AddTileSet(k2Tiles);
	graphics->FreeSprite(sprite);
	

	bmp = graphics->LoadBMP("./gfx/title.bmp");
	sprite = graphics->BMPToSprite(&bmp, 1);
	graphics->FreeBMP(&bmp);

	TileSet* titleTiles = graphics->ExtractTileSet( ASSET_TITLE_TILES, sprite, Vector2D( 0,0 ), 16, 16, 16, 7); //tileSetId = 103 (hast to mach property in TMXFile)
	graphics->AddTileSet(titleTiles);
	graphics->FreeSprite(sprite);
	//---------------------------------------------------------------------
}
//==========================================================


//==================== Level Methods =======================
TMXMap GameEngine::LoadTMXMap( const char* filePath )
{
	/*	The Method loads a tmx file in xml format (no json file etc).
		It does not do any error checking. Program will crash if File is inconsistent
		It can only handle square grids. no hex or isometric stuff
		it doesnt do text or polyline objects
		it cant handle groups (restructuring in subfunctions would be neccesary)
		lots of stuff is untested, like files withoud embedded tileset data or empty tilesets
		Its rather slow too...
		Use with caution 
	*/


	TMXMap newTMXMap;

	bool debug = false;

	char XMLTag[30];

	//open File, read only (txt mode)
	FILE* file = fopen( filePath, "r" );
	if( file == NULL )
	{
		Quit( "Error opening TMX File" );
	}

	//read XML Data
	fscanf( file, "<?xml version=\"%f\" encoding=\"%[^\"]\"?>\n", &newTMXMap.XMLVersion, newTMXMap.encoding );
	if(debug)
	{
		printf( "xml version 	= %f \n", newTMXMap.XMLVersion 	);
		printf( "xml encoding 	= %s \n", newTMXMap.encoding 	);
		getch();
	}

	//read map data:
	fscanf( file, "<%[^> ]", XMLTag );
	if( debug )
	{
		printf( "XMLTag= %s \n", XMLTag );
	}
	if( strcmp( XMLTag, "map" ) != 0 )
	{
		Quit( "couldnt find map XML tag. Invalid TMX File" );
	}

	fscanf(	file, " version=\"%f\" tiledversion=\"%[^\"]\" orientation=\"%[^\"]\" renderorder=\"%[^\"]\" width=\"%i\" height=\"%i\" tilewidth=\"%i\" tileheight=\"%i\" infinite=\"%i\" nextlayerid=\"%i\" nextobjectid=\"%i\">\n", 
	       	&newTMXMap.version, newTMXMap.tiledVersion, newTMXMap.orientation, newTMXMap.renderOrder, &newTMXMap.width, &newTMXMap.height, &newTMXMap.tileWidth, &newTMXMap.tileHeight, &newTMXMap.infinite, &newTMXMap.nextLayerID, &newTMXMap.nextObjectID );

	if( debug )
	{
		printf( "version 		= %f \n", newTMXMap.version 		);
		printf( "tiledVersion 	= %s \n", newTMXMap.tiledVersion 	);
		printf( "orientation 	= %s \n", newTMXMap.orientation 	);
		printf( "renderOrder 	= %s \n", newTMXMap.renderOrder 	);
		printf( "width 			= %i \n", newTMXMap.width 			);
		printf( "height 		= %i \n", newTMXMap.height 			);
		printf( "tileWidth 		= %i \n", newTMXMap.tileWidth 		);
		printf( "tileheight 	= %i \n", newTMXMap.tileHeight 		);
		printf( "infinite 		= %i \n", newTMXMap.infinite		);
		printf( "nextLayerID 	= %i \n", newTMXMap.nextLayerID 	);
		printf( "nextObjectID 	= %i \n", newTMXMap.nextObjectID 	);
		getch();
	}


	while( strcmp( XMLTag, "/map" ) != 0 )
	{
		fscanf( file, "<%[^> ]", XMLTag );
		if( debug )
		{
			printf( "	XMLTag= %s \n", XMLTag );
		}


		if( strcmp( XMLTag, "properties" ) == 0 )
		{
			fscanf( file, ">\n" );

			while( strcmp( XMLTag, "/properties" ) != 0 )
			{
				fscanf( file, "<%[^> ]", XMLTag );
				if( debug )
				{
					printf( "		XMLTag= %s \n", XMLTag );
					getch();
				}


				if( strcmp( XMLTag, "property" ) == 0 )
				{
					TMXProperty newProperty;
					fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
					newTMXMap.properties.push_back( newProperty );
					if( debug )
					{
						printf( "			name 	= %s \n", newTMXMap.properties.back().name			);
						printf( "			type 	= %s \n", newTMXMap.properties.back().type 			);
						printf( "			value 	= %s \n", newTMXMap.properties.back().stringValue 	);
						getch();
					}
					
				}

				if(strcmp( XMLTag, "/properties" ) == 0 )
				{
					fscanf(file, ">\n");
				}
			}
		}
		if( strcmp(XMLTag, "tileset") == 0 )
		{
			TMXTileSet newTileSet;
			fscanf( file, " firstgid=\"%i\" name=\"%[^\"]\" tilewidth=\"%i\" tileheight=\"%i\" tilecount=\"%i\" columns=\"%i\">\n", &newTileSet.firstGID, newTileSet.name, &newTileSet.tileHeight, &newTileSet.tileWidth, &newTileSet.tileCount, &newTileSet.columns );
			newTileSet.tileSetID = -1;
			newTMXMap.tileSets.push_back(newTileSet);

			if(debug)
			{
				printf( "		firtGD 			= %i \n", newTMXMap.tileSets.back().firstGID 		);
				printf( "		name 			= %s \n", newTMXMap.tileSets.back().name 			);
				printf( "		tileWidth 		= %i \n", newTMXMap.tileSets.back().tileHeight 		);
				printf( "		tileHeight 		= %i \n", newTMXMap.tileSets.back().tileWidth 		);
				printf( "		tileCount 		= %i \n", newTMXMap.tileSets.back().tileCount 		);
				printf( "		columns 		= %i \n", newTMXMap.tileSets.back().columns 		);
				printf( "		source 			= %s \n", newTMXMap.tileSets.back().source 			);
				printf( "		sourceWidth 	= %i \n", newTMXMap.tileSets.back().sourceWidth 	);
				printf( "		sourceHeight 	= %i \n", newTMXMap.tileSets.back().sourceHeight 	);
				getch();
			}

			while( strcmp(XMLTag, "/tileset") != 0 )
			{
				fscanf( file, "<%[^> ]", XMLTag );
				if( debug )
				{
					printf( "		XMLTag= %s \n", XMLTag );
					getch();
				}

				if( strcmp( XMLTag, "image" ) == 0 )
				{
					fscanf( file, " source=\"%[^\"]\" width=\"%i\" height=\"%i\"/>\n", newTMXMap.tileSets.back().source, &newTMXMap.tileSets.back().sourceHeight, &newTMXMap.tileSets.back().sourceWidth );
				}

				if( strcmp( XMLTag, "/tileset" ) == 0 )
				{
					fscanf( file, ">\n" );
				}

				if( strcmp( XMLTag, "tile" ) == 0 )
				{
					TMXTile newTile;
					fscanf( file, " id=\"%i\">\n", &newTile.id );
					newTMXMap.tileSets.back().tiles.push_back( newTile );
					newTile.typeID = -1;
					if( debug )
					{
						printf( "			tileID= %i \n", newTile.id );
						//getch();
					}

					while( strcmp( XMLTag, "/tile" ) != 0 )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "			XMLTag= %s \n", XMLTag );
							//getch();
						}

						if( strcmp( XMLTag, "/tile" ) == 0 )
						{
							fscanf( file, ">\n" );
						}

						if( strcmp( XMLTag, "properties" ) == 0 )
						{
							fscanf( file, ">\n" );

							while( strcmp( XMLTag, "/properties" ) != 0 )
							{
								fscanf( file, "<%[^> ]", XMLTag );
								if( debug )
								{
									printf( "				XMLTag= %s \n", XMLTag );
									//getch();
								}


								if( strcmp( XMLTag, "property" ) == 0 )
								{
									TMXProperty newProperty;
									fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
									newTMXMap.tileSets.back().tiles.back().properties.push_back( newProperty );
									if(debug)
									{
										printf( "					name 	= %s \n", newTMXMap.tileSets.back().tiles.back().properties.back().name 		);
										printf( "					type 	= %s \n", newTMXMap.tileSets.back().tiles.back().properties.back().type 		);
										printf( "					value 	= %s \n", newTMXMap.tileSets.back().tiles.back().properties.back().stringValue 	);
										//getch();
									}
									if( strcmp(newProperty.name, "typeID") == 0 )
									{
										newTMXMap.tileSets.back().tiles.back().typeID = atoi(newProperty.stringValue);
									}
									
								}

								if( strcmp( XMLTag, "/properties" ) == 0 )
								{
									fscanf( file, ">\n" );
									//printf("jo");
								}
							}
						}
					}
				}


				if( strcmp( XMLTag, "properties" ) == 0 )
				{
					fscanf( file, ">\n" );

					while( strcmp( XMLTag, "/properties" ) != 0 )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "		XMLTag= %s \n", XMLTag );
							getch();
						}


						if( strcmp( XMLTag, "property" ) == 0 )
						{
							TMXProperty newProperty;
							fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
							newTMXMap.tileSets.back().properties.push_back( newProperty );
							if(debug)
							{
								printf( "			name 	= %s \n", newTMXMap.tileSets.back().properties.back().name 			);
								printf( "			type 	= %s \n", newTMXMap.tileSets.back().properties.back().type 			);
								printf( "			value 	= %s \n", newTMXMap.tileSets.back().properties.back().stringValue 	);
								getch();
							}
							if( strcmp(newProperty.name, "tileSetID") == 0 )
							{
								newTMXMap.tileSets.back().tileSetID = atoi(newProperty.stringValue);
							}
							
						}

						if( strcmp( XMLTag, "/properties" ) == 0 )
						{
							fscanf( file, ">\n" );
						}
					}
				}
			}
		}

		if( strcmp( XMLTag, "layer" ) == 0 )
		{
			TMXLayer newTMXLayer;
			//fscanf(file, " id=\"%i\" name=\"%[^\"]\" width=\"%i\" height=\"%i\">\n", &newTMXLayer.id, newTMXLayer.name, &newTMXLayer.width, &newTMXLayer.height);
			
			//dealing with visible, locked and offsets
			newTMXLayer.visible 	= true;
			newTMXLayer.locked 		= false;
			newTMXLayer.offsetX 	= 0;
			newTMXLayer.offsetY 	= 0;

			char whitespace = ' ';
			fscanf( file, "%c", &whitespace );
			while( whitespace != '>' )
			{
				char name[30];

				fscanf( file, "%[^=]=", name );
				if( strcmp( name, "id" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.id );
					//printf("id= %i ", newTMXLayer.id);
				}
				if( strcmp( name, "name" ) == 0 )
				{
					fscanf( file, "\"%[^\"]\"", newTMXLayer.name );
					//printf("name= %s ", newTMXLayer.name);
				}
				if( strcmp( name, "width" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.width );
					//printf("width= %i ", newTMXLayer.width);
				}
				if( strcmp( name, "height" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.height );
					//printf("height= %i ", newTMXLayer.height);
				}
				if( strcmp( name, "visible" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.visible );
					//printf("visible= %i ", newTMXLayer.visible);
				}
				if( strcmp( name, "locked" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.locked );
					//printf("locked= %i ", newTMXLayer.locked);
				}
				if( strcmp( name, "offsetx") == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.offsetX );
					//printf("offsetx= %i ", newTMXLayer.offsetX);
				}
				if( strcmp( name, "offsety" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newTMXLayer.offsetY );
					//printf("offsety= %i ", newTMXLayer.offsetY);
				}
				fscanf( file, "%c", &whitespace );
				if( debug )
				{
					printf( "whitespace = %c;", whitespace );
					//getch();
				}
			}
			fscanf( file, "\n" );
			if( debug )
			{
				printf( "\n" );
			}

			newTMXMap.layers.push_back( newTMXLayer );
			if( debug )
			{
				printf( "		id 			= %i \n", newTMXMap.layers.back().id 		);
				printf( "		name		= %s \n", newTMXMap.layers.back().name 		);
				printf( "		Width 		= %i \n", newTMXMap.layers.back().width 	);
				printf( "		Height 		= %i \n", newTMXMap.layers.back().height 	);
				printf( "		visible		= %i \n", newTMXMap.layers.back().visible 	);
				printf( "		locked		= %i \n", newTMXMap.layers.back().locked 	);
				printf( "		offsetX		= %i \n", newTMXMap.layers.back().offsetX 	);
				printf( "		offsetY		= %i \n", newTMXMap.layers.back().offsetY 	);
				getch();
			}

			while( strcmp( XMLTag, "/layer" ) != 0 )
			{

				fscanf( file, "<%[^> ]", XMLTag );
				if( debug )
				{
					printf( "		XMLTag= %s \n", XMLTag );
					getch();
				}

				if( strcmp( XMLTag, "/layer" ) == 0 )
				{
					fscanf( file, ">\n" );
				}

				if( strcmp( XMLTag, "properties" ) == 0 )
				{
					fscanf(file, ">\n");

					while( strcmp( XMLTag, "/properties" ) != 0 )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "			XMLTag= %s \n", XMLTag );
							getch();
						}


						if( strcmp( XMLTag, "property" ) == 0 )
						{
							TMXProperty newProperty;
							fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
							newTMXMap.layers.back().properties.push_back(newProperty);
							if( debug )
							{
								printf( "				name 	= %s \n", newTMXMap.layers.back().properties.back().name 			);
								printf( "				type 	= %s \n", newTMXMap.layers.back().properties.back().type 			);
								printf( "				value 	= %s \n", newTMXMap.layers.back().properties.back().stringValue 	);
								getch();
							}
						}

						if(strcmp( XMLTag, "/properties" ) == 0 )
						{
							fscanf( file, ">\n" );
						}
					}
				}

				if( strcmp( XMLTag, "data" ) == 0 )
				{
					fscanf( file, " encoding=\"%[^\"]\">\n", newTMXLayer.dataEncoding );
					if( debug )
					{
						printf( "			encoding 	= %s \n", newTMXLayer.dataEncoding );
						getch();
					}

					newTMXMap.layers.back().data = ( int* )malloc( newTMXLayer.height * newTMXLayer.width * sizeof( int ) );
					for( unsigned int y = 0; y < newTMXLayer.height; y++ )
					{
						for( unsigned int x = 0; x < newTMXLayer.width; x++ )
						{
							fscanf( file, "%i,", &newTMXMap.layers.back().data[y * newTMXLayer.width + x] );
							//printf( "%i,", newTMXLayer.data[y * newTMXLayer.width + x] );
						}
						fscanf( file, "\n" );
					}

					if( debug )
					{
						for( unsigned int y = 0; y < newTMXLayer.height; y++ )
						{
							for( unsigned int x = 0; x < newTMXLayer.width; x++ )
							{
								printf( "%i,", newTMXMap.layers.back().data[y * newTMXLayer.width + x] );
							}
							printf( "\n" );
						}
					}

					fscanf( file, "</data>\n" );
				}
			}
		}

		if(strcmp( XMLTag, "objectgroup" ) == 0 )
		{
			TMXObjectGroup newObjectGroup;
			//fscanf( file, " id=\"%i\" name=\"%[^\"]\" width=\"%i\" height=\"%i\">\n", &newTMXLayer.id, newTMXLayer.name, &newTMXLayer.width, &newTMXLayer.height );
			
			//dealing with visible, locked and offsets
			newObjectGroup.visible = true;
			newObjectGroup.locked  = false;
			newObjectGroup.offsetX = 0;
			newObjectGroup.offsetY = 0;
			newObjectGroup.opacity = 1;

			char whitespace = ' ';
			fscanf( file, "%c", &whitespace );
			while( whitespace != '>' )
			{
				char name[30];

				fscanf( file, "%[^=]=", name );
				if( strcmp( name, "id" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newObjectGroup.id );
					//printf( "id= %i ", newObjectGroup.id );
				}
				if( strcmp( name, "name" ) == 0 )
				{
					fscanf( file, "\"%[^\"]\"", newObjectGroup.name );
					//printf( "name= %s ", newObjectGroup.name );
				}
				if( strcmp(name, "visible") == 0 )
				{
					fscanf( file, "\"%i\"", &newObjectGroup.visible );
					//printf( "visible= %i ", newObjectGroup.visible );
				}
				if( strcmp( name, "locked" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newObjectGroup.locked );
					//printf( "locked= %i ", newObjectGroup.locked );
				}
				if( strcmp( name, "offsetx" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newObjectGroup.offsetX );
					//printf( "offsetx= %i ", newObjectGroup.offsetX );
				}
				if( strcmp( name, "offsety" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newObjectGroup.offsetY );
					//printf( "offsety= %i ", newObjectGroup.offsetY );
				}
				fscanf( file, "%c", &whitespace );
				if( debug )
				{
					printf( "whitespace = %c;", whitespace );
					//getch();
				}
			}
			fscanf( file, "\n" );
			if( debug )
			{
				printf( "\n" );
			}

			newTMXMap.objectGroups.push_back( newObjectGroup );
			if( debug )
			{
				printf( "		id 			= %i \n", newTMXMap.objectGroups.back().id 		);
				printf( "		name		= %s \n", newTMXMap.objectGroups.back().name 	);
				printf( "		visible		= %i \n", newTMXMap.objectGroups.back().visible );
				printf( "		locked		= %i \n", newTMXMap.objectGroups.back().locked 	);
				printf( "		offsetX		= %i \n", newTMXMap.objectGroups.back().offsetX );
				printf( "		offsetY		= %i \n", newTMXMap.objectGroups.back().offsetY );
				getch();
			}

			while( strcmp( XMLTag, "/objectgroup" ) != 0 )
			{

				fscanf( file, "<%[^> ]", XMLTag );
				if( debug )
				{
					printf( "		XMLTag= %s \n", XMLTag );
					getch();
				}

				if( strcmp( XMLTag, "/objectgroup" ) == 0 )
				{
					fscanf( file, ">\n" );
				}

				if( strcmp( XMLTag, "properties" ) == 0 )
				{
					fscanf( file, ">\n" );

					while( strcmp( XMLTag, "/properties" ) != 0 )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "			XMLTag= %s \n", XMLTag );
							getch();
						}


						if( strcmp( XMLTag, "property" ) == 0 )
						{
							TMXProperty newProperty;
							fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
							newTMXMap.objectGroups.back().properties.push_back( newProperty );
							if( debug )
							{
								printf( "				name 	= %s \n", newTMXMap.objectGroups.back().properties.back().name	 		);
								printf( "				type 	= %s \n", newTMXMap.objectGroups.back().properties.back().type 			);
								printf( "				value 	= %s \n", newTMXMap.objectGroups.back().properties.back().stringValue 	);
								getch();
							}
							
						}

						if( strcmp( XMLTag, "/properties" ) == 0 )
						{
							fscanf( file, ">\n" );
						}
					}
				}

				//read objects
				if( strcmp( XMLTag, "object" ) == 0 )
				{
					TMXObject newObject;
					
					//dealing with visible, locked and offsets
					newObject.visible 	= true;
					newObject.width 	= 0;
					newObject.height 	= 0;
					newObject.rotation 	= 0;
					newObject.gid 		= 0;
					newObject.typeID 	= -1;
					strcpy(newObject.type, "point"); //a rect is also a point. just with aditional with and height


					char whitespace = ' ';
					fscanf( file, "%c", &whitespace );
					bool objectEnd = false; //ugly -.-
					while( whitespace != '>' )
					{
						char name[30];

						fscanf( file, "%[^=]=", name );
						if( strcmp( name, "id" ) == 0 )
						{
							fscanf( file, "\"%i\"", &newObject.id );
						}
						if( strcmp( name, "x" ) == 0 )
						{
							fscanf( file, "\"%f\"", &newObject.pos.x );
						}
						if( strcmp(name, "y" ) == 0 )
						{
							fscanf( file, "\"%f\"", &newObject.pos.y );
						}
						if( strcmp( name, "width" ) == 0 )
						{
							fscanf( file, "\"%i\"", &newObject.width );
						}
						if( strcmp( name, "height" ) == 0 )
						{
							fscanf( file, "\"%i\"", &newObject.height );
						}
						if( strcmp( name, "name" ) == 0 )
						{
							fscanf( file, "\"%[^\"]\"", newObject.name );
						}
						if( strcmp( name, "gid" ) == 0 )
						{
							fscanf( file, "\"%i\"", &newObject.gid );
						}
						if( strcmp( name, "rotation" ) == 0 )
						{
							fscanf( file, "\"%f\"", &newObject.rotation );
						}
						fscanf( file, "%c", &whitespace );
						if( whitespace == '/' )
						{
							fscanf( file, "%c", &whitespace );
							objectEnd = true;
						}
						if( debug )
						{
							printf( "whitespace = %c;", whitespace );
							//getch();
						}
					}
					fscanf( file, "\n" );
					if( debug )
					{
						printf( "\n" );
					}

					newTMXMap.objectGroups.back().objects.push_back( newObject );
					if(debug)
					{
						printf( "		id 			= %i \n", newTMXMap.objectGroups.back().objects.back().id 		);
						printf( "		X			= %f \n", newTMXMap.objectGroups.back().objects.back().pos.x 	);
						printf( "		Y			= %f \n", newTMXMap.objectGroups.back().objects.back().pos.y 	);
						printf( "		name		= %s \n", newTMXMap.objectGroups.back().objects.back().name 	);
						printf( "		visible		= %i \n", newTMXMap.objectGroups.back().objects.back().visible 	);
						printf( "		width		= %i \n", newTMXMap.objectGroups.back().objects.back().width 	);
						printf( "		height		= %i \n", newTMXMap.objectGroups.back().objects.back().height 	);
						getch();
					}

					while( strcmp( XMLTag, "/object" ) != 0 && !objectEnd )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "			XMLTag= %s \n", XMLTag );
							getch();
						}

						if( strcmp( XMLTag, "/object" ) == 0 )
						{
							fscanf( file, ">\n" );
						}

						if( strcmp( XMLTag, "properties" ) == 0 )
						{
							fscanf( file, ">\n" );

							while( strcmp( XMLTag, "/properties" ) != 0 )
							{
								fscanf( file, "<%[^> ]", XMLTag );
								if( debug )
								{
									printf( "				XMLTag= %s \n", XMLTag );
									getch();
								}


								if( strcmp( XMLTag, "property" ) == 0 )
								{
									TMXProperty newProperty;
									fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
									newTMXMap.objectGroups.back().objects.back().properties.push_back( newProperty );
									if( debug )
									{
										printf( "					name 	= %s \n", newTMXMap.objectGroups.back().objects.back().properties.back().name 		);
										printf( "					type 	= %s \n", newTMXMap.objectGroups.back().objects.back().properties.back().type 		);
										printf( "					value 	= %s \n", newTMXMap.objectGroups.back().objects.back().properties.back().stringValue);
										
										//getch();
									}
									if( strcmp( newProperty.name, "typeID" ) == 0 )
									{
										newTMXMap.objectGroups.back().objects.back().typeID = atoi( newProperty.stringValue );
									}
									
								}

								if( strcmp( XMLTag, "/properties" ) == 0 )
								{
									fscanf(file, ">\n");
									//printf("jo");
								}
							}
						}

						if( strcmp( XMLTag, "ellipse/" ) == 0 )
						{
							strcpy( newObject.type, "ellipse" );
							fscanf( file, ">\n" );
						}

						if( strcmp( XMLTag, "point/" ) == 0 )
						{
							strcpy( newObject.type, "point" );
							fscanf( file, ">\n" );
						}


						//text (complicated to parse...)
					}
				}
			}
		}

		if( strcmp( XMLTag, "imagelayer" ) == 0 )
		{
			TMXImageLayer newImageLayer;
			//dealing with visible, locked and offsets
			newImageLayer.visible = true;
			newImageLayer.offsetX = 0;
			newImageLayer.offsetY = 0;
			newImageLayer.opacity = 1;



			char whitespace = ' ';
			bool objectEnd = false; //ugly -.-
			fscanf( file, "%c", &whitespace );
			while( whitespace != '>' )
			{
				char name[30];

				fscanf( file, "%[^=]=", name );
				if(strcmp( name, "id") == 0 )
				{
					fscanf( file, "\"%i\"", &newImageLayer.id );
					//printf( "id= %i ", newObjectGroup.id );
				}
				if( strcmp( name, "name" ) == 0 )
				{
					fscanf( file, "\"%[^\"]\"", newImageLayer.name );
					//printf( "name= %s ", newObjectGroup.name );
				}
				if( strcmp( name, "visible" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newImageLayer.visible );
					//printf( "visible= %i ", newObjectGroup.visible );
				}
				if( strcmp( name, "offsetx" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newImageLayer.offsetX );
					//printf( "offsetx= %i ", newObjectGroup.offsetX );
				}
				if( strcmp( name, "offsety" ) == 0 )
				{
					fscanf( file, "\"%i\"", &newImageLayer.offsetY );
					//printf( "offsety= %i ", newObjectGroup.offsetY );
				}
				if( strcmp( name, "opacity" ) == 0 )
				{
					fscanf( file, "\"%f\"", &newImageLayer.opacity );
					//printf( "offsety= %i ", newObjectGroup.offsetY );
				}
				fscanf( file, "%c", &whitespace );
				if( whitespace == '/' )
				{
					fscanf( file, "%c", &whitespace );
					objectEnd = true;
				}
				if( debug )
				{
					printf( "whitespace = %c;", whitespace );
					//getch();
				}
			}
			fscanf( file, "\n" );
			if( debug )
			{
				printf( "\n" );
			}

			newTMXMap.imageLayers.push_back( newImageLayer );
			if(debug)
			{
				printf( "		id 			= %i \n", newTMXMap.imageLayers.back().id 		);
				printf( "		name		= %s \n", newTMXMap.imageLayers.back().name 	);
				printf( "		visible		= %i \n", newTMXMap.imageLayers.back().visible 	);
				printf( "		offsetX		= %i \n", newTMXMap.imageLayers.back().offsetX 	);
				printf( "		offsetY		= %i \n", newTMXMap.imageLayers.back().offsetY 	);
				printf( "		opacity		= %f \n", newTMXMap.imageLayers.back().opacity 	);
				getch();
			}

			while( strcmp(XMLTag, "/imagelayer" ) != 0 && !objectEnd )
			{
				fscanf( file, "<%[^> ]", XMLTag );
				if( debug )
				{
					printf( "		XMLTag= %s \n", XMLTag );
					getch();
				}

				if( strcmp( XMLTag, "/imagelayer" ) == 0 )
				{
					fscanf(file, ">\n");
				}

				if( strcmp( XMLTag, "properties" ) == 0 )
				{
					fscanf( file, ">\n" );

					while( strcmp( XMLTag, "/properties" ) != 0 )
					{
						fscanf( file, "<%[^> ]", XMLTag );
						if( debug )
						{
							printf( "			XMLTag= %s \n", XMLTag );
							getch();
						}


						if( strcmp( XMLTag, "property" ) == 0 )
						{
							TMXProperty newProperty;
							fscanf( file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue );
							newTMXMap.imageLayers.back().properties.push_back(newProperty);
							if(debug)
							{
								printf( "				name 	= %s \n", newTMXMap.imageLayers.back().properties.back().name 		);
								printf( "				type 	= %s \n", newTMXMap.imageLayers.back().properties.back().type 		);
								printf( "				value 	= %s \n", newTMXMap.imageLayers.back().properties.back().stringValue);
								getch();
							}
							
						}

						if( strcmp( XMLTag, "/properties" ) == 0 )
						{
							fscanf( file, ">\n" );
						}
					}
				}

				if( strcmp( XMLTag, "image" ) == 0 )
				{
					TMXImage newImage;

					//char format[30];
					//char trans[30];
				
					fscanf( file, " source=\"%[^\"]\" width=\"%i\" height=\"%i\"/>\n", newImage.source, &newImage.width, &newImage.height );
				 	newTMXMap.imageLayers.back().image = newImage;
				 	if( debug )
					{
						printf( "				source 	= %s \n", newTMXMap.imageLayers.back().image.source 	);
						printf( "				width 	= %i \n", newTMXMap.imageLayers.back().image.width 		);
						printf( "				height 	= %i \n", newTMXMap.imageLayers.back().image.height 	);
						getch();
					}
				 	
				}
			}
		}		
	}
	fscanf( file, ">\n" );

	if( debug )
	{
		printf( "done reading tmx file!\n" );
		getch();
	}

	return newTMXMap;
}
int GameEngine::GetFirstGid( TMXMap* in, int tileSetID )
{
	return in->tileSets[tileSetID].firstGID;
	//
}
int GameEngine::GetTypeID( TMXMap* in, int mapValue, int TMXTileSetIndex )
{
	mapValue = mapValue - GetFirstGid( in, TMXTileSetIndex );

	for( unsigned int u = 0; u < in->tileSets[TMXTileSetIndex].tiles.size(); u++ )
	{
		if( in->tileSets[TMXTileSetIndex].tiles[u].id == mapValue )
		{
			return in->tileSets[TMXTileSetIndex].tiles[u].typeID;
		}
	}

	return -1;
}
int GameEngine::GetTMXTileSetIndex( TMXMap* in, int mapValue )
{
	//gets the TileSetIndex in the TMX File

	//printf( " tilesets %i \n ", in->tileSets.size() );
	for(int i = (int)in->tileSets.size()-1; i >= 0; i-- )
	{
		//printf( "index= %i, gid=%i checking for=%i \n ", i, in->tileSets[i].firstGID, mapValue - in->tileSets[i].firstGID );
		//getch();
		if( mapValue - in->tileSets[i].firstGID >= 0 )
		{
			//printf( "found at %i checked for %i \n", in->tileSets[i].firstGID, mapValue - in->tileSets[i].firstGID  );
			return i;
		}
	}

	return -1;
}
int GameEngine::GetTileSetID( TMXMap* in, int TMXTileSetIndex )
{
	return in->tileSets[TMXTileSetIndex].tileSetID;
	//
}
int GameEngine::GetTileID( TMXMap* in, int mapValue, int TMXTileSetIndex )
{
	return mapValue - GetFirstGid(in, TMXTileSetIndex);
	//
}
void GameEngine::CreateObjectsFromMap( TMXMap* in )
{
	/*unsigned int width, height;
	unsigned int tileWidth, tileHeight;
	vector<TMXTileSet> tileSets;
	vector<TMXLayer> layers;
	vector<TMXObjectGroup> objectGroups;*/

	for( unsigned int i = 0; i < in->layers.size(); i++ )
	{
		for( unsigned int y = 0; y < in->layers[i].height; y++ )
		{
			for( unsigned int x = 0; x < in->layers[i].width; x++ )
			{
				int mapValue = in->layers[i].data[y * in->layers[i].width + x];
				//printf( "mapValue = %i\n", mapValue );
				if( mapValue > 0 )
				{
					GameObject* newObject = NULL;

					
					
					//int tileSetID 	= GetTileSetID( in, mapValue );
					int tileSetIndexInTMX  	= GetTMXTileSetIndex( in, mapValue );
					int tileSetID 			= GetTileSetID( in, tileSetIndexInTMX );
					int typeID 				= GetTypeID( in, mapValue, tileSetIndexInTMX );
					int tileID 				= GetTileID( in, mapValue, tileSetIndexInTMX );
					int tileHeight 			= in->tileSets[tileSetIndexInTMX].tileHeight;
					int tileWidth 			= in->tileSets[tileSetIndexInTMX].tileWidth;
					
					Vector2D newPos;
					newPos.x 	= ( x * tileWidth ) + in->layers[i].offsetX;
					newPos.y 	= ( y * tileHeight ) + in->layers[i].offsetY;
					//newPos 		= newPos + offset;	//additional offset from paramteer list

					//printf("O %i:%i:%i mapValue=%i tileSetTMX=%i TMXSource=%s tileSetID=%i tileID=%i typeID=%i pos=%f:%f\n", i, y, x, mapValue, tileSetIndexInTMX, in->tileSets[tileSetIndexInTMX].source, tileSetID, tileID, typeID, newPos.x, newPos.y);
					

					if( typeID == TYPE_PLAYER ) //placeholder
					{
						//printf("create!\n");
						newObject = new Player( this );

					}
					else if( typeID == TYPE_SOLID ) //placeholder
					{
						//printf("create!\n");
						newObject = new Solid( this );

					} else if( typeID == TYPE_SOLID_TOP ) //placeholder
					{
						//printf("create!\n");
						newObject = new SolidTop( this );

					} else if( typeID == TYPE_BACK_GROUND ) //placeholder
					{
						//printf("create!\n");
						newObject = new BackGround( this );
						
					}
					else if( typeID == TYPE_BACK_GROUND_ANIMATION ) //placeholder
					{
						//printf("create!\n");
						newObject = new BackGroundAnimation( this );
						
					}
					else
					{
						//printf("create!\n");
						if(tileSetID != -1)
						{
							newObject = new GameObject( this );
					
						}
					}
					if( newObject != NULL )
					{
						newObject->SetTypeID	( typeID );
						newObject->SetPos		( newPos );
						newObject->SetDimensions( tileWidth, tileHeight );
						newObject->SetTileSetID	( tileSetID );
						newObject->SetTileIndex	( tileID );
						newObject->SetDrawOrder	( i );

						AddObject( newObject );
					}
					//getch();
				}
			}
		}
	}

	for( unsigned int i = 0; i < in->objectGroups.size(); i++ )
	{
		for( unsigned int u = 0; u < in->objectGroups[i].objects.size(); u++ )
		{
			TMXObject* object;
			object = &in->objectGroups[i].objects[u];

			if( object->typeID != -1 )
			{
				GameObject* newObject = NULL;

				Vector2D newPos = object->pos;
				newPos.x 	= newPos.x + in->layers[i].offsetX;
				newPos.y 	= newPos.y + in->layers[i].offsetY;
				//newPos 		= newPos + offset;	//additional offset from paramteer list
						
				if( object->typeID == TYPE_PLAYER ) //placeholder
				{
					//printf("create!\n");
					newObject = new Player( this );
					newObject->SetDimensions( 16, 24 ); 	//hadcoded
					newObject->SetTileSetID	( 10 );			//hardcoded
					newObject->SetTileIndex	( 0 );			//hardcoded
					newObject->SetDrawOrder	( 2 );			//hardcoded

				}

				if( newObject != NULL )
				{
					newObject->SetTypeID	( object->typeID ); 		//get from property
					newObject->SetPos		( newPos );					//get from pos
					

					AddObject( newObject );
				}
			}
		}
	}
}
void GameEngine::CreateObjectsFromMap( TMXMap* in, Vector2D offset )
{
	/*unsigned int width, height;
	unsigned int tileWidth, tileHeight;
	vector<TMXTileSet> tileSets;
	vector<TMXLayer> layers;
	vector<TMXObjectGroup> objectGroups;*/

	for( unsigned int i = 0; i < in->layers.size(); i++ )
	{
		for( unsigned int y = 0; y < in->layers[i].height; y++ )
		{
			for( unsigned int x = 0; x < in->layers[i].width; x++ )
			{
				int mapValue = in->layers[i].data[y * in->layers[i].width + x];
				//printf( "mapValue = %i\n", mapValue );
				if( mapValue > 0 )
				{
					GameObject* newObject = NULL;

					
					
					//int tileSetID 	= GetTileSetID( in, mapValue );
					int tileSetIndexInTMX  	= GetTMXTileSetIndex( in, mapValue );
					int tileSetID 			= GetTileSetID( in, tileSetIndexInTMX );
					int typeID 				= GetTypeID( in, mapValue, tileSetIndexInTMX );
					int tileID 				= GetTileID( in, mapValue, tileSetIndexInTMX );
					int tileHeight 			= in->tileSets[tileSetIndexInTMX].tileHeight;
					int tileWidth 			= in->tileSets[tileSetIndexInTMX].tileWidth;
					
					Vector2D newPos;
					newPos.x 	= ( x * tileWidth ) + in->layers[i].offsetX;
					newPos.y 	= ( y * tileHeight ) + in->layers[i].offsetY;
					newPos 		= newPos + offset;	//additional offset from paramteer list

					//printf("O %i:%i:%i mapValue=%i tileSetTMX=%i TMXSource=%s tileSetID=%i tileID=%i typeID=%i pos=%f:%f\n", i, y, x, mapValue, tileSetIndexInTMX, in->tileSets[tileSetIndexInTMX].source, tileSetID, tileID, typeID, newPos.x, newPos.y);
					

					if( typeID == TYPE_PLAYER ) //placeholder
					{
						//printf("create!\n");
						newObject = new Player( this );

					}
					else if( typeID == TYPE_SOLID ) //placeholder
					{
						//printf("create!\n");
						newObject = new Solid( this );

					} else if( typeID == TYPE_SOLID_TOP ) //placeholder
					{
						//printf("create!\n");
						newObject = new SolidTop( this );

					} else if( typeID == TYPE_BACK_GROUND ) //placeholder
					{
						//printf("create!\n");
						newObject = new BackGround( this );
						
					}
					else
					{
						//printf("create!\n");
						if(tileSetID != -1)
						{
							newObject = new GameObject( this );
					
						}
					}
					if( newObject != NULL )
					{
						newObject->SetTypeID	( typeID );
						newObject->SetPos		( newPos );
						newObject->SetDimensions( tileWidth, tileHeight );
						newObject->SetTileSetID	( tileSetID );
						newObject->SetTileIndex	( tileID );
						newObject->SetDrawOrder	( i );

						AddObject( newObject );
					}
					//getch();
				}
			}
		}
	}

	for( unsigned int i = 0; i < in->objectGroups.size(); i++ )
	{
		for( unsigned int u = 0; u < in->objectGroups[i].objects.size(); u++ )
		{
			TMXObject* object;
			object = &in->objectGroups[i].objects[u];

			if( object->typeID != -1 )
			{
				GameObject* newObject = NULL;

				Vector2D newPos = object->pos;
				newPos.x 	= newPos.x + in->layers[i].offsetX;
				newPos.y 	= newPos.y + in->layers[i].offsetY;
				newPos 		= newPos + offset;	//additional offset from paramteer list
						
				if( object->typeID == TYPE_PLAYER ) //placeholder
				{
					//printf("create!\n");
					newObject = new Player( this );
					newObject->SetDimensions( 16, 24 ); 	//hadcoded
					newObject->SetTileSetID	( 10 );			//hardcoded
					newObject->SetTileIndex	( 0 );			//hardcoded
					newObject->SetDrawOrder	( 2 );			//hardcoded

				}

				if( newObject != NULL )
				{
					newObject->SetTypeID	( object->typeID ); 		//get from property
					newObject->SetPos		( newPos );					//get from pos
					

					AddObject( newObject );
				}
			}
		}
	}
}
//==========================================================

//================ Misc =======================
void GameEngine::Quit(const char* message)
{
	//attempt to get back to text mode
	graphics->BackToTextMode();

	//Print Message
	printf( "Error: %s \n", message );
	printf( "Terminating Program" );

	//exit Program
	exit(EXIT_FAILURE);
}
//=============================================

//======================= Random ===========================
unsigned char GameEngine::GetRandom()
{
	unsigned char number = randomTable[randomIndex];
	randomIndex = number;
	return number;
}
//==========================================================



//======================= Load/Save ========================
//==========================================================


//=================== FindingObjects =======================
vector<GameObject*> GameEngine::GetAllObjects()
{
	return objects;
	//
}
vector<GameObject*> GameEngine::GetAllObjects( int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( objects[i]->GetTypeID() == typeID )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsAtPos( Vector2D pos )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x >= objects[i]->GetPos(). x && pos.y >= objects[i]->GetPos().y 
			&& 	pos.x < objects[i]->GetPos().x  + objects[i]->GetWidth() && pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsAtPos( Vector2D pos, int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x >= objects[i]->GetPos(). x && pos.y >= objects[i]->GetPos().y && pos.x < objects[i]->GetPos().x  + objects[i]->GetWidth() 
			&& 	pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() && objects[i]->GetTypeID() == typeID)
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsInArea( Vector2D pos, int width, int height)
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x + width > objects[i]->GetPos().x && pos.x < objects[i]->GetPos().x + objects[i]->GetWidth()
			&& 	pos.y + height > objects[i]->GetPos().y && pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsInArea( Vector2D pos, int width, int height, int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x + width > objects[i]->GetPos().x && pos.x < objects[i]->GetPos().x + objects[i]->GetWidth()
			&& 	pos.y + height > objects[i]->GetPos().y && pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() 
			&& 	objects[i]->GetTypeID() == typeID )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
GameObject* GameEngine::GetFirstObjectAtPos( Vector2D pos )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x >= objects[i]->GetPos(). x && pos.y >= objects[i]->GetPos().y 
			&& 	pos.x < objects[i]->GetPos().x  + objects[i]->GetWidth() && pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() )
		{
			return objects[i];
		}
	}
	
	return NULL;
}
GameObject* GameEngine::GetFirstObjectAtPos( Vector2D pos, int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		if( 	pos.x >= objects[i]->GetPos(). x && pos.y >= objects[i]->GetPos().y && pos.x < objects[i]->GetPos().x  + objects[i]->GetWidth() 
			&& 	pos.y < objects[i]->GetPos().y + objects[i]->GetHeight() && objects[i]->GetTypeID() == typeID)
		{
			return objects[i];
		}
	}
	
	return NULL;
}
vector<GameObject*> GameEngine::GetObjectsInRadius( Vector2D pos, int radius )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		Vector2D centerPos = objects[i]->GetPos();
		centerPos.x = centerPos.x + ( objects[i]->GetWidth() / 2 );
		centerPos.y = centerPos.y + ( objects[i]->GetHeight() / 2 );
		if( pos.DistanceFrom( objects[i]->GetPos() ) <= radius )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsInRadius( Vector2D pos, int radius, int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		Vector2D centerPos = objects[i]->GetPos();
		centerPos.x = centerPos.x + ( objects[i]->GetWidth() / 2 );
		centerPos.y = centerPos.y + ( objects[i]->GetHeight() / 2 );
		if( pos.DistanceFrom( objects[i]->GetPos() ) <= radius && objects[i]->GetTypeID() == typeID )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsAlongRay( Vector2D origin, Vector2D direction )
{
	/*	Doesnt Work... */

	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		Vector2D testPos = objects[i]->GetPos() - origin;

		//4 Rays
		int slope1A = testPos.x / testPos.y;
		int slope1B = (testPos.x + objects[i]->GetWidth() ) / ( testPos.y + objects[i]->GetHeight() );

		int slope2A = testPos.x / ( testPos.y + objects[i]->GetHeight() );
		int slope2B = ( testPos.x + objects[i]->GetHeight() ) /  testPos.y;

		//own Ray
		direction = direction - origin;
		int ownSlope = direction.x / direction.y;

		if( ( ownSlope > slope1A && ownSlope < slope2B ) || ( ownSlope > slope2A && ownSlope < slope2B ) )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
vector<GameObject*> GameEngine::GetObjectsAlongRay( Vector2D origin, Vector2D direction, int typeID )
{
	vector<GameObject*> outObjects;
	for( unsigned int i = 0; i < objects.size(); i++ )
	{
		Vector2D testPos = objects[i]->GetPos() - origin;

		//4 Rays
		int slope1A = testPos.x / testPos.y;
		int slope1B = (testPos.x + objects[i]->GetWidth() ) / ( testPos.y + objects[i]->GetHeight() );

		int slope2A = testPos.x / ( testPos.y + objects[i]->GetHeight() );
		int slope2B = ( testPos.x + objects[i]->GetHeight() ) /  testPos.y;

		//own Ray
		direction = direction - origin;
		int ownSlope = direction.x / direction.y;

		if( ( ( ownSlope > slope1A && ownSlope < slope2B ) || ( ownSlope > slope2A && ownSlope < slope2B ) ) && objects[i]->GetTypeID() == typeID )
		{
			outObjects.push_back( objects[i] );
		}
	}
	
	return outObjects;
}
GameObject* GameEngine::GetFirstObjectAlongRay( Vector2D origin, Vector2D direction )
{

}
GameObject* GameEngine::GetFirstObjectAlongRay( Vector2D origin, Vector2D direction, int typeID )
{

}
//==========================================================


//==================== math trasitions =====================
float GameEngine::Linear()
{

}
float GameEngine::Clip( float in, float min, float max )
{
	if( in >= max )
	{
		return max;
	}
	if( in <= min )
	{
		return min;
	}

	return in;
}
//==========================================================