///////////////////////////////////////////////////////////////////////////////////////
//
//	GraphicsEngine.cpp
//
//	by Sebastian Gurlin
//
//	Description:
//	Memberfunctions of GraphicEngine
//	check GraphicsEngine.h for details
//
///////////////////////////////////////////////////////////////////////////////////////


#include "GraphicsEngine.h"

inline int Clip( int input, int min, int max )
{
	if( input >= max )
	{
		return max;
	}
	else if( input <= min )
	{
		return min;
	}
	else
	{
		return input;
	}
}

GraphicsEngine::GraphicsEngine()
{
	//seems like there is nothing to do
	screenPadding 	= 64;
	//__djgpp_nearptr_enable();
	backBuffer 		= NULL;
	screenMemory 	= NULL;
	flip 			= false;
}
GraphicsEngine::~GraphicsEngine()
{
	Destroy();
	//__djgpp_nearptr_disable(); better call that in engine proper
}

//Set/Get
int GraphicsEngine::GetScreenWidth()
{
	//
	return screenWidth;
}
int GraphicsEngine::GetScreenHeight()
{
	//
	return screenHeight;
}
int GraphicsEngine::GetBitDepth()
{
	//
	return bitDepth;
}
Vector2D GraphicsEngine::GetCamPos()
{
	//
	return camPos;
}
void GraphicsEngine::SetCamPos( Vector2D newPos )
{
	//
	camPos = newPos;
}
void GraphicsEngine::SetCamCenter( Vector2D newPos )
{
	newPos.x = newPos.x - (  screenWidth / 2  );
	newPos.y = newPos.y - (  screenHeight / 2  );
	camPos = newPos;
}
void GraphicsEngine::SetCamPos( int newPosX, int newPosY )
{
	camPos.x = newPosX;
	camPos.y = newPosY;
}

