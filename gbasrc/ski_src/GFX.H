// Gfx.h created 30 Nov 2001
// Desc: Store some of the main graphics routines used by the game
#ifndef GFX_H
#define GFX_H

#include "gba.h"

//--------------------------------------------------------------------------------------------
//	DEFINES/MACROS
//--------------------------------------------------------------------------------------------
#define RGB(r,g,b) ((r)+((g)<<5)+((b)<<10))
#define SCREENSIZE 160*240
#define SCREENSIZE_U16 160*(240/2)
#define RAND_MAX 32767
volatile s32 RAND_RandomData;

//--------------------------------------------------------------------------------------------
//	Function Prototypes
//--------------------------------------------------------------------------------------------
void Sleep(int i); // do something to stall GBA (125 is good delay for still picture display)
void PlotPixel(int x,int y, unsigned short int c);
void PlotPixelTransparent(int x,int y, unsigned short int c);
void Flip(void);			// flips between the back/front buffer
void WaitForVblank(void);	// waits for the drawer to get to end before flip
void EraseScreen(void);		// erases the screen in mode4
void FadeOut(u32 aWait);	// fades the screen out
void FadeIn(u32 aWait);		// fade the screen in

// Random number generation code taken from Ivan Mackintosh's Mines (off gbadev.org) author said:
// random stuff extracted from an email from 'ninge1'
void SeedRandom(void);	//usage somwhere in program SeedRandom();
s32 RAND(s32 Value);	//then xrand = RAND(MAX_X); where MAX_X is range upper bound for value you want returned!

#endif

