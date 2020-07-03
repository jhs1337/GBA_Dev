//--------------------------------------------------------------------------------------------
// SPRITE.C
//--------------------------------------------------------------------------------------------
#include "sprite.h"

extern GBA_OAMEntry	gOAM[128]; // global for sprites (from main.c)

// move sprite number _u16id to coords x, y
void GBA_SpriteMove(u16 num, int x, int y)
{
	if(x < 0) //if it is off the left correct
		x = 512 + x;
	if(y < 0) //if off the top correct
		y = 256 + y;
 
	gOAM[num].attribute0 &= ~OBJ_Y_MASK;
	gOAM[num].attribute0 |= y;

	gOAM[num].attribute1 &= ~OBJ_X_MASK;
	gOAM[num].attribute1 |= x;
}

// updates OAM to system
void GBA_UpdateOAM()
{
	u16  i;
	u16 *p = (u16 *) gOAM;
	
	for (i = 0; i < 40 * 4; i++) // if you are using all sprites you need to update all 128
	{
		*((u16 *) OAM_BASE + i) = *(p++);
	}
}


void GBA_InitializeOAM()
{
	u8	i;
	// Set all sprites to be off screen
	for (i = 0; i < 128; i++)
	{
		gOAM[i].attribute0 = 160;  // y to > 159
		gOAM[i].attribute1 = 240;  // x to > 239
		gOAM[i].attribute2 = 0x0000;  // x to > 239
	}
}
