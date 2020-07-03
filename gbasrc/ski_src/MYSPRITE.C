//--------------------------------------------------------------------------------------------
// MYSPRITE.C
//--------------------------------------------------------------------------------------------
#include "mysprite.h"
#include "sprite.h"

#include "sprite\objects.h"	// all the sprite objects
#include "num\numbers.h"	// the number and head for the HUD

extern GBA_OAMEntry gOAM[128]; // global for sprites
extern SEntry gOBJInfo[NMAX];	// global for attributes of the sprites (tree/skiier)
extern SEntry gHudInfo[HUDMAX]; // global for attributes of the hud sprites (numbers/head)

u16* OBJpalette = OBJPaletteMem; // from define in GBA.h

void GBA_LoadOBJPalette()
{
	int loop;
	for (loop = 0; loop < 256; loop++)
		OBJpalette[loop] = objPalette[loop];	//object palette
}

void GBA_LoadSprites()
{
	u16  i;
	u16  n = 512;
	u16 *c = (u16 *) (CHR_BASE + (32 * n));

	for(i= 0; i < 512; i++) // load the guy front
		*(c++) = objgfrontData[i];  
	gOBJInfo[NGUYF].atr[2] = 512;
	gOBJInfo[NGUYF].atr[0] = COLOR_256 | SQUARE;
	gOBJInfo[NGUYF].atr[1] = SIZE_32;
	
	for(i= 0; i < 512; i++) // load the guy left
		*(c++) = objgleftData[i];  
	gOBJInfo[NGUYL].atr[2] = (gOBJInfo[NGUYF].atr[2]+32);
	gOBJInfo[NGUYL].atr[0] = COLOR_256 | SQUARE;
	gOBJInfo[NGUYL].atr[1] = SIZE_32;
	
	for(i= 0; i < 512; i++) // load the guy right
		*(c++) = objgrightData[i];  
	gOBJInfo[NGUYR].atr[2] = (gOBJInfo[NGUYL].atr[2]+32);
	gOBJInfo[NGUYR].atr[0] = COLOR_256 | SQUARE;
	gOBJInfo[NGUYR].atr[1] = SIZE_32;

	for(i= 0; i < 2048; i++) // load the tree
		*(c++) = objtreeData[i];  
	gOBJInfo[NTREE].atr[2] = (gOBJInfo[NGUYR].atr[2]+32);
	gOBJInfo[NTREE].atr[0] = COLOR_256 | SQUARE;
	gOBJInfo[NTREE].atr[1] = SIZE_64;

	for(i= 0; i < 1024; i++) // load the tree
		*(c++) = objtreeshadowData[i];  
	gOBJInfo[NTREES].atr[2] = (gOBJInfo[NTREE].atr[2]+128);
	gOBJInfo[NTREES].atr[0] = COLOR_256 | TALL;
	gOBJInfo[NTREES].atr[1] = SIZE_64;

	for(i= 0; i < 2048; i++) // load the tree
		*(c++) = objpineData[i];  
	gOBJInfo[NPINE].atr[2] = (gOBJInfo[NTREES].atr[2]+64);
	gOBJInfo[NPINE].atr[0] = COLOR_256 | SQUARE;
	gOBJInfo[NPINE].atr[1] = SIZE_64;

	for(i= 0; i < 1024; i++) // load the tree
	{
		*(c++) = objpineshadowData[i];  
	}//end index loop
	gOBJInfo[NPINES].atr[2] = (gOBJInfo[NPINE].atr[2]+128);
	gOBJInfo[NPINES].atr[0] = COLOR_256 | TALL;
	gOBJInfo[NPINES].atr[1] = SIZE_64;

	for(i= 0; i < 32; i++) // load the number0
		*(c++) = n0Data[i];  
	for(i= 0; i < 32; i++) // load the number1
		*(c++) = n1Data[i];  
	for(i= 0; i < 32; i++) // load the number2
		*(c++) = n2Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n3Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n4Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n5Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n6Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n7Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n8Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = n9Data[i];  
	for(i= 0; i < 32; i++) 
		*(c++) = headData[i];  

	for(i=0; i < HUDMAX; i++)
	{
		gHudInfo[i].atr[2] = (gOBJInfo[NPINES].atr[2]+64) + i * 2;
		gHudInfo[i].atr[0] = COLOR_256 | SQUARE;
		gHudInfo[i].atr[1] = SIZE_8;
	}
}