//Init
void GraphicsEngine::SetGraphicsMode( int newMode )
{
	struct VbeInfoBlock
	{
		unsigned char 	VbeSignature[4];
		unsigned short	VbeVersion;
		unsigned long	OemStrPtr;
		unsigned char	Capabilities[4];
		unsigned long	VideoModePtr;
		unsigned short	TotalMemory;
		unsigned short	OemSoftwareRev;
		unsigned long	OemVendorNamePtr;
		unsigned long	OemProductNamePtr;
		unsigned long	OemProductRevPtr;
		unsigned char	Reserved[222];
		unsigned char	OemData[256];
	}__attribute__( ( packed ) );

	long dosBuffer;
	dosBuffer = __tb & 0xFFFFF;

	__dpmi_regs r;
	r.x.ax = 0x4F00;
    r.x.es = ( dosBuffer>>4 ) & 0xFFFF;
    r.x.di = dosBuffer & 0xF;
	__dpmi_int( 0x10, &r );

	VbeInfoBlock vbeInfoBlock;

	dosmemget( dosBuffer, sizeof( VbeInfoBlock ), &vbeInfoBlock );	

	bool debug = false;
	if( debug )
	{	
		long mode_ptr; 
		printf( "VBE Signature: %s\n", vbeInfoBlock.VbeSignature );
		printf( "VBE Version: %hu \n", vbeInfoBlock.VbeVersion );
		mode_ptr = ( ( vbeInfoBlock.OemStrPtr & 0xFFFF0000 ) >> 12 ) + ( vbeInfoBlock.OemStrPtr & 0xFFFF );
		printf( "OEM String: %s \n", ( char* )mode_ptr + __djgpp_conventional_base );
		printf( "Capabilities: %li \n", ( long )vbeInfoBlock.Capabilities );
		printf( "VideoModes Ptr: %p \n", ( void* )vbeInfoBlock.VideoModePtr );
		printf( "total Memory: %hu \n", vbeInfoBlock.TotalMemory );
		printf( "software rev: %hu \n", vbeInfoBlock.OemSoftwareRev );
		mode_ptr = ( ( vbeInfoBlock.OemVendorNamePtr & 0xFFFF0000 ) >> 12 ) + ( vbeInfoBlock.OemVendorNamePtr & 0xFFFF );
		printf( "Vendor Name: %s \n", ( char* )mode_ptr + __djgpp_conventional_base );
		mode_ptr = ( ( vbeInfoBlock.OemProductNamePtr & 0xFFFF0000 ) >> 12 ) + ( vbeInfoBlock.OemProductNamePtr & 0xFFFF );
		printf( "Product Name: %s \n", ( char* )mode_ptr + __djgpp_conventional_base );
		mode_ptr = ( ( vbeInfoBlock.OemProductRevPtr & 0xFFFF0000 ) >> 12 ) + ( vbeInfoBlock.OemProductRevPtr & 0xFFFF );
		printf( "Product Rev: %s \n", ( char* )mode_ptr + __djgpp_conventional_base );
	}

	//Get Old Mode:
	r.x.ax = 0x4F03;
	__dpmi_int( 0x10, &r );
	oldMode = r.x.bx;

	if( debug )
	{
		printf( "oldMode: %x \n", oldMode );
	}


	if( newMode == 0x13 )
	{
		//set mode:
		r.x.ax = 0x13;
      	__dpmi_int( 0x10, &r );
      	screenHeight = 200;
      	screenWidth  = 320;

      	bitDepth = 256;
		//screenMemory = ( char* )0xA0000;
		screenMemory = ( char* )( 0xA0000 + __djgpp_conventional_base );

      	//set logical screen width
		logicalScreenWidth = screenWidth + ( screenPadding*2 );
		logicalScreenHeight = screenHeight + ( screenPadding*2 );

		//init backbuffer:
		if( backBuffer != NULL )
			free( backBuffer );
		
		backBuffer = ( char* )malloc( logicalScreenWidth*logicalScreenHeight );
		
		for( int i = 0; i < logicalScreenWidth*logicalScreenHeight; i++ )
			backBuffer[i] = 0;
	
		currentBackBuffer = backBuffer;
		currentScreenMemory = screenMemory;
	}
	if( 	newMode == 0x100 ||
			newMode == 0x101 ||
			newMode == 0x103 ||
			newMode == 0x105 ||
			newMode == 0x107 )
	{
		struct ModeInfoBlock
		{
			unsigned short	ModeAttributes;
			unsigned char	WinAAttributes;
			unsigned char	WinBAttributes;
			unsigned short	WinGranularity;
			unsigned short	WinSize;
			unsigned short	WinASegment;
			unsigned short	WinBSegment;
			unsigned long	WinFuncPtr;
			unsigned short	BytesPerScanLine;
			unsigned short	XResolution;
			unsigned short	YResolution;
			unsigned char	XCharSize;
			unsigned char	YCharSize;
			unsigned char	NumberOfPlanes;
			unsigned char	BitsPerPixel;
			unsigned char	NumberOfBanks;
			unsigned char	MemoryModel;
			unsigned char	BankSize;
			unsigned char	NumberOfImagesPages;
			unsigned char	Reserved1;
			unsigned char	RedMaskSize;
			unsigned char	RedFieldPosition;
			unsigned char	GreenMaskSize;
			unsigned char	GreenFieldPosition;
			unsigned char	BlueMaskSize;
			unsigned char	BlueFieldPosition;
			unsigned char	RsvdMaskSize;
			unsigned char	RsvdFieldPosition;
			unsigned char 	DirectColorModeInfo;
			unsigned long	PhysBasePtr;
			unsigned long	OffScreenMemOffset;
			unsigned char	Reserved2[206];
		}__attribute__( ( packed ) );

		dosBuffer = __tb & 0xFFFFF;

		r.x.ax = 0x4F01;
		r.x.cx = newMode;
	    r.x.es = ( dosBuffer>>4 ) & 0xFFFF;
	    r.x.di = dosBuffer & 0xF;
		__dpmi_int( 0x10, &r );

		ModeInfoBlock modeInfoBlock;

		dosmemget( dosBuffer, sizeof( ModeInfoBlock ), &modeInfoBlock );

		if( debug )
		{
			printf( "ModeAttributes: %u \n", 	modeInfoBlock.ModeAttributes 		);
			printf( "WinSize: %u \n", 			modeInfoBlock.WinSize 				);
			printf( "WinFuncPtr: %p \n", 		( void* )modeInfoBlock.WinFuncPtr 	);
			printf( "BytesPerScanline: %u \n", 	modeInfoBlock.BytesPerScanLine 		);
			printf( "XResolution: %hu \n", 		modeInfoBlock.XResolution 			);
			printf( "YResolution: %hu \n", 		modeInfoBlock.YResolution 			);
			printf( "XCharSize: %hu \n", 		modeInfoBlock.XCharSize 			);
			printf( "YCharSize: %hu \n", 		modeInfoBlock.YCharSize 			);
			printf( "NumberOfPlanes: %hu \n", 	modeInfoBlock.NumberOfPlanes 		);
			printf( "BitsPerPixel: %hu \n", 	modeInfoBlock.BitsPerPixel 			);
			printf( "NumberOfBanks: %hu \n", 	modeInfoBlock.NumberOfBanks 		);
			printf( "MemoryModel: %hu \n", 		modeInfoBlock.MemoryModel 			);
			printf( "BankSize: %hu \n", 		modeInfoBlock.BankSize 				);
			printf( "PhysBasePtr: %p \n", 		( void* )modeInfoBlock.PhysBasePtr 	);
			printf( "ModeAttributes: %u \n", 	modeInfoBlock.ModeAttributes 		);
			getch();
		}

		//save mode infos:
		screenWidth 	= modeInfoBlock.XResolution;
		screenHeight 	= modeInfoBlock.YResolution;
		bitDepth  		= modeInfoBlock.BitsPerPixel;
		//screenMemory = ( char* )modeInfoBlock.PhysBasePtr;
		__dpmi_meminfo mapping;
		mapping.address 	= modeInfoBlock.PhysBasePtr;
		mapping.size 		= vbeInfoBlock.TotalMemory << 16;
		__dpmi_physical_address_mapping( &mapping );
		screenMemory 		= ( char* )modeInfoBlock.PhysBasePtr + __djgpp_conventional_base;

		
		//set logical screen width
		logicalScreenWidth = screenWidth + ( screenPadding*2 );
		logicalScreenHeight = screenHeight + ( screenPadding*2 );

		backBuffer = screenMemory+( logicalScreenWidth*logicalScreenHeight );

		if( debug )
		{
			printf( "ScreenMemoryAddress: 	%p \n", 		( void* )screenMemory 	);
			printf( "screenWidth: 			%i \n", 				 screenWidth 	);
			printf( "screenHeight: 			%i \n", 		         screenHeight 	);
			printf( "screenPadding:		 	%i \n", 		         screenPadding 	);
			printf( "logicalWidth: 			%i \n", 		logicalScreenWidth 	);
			printf( "logicalHeight: 		%i \n", 		logicalScreenHeight 	);
			printf( "lWidth*lHeight: 		%d \n", 		logicalScreenWidth*logicalScreenHeight	);
			printf( "backBufferAddress: 	%p \n", 		( void* )backBuffer 	);
			getch();	
		}

		//set mode!
		r.x.ax = 0x4F02;
	    r.x.bx = newMode | 0b0100000000000000;
		__dpmi_int( 0x10, &r );
		
		

		r.x.ax = 0x4F06;
		r.h.bl = 00;
		r.x.cx = logicalScreenWidth;
		__dpmi_int( 0x10, &r );

		//setup for Flip	
		flip = true;
		currentScreenMemory = screenMemory;
		currentBackBuffer = backBuffer;

		//set display window to proper position
		r.x.ax = 0x4F07;
		r.h.bh = 0;
		r.h.bl = 0;
	    r.x.cx = screenPadding;
	    r.x.dx = screenPadding;
		__dpmi_int( 0x10, &r );

	}
	else
	{
		//invalid mode
		return;
	}	
}
int GraphicsEngine::GetMode()
{
	//
	return mode;
}

