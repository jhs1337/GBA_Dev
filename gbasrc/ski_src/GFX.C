// Implementation of gfx.h stuff

#include "screenmode.h"
#include "gfx.h"

extern u16* videoBuffer; // from main.c (which screen are we drawing too)

volatile u16* ScanlineCounter = (volatile u16*)0x4000006;
u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;

// used to slow down the gba between frames, and for displaying stuff pretty cool eh?
void Sleep(int i)
{
	int x, y;
	int c;
	for (y = 0; y < i; y++)
	{
		for (x = 0; x < 30000; x++)
			c = c + 2; // do something to slow things down
	}
}

// seeds the random number generator
void SeedRandom(void)
{
   RAND_RandomData = REG_VCOUNT;
}

// returns a random number between 0 and value
s32 RAND(s32 Value)
{
   RAND_RandomData *= 20077;
   RAND_RandomData += 12345;

   return ((((RAND_RandomData >> 16) & RAND_MAX) * Value) >> 15);
}

// Mode 4 is 240(120)x160 by 8bit
void PlotPixel(int x,int y, unsigned short int c)	
{ 
	videoBuffer[(y) *120 + (x)] = (c); 
}

// Mode 4 is 240(120)x160 by 8bit for software sprites
void PlotPixelTransparent(int x,int y, unsigned short int c)	
{ 
	unsigned short int temp;
	if ((c & 0x00FF) == 0) // bottom is transparent
	{
		if ((c & 0xFF00) == 0) // top is also transparent
			return;
		// so bottom transparent, top is not so....
		temp = ((videoBuffer[(y) * 120 + (x)]) & 0x00FF);
		temp |= c; // or it with c (to get the highpart of c in
		videoBuffer[(y) *120 + (x)] = (temp);
	}
	else
	if 	((c & 0xFF00) == 0) // only the top is transprent
	{
		temp = ((videoBuffer[(y) * 120 + (x)]) & 0xFF00);
		temp |= c;
		videoBuffer[(y) *120 + (x)] = (temp);
	}
	else // no part is transparent
		videoBuffer[(y) *120 + (x)] = (c); 
}

void Flip(void)			// flips between the back/front buffer
{
	if(REG_DISPCNT & BACKBUFFER) //back buffer is the current buffer so we need to switch it to the font buffer
	{ 
		REG_DISPCNT &= ~BACKBUFFER; //flip active buffer to front buffer by clearing back buffer bit
		videoBuffer = BackBuffer; //now we point our drawing buffer to the back buffer
    }
    else //front buffer is active so switch it to backbuffer
    { 
		REG_DISPCNT |= BACKBUFFER; //flip active buffer to back buffer by setting back buffer bit
		videoBuffer = FrontBuffer; //now we point our drawing buffer to the front buffer
	}
}

void WaitForVblank(void)	// waits for the drawer to get to end before flip
{
	while(*ScanlineCounter < 160) {	// do nothing
	}//now we are in the vblank period
}

void EraseScreen(void)
{
	u16* destBuffer =videoBuffer;
	u16* finalAdr=destBuffer+SCREENSIZE_U16;

	while(destBuffer<finalAdr)
		*destBuffer++=0x0000;
	WaitForVblank();
	Flip();
}

void FadeOut( u32 aWait )
{
	s8 Phase;
	REG_BLDMOD = 0 | 1 | 2 | 4 | 8 | 128 | 64 | 32;
	for( Phase = 0; Phase < 16; Phase++ )
	{
		REG_COLEY = Phase;
		Sleep( aWait );
	}
}

// The same as above, but reverse Change 10 to 16 for complete fade and add BIT05 to the BLDMOD write for OBJ fade

void FadeIn( u32 aWait )
{
	s8 Phase;
	REG_BLDMOD = 0 | 1 | 2 | 4 | 8 | 128 | 64 | 32;
	for( Phase = 0; Phase < 16; Phase++ )
	{
		REG_COLEY = 16-Phase;
		Sleep ( aWait );
	}
}
