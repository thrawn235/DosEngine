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
Map* GameEngine::LoadTMXMap(const char* filePath)
{
	TMXMap newTMXMap;
	bool debug = true;

	char XMLTag[30];

	//open File, read only (txt mode)
	FILE* file = fopen(filePath, "r");

	//read XML Data
	fscanf(file, "<?xml version=\"%f\" encoding=\"%[^\"]\"?>\n", &newTMXMap.XMLVersion, newTMXMap.encoding);
	if(debug)
	{
		printf("xml version= %f \n", newTMXMap.XMLVersion);
		printf("xml encoding= %s \n", newTMXMap.encoding);
		getch();
	}

	//read map data:
	fscanf(file, "<%[^> ]", &XMLTag);
	if(debug)
		printf("XMLTag= %s \n", XMLTag);

	fscanf(file, " version=\"%f\" tiledversion=\"%[^\"]\" orientation=\"%[^\"]\" renderorder=\"%[^\"]\" width=\"%i\" height=\"%i\" tilewidth=\"%i\" tileheight=\"%i\" infinite=\"%i\" nextlayerid=\"%i\" nextobjectid=\"%i\">\n", 
	       &newTMXMap.version, newTMXMap.tiledVersion, newTMXMap.orientation, newTMXMap.renderOrder, &newTMXMap.width, &newTMXMap.height, &newTMXMap.tileWidth, &newTMXMap.tileHeight, &newTMXMap.infinite, &newTMXMap.nextLayerID, &newTMXMap.nextObjectID);

	if(debug)
	{
		printf("version 	= %f \n", newTMXMap.version);
		printf("tiledVersion= %s \n", newTMXMap.tiledVersion);
		printf("orientation = %s \n", newTMXMap.orientation);
		printf("renderOrder = %s \n", newTMXMap.renderOrder);
		printf("width 		= %i \n", newTMXMap.width);
		printf("height 		= %i \n", newTMXMap.height);
		printf("tileWidth 	= %i \n", newTMXMap.tileWidth);
		printf("tileheight 	= %i \n", newTMXMap.tileHeight);
		printf("infinite 	= %i \n", newTMXMap.infinite);
		printf("nextLayerID = %i \n", newTMXMap.nextLayerID);
		printf("nextObjectID= %i \n", newTMXMap.nextObjectID);
		getch();
	}

	while(strcmp(XMLTag, "/map") != 0)
	{
		fscanf(file, "<%[^> ]", &XMLTag);
		if(debug)
			printf("	XMLTag= %s \n", XMLTag);


		if(strcmp(XMLTag, "properties") == 0)
		{
			fscanf(file, ">\n");

			while(strcmp(XMLTag, "/properties") != 0)
			{
				fscanf(file, "<%[^> ]", &XMLTag);
				if(debug)
				{
					printf("		XMLTag= %s \n", XMLTag);
					getch();
				}


				if(strcmp(XMLTag, "property") == 0)
				{
					TMXProperty newProperty;
					fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
					if(debug)
					{
						printf("			name 	= %s \n", newProperty.name);
						printf("			type 	= %s \n", newProperty.type);
						printf("			value 	= %s \n", newProperty.stringValue);
						getch();
					}
					newTMXMap.properties.push_back(newProperty);
				}

				if(strcmp(XMLTag, "/properties") == 0)
				{
					fscanf(file, ">\n");
					//printf("jo");
				}
			}
		}
		if(strcmp(XMLTag, "tileset") == 0)
		{
			TMXTileSet newTileSet;
			fscanf(file, " firstgid=\"%i\" name=\"%[^\"]\" tilewidth=\"%i\" tileheight=\"%i\" tilecount=\"%i\" columns=\"%i\">\n", &newTileSet.firstGID, newTileSet.name, &newTileSet.tileHeight, &newTileSet.tileWidth, &newTileSet.tileCount, &newTileSet.columns);
			fscanf(file, "<image source=\"%[^\"]\" width=\"%i\" height=\"%i\"/>\n", &newTileSet.source, &newTileSet.sourceHeight, &newTileSet.sourceWidth);
			newTMXMap.tileSets.push_back(newTileSet);

			if(debug)
			{
				printf("		firtGD 			= %i \n", newTileSet.firstGID);
				printf("		name 			= %s \n", newTileSet.name);
				printf("		tileWidth 		= %i \n", newTileSet.tileHeight);
				printf("		tileHeight 		= %i \n", newTileSet.tileWidth);
				printf("		tileCount 		= %i \n", newTileSet.tileCount);
				printf("		columns 		= %i \n", newTileSet.columns);
				printf("		source 			= %s \n", newTileSet.source);
				printf("		sourceWidth 	= %i \n", newTileSet.sourceWidth);
				printf("		sourceHeight 	= %i \n", newTileSet.sourceHeight);
				getch();
			}

			while(strcmp(XMLTag, "/tileset") != 0)
			{
				fscanf(file, "<%[^> ]", &XMLTag);
				if(debug)
				{
					printf("		XMLTag= %s \n", XMLTag);
					//getch();
				}

				if(strcmp(XMLTag, "/tileset") == 0)
				{
					fscanf(file, ">\n");
				}

				if(strcmp(XMLTag, "tile") == 0)
				{
					TMXTile newTile;
					fscanf(file, " id=\"%i\">\n", &newTile.id);
					newTMXMap.tileSets.back().tiles.push_back(newTile);
					if(debug)
					{
						printf("			tileID= %i \n", newTile.id);
						//getch();
					}

					while(strcmp(XMLTag, "/tile") != 0)
					{
						fscanf(file, "<%[^> ]", &XMLTag);
						if(debug)
						{
							printf("			XMLTag= %s \n", XMLTag);
							//getch();
						}

						if(strcmp(XMLTag, "/tile") == 0)
						{
							fscanf(file, ">\n");
						}

						if(strcmp(XMLTag, "properties") == 0)
						{
							fscanf(file, ">\n");

							while(strcmp(XMLTag, "/properties") != 0)
							{
								fscanf(file, "<%[^> ]", &XMLTag);
								if(debug)
								{
									printf("				XMLTag= %s \n", XMLTag);
									//getch();
								}


								if(strcmp(XMLTag, "property") == 0)
								{
									TMXProperty newProperty;
									fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
									if(debug)
									{
										printf("					name 	= %s \n", newProperty.name);
										printf("					type 	= %s \n", newProperty.type);
										printf("					value 	= %s \n", newProperty.stringValue);
										//getch();
									}
									newTMXMap.tileSets.back().tiles.back().properties.push_back(newProperty);
								}

								if(strcmp(XMLTag, "/properties") == 0)
								{
									fscanf(file, ">\n");
									//printf("jo");
								}
							}
						}
					}
				}



				if(strcmp(XMLTag, "properties") == 0)
				{
					fscanf(file, ">\n");

					while(strcmp(XMLTag, "/properties") != 0)
					{
						fscanf(file, "<%[^> ]", &XMLTag);
						if(debug)
						{
							printf("		XMLTag= %s \n", XMLTag);
							getch();
						}


						if(strcmp(XMLTag, "property") == 0)
						{
							TMXProperty newProperty;
							fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
							if(debug)
							{
								printf("			name 	= %s \n", newProperty.name);
								printf("			type 	= %s \n", newProperty.type);
								printf("			value 	= %s \n", newProperty.stringValue);
								getch();
							}
							newTMXMap.tileSets.back().properties.push_back(newProperty);
						}

						if(strcmp(XMLTag, "/properties") == 0)
						{
							fscanf(file, ">\n");
							//printf("jo");
						}
					}
				}
			}
		}

		if(strcmp(XMLTag, "layer") == 0)
		{
			TMXLayer newTMXLayer;
			//fscanf(file, " id=\"%i\" name=\"%[^\"]\" width=\"%i\" height=\"%i\">\n", &newTMXLayer.id, newTMXLayer.name, &newTMXLayer.width, &newTMXLayer.height);
			
			//dealing with visible, locked and offsets
			newTMXLayer.visible = true;
			newTMXLayer.locked = false;
			newTMXLayer.offsetX = 0;
			newTMXLayer.offsetY = 0;
			char whitespace = ' ';
			fscanf(file, "%c", &whitespace);
			while(whitespace != '>')
			{
				char name[30];

				fscanf(file, "%[^=]=", name);
				if(strcmp(name, "id") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.id);
					//printf("id= %i ", newTMXLayer.id);
				}
				if(strcmp(name, "name") == 0)
				{
					fscanf(file, "\"%[^\"]\"", newTMXLayer.name);
					//printf("name= %s ", newTMXLayer.name);
				}
				if(strcmp(name, "width") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.width);
					//printf("width= %i ", newTMXLayer.width);
				}
				if(strcmp(name, "height") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.height);
					//printf("height= %i ", newTMXLayer.height);
				}
				if(strcmp(name, "visible") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.visible);
					//printf("visible= %i ", newTMXLayer.visible);
				}
				if(strcmp(name, "locked") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.locked);
					//printf("locked= %i ", newTMXLayer.locked);
				}
				if(strcmp(name, "offsetx") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.offsetX);
					//printf("offsetx= %i ", newTMXLayer.offsetX);
				}
				if(strcmp(name, "offsety") == 0)
				{
					fscanf(file, "\"%i\"", &newTMXLayer.offsetY);
					//printf("offsety= %i ", newTMXLayer.offsetY);
				}
				fscanf(file, "%c", &whitespace);
				if(debug)
				{
					printf("whitespace = %c;", whitespace);
					//getch();
				}
			}
			fscanf(file, "\n");
			if(debug)
			{
				printf("\n");
			}

			newTMXMap.layers.push_back(newTMXLayer);
			if(debug)
			{
				printf("		id 			= %i \n", newTMXLayer.id);
				printf("		name		= %s \n", newTMXLayer.name);
				printf("		Width 		= %i \n", newTMXLayer.width);
				printf("		Height 		= %i \n", newTMXLayer.height);
				printf("		visible		= %i \n", newTMXLayer.visible);
				printf("		locked		= %i \n", newTMXLayer.locked);
				printf("		offsetX		= %i \n", newTMXLayer.offsetX);
				printf("		offsetY		= %i \n", newTMXLayer.offsetY);
				getch();
			}

			while(strcmp(XMLTag, "/layer") != 0)
			{

				fscanf(file, "<%[^> ]", &XMLTag);
				if(debug)
				{
					printf("		XMLTag= %s \n", XMLTag);
					getch();
				}

				if(strcmp(XMLTag, "/layer") == 0)
				{
					fscanf(file, ">\n");
				}

				if(strcmp(XMLTag, "properties") == 0)
				{
					fscanf(file, ">\n");

					while(strcmp(XMLTag, "/properties") != 0)
					{
						fscanf(file, "<%[^> ]", &XMLTag);
						if(debug)
						{
							printf("			XMLTag= %s \n", XMLTag);
							getch();
						}


						if(strcmp(XMLTag, "property") == 0)
						{
							TMXProperty newProperty;
							fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
							if(debug)
							{
								printf("				name 	= %s \n", newProperty.name);
								printf("				type 	= %s \n", newProperty.type);
								printf("				value 	= %s \n", newProperty.stringValue);
								getch();
							}
							newTMXMap.layers.back().properties.push_back(newProperty);
						}

						if(strcmp(XMLTag, "/properties") == 0)
						{
							fscanf(file, ">\n");
							//printf("jo");
						}
					}
				}

				if(strcmp(XMLTag, "data") == 0)
				{
					fscanf(file, " encoding=\"%[^\"]\">\n", newTMXLayer.dataEncoding);
					if(debug)
					{
						printf("			encoding 	= %s \n", newTMXLayer.dataEncoding);
						getch();
					}

					newTMXLayer.data = (int*)malloc(newTMXLayer.height * newTMXLayer.width * sizeof(int));
					for(int y = 0; y < newTMXLayer.height; y++)
					{
						for(int x = 0; x < newTMXLayer.width; x++)
						{
							fscanf(file, "%i,", &newTMXLayer.data[y * newTMXLayer.width + x]);
							printf("%i,", newTMXLayer.data[y * newTMXLayer.width + x]);
						}
						fscanf(file, "\n");
					}

					if(debug)
					{
						for(int y = 0; y < newTMXLayer.height; y++)
						{
							for(int x = 0; x < newTMXLayer.width; x++)
							{
								printf("%i,", newTMXLayer.data[y * newTMXLayer.width + x]);
							}
							printf("\n");
						}
					}

					fscanf(file, "</data>\n");
				}
			}
		}

		if(strcmp(XMLTag, "objectgroup") == 0)
		{
			TMXObjectGroup newObjectGroup;
			//fscanf(file, " id=\"%i\" name=\"%[^\"]\" width=\"%i\" height=\"%i\">\n", &newTMXLayer.id, newTMXLayer.name, &newTMXLayer.width, &newTMXLayer.height);
			
			//dealing with visible, locked and offsets
			newObjectGroup.visible = true;
			newObjectGroup.locked = false;
			newObjectGroup.offsetX = 0;
			newObjectGroup.offsetY = 0;
			newObjectGroup.opacity = 1;
			char whitespace = ' ';
			fscanf(file, "%c", &whitespace);
			while(whitespace != '>')
			{
				char name[30];

				fscanf(file, "%[^=]=", name);
				if(strcmp(name, "id") == 0)
				{
					fscanf(file, "\"%i\"", &newObjectGroup.id);
					//printf("id= %i ", newObjectGroup.id);
				}
				if(strcmp(name, "name") == 0)
				{
					fscanf(file, "\"%[^\"]\"", newObjectGroup.name);
					//printf("name= %s ", newObjectGroup.name);
				}
				if(strcmp(name, "visible") == 0)
				{
					fscanf(file, "\"%i\"", &newObjectGroup.visible);
					//printf("visible= %i ", newObjectGroup.visible);
				}
				if(strcmp(name, "locked") == 0)
				{
					fscanf(file, "\"%i\"", &newObjectGroup.locked);
					//printf("locked= %i ", newObjectGroup.locked);
				}
				if(strcmp(name, "offsetx") == 0)
				{
					fscanf(file, "\"%i\"", &newObjectGroup.offsetX);
					//printf("offsetx= %i ", newObjectGroup.offsetX);
				}
				if(strcmp(name, "offsety") == 0)
				{
					fscanf(file, "\"%i\"", &newObjectGroup.offsetY);
					//printf("offsety= %i ", newObjectGroup.offsetY);
				}
				fscanf(file, "%c", &whitespace);
				if(debug)
				{
					printf("whitespace = %c;", whitespace);
					//getch();
				}
			}
			fscanf(file, "\n");
			if(debug)
			{
				printf("\n");
			}

			newTMXMap.objectGroups.push_back(newObjectGroup);
			if(debug)
			{
				printf("		id 			= %i \n", newObjectGroup.id);
				printf("		name		= %s \n", newObjectGroup.name);
				printf("		visible		= %i \n", newObjectGroup.visible);
				printf("		locked		= %i \n", newObjectGroup.locked);
				printf("		offsetX		= %i \n", newObjectGroup.offsetX);
				printf("		offsetY		= %i \n", newObjectGroup.offsetY);
				getch();
			}

			while(strcmp(XMLTag, "/objectgroup") != 0)
			{

				fscanf(file, "<%[^> ]", &XMLTag);
				if(debug)
				{
					printf("		XMLTag= %s \n", XMLTag);
					getch();
				}

				if(strcmp(XMLTag, "/objectgroup") == 0)
				{
					fscanf(file, ">\n");
				}

				if(strcmp(XMLTag, "properties") == 0)
				{
					fscanf(file, ">\n");

					while(strcmp(XMLTag, "/properties") != 0)
					{
						fscanf(file, "<%[^> ]", &XMLTag);
						if(debug)
						{
							printf("			XMLTag= %s \n", XMLTag);
							getch();
						}


						if(strcmp(XMLTag, "property") == 0)
						{
							TMXProperty newProperty;
							fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
							if(debug)
							{
								printf("				name 	= %s \n", newProperty.name);
								printf("				type 	= %s \n", newProperty.type);
								printf("				value 	= %s \n", newProperty.stringValue);
								getch();
							}
							newTMXMap.objectGroups.back().properties.push_back(newProperty);
						}

						if(strcmp(XMLTag, "/properties") == 0)
						{
							fscanf(file, ">\n");
							//printf("jo");
						}
					}
				}

				//read objects
				if(strcmp(XMLTag, "object") == 0)
				{
					TMXObject newObject;
					
					//dealing with visible, locked and offsets
					newObject.visible = true;
					newObject.width = 0;
					newObject.height = 0;
					newObject.rotation = 0;
					newObject.gid = 0;
					strcpy(newObject.type, "point"); //a rect is also a point. just with aditional with and height


					char whitespace = ' ';
					fscanf(file, "%c", &whitespace);
					bool objectEnd = false; //ugly -.-
					while(whitespace != '>')
					{
						char name[30];

						fscanf(file, "%[^=]=", name);
						if(strcmp(name, "id") == 0)
						{
							fscanf(file, "\"%i\"", &newObject.id);
						}
						if(strcmp(name, "x") == 0)
						{
							fscanf(file, "\"%f\"", &newObject.pos.x);
						}
						if(strcmp(name, "y") == 0)
						{
							fscanf(file, "\"%f\"", &newObject.pos.y);
						}
						if(strcmp(name, "width") == 0)
						{
							fscanf(file, "\"%i\"", &newObject.width);
						}
						if(strcmp(name, "height") == 0)
						{
							fscanf(file, "\"%i\"", &newObject.height);
						}
						if(strcmp(name, "name") == 0)
						{
							fscanf(file, "\"%[^\"]\"", newObject.name);
						}
						if(strcmp(name, "gid") == 0)
						{
							fscanf(file, "\"%i\"", &newObject.gid);
						}
						if(strcmp(name, "rotation") == 0)
						{
							fscanf(file, "\"%f\"", &newObject.rotation);
						}
						fscanf(file, "%c", &whitespace);
						if(whitespace == '/')
						{
							fscanf(file, "%c", &whitespace);
							objectEnd = true;
						}
						if(debug)
						{
							printf("whitespace = %c;", whitespace);
							//getch();
						}
					}
					fscanf(file, "\n");
					if(debug)
					{
						printf("\n");
					}

					newTMXMap.objectGroups.back().objects.push_back(newObject);
					if(debug)
					{
						printf("		id 			= %i \n", newObject.id);
						printf("		X			= %f \n", newObject.pos.x);
						printf("		Y			= %f \n", newObject.pos.y);
						printf("		name		= %s \n", newObject.name);
						printf("		visible		= %i \n", newObject.visible);
						printf("		width		= %i \n", newObject.width);
						printf("		height		= %i \n", newObject.height);
						getch();
					}

					while(strcmp(XMLTag, "/object") != 0 && !objectEnd)
					{
						fscanf(file, "<%[^> ]", &XMLTag);
						if(debug)
						{
							printf("			XMLTag= %s \n", XMLTag);
							getch();
						}

						if(strcmp(XMLTag, "/object") == 0)
						{
							fscanf(file, ">\n");
						}

						if(strcmp(XMLTag, "properties") == 0)
						{
							fscanf(file, ">\n");

							while(strcmp(XMLTag, "/properties") != 0)
							{
								fscanf(file, "<%[^> ]", &XMLTag);
								if(debug)
								{
									printf("				XMLTag= %s \n", XMLTag);
									getch();
								}


								if(strcmp(XMLTag, "property") == 0)
								{
									TMXProperty newProperty;
									fscanf(file, " name=\"%[^\"]\" type=\"%[^\"]\" value=\"%[^\"]\"/>\n", newProperty.name, newProperty.type, newProperty.stringValue);
									if(debug)
									{
										printf("					name 	= %s \n", newProperty.name);
										printf("					type 	= %s \n", newProperty.type);
										printf("					value 	= %s \n", newProperty.stringValue);
										getch();
									}
									newTMXMap.objectGroups.back().objects.back().properties.push_back(newProperty);
								}

								if(strcmp(XMLTag, "/properties") == 0)
								{
									fscanf(file, ">\n");
									//printf("jo");
								}
							}
						}

						if(strcmp(XMLTag, "ellipse/") == 0)
						{
							strcpy(newObject.type, "ellipse");
							fscanf(file, ">\n");
						}


						//text (complicated to parse...)
					}
				}
			}
		}
	}
	fscanf(file, ">\n");

	if(debug)
	{
		printf("done reading tmx file!\n");
		getch();
	}

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