//BMP
BMP GraphicsEngine::LoadBMP( const char* filePath )
{
	FILE *file = fopen( filePath, "rb" );
	if( file == NULL )
	{
		printf( "Error Loading File!\n"  );
	}
	else
	{
		//file succesfully opend...

		BMP bmp;
		strcpy( bmp.source,filePath );
		//reading BMP header
		fread( &bmp.fileHeader, sizeof( bmp.fileHeader ), 1, file );

		bool debug = false;
		if( debug )
		{
			printf( "%c%c\n", bmp.fileHeader.header[0], bmp.fileHeader.header[1] );
		}
		if( bmp.fileHeader.header[0] != 'B' || bmp.fileHeader.header[1] != 'M' )
		{
			printf( "not a valid bmp file!" );
		}
		else
		{
			//valid bmp...
			if( debug )
			{
				printf( "filesize= %li\n", bmp.fileHeader.sizeInBytes );
				printf( "pixelArrayOffset= %li\n", bmp.fileHeader.pixelArrayOffset );
			}

			//read DIBHeader
			fread( &bmp.DIBHeader, sizeof( bmp.DIBHeader ), 1, file );
			if( debug )
			{
				printf( "sizeOfDIBHeader= %lu\n", bmp.DIBHeader.size );
				printf( "BitmapWidth= %li\n", bmp.DIBHeader.width );
				printf( "BitmapHeight= %li\n", bmp.DIBHeader.height );
				printf( "NumColorPlanes= %i\n", bmp.DIBHeader.numColorPlanes );
				printf( "bitsPerPixel= %i\n", bmp.DIBHeader.bitsPerPixel );
				printf( "compression= %lu\n", bmp.DIBHeader.compression );
				printf( "RawImageSize= %lu\n", bmp.DIBHeader.bitmapSize );
				printf( "horizontalResolution= %li\n", bmp.DIBHeader.horizontalResolution );
				printf( "verticalResolution= %li\n", bmp.DIBHeader.verticalResolution );
				printf( "numColors= %lu\n", bmp.DIBHeader.numColors );
				printf( "numImportantColors= %lu\n", bmp.DIBHeader.numImportantColors );
				getch();
			}

			if( bmp.DIBHeader.size == 40 )
			{
				int reservedNumBytes = 40 - sizeof( bmp.DIBHeader );
				char extraBitMasks[reservedNumBytes];
				fread( extraBitMasks, reservedNumBytes, 1, file );
				if( debug )
				{
					printf( "reservedNumBytes= %i\n", reservedNumBytes );
				}
			}

			//read Color Table:
			bmp.colorTable = ( char* )malloc( bmp.DIBHeader.numColors * 4 );
			fread( bmp.colorTable, bmp.DIBHeader.numColors * 4, 1, file );

			if( debug )
			{
				int index = 0;
				for( int i = 0; i < bmp.DIBHeader.numColors; i++ )
				{
					printf( "R=%hhx; G=%hhx; B=%hhx; A=%hhx \n", bmp.colorTable[index+0], bmp.colorTable[index+1], bmp.colorTable[index+2], bmp.colorTable[index+3] );
					index = index + 4;
				}
			}

			//read pixelArray:
			bmp.pixelArray = ( char* )malloc( bmp.DIBHeader.bitmapSize );
			//position file pointer correctly
			fseek( file, bmp.fileHeader.pixelArrayOffset, SEEK_SET );
			fread( bmp.pixelArray, bmp.DIBHeader.bitmapSize, 1, file );

			if( debug )
			{
				for( int y = 0; y < bmp.DIBHeader.height; y++ )
				{
					for( int x = 0; x < bmp.DIBHeader.width; x++ )
					{
						printf( "%hhu;", bmp.pixelArray[y * bmp.DIBHeader.width + x] );
						
					}
					printf( "\n" );
					getch();
				}
				getch();
			}
		}
		fclose( file );
		return bmp;
	}
}
Sprite* GraphicsEngine::BMPToSprite( BMP* in, int id )
{
	Sprite* newSprite = new Sprite;
	newSprite->id = id;
	newSprite->width = in->DIBHeader.width;
	newSprite->height = in->DIBHeader.height;
	newSprite->pixelData = ( char* )malloc( in->DIBHeader.bitmapSize );
	strcpy( newSprite->source, in->source );
	
	//bmps are stored upside down!!
	//going to correct it for sprites
	//rather slow, but thats ok in that case
	int destY = in->DIBHeader.height;
	for( int y = 0; y < in->DIBHeader.height; y++ )
	{
		destY--;
		for( int x = 0; x < in->DIBHeader.width; x++ )
		{
			newSprite->pixelData[destY * newSprite->width + x] = in->pixelArray[y * in->DIBHeader.width + x];
		}
	}

	return newSprite;
}
void GraphicsEngine::FreeBMP( BMP* in )
{
	free( in->pixelArray );
	free( in->colorTable );
}

//Palette
void GraphicsEngine::SetPaletteColor( unsigned char index, unsigned char r, unsigned char g, unsigned char b )
{
	outportb( 0x03c8, index );
	outportb( 0x03c9, r );
	outportb( 0x03c9, g );
	outportb( 0x03c9, b );
}
void GraphicsEngine::SetPalette( char* palette, unsigned char numColors )
{
	outportb( 0x03c8, 0 );
	int index = 0;
	for( int i = 0; i < numColors; i++ )
	{
		outportb( 0x03c9, palette[index + 2] );
		outportb( 0x03c9, palette[index + 1] );
		outportb( 0x03c9, palette[index + 0] );
		index = index + 4;
	}
}
void GraphicsEngine::ChangePaletteBrightness( int delta )
{
	char* palette = GetPalette();

	outportb( 0x03c8, 0 );
	int index = 0;
	for( int i = 0; i < 256; i++ )
	{
		outportb( 0x03c9, Clip( palette[index + 2] + delta, 0, 255 ) );
		outportb( 0x03c9, Clip( palette[index + 1] + delta, 0, 255 ) );
		outportb( 0x03c9, Clip( palette[index + 0] + delta, 0, 255 ) );
		index = index + 4;
	}
	free( palette );
}
void GraphicsEngine::ChangePaletteHue( int deltaR, int deltaG, int deltaB )
{

}
char* GraphicsEngine::GetPalette()
{
	char* palette = ( char* )malloc( 256*4 );

	int index = 0;
	for( int i = 0; i < 256; i++ )
	{
		outportb( 0x03c7, i );
		palette[index + 2] = inportb( 0x03c9 );
		palette[index + 1] = inportb( 0x03c9 );
		palette[index + 0] = inportb( 0x03c9 );
		index = index +4;
	}

	return palette;
}

