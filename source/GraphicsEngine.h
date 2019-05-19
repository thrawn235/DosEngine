///////////////////////////////////////////////////////////////////////////////////////
//
//	GameObject.h
//
//	by Sebastian Gurlin
//
//	Description:
//	All The Drawing Functions for Primitives and Sprites
//	It also saves tilesets and loads bitmap files
//
///////////////////////////////////////////////////////////////////////////////////////


#ifndef GRAPHICSENGINE
#define GRAPHICSENGINE


//================== includes ==========================
//c++ includes:
#include <vector>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

//djgpp includes:
#include <sys/nearptr.h>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>

//own includes:
#include "Vector2D.h"
//=======================================================

using namespace std;

//BMP Structs================================
struct BMPFileHEader
{
	char header[2];
	long sizeInBytes;
	char reserved[4];
	long pixelArrayOffset;
}__attribute__( ( packed ) );

struct BMPDIBHeader
{
	unsigned long  	size;
	long int 		width;
	long int 		height;
	short 			numColorPlanes;
	short 			bitsPerPixel;
	long 			compression;
	long 			bitmapSize;
	long int 		horizontalResolution;
	long int 		verticalResolution;
	long int 		numColors;
	long int 		numImportantColors;
}__attribute__( ( packed ) );

struct BMP
{
	char 			source[30];
	BMPFileHEader 	fileHeader;
	BMPDIBHeader	DIBHeader;
	char 			extraBitMasks[16];
	unsigned char* 	colorTable;
	char* 			pixelArray;
};
//============================================


struct Sprite
{
	int 	id;
	int 	width, height;
	char* 	pixelData;
	char 	source[30];
};
struct TileSet
{
	int 			id;
	char 			source[30];
	int 			tileWidth, tileHeight;
	vector<Sprite*>	tiles;
};

struct Animation
{
	int id;
	int tileSetID;
	int firstTileIndex;
	int numSprites;
	int currentFrame;
	int speed;
	int currentSpeedStep;
};



class GraphicsEngine
{
protected:
	//Graphicsmode Infos
	int 				screenWidth, screenHeight;
	int 				logicalScreenWidth, logicalScreenHeight;
	int 				screenPadding;
	int 				bitDepth;
	char* 				screenMemory;
	int 				totalScreenMemory;
	char* 				backBuffer;
	int 				mode;
	int 				oldMode; //mode to go back to

	//double buffering
	bool 				flip;
	char* 				currentBackBuffer;
	char* 				currentScreenMemory;

	//camera
	Vector2D 			camPos;


	//tileset/ sprite and animation data
	vector<TileSet*> 	tileSets;
	vector<Sprite*> 	sprites;

	//fadein/out
	unsigned char savedPalette[256*4];
	unsigned char currentPalette[256*4];
	unsigned int fadeOut;
	unsigned int fadeIn;
public:
	GraphicsEngine 						();
	~GraphicsEngine 					();
	
	//Set/Get
	int 		GetScreenWidth 			();
	int 		GetScreenHeight			();
	int 		GetBitDepth 			();
	Vector2D 	GetCamPos 				();
	void 		SetCamPos 				( Vector2D newPos );
	void 		SetCamCenter 			( Vector2D newPos );
	void 		SetCamPos 				( int newPosX, int newPosY );
	
	//Init
	void 		SetGraphicsMode 		( int newMode );
	int 		GetMode 				();
	
	//BMP
	BMP 		LoadBMP 				( const char* filePath );
	Sprite* 	BMPToSprite 			( BMP* in, int id );
	void 		FreeBMP 				( BMP* in );
	
	//Palette
	void 		SetPaletteColor 		( unsigned char index, unsigned char r, unsigned char g, unsigned char b );
	void 		SetPalette 				( unsigned char* inPalette, int numColors );
	void 		BlackOut 				();
	void 		ChangePaletteBrightness ( int delta );
	void 		ChangePaletteHue 		( int deltaR, int deltaG, int deltaB );
	char* 		GetPalette 				();
	void		SaveCurrentPalette 		();
	void		SavePaletteToBank 		();
	bool		FadeOut					();
	bool		FadeIn 					();
	
