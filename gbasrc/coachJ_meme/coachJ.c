// display.c : sample program to display pcx, and get key input
typedef unsigned short u16; // two type definitions
typedef unsigned long u32;

#include "keypad.h"		//keypad definitions 
#include "ppic.h"

#define REG_DISPCNT    *(u32*)0x4000000
#define MODE_4 0x4
#define BG2_ENABLE 0x400
u16* paletteMem = (u16*)0x5000000;	//PalMemory is 256 16 bit BGR values
u16* videoBuffer = (u16*)0x6000000; // video memory

void PlotPixel(int x,int y, unsigned short int c) 
{
	videoBuffer[(y) *120 + (x)] = (c);
}

int main(void)
{
	int x, y, blockX, blockY, loop; // variables

	REG_DISPCNT = MODE_4 | BG2_ENABLE; // switch to mode4
	for(loop = 0; loop < 256; loop++)	// copy palette to memory
		paletteMem[loop] = picPalette[loop];	

	blockX = 50;	// give our block a start position
	blockY = 50;

	while (1) // run forever
	{
		// process input
		if(!((*KEYS) & KEY_UP)) // the pushed up
			blockY--; // subtract from y (move up)
		if(!((*KEYS) & KEY_DOWN))
			blockY++; 
		if(!((*KEYS) & KEY_LEFT))
			blockX--;  
		if(!((*KEYS) & KEY_RIGHT))
			blockX++; 
		// draw the picture
		for(y = 0; y < 160; y++)
		{
			for(x = 0; x < 120; x++)
			{
				PlotPixel(x,y,picData[y*120+x]);
			}
		}
		PlotPixel(blockX, blockY, (75<<8)+75);
		PlotPixel(blockX+1, blockY, (75<<8)+75);
		PlotPixel(blockX, blockY+1, (75<<8)+75);
		PlotPixel(blockX+1, blockY+1, (75<<8)+75);
	}
	return 0;
}