//Graphics Primitives
void GraphicsEngine::DrawPixel( Vector2D pos, char color )
{
	pos = pos + screenPadding;
	pos = pos - camPos;
	//and assembly
	if( pos.x > 0 && pos.y > 0 && pos.x < logicalScreenWidth && pos.y < logicalScreenHeight )
	{
		currentBackBuffer[( int )( ( int )pos.y * logicalScreenWidth+ ( int )pos.x )] = color;
	}
}
void GraphicsEngine::DrawPixel( int x, int y, char color )
{
	x = x + screenPadding;
	y = y + screenPadding;
	x = x - camPos.x;
	y = y - camPos.y;
	//and assembly
	if( x > 0 && y > 0 && x < logicalScreenWidth && y < logicalScreenHeight )
	{
		currentBackBuffer[y * logicalScreenWidth + x] = color;
	}
}
void GraphicsEngine::DrawLine( Vector2D start, Vector2D end, char color )
{
	//not by me. method is from brakeen
	//no boundary checks
	start = start + screenPadding;
	end = end + screenPadding;
	start = start - camPos;
	end = end - camPos;

	if( 	start.x > 0 && start.y > 0 && start.x < logicalScreenWidth && start.y < logicalScreenHeight &&
		end.x > 0 && end.y > 0 && end.x < logicalScreenWidth && end.y < logicalScreenHeight )
	{
		#define sgn( x ) ( ( x<0 )?-1:( ( x>0 )?1:0 ) )

		

		int x1 = start.x;
		int y1 = start.y;
		int x2 = end.x;
		int y2 = end.y;

		int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

		dx=x2-x1;      /* the horizontal distance of the line */
		dy=y2-y1;      /* the vertical distance of the line */
		dxabs=abs( dx );
		dyabs=abs( dy );
		sdx=sgn( dx );
		sdy=sgn( dy );
		x=dyabs>>1;
		y=dxabs>>1;
		px=x1;
		py=y1;

		//VGA[( py<<8 )+( py<<6 )+px]=color;
		currentBackBuffer[py * logicalScreenWidth + px] = color;

		if ( dxabs>=dyabs ) /* the line is more horizontal than vertical */
		{
			for( i=0;i<dxabs;i++ )
			{
				y+=dyabs;
				if ( y>=dxabs )
				{
					y-=dxabs;
					py+=sdy;
				}
				px+=sdx;
				//plot_pixel( px,py,color );
				currentBackBuffer[py * logicalScreenWidth + px] = color;
			}
		}
		else /* the line is more vertical than horizontal */
		{
			for( i=0;i<dyabs;i++ )
			{
				x+=dxabs;
				if ( x>=dyabs )
				{
					x-=dyabs;
					px+=sdx;
				}
				py+=sdy;
				//plot_pixel( px,py,color );
				currentBackBuffer[py * logicalScreenWidth + px] = color;
			}
		}
	}
}
void GraphicsEngine::DrawHLine( Vector2D start, int length, char color )
{
	//assembly and long pointers could speed it up. probably not worth it though
	start = start + screenPadding;
	start = start - camPos;
	
	if( 	start.x > 0 && start.y > 0 && start.x < logicalScreenWidth && start.y < logicalScreenHeight &&
		start.x + length > 0 && start.x + length < logicalScreenWidth )
	{
		
		int startAddress = ( int )start.y * logicalScreenWidth + ( int )start.x;
		for( int i = 0; i < length; i++ )
		{
			currentBackBuffer[startAddress+i] = color;
		}
	}
}
void GraphicsEngine::DrawVLine( Vector2D start, int length, char color )
{
	start = start + screenPadding;
	start = start - camPos;

	if( 	start.x > 0 && start.y > 0 && start.x < logicalScreenWidth && start.y < logicalScreenHeight &&
		start.y + length > 0 && start.y + length < logicalScreenHeight )
	{
		
		int startAddress = ( int )start.y * logicalScreenWidth + ( int )start.x;
		for( int i = 0; i < length; i++ )
		{
			currentBackBuffer[startAddress] = color;
			startAddress = startAddress + logicalScreenWidth;
		}
	}
}
void GraphicsEngine::DrawRect( Vector2D pos, int width, int height, char color )
{
	//Boundary Checks are done in Line Methods
	DrawVLine( pos, height+1, color );
	DrawVLine( Vector2D( pos.x+width, pos.y ), height+1, color );
	DrawHLine( pos, width, color );
	DrawHLine( Vector2D( pos.x, pos.y+height ), width, color );
}
void GraphicsEngine::DrawFilledRect( Vector2D pos, int width, int height, char color )
{
	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x < logicalScreenWidth && pos.y < logicalScreenHeight )
	{
		if( pos.x+width > 0 && pos.y+height > 0 && pos.x+width < logicalScreenWidth && pos.y+height < logicalScreenHeight )
		{
			if( width % 4 == 0 )
			{
				int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );
				asm( "mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"mov $0, %%ebx;"
					"loop1%=:;"
					"	mov $0, %%ecx;"
					"	loop2%=:;"
					"		mov %%eax, ( %%edi, %%ecx );"
					"		add $4, %%ecx;"
					"		cmp %2, %%ecx;"
					"		jb loop2%=;"
					"	add %1, %%edi;"
					"	inc %%ebx;"
					"	cmp %3, %%ebx;"
					"	jb loop1%=;"
					:
					:"D"( startAddress ), "m"( logicalScreenWidth ), "m"( width ), "m"( height )
					:"eax", "ebx", "ecx", "memory" ); 
			}
			else
			{
				int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );
				int widthMinus4 = width - 4;
				asm( "mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"shl $8, %%eax;"
					"mov %2, %%al;"
					"mov $0, %%ebx;"
					"loop1%=:;"
					"	mov $0, %%ecx;"
					"	loop2%=:;"
					"		mov %%eax, ( %%edi, %%ecx );"
					"		add $4, %%ecx;"
					"		cmp %2, %%ecx;"
					"		jb loop2%=;"
					"	loop3%=:;"
					"		movb %%al, ( %%edi, %%ecx );"
					"		inc %%ecx;"
					"		cmp %4, %%ecx;"
					"		jb loop3%=;"
					"	add %1, %%edi;"
					"	inc %%ebx;"
					"	cmp %3, %%ebx;"
					"	jb loop1%=;"
					:
					:"D"( startAddress ), "m"( logicalScreenWidth ), "m"( widthMinus4 ), "m"( height ), "m"( width )
					:"eax", "ebx", "ecx", "memory" ); 
			}
		}
	}
}
void GraphicsEngine::DrawCircle( Vector2D pos, int radius, char color )
{
	//incredibly dumb and horrendously slow!

	pos = pos + screenPadding;
	pos = pos - camPos;

	Vector2D startCoord( pos.x - radius, pos.y - radius );
	if( 	startCoord.x > 0 && startCoord.y > 0 && startCoord.x < logicalScreenWidth && startCoord.y < logicalScreenHeight &&
		startCoord.x + ( 2*radius ) > 0 && startCoord.y + ( 2*radius ) > 0 && startCoord.x + ( 2*radius ) < logicalScreenWidth && startCoord.y + ( 2*radius ) < logicalScreenHeight )
	{
		for( int x = startCoord.x; x <= startCoord.x + ( 2*radius ); x++ )
		{
			for( int y = startCoord.y; y <= startCoord.y + ( 2*radius ); y++ )
			{
				if( ( int )pos.DistanceFrom( Vector2D( x,y ) ) == radius )
				{
					currentBackBuffer[y * logicalScreenWidth + x] = color;
				}
			}
		} 
	}
}
void GraphicsEngine::DrawFilledCircle( Vector2D pos, int radius, char color )
{
	//incredibly dumb and horrendously slow!

	pos = pos + screenPadding;
	pos = pos - camPos;

	Vector2D startCoord( pos.x - radius, pos.y - radius );
	if( 	startCoord.x > 0 && startCoord.y > 0 && startCoord.x < logicalScreenWidth && startCoord.y < logicalScreenHeight &&
		startCoord.x + ( 2*radius ) > 0 && startCoord.y + ( 2*radius ) > 0 && startCoord.x + ( 2*radius ) < logicalScreenWidth && startCoord.y + ( 2*radius ) < logicalScreenHeight )
	{
		for( int x = startCoord.x; x <= startCoord.x + ( 2*radius ); x++ )
		{
			for( int y = startCoord.y; y <= startCoord.y + ( 2*radius ); y++ )
			{
				if( ( int )pos.DistanceFrom( Vector2D( x,y ) ) <= radius )
				{
					currentBackBuffer[y * logicalScreenWidth + x] = color;
				}
			}
		} 
	}
}
void GraphicsEngine::DrawVector( Vector2D pos, Vector2D vec, float scale, unsigned char color )
{
	//Draw Dot at origin
	DrawPixel( pos.x-1, pos.y-1, color );
	DrawPixel( pos.x, pos.y-1, color );
	DrawPixel( pos.x+1, pos.y-1, color );
	DrawPixel( pos.x-1, pos.y, color );
	DrawPixel( pos.x, pos.y, color );
	DrawPixel( pos.x+1, pos.y, color );
	DrawPixel( pos.x-1, pos.y+1, color );
	DrawPixel( pos.x, pos.y+1, color );
	DrawPixel( pos.x+1, pos.y+1, color );

	//Draw Line for Direction
	DrawLine( pos, ( vec * scale )+pos, color );
}
void GraphicsEngine::ClearScreen( char color )
{
	int startAddress = ( int )currentBackBuffer + ( screenPadding * logicalScreenWidth + screenPadding );
	int endAddress = startAddress + ( screenHeight * logicalScreenWidth );

	asm( "mov %2, %%al;"
		"shl $8, %%eax;"
		"mov %2, %%al;"
		"shl $8, %%eax;"
		"mov %2, %%al;"
		"shl $8, %%eax;"
		"mov %2, %%al;"
		"loop%=:;"
		"	mov %%eax, ( %%edi );"
		"	add $4, %%edi;"
		"	cmp %1, %%edi;"
		"	jb loop%=;"
		:
		:"D"( startAddress ), "m"( endAddress ), "m"( color )
		:"eax", "memory" );
}
void GraphicsEngine::Flip() //Flip or buffer copy
{

	WaitForRetrace();

	if( flip )
	{
		__dpmi_regs r;
		if( currentScreenMemory == screenMemory )
		{
			currentScreenMemory = backBuffer;
			currentBackBuffer = screenMemory;


			r.x.ax = 0x4F07;
			r.h.bh = 0;
			r.h.bl = 0;
		    r.x.cx = screenPadding;
		    r.x.dx = logicalScreenHeight + screenPadding;
			__dpmi_int( 0x10, &r );
		}
		else
		{
			currentScreenMemory = screenMemory;
			currentBackBuffer = backBuffer;


			r.x.ax = 0x4F07;
			r.h.bh = 0;
			r.h.bl = 0;
		    r.x.cx = screenPadding;
		    r.x.dx = screenPadding;
			__dpmi_int( 0x10, &r );
		}
	}
	else
	{
		long sourceAddress = ( long )&backBuffer[0] + ( screenPadding * logicalScreenWidth + screenPadding );

		//optimized asm:
		asm( "mov %3, %%ebx;"
			"loop1%=:;"
			"	mov %2, %%ecx;"
			"	loop2%=:;"
			"		sub $4, %%ecx;"
			"		mov ( %%esi, %%ecx ), %%eax;"
			"		mov %%eax, ( %%edi, %%ecx );"
			"		ja loop2%=;"
			"	add %2, %%edi;"
			"	add %4, %%esi;"
			"	dec %%ebx;"
			"	ja loop1%=;"
			:
			:"S"( sourceAddress ), "D"( &screenMemory[0] ), "m"( screenWidth ), "m"( screenHeight ), "m"( logicalScreenWidth )
			:"eax", "ebx", "ecx", "memory" );
	}
}
void GraphicsEngine::WaitForRetrace()
{
    /* wait until done with vertical retrace */
    while  ( ( inportb( 0x03da ) & 0x08 ) != 8 ) {};
    /* wait until done refreshing */
    while ( ( inportb( 0x03da ) & 0x08 ) == 8 ) {};
}