	//Graphics Primitives
	void 		DrawPixel 				( Vector2D pos, char color );
	void 		DrawPixel 				( int x, int y, char color );
	void 		DrawLine 				( Vector2D start, Vector2D end, char color );
	void 		DrawHLine 				( Vector2D start, int length, char color );
	void 		DrawVLine 				( Vector2D start, int length, char color );
	void 		DrawRect 				( Vector2D pos, int width, int height, char color );
	void 		DrawFilledRect 			( Vector2D pos, int width, int height, char color );
	void 		DrawCircle 				( Vector2D pos, int radius, char color );
	void 		DrawFilledCircle 		( Vector2D pos, int radius, char color );
	void 		DrawVector 				( Vector2D pos, Vector2D vec, float scale, unsigned char color );
	void 		ClearScreen 			( char color );
	void 		Flip 					(); //Flip or buffer copy
	void 		WaitForRetrace 			();

	
	//Sprite Methods
	void 		DrawSprite 				( Vector2D pos, Sprite* in );
	void 		DrawSprite 				( Vector2D pos, Sprite* in, char transparentColor );
	void 		DrawSprite 				( Vector2D pos, Sprite* in, bool flippedHorizontal, bool flippedVertical );
	void 		DrawSprite 				( Vector2D pos, Sprite* in, char transparentColor, bool flippedHorizontal, bool flippedVertical );
	void 		DrawSprite 				( Vector2D pos, int tileSetID, int tileIndex );
	void 		DrawSprite 				( Vector2D pos, int tileSetID, int tileIndex, char transparentColor );
	void 		DrawSprite 				( Vector2D pos, int tileSetID, int tileIndex, bool flippedHorizontal, bool flippedVertical );
	void 		DrawSprite 				( Vector2D pos, int tileSetID, int tileIndex, char transparentColor, bool flippedHorizontal, bool flippedVertical );
	Sprite* 	CropSprite 				( Sprite* in, int newID, Vector2D pos, int width, int height );
	void 		AddSprite 				( Sprite* newSprite );
	Sprite* 	GetSprite 				( int id );
	void 		FreeSprite 				( Sprite* in );
	
	//tileSets
	TileSet* 	NewEmptyTileSet 		( int newID, int tileWidth, int tileHeight ); //just dont
	TileSet* 	ExtractTileSet 			( int newID, Sprite* in, Vector2D startPos, int tileWidth, int tileHeight, int numTliesHorizontal, int numTilesVertical );
	void 		AddTileSet 				( TileSet* newTileSet );
	TileSet* 	GetTileSet 				( int id );
	TileSet* 	GetTileSetByIndex 		( unsigned int index );
	int 		GetNumTileSets 			();
	Sprite* 	GetTile 				( int tileSetID, int tileIndex );
	void 		AddTile 				( int tlieSetID, Sprite* in ); //just dont
	//void RemoveTile( int id ); //just dont
	void 		DrawTileSet 			( TileSet* in, Vector2D pos, int columnWidth ); // just for debugging
	
	//Animations
	bool 		PlayAnimation 			( Animation* in, Vector2D pos );
	bool 		PlayAnimation 			( Animation* in, Vector2D pos, char transparentColor );
	bool 		PlayAnimation 			( Animation* in, Vector2D pos, bool flippedHorizontal, bool flippedVertical );
	bool 		PlayAnimation 			( Animation* in, Vector2D pos, char transparentColor, bool flippedHorizontal, bool flippedVertical );
	
	bool 		PlayAnimationDelta		( Animation* in, Vector2D pos, float delta );
	bool 		PlayAnimationDelta		( Animation* in, Vector2D pos, char transparentColor, int delta );
	bool 		PlayAnimationDelta		( Animation* in, Vector2D pos, bool flippedHorizontal, bool flippedVertical, int delta );
	bool 		PlayAnimationDelta		( Animation* in, Vector2D pos, char transparentColor, bool flippedHorizontal, bool flippedVertical, int delta );

	//Windows:
	void 		DrawWindowBorder 		( Vector2D pos, int width, int height, int tileSetID, int topLeftCorner, int topRightCorner, int bottomLeftCorner, int bottomRightCorner, int horizontalLine, int verticalLine );
	void 		DrawWindow 				( Vector2D pos, int width, int height, int tileSetID, int topLeftCorner, int topRightCorner, int bottomLeftCorner, int bottomRightCorner, int horizontalLine, int verticalLine, int backGround );

	//Text
	void 		DrawText 				( Vector2D pos, int tileSetID, int firstTileIndex, const char* text );
	void 		DrawText 				( Vector2D pos, int tileSetID, int firstTileIndex, const char* text, int transparentColor );
	
	//Destroy
	void 		DestroyTileSet 			( TileSet *in );
	void 		DestroyTileSets 		();
	void 		DestroySprite 			( Sprite* in );
	void 		DestroySprites 			();
	void		BackToTextMode 			();
	void 		Destroy 				();
};

//Note:
//Engine needs to provide functions for smooth transitions. that can be used everywhere,
//but especially with regards to positions. like camera position. for smooth camera movement

#endif