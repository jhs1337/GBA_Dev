//--------------------------------------------------------------------------------------------
// SPRITE.H (routines for sprite manipulation and loading)
//--------------------------------------------------------------------------------------------
#ifndef SPRITE_H
#define SPRITE_H

#include "gba.h"

//--------------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------------
#define OBJ_Y_MASK			0xFF		// Y Position Bit mask
#define OBJ_SPECIAL			0x100		// Rotation
#define OBJ_SDOUBLE			0x200		// Size double
#define OBJ_BLEND			0x400		// Alpha blend
#define OBJ_WINDOW			0x800		// Windowed mode
#define OBJ_MOSAIC			0x1000		// Mosaic Mode
#define OBJ_COLOUR_256x1	0x2000		// Object has 256 colours from 1 palette
#define OBJ_COLOUR_16x16	0			// Object has 16 colours from 1 of 16 palettes
#define OBJ_SQUARE			0			// Object is square (1x1 aspect)
#define OBJ_WIDE			0x4000		// Object is wider than tall. (2x1 aspect)
#define OBJ_TALL			0x8000		// Object is taller than wide (1x2 aspect)

#define OBJ_X_MASK			0x1FF		// X Position bit mask
#define OBJ_HFLIP			0x1000
#define OBJ_VFLIP			0x2000
#define OBJ_SIZE_1X			0			// Object is normal size
#define OBJ_SIZE_2X			0x4000		// Object is double size
#define OBJ_SIZE_4X			0x8000		// ...
#define OBJ_SIZE_8X			0xc000		// ...

#define	OBJ_PRIORITY(n)		((n) << 10)
#define	PRIORITY3			0x0600
#define	PRIORITY2			0x0400
#define	PRIORITY1			0x0200
#define	PRIORITY0			0x0000
#define OBJ_PALETTE0(n)		((n) << 12)

//Atribute0 stuff
#define ROTATION_FLAG           0x100
#define SIZE_DOUBLE                     0x200
#define MODE_NORMAL             0x0
#define MODE_TRANSPERANT        0x400
#define MODE_WINDOWED           0x800
#define MOSAIC                          0x1000
#define COLOR_16                        0x0000
#define COLOR_256                       0x2000
#define SQUARE                  0x0
#define TALL                    0x4000
#define WIDE                    0x8000

//Atribute1 stuff
#define ROTDATA(n)              ((n) << 9)
#define HORIZONTAL_FLIP         0x1000
#define VERTICAL_FLIP           0x2000
#define SIZE_8                  0x0
#define SIZE_16                 0x4000
#define SIZE_32                 0x8000
#define SIZE_64                 0xC000

#define OAM_BASE	0x7000000
#define CHR_BASE	0x6010000

//--------------------------------------------------------------------------------------------
// STRUCTURE DEFINES
//--------------------------------------------------------------------------------------------

typedef struct sGBA_OAMEntry
{
	u16	attribute0;
	u16	attribute1;
	u16	attribute2;
	u16	attribute3;
} GBA_OAMEntry;

typedef GBA_OAMEntry	GBA_Sprite;

/*typedef struct sGBA_OAMRotationEntry
{
	u16	filler1[3];
	u16	pa;
	u16	filler2[3];
	u16	pb;
	u16	filler3[3];
	u16	pc;
	u16	filler4[3];
	u16	pd;
} GBA_OAMRotationEntry;*/

//--------------------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------
void GBA_InitializeOAM();
void GBA_SpriteMove(u16 num, int x, int y);
void GBA_UpdateOAM();

#endif