//Sprite Methods
void GraphicsEngine::DrawSprite( Vector2D pos, Sprite* in )
{
	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		asm( "mov %4, %%ebx;"
			"loop1%=:;"	
			"	mov %2, %%ecx;"
			"	loop2%=:;"
			"		sub $4, %%ecx;"
			"		mov ( %%esi, %%ecx ), %%eax;"
			"		mov %%eax, ( %%edi, %%ecx );"
			"		ja loop2%=;"
			"	add %2, %%esi;"
			"	add %3, %%edi;"
			"	dec %%ebx;"
			"	ja loop1%=;"
			:
			:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
			:"eax", "ebx", "ecx", "memory" );
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, Sprite* in, char transparentColor )
{
	//last pixel is not allright

	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		//if transparent, slower because we have to go pixel by pixel
		asm( "mov %4, %%ebx;"
			"loop1%=:;"
			"	mov $0, %%ecx;"
			"	loop2%=:;"
			"		mov ( %%esi, %%ecx ), %%al;"
			"		cmp %5, %%al;"
			"		je notDraw%=;"
			"		mov %%al, ( %%edi, %%ecx );"
			"		notDraw%=:;"
			"		inc %%ecx;"
			"		cmp %2, %%ecx;"
			"		jbe loop2%=;"
			"	add %3, %%edi;"
			"	add %2, %%esi;"
			"	dec %%ebx;"
			"	ja loop1%=;"
			:
			:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
			:"eax", "ebx", "ecx", "memory" );
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, Sprite* in, bool flippedHorizontal, bool flippedVertical )
{
	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		if( !flippedVertical && !flippedHorizontal )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	mov %2, %%ecx;"
				"	loop2%=:;"
				"		sub $4, %%ecx;"
				"		mov ( %%esi, %%ecx ), %%eax;"
				"		mov %%eax, ( %%edi, %%ecx );"
				"		ja loop2%=;"
				"	add %2, %%esi;"
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && !flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov %2, %%ecx;"
				"	loop2%=:;"
				"		sub $4, %%ecx;"
				"		mov ( %%esi, %%ecx ), %%eax;"
				"		mov %%eax, ( %%edi, %%ecx );"
				"		ja loop2%=;"
				
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedHorizontal && !flippedVertical )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"	
				"	mov $0, %%ecx;"
				"	push %%ebx;"
				"	mov %2, %%ebx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	pop %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov $0, %%ecx;"
				"	push %%ebx;"
				"	mov %2, %%ebx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"	
				"	pop %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, Sprite* in, char transparentColor, bool flippedHorizontal, bool flippedVertical )
{
	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		//if transparent, slower because we have to go pixel by pixel
		if( !flippedVertical && !flippedHorizontal )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ecx );"
				"		notDraw%=:;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && !flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ecx );"
				"		notDraw%=:;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedHorizontal && !flippedVertical )
		{
			int temp = 0;
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				
				"	mov %%ebx, %6;"
				"	mov %2, %%ebx;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		notDraw%=:;"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	mov %6, %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor ), "m"( temp )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			int temp = 0;
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov %%ebx, %6;"
				"	mov %2, %%ebx;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		notDraw%=:;"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				
				"	add %3, %%edi;"
				"	mov %6, %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor ), "m"( temp )
				:"eax", "ebx", "ecx", "memory" );
		}
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, int tileSetID, int tileIndex )
{
	Sprite* in = GetTile( tileSetID, tileIndex );

	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		asm( "mov %4, %%ebx;"
			"loop1%=:;"	
			"	mov %2, %%ecx;"
			"	loop2%=:;"
			"		sub $4, %%ecx;"
			"		mov ( %%esi, %%ecx ), %%eax;"
			"		mov %%eax, ( %%edi, %%ecx );"
			"		ja loop2%=;"
			"	add %2, %%esi;"
			"	add %3, %%edi;"
			"	dec %%ebx;"
			"	ja loop1%=;"
			:
			:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
			:"eax", "ebx", "ecx", "memory" );
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, int tileSetID, int tileIndex, char transparentColor )
{
	Sprite* in = GetTile( tileSetID, tileIndex );

	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		//if transparent, slower because we have to go pixel by pixel
		asm( "mov %4, %%ebx;"
			"loop1%=:;"
			"	mov $0, %%ecx;"
			"	loop2%=:;"
			"		mov ( %%esi, %%ecx ), %%al;"
			"		cmp %5, %%al;"
			"		je notDraw%=;"
			"		mov %%al, ( %%edi, %%ecx );"
			"		notDraw%=:;"
			"		inc %%ecx;"
			"		cmp %2, %%ecx;"
			"		jb loop2%=;"
			"	add %3, %%edi;"
			"	add %2, %%esi;"
			"	dec %%ebx;"
			"	ja loop1%=;"
			:
			:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
			:"eax", "ebx", "ecx", "memory" );
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, int tileSetID, int tileIndex, bool flippedHorizontal, bool flippedVertical )
{
	Sprite* in = GetTile( tileSetID, tileIndex );

	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		if( !flippedVertical && !flippedHorizontal )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	mov %2, %%ecx;"
				"	loop2%=:;"
				"		sub $4, %%ecx;"
				"		mov ( %%esi, %%ecx ), %%eax;"
				"		mov %%eax, ( %%edi, %%ecx );"
				"		ja loop2%=;"
				"	add %2, %%esi;"
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && !flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov %2, %%ecx;"
				"	loop2%=:;"
				"		sub $4, %%ecx;"
				"		mov ( %%esi, %%ecx ), %%eax;"
				"		mov %%eax, ( %%edi, %%ecx );"
				"		ja loop2%=;"
				
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedHorizontal && !flippedVertical )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"	
				"	mov $0, %%ecx;"
				"	push %%ebx;"
				"	mov %2, %%ebx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	pop %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov $0, %%ecx;"
				"	push %%ebx;"
				"	mov %2, %%ebx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jbe loop2%=;"
				"	add %3, %%edi;"	
				"	pop %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height )
				:"eax", "ebx", "ecx", "memory" );
		}
	}
}
void GraphicsEngine::DrawSprite( Vector2D pos, int tileSetID, int tileIndex, char transparentColor, bool flippedHorizontal, bool flippedVertical )
{
	Sprite* in = GetTile( tileSetID, tileIndex );

	pos = pos + screenPadding;
	pos = pos - camPos;

	if( pos.x > 0 && pos.y > 0 && pos.x+in->width < logicalScreenWidth && pos.y + in->height < logicalScreenHeight )
	{
		int startAddress = ( int )currentBackBuffer + ( ( int )pos.y * logicalScreenWidth + ( int )pos.x );

		//if transparent, slower because we have to go pixel by pixel
		if( !flippedVertical && !flippedHorizontal )
		{
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ecx );"
				"		notDraw%=:;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jb loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && !flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ecx );"
				"		notDraw%=:;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jb loop2%=;"
				
				"	add %3, %%edi;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedHorizontal && !flippedVertical )
		{
			int temp = 0;
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				
				"	mov %%ebx, %6;"
				"	mov %2, %%ebx;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		notDraw%=:;"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jb loop2%=;"
				"	add %3, %%edi;"
				"	add %2, %%esi;"
				"	mov %6, %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( &in->pixelData[0] ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor ), "m"( temp )
				:"eax", "ebx", "ecx", "memory" );
		}
		if( flippedVertical && flippedHorizontal )
		{
			int lastRow = ( int )&in->pixelData[0] + ( in->width * in->height );
			int temp = 0;
			asm( "mov %4, %%ebx;"
				"loop1%=:;"
				"	sub %2, %%esi;"
				"	mov %%ebx, %6;"
				"	mov %2, %%ebx;"
				"	mov $0, %%ecx;"
				"	loop2%=:;"
				"		mov ( %%esi, %%ecx ), %%al;"
				"		cmp %5, %%al;"
				"		je notDraw%=;"
				"		mov %%al, ( %%edi, %%ebx );"
				"		notDraw%=:;"
				"		dec %%ebx;"
				"		inc %%ecx;"
				"		cmp %2, %%ecx;"
				"		jb loop2%=;"
				
				"	add %3, %%edi;"
				"	mov %6, %%ebx;"
				"	dec %%ebx;"
				"	ja loop1%=;"
				:
				:"D"( startAddress ), "S"( lastRow ), "m"( in->width ), "m"( logicalScreenWidth ), "m"( in->height ), "m"( transparentColor ), "m"( temp )
				:"eax", "ebx", "ecx", "memory" );
		}
	}
}
Sprite* GraphicsEngine::CropSprite( Sprite* in, int newID, Vector2D pos, int width, int height )
{
	if( pos.x >= 0 && pos.y >= 0 && pos.x < in->width && pos.y < in->height )
	{
		Sprite* newSprite = new Sprite;
		newSprite->id = newID;
		newSprite->width = width;
		newSprite->height = height;
		if( pos.x + width > in->width )
		{
			newSprite->width = in->width - pos.x;
		}
		if( pos.y + height > in->height )
		{
			newSprite->height = in->height - pos.y;
		}

		newSprite->pixelData = ( char* )malloc( width*height*sizeof( char ) );
		
		for( int y = 0; y < height; y++ )
		{
			for( int x = 0; x < width; x++ )
			{
				newSprite->pixelData[y * width + x] = in->pixelData[( ( int )pos.y + y ) * in->width + ( ( int )pos.x + x )];
			}
		}

		return newSprite;
	}
	return NULL;
}
void GraphicsEngine::AddSprite( Sprite* newSprite )
{
	//
	sprites.push_back( newSprite );
}
Sprite* GraphicsEngine::GetSprite( int id )
{
	for( unsigned int i = 0; i < sprites.size(); i++ )
	{
		if( sprites[i]->id == id )
		{
			return sprites[i];
		}
	}
	return NULL;
}
void GraphicsEngine::FreeSprite( Sprite* in )
{
	//
	free( in->pixelData );
	delete in;
}

//tileSets
TileSet* GraphicsEngine::NewEmptyTileSet( int newID, int tileWidth, int tileHeight ) //just dont
{
	TileSet* newTileSet = new TileSet;
	newTileSet->id = newID;
	newTileSet->tileWidth = tileWidth; //just a guess. not reliable
	newTileSet->tileHeight = tileHeight; //just a guess. not reliable

	return newTileSet;
}
TileSet* GraphicsEngine::ExtractTileSet( int newID, Sprite* in, Vector2D startPos, int tileWidth, int tileHeight, int numTliesHorizontal, int numTilesVertical )
{
	TileSet* newTileSet = new TileSet;
	newTileSet->id = newID;
	newTileSet->tileWidth = tileWidth;
	newTileSet->tileHeight = tileHeight;
	strcpy( newTileSet->source, in->source );

	for( int y = 0; y < numTilesVertical; y++ )
	{
		for( int x = 0; x < numTliesHorizontal; x++ )
		{
			int tileIndex = y * numTliesHorizontal + x;
			Vector2D tilePosOffset = Vector2D( x*tileWidth, y*tileHeight );
			newTileSet->tiles.push_back( CropSprite( in, tileIndex, startPos + tilePosOffset, tileWidth, tileHeight ) );
		}
	}

	return newTileSet;
}
void GraphicsEngine::AddTileSet( TileSet* newTileSet )
{
	//
	tileSets.push_back( newTileSet );
}
TileSet* GraphicsEngine::GetTileSet( int id )
{
	for(  unsigned int i = 0; i < tileSets.size(); i++  )
	{
		if(  tileSets[i]->id == id  )
		{
			return tileSets[i];
		}
	}
	return NULL;
}
int GraphicsEngine::GetNumTileSets()
{
	return tileSets.size();
	//
}
TileSet* GraphicsEngine::GetTileSetByIndex( unsigned int index )
{
	if( index >= 0 && index < tileSets.size() )
	{
		return tileSets[index];
	}

	return NULL;
}
Sprite* GraphicsEngine::GetTile( int tileSetID, int tileIndex )
{
	//
	TileSet* tileSet = GetTileSet( tileSetID );
	return tileSet->tiles[tileIndex];
}
void GraphicsEngine::AddTile( int tileSetID, Sprite* in ) //just dont
{
	TileSet* tileSet = GetTileSet( tileSetID );
	in->id = tileSet->tiles.size();
	tileSet->tiles.push_back( in );
}
/*void GraphicsEngine::RemoveTile( int id ) //just dont
{

}*/
void GraphicsEngine::DrawTileSet( TileSet* in, Vector2D pos, int columnWidth ) // just for debugging
{
	Vector2D drawPos = pos;
	int x = 0;
	for( unsigned int i = 0; i < in->tiles.size(); i++ )
	{
		if( x == columnWidth )
		{
			x = 0;
			drawPos.y = drawPos.y + in->tileHeight;
			drawPos.x = pos.x;
		}
		DrawSprite( drawPos, in->tiles[i] );
		x++;
		drawPos.x = drawPos.x + in->tileWidth;
		
	}
}

//Animations
bool GraphicsEngine::PlayAnimation( Animation* in, Vector2D pos )
{
	//eventually delta time should be incorporated

	DrawSprite( pos, in->tileSetID, in->firstTileIndex + in->currentFrame );
	if( in->currentSpeedStep >= in->speed )
	{
		in->currentSpeedStep = 0;
		in->currentFrame++;
	}
	in->currentSpeedStep++;

	if( in->numSprites != 0 )
	{
		if( in->currentFrame >= in->numSprites )
		{
			in->currentFrame = 0;
			in->currentSpeedStep = 0;
			return true;
		}
	}
	else
	{
		if( in->currentFrame >= ( int )GetTileSet( in->tileSetID )->tiles.size() )
		{
			in->currentFrame = 0;
			in->currentSpeedStep = 0;
			return true;
		}
	}
	return false;
}
bool GraphicsEngine::PlayAnimation( Animation* in, Vector2D pos, char transparentColor )
{
	//eventually delta time should be incorporated

	DrawSprite( pos, in->tileSetID, in->firstTileIndex + in->currentFrame, transparentColor );
	if( in->currentSpeedStep >= in->speed )
	{
		in->currentSpeedStep = 0;
		in->currentFrame++;
	}
	in->currentSpeedStep++;

	if( in->numSprites != 0 )
	{
		if( in->currentFrame >= in->numSprites )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	else
	{
		if( in->currentFrame >= ( int )GetTileSet( in->tileSetID )->tiles.size() )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	return false;
}
bool GraphicsEngine::PlayAnimation( Animation* in, Vector2D pos, bool flippedHorizontal, bool flippedVertical )
{
	//eventually delta time should be incorporated

	DrawSprite( pos, in->tileSetID, in->firstTileIndex + in->currentFrame, flippedHorizontal, flippedVertical );
	if( in->currentSpeedStep >= in->speed )
	{
		in->currentSpeedStep = 0;
		in->currentFrame++;
	}
	in->currentSpeedStep++;

	if( in->numSprites != 0 )
	{
		if( in->currentFrame >= in->numSprites )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	else
	{
		if( in->currentFrame >= ( int )GetTileSet( in->tileSetID )->tiles.size() )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	return false;
}
bool GraphicsEngine::PlayAnimation( Animation* in, Vector2D pos, char transparentColor, bool flippedHorizontal, bool flippedVertical )
{
	//eventually delta time should be incorporated

	DrawSprite( pos, in->tileSetID, in->firstTileIndex + in->currentFrame, transparentColor, flippedHorizontal, flippedVertical );
	if( in->currentSpeedStep >= in->speed )
	{
		in->currentSpeedStep = 0;
		in->currentFrame++;
	}
	in->currentSpeedStep++;

	if( in->numSprites != 0 )
	{
		if( in->currentFrame >= in->numSprites )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	else
	{
		if( in->currentFrame >= ( int )GetTileSet( in->tileSetID )->tiles.size() )
		{
			in->currentFrame = 0;
			return true;
		}
	}
	return false;
}

//Window
void GraphicsEngine::DrawWindowBorder( Vector2D pos, int width, int height, int tileSetID, int topLeftCorner, int topRightCorner, int bottomLeftCorner, int bottomRightCorner, int horizontalLine, int verticalLine )
{
	TileSet *tileSet = GetTileSet( tileSetID );
	int widthInTiles = width;
	int heightInTiles = height;
	width = width * tileSet->tileWidth;
	height = height * tileSet->tileHeight;

	//Corners:
	DrawSprite( pos, tileSetID, topLeftCorner );
	DrawSprite( Vector2D( pos.x + width, pos.y ), tileSetID, topRightCorner );
	DrawSprite( Vector2D( pos.x, pos.y + height ), tileSetID, bottomLeftCorner );
	DrawSprite( Vector2D( pos.x + width, pos.y + height ), tileSetID, bottomRightCorner );

	//Vertical Lines:
	Vector2D drawPos = pos;
	for( int i = 0; i < widthInTiles-1; i++ )
	{
		drawPos.x = drawPos.x + tileSet->tileWidth;
		DrawSprite( drawPos, tileSetID, horizontalLine );
	}
	drawPos = pos;
	drawPos.y = drawPos.y + height;
	for( int i = 0; i < widthInTiles-1; i++ )
	{
		drawPos.x = drawPos.x + tileSet->tileWidth;
		DrawSprite( drawPos, tileSetID, horizontalLine );
	}

	//Horizontal Lines:
	drawPos = pos;
	for( int i = 0; i < heightInTiles-1; i++ )
	{
		drawPos.y = drawPos.y + tileSet->tileHeight;
		DrawSprite( drawPos, tileSetID, verticalLine );
	}
	drawPos = pos;
	drawPos.x = drawPos.x + width; 
	for( int i = 0; i < heightInTiles-1; i++ )
	{
		drawPos.y = drawPos.y + tileSet->tileHeight;
		DrawSprite( drawPos, tileSetID, verticalLine );
	}
}
void GraphicsEngine::DrawWindow( Vector2D pos, int width, int height, int tileSetID, int topLeftCorner, int topRightCorner, int bottomLeftCorner, int bottomRightCorner, int horizontalLine, int verticalLine, int backGround )
{
	//DrawBorder
	DrawWindowBorder( pos, width, height, tileSetID, topLeftCorner, topRightCorner, bottomLeftCorner, bottomRightCorner, horizontalLine, verticalLine );

	//FillWindow
	TileSet *tileSet = GetTileSet( tileSetID );
	Vector2D drawPos = pos;
	drawPos.x = drawPos.x + tileSet->tileWidth;
	drawPos.y = drawPos.y + tileSet->tileHeight;
	for( int y = 0; y < height-1; y++ )
	{
		for( int x = 0; x < width-1; x++ )
		{
			DrawSprite( drawPos, tileSetID, backGround );
			drawPos.x = drawPos.x + tileSet->tileWidth;
		}
		drawPos.x = pos.x + tileSet->tileWidth;
		drawPos.y = drawPos.y + tileSet->tileWidth;
	}
}

//Text
void GraphicsEngine::DrawText( Vector2D pos, int tileSetID, int firstTileIndex, const char* text )
{
	TileSet* tileSet = GetTileSet( tileSetID );
	int tileWidth = tileSet->tileWidth;
	int tileHeight = tileSet->tileHeight;
	char *currentCharacter = ( char* )&text[0];
	Vector2D drawPos = pos;
	while( *currentCharacter != '\0' )
	{
		int ascii = *currentCharacter;
		if( *currentCharacter != '\n' )
		{
			DrawSprite( drawPos, tileSetID, ascii - 32 );
			drawPos.x = drawPos.x + tileWidth;
		}
		else
		{
			drawPos.x = pos.x;
			drawPos.y = drawPos.y + tileHeight;
		}
		currentCharacter++;
	}
}
void GraphicsEngine::DrawText( Vector2D pos, int tileSetID, int firstTileIndex, const char* text, int transparentColor )
{
	TileSet* tileSet = GetTileSet( tileSetID );
	int tileWidth = tileSet->tileWidth;
	int tileHeight = tileSet->tileHeight;
	char *currentCharacter = ( char* )&text[0];
	Vector2D drawPos = pos;
	while( *currentCharacter != '\0' )
	{
		int ascii = *currentCharacter;
		if( *currentCharacter != '\n' )
		{
			DrawSprite( drawPos, tileSetID, ascii - 32, transparentColor );
			drawPos.x = drawPos.x + tileWidth;
		}
		else
		{
			drawPos.x = pos.x;
			drawPos.y = drawPos.y + tileHeight;
		}
		currentCharacter++;
	}
}

//Destroy
void GraphicsEngine::DestroyTileSet( TileSet *in )
{
	for( unsigned int i = 0; i < in->tiles.size(); i++ )
	{
		FreeSprite( in->tiles[i] );
		//delete in->tiles[i];
	}
	in->tiles.clear();
	delete in;
}
void GraphicsEngine::DestroyTileSets()
{
	for( unsigned int i = 0; i < tileSets.size(); i++ )
	{
		DestroyTileSet( tileSets[i] );
	}
	tileSets.clear();
}
void GraphicsEngine::DestroySprite( Sprite* in )
{
	//
	FreeSprite( in );
}
void GraphicsEngine::DestroySprites()
{
	for( unsigned int i = 0; i < sprites.size(); i++ )
	{
		FreeSprite( sprites[i] );
	}
	sprites.clear();
}
void GraphicsEngine::BackToTextMode()
{
	//set mode:
	__dpmi_regs r;
	r.x.ax = 03;
  	__dpmi_int( 0x10, &r );
}
void GraphicsEngine::Destroy()
{
	DestroySprites();
	DestroyTileSets();
}