/*********************************************************************************************
// Skiing Advance by Loirak Development
//--------------------------------------------------------------------------------------------
Programming: Ben Rhoades (benr@loirak.com)
Programming/Artwork: Joe McCombs (joe@loirak.com)

x = implemented
o = possible future addition
HUD = heads up display (status bar)

Version 1.00 
x coded on 2 Feb 2001(in one day) for the GBADev competition
x menu/splash/credits screen, complete with fades
x no global variables
x guy skiing down the mountain, with tree/shadows

Version 2.00
x fixed after passing a tree, tree is drawn under skiier
x moved code from 1 C files to 4 C files (improved readability)
x added some globals to eliminate passing so many args in functions
x fixed collision detection, and key input
x switch from armgcc to devkitadv
o added sound
o redo some screens (credits/win/lose) for dim lighting of gba

Version X.XX
o make it into a tile mode game
o add more sprites hills, jumps, rocks, signs, spectators, etc. 
o full font avaiable for text
o have highscores list saved to SRAM
o more animations for the skiier
o have choice to be a snowboarder/skiier
o maybe (actual levels, with planned map, and not random tree generation
o competition against someone else with a link cable

//--------------------------------------------------------------------------------------------
// Greets/Acknowledgements
//--------------------------------------------------------------------------------------------
Dovoto- The Pern Project
gbadev.org
devrs.com/gba
Visoly/Lik-Sang! 
Heinrich Tillack <tillack@gmx.net> <-- for some speedups with blitting scrolling bg to screen
Darren Sillett <- GBAStarfield Demo (code for implementing sprites)
*********************************************************************************************/

//--------------------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------------------
#include "gba.h"		// register data type defines
#include "screenmode.h"	// screen mode defines
#include "keypad.h"		// keypad defines
#include "gfx.h"		// misc code for random/sleep/etc.
#include "sprite.h"		// code for sprite manipulation
#include "mysprite.h"	// Skiing specific sprite code/loads/etc.

#include "gfx\logo.h"		// header file created by pcx2gba
#include "gfx\ski.h"		// ski menu
#include "gfx\menu1.h"		// software sprite menu (where credits is highlighted)
#include "gfx\gamebg.h"		// in game scrolling snow background 
#include "gfx\ccrash.h"		// guy crashed in software sprite since I ran out of sprite room
#include "gfx\credits.h"	// the credits screen
#include "gfx\end.h"		// the win/lose screen

//--------------------------------------------------------------------------------------------
// DEFINES 
//--------------------------------------------------------------------------------------------
#define	GUYMOVE 3		// how fast guy moves
//#define MENUOFF                 // if defined Menu will not run, it will go straight to game
//#define INVINCIBLE		// if defined lives will not be subtracted

//--------------------------------------------------------------------------------------------
// Global Variables / Memory Location Defines!
//--------------------------------------------------------------------------------------------
u16* videoBuffer;					// which buffer do we write too (front or back)
GBA_OAMEntry gOAM[128];				// global for sprites
SEntry gOBJInfo[NMAX];				// global standard attributes of sprites (tree/skiier)
SEntry gHudInfo[HUDMAX];			// global standard attributes of hud sprites (numbers/head)
u16* paletteMem = (u16*)0x5000000;	// 256 16 bit BGR values starting at 0x5000000

//--------------------------------------------------------------------------------------------
// Function Prototypes
//--------------------------------------------------------------------------------------------
void DoIntro(void);			// display the loirak screen, and sleep for a second or two
void DoGame(void);			// run the game
void RunMenu(void);			// the titlescreen/menu
void WaitForStart(void);	// waits for the start key to be pressed
void DrawStatusBar(int lives, int score); // draw the status bar at the bottom of the screen
void DrawBackground(int y);	// draw the scrolling background!

//--------------------------------------------------------------------------------------------
// AgbMain Entry Point
//--------------------------------------------------------------------------------------------
int main(void)//void AgbMain(void)
{
	SetMode(MODE_4 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D); //set mode 4, objects enabled

	EraseScreen();

#ifndef MENUOFF
	DoIntro();
#endif 
	
	SeedRandom();

	GBA_LoadSprites(gOAM);	// load the sprites into OAM memory
	GBA_LoadOBJPalette();	// load the sprite palette
	GBA_InitializeOAM();	// move the sprites offscreen 
	GBA_UpdateOAM();		// update this info to GBA

	while (1) // always stay in this loop
	{
#ifndef MENUOFF
		EraseScreen();
		RunMenu();
#endif
		DoGame();
	} // end while(1)
	return 0;
} // end Agbmain

//--------------------------------------------------------------------------------------------
// DoIntro
//--------------------------------------------------------------------------------------------
void DoIntro(void)
{
	int x, y;
	for(x = 0; x < 256; x++)				// palette for intro screen
		paletteMem[x] = logoPalette[x];	
	for(y = 0; y < 160; y++)
		for(x = 0; x < 120; x++)
			PlotPixel(x,y,logoData[y*120+x]);//logoData contains the color values of your pict
	WaitForVblank();
	Flip();
	FadeIn(2);
	Sleep(115);
	FadeOut(3);
}

//--------------------------------------------------------------------------------------------
// RunMenu
//--------------------------------------------------------------------------------------------
void RunMenu(void)
{
	int x, y;
	int toggle = 0; // 0 draw top, 1 draw bottom
	int redraw = 0; // do we need to redraw menu (0 no, 1 yes)
	int start_on = 0; // if start is being pressed before menu starts, wait to process till it is released
	int a_on = 0; // if a is being pressed before menu starts, wait to process till it is released
	u32 countFrame = 0; // for fixing up/down on keystrokes
	u32 tmpCount = 0;

	if ((! ((*KEYS) & KEY_START) ))
		start_on = 1;
	if ((! ((*KEYS) & KEY_A) ))
		a_on = 1;

	u16* srcPtr=(u16 *)&skiData;
	u16* destBuffer =videoBuffer;
	u16* finalAdr=destBuffer+SCREENSIZE_U16;//screensize in u16 einheiten
	while(destBuffer<finalAdr)
		*destBuffer++=*srcPtr++;
	for(x = 0; x < 256; x++)	// set the background palette
		paletteMem[x] = skiPalette[x];	
	WaitForVblank();
	Flip();
	FadeIn(2);

	while (1) // loops infinitely till they press start
	{
		countFrame++;

		if ((! ((*KEYS) & KEY_UP) ) ||
			(! ((*KEYS) & KEY_DOWN) ))
		{
			if (tmpCount != 0)
			{
				tmpCount = countFrame - tmpCount;
				if (tmpCount > 8)
					tmpCount = 0;
				else
					tmpCount = countFrame - tmpCount;
			}
			if (tmpCount == 0)
			{
				tmpCount = countFrame;

				redraw = 1; // force to redraw menu!
				toggle = (toggle ? 0 : 1);
			}
		}

		if (redraw)
		{
			if (toggle) // draw credits highlighted
			{
				srcPtr = (u16 *)&skiData;
				destBuffer =videoBuffer;
				finalAdr=destBuffer+SCREENSIZE_U16;
				while(destBuffer<finalAdr)
					*destBuffer++=*srcPtr++;
				for (y = 0; y < menu1_HEIGHT; y++)
					for (x = 0; x < (menu1_WIDTH/2); x++) // actual width 120
						PlotPixel(x+30, y+85, menu1Data[y*(menu1_WIDTH/2)+x]);
			}
			else // draw start game highlighted
			{
				srcPtr = (u16 *)&skiData;
				destBuffer =videoBuffer;
				finalAdr=destBuffer+SCREENSIZE_U16;
				while(destBuffer<finalAdr)
					*destBuffer++=*srcPtr++;
			}
			WaitForVblank();
			Flip();
		}

		if (((*KEYS) & KEY_START)) start_on = 0;
		if (((*KEYS) & KEY_A)) a_on = 0;

		if ((! ((*KEYS) & KEY_START) ) ||
			(! ((*KEYS) & KEY_A) ))
		{
			if ( ((start_on != 1) && (! ((*KEYS) & KEY_START) ))   
				|| ( (a_on !=1) && (! ((*KEYS) & KEY_A)) ) )
			{

				if (!toggle) return; // they selected start game

				if (! ((*KEYS) & KEY_START) ) start_on = 1;
				if (! ((*KEYS) & KEY_A) ) a_on = 1;

				// Draw the credits screen
				FadeOut(2);
				for(x = 0; x < 256; x++)	// set the palette for credits screen
					paletteMem[x] = creditsPalette[x];	
				for(y = 0; y < 160; y++)	// draw the screen
					for(x = 0; x < 120; x++)
						PlotPixel(x,y,creditsData[y*120+x]);
				WaitForVblank();
				Flip();
				FadeIn(2);
				while (1)
				{
					if (!start_on && !a_on) break;
					if (((*KEYS) & KEY_START)) start_on = 0;
					if (((*KEYS) & KEY_A)) a_on = 0;
				}
				WaitForStart();
				if (! ((*KEYS) & KEY_START) ) start_on = 1;
				if (! ((*KEYS) & KEY_A) ) a_on = 1;

				FadeOut(2);
				// redraw the menu screen
				for(x = 0; x < 256; x++)	// restore the menu palette
					paletteMem[x] = skiPalette[x];	
				srcPtr = (u16 *)&skiData;
				destBuffer =videoBuffer;
				finalAdr=destBuffer+SCREENSIZE_U16;
				while(destBuffer<finalAdr)
					*destBuffer++=*srcPtr++;
				WaitForVblank();
				Flip();
				FadeIn(2);
				toggle = 0;
			}
		}
	} // end while (1)
}	// end void DoGame(void);

//--------------------------------------------------------------------------------------------
// WaitForStart
//--------------------------------------------------------------------------------------------
void WaitForStart(void)
{
	while (1) // loops infinitely till they press start
		if ((! ((*KEYS) & KEY_START) ) ||
			(! ((*KEYS) & KEY_A) ) )
			break;
}

//--------------------------------------------------------------------------------------------
// DrawBackground
//--------------------------------------------------------------------------------------------
void DrawBackground(int y)		// scrolls the background onto the screen
{
	int diffy;
	diffy = 320-y;
	if (y > 160)
	{
		// draw the remaining
		u16* srcPtr=(u16 *)&gamebgData + y * 120;//contains the background color values 

		u16* destBuffer =videoBuffer;
		u16* finalAdr=destBuffer+(diffy*120);//screensize in u16 einheiten
		while(destBuffer<finalAdr)
			*destBuffer++=*srcPtr++;

		// draw from the top
		diffy = 160-diffy; // remaing lines to be drawn
		srcPtr=(u16 *)&gamebgData;//starData contains the background color values 
		finalAdr=destBuffer+(diffy*120);//screensize in u16 einheiten
		while(destBuffer<finalAdr)
			*destBuffer++=*srcPtr++;
	}
	else // dont need to scroll
	{
		u16* srcPtr=(u16 *)&gamebgData + y * 120;//contains the background color values 
		u16* destBuffer =videoBuffer;
		u16* finalAdr=destBuffer+SCREENSIZE_U16;//screensize in u16 einheiten
		while(destBuffer<finalAdr)
			*destBuffer++=*srcPtr++;
	}
}

//--------------------------------------------------------------------------------------------
// DrawStatusBar
//--------------------------------------------------------------------------------------------
void DrawStatusBar(int lives, int score) // draw the status bar at the bottom of the screen
{
	int tscore = 0;
	int t2score = 0;

	tscore = score / 100;	// hundreds digit
	gOAM[0].attribute0 = gHudInfo[tscore].atr[0] | HUDY;
	gOAM[0].attribute1 = gHudInfo[tscore].atr[1] | HUDX1;
	gOAM[0].attribute2 = gHudInfo[tscore].atr[2];
	tscore = score - (tscore*100);
	t2score = tscore / 10; // tens digit
	gOAM[1].attribute0 = gHudInfo[t2score].atr[0] | HUDY;
	gOAM[1].attribute1 = gHudInfo[t2score].atr[1] | HUDX2;
	gOAM[1].attribute2 = gHudInfo[t2score].atr[2];
	tscore = tscore - (t2score*10); // ones digit
	gOAM[2].attribute0 = gHudInfo[tscore].atr[0] | HUDY;
	gOAM[2].attribute1 = gHudInfo[tscore].atr[1] | HUDX3;
	gOAM[2].attribute2 = gHudInfo[tscore].atr[2];
	
	// turn off the heads
	gOAM[3].attribute0 = gHudInfo[HUDHEAD].atr[0] | 160;
	gOAM[4].attribute0 = gHudInfo[HUDHEAD].atr[0] | 160;
	gOAM[5].attribute0 = gHudInfo[HUDHEAD].atr[0] | 160;
	if (lives == 3) // they have three
	{
		gOAM[3].attribute0 = gHudInfo[HUDHEAD].atr[0] | HUDY;
		gOAM[3].attribute1 = gHudInfo[HUDHEAD].atr[1] | HUDX4;
		gOAM[3].attribute2 = gHudInfo[HUDHEAD].atr[2];
	}
	if (lives > 1) // they have atleast two
	{
		gOAM[4].attribute0 = gHudInfo[HUDHEAD].atr[0] | HUDY;
		gOAM[4].attribute1 = gHudInfo[HUDHEAD].atr[1] | HUDX5;
		gOAM[4].attribute2 = gHudInfo[HUDHEAD].atr[2];

	}
	if (lives > 0) // they have one!
	{
		gOAM[5].attribute0 = gHudInfo[HUDHEAD].atr[0] | HUDY;
		gOAM[5].attribute1 = gHudInfo[HUDHEAD].atr[1] | HUDX6;
		gOAM[5].attribute2 = gHudInfo[HUDHEAD].atr[2];
	}
}

//--------------------------------------------------------------------------------------------
// DoGame (this is where the magic all happens!
//--------------------------------------------------------------------------------------------
void DoGame(void)
{
	OBJTree tree[NUMTREES];
	int bgscroll;	// how fast the background and trees are scrolling
	int bgy;		// current level for background scrolling
	int lives;		// number of lives
	int score;		// their score
	int loop;		// temp loop variable
	int rnum;		// temp storage of random number
	
	int guyx = 10;	// coordinates of the skiier
	int guyy = 5;	// y coord of skiier (constant throughout game)

	for(loop = 0; loop < 256; loop++)
		paletteMem[loop] = gamebgPalette[loop];	// background palette

	for (loop = 0; loop < NUMTREES; loop++) // initialize the onscreen trees
	{
		tree[loop].type = 0;	// tree type usually NPINE or NTREE
		tree[loop].x = 0;		// trees coordinates
		tree[loop].y = 0;
		tree[loop].shadow = 0;	// gOAM[tree.shadow]
		tree[loop].on = 0;		// if not 0, it is where tree is in gOAM, (gOAM[tree.on])
		tree[loop].passed = 0;	// has the tree passed the skier.
	}					

	gOAM[GUYNUM].attribute0 = gOBJInfo[NGUYF].atr[0] | guyy; // initialize guy sprite onto screen
	gOAM[GUYNUM].attribute1 = gOBJInfo[NGUYF].atr[1] | guyx;
	gOAM[GUYNUM].attribute2 = gOBJInfo[NGUYF].atr[2];

	lives = 3;		// how many lives (heads in the lower right corner
	score = 0;		// their score
	bgy = 0;		// the current y of the scroll between 0..319
	bgscroll = 3;	// how fast background scrolls

	while (lives > 0 && score < 1000) // play until 1000 points or they've died 3 times
	{
		// Parts of the Main game loop
		// o draw the bg
		// o draw the trees, guy, etc. 
		// o flip it to the screen
		// o check for user input
		// o move the sprites
		// o check for collision
		// o subtract from lives and do the crash sprite

		DrawBackground(bgy); 
		bgy+=bgscroll;			// scroll the background 
		if (bgy > 319) bgy = 0;	// reset the scroll, at 319
		DrawStatusBar(lives, score);	// draw the Hud(heads up display)
		WaitForVblank();
		GBA_UpdateOAM(); // draws the tree/guy
		Flip();	// draw the screen

		gOAM[GUYNUM].attribute2 = gOBJInfo[NGUYF].atr[2];	// we want the guy to be looking 
															// forward when he skies unless
															// right/left are being pressed
		if (! ((*KEYS) & KEY_RIGHT) )
		{
			guyx+=GUYMOVE;	// move the guy right guymove units
			gOAM[GUYNUM].attribute2 = gOBJInfo[NGUYR].atr[2]; // change spr to face right
		}
		if (! ((*KEYS) & KEY_LEFT) )
		{
			guyx-=GUYMOVE; // move the guy left guymove units
			gOAM[GUYNUM].attribute2 = gOBJInfo[NGUYL].atr[2]; // change spr to face left
		}
		
		if (guyx >= 240 - 32) // check if they are trying to move the skiier off screen
			guyx = 240 - 32;
		else if (guyx <= 0)
			guyx = 0;
		
		GBA_SpriteMove(GUYNUM, guyx, guyy); // Move our skiier
	
		for (loop = 0; loop < NUMTREES; loop++) // Move the trees, signs, etc. 
		{
			tree[loop].y -= bgscroll; // scroll the trees along with the background

			if (tree[loop].on) // tree is on!
			{
				GBA_SpriteMove(tree[loop].on, tree[loop].x, tree[loop].y); // move tree

				if (tree[loop].type == NTREE) // its a normal tree
					GBA_SpriteMove(tree[loop].shadow, tree[loop].x-33, tree[loop].y+28); // move tree shadow
				else // its a pine tree
					GBA_SpriteMove(tree[loop].shadow, tree[loop].x-22, tree[loop].y+29); // move pine tree shadow

				if (tree[loop].y < -63) // calculate if moved off the screen
				{
					if (score == 150) bgscroll++; // increase the speed of the game
					score++; // add to their score
					tree[loop].on = 0;  // turn the tree off
				}

				if ((guyy > (tree[loop].y + 28)) && (tree[loop].passed == 0)) // the tree is high than skiier
				{
					// when the guy passes a tree, then where the shadow was becomes the 
					// tree, and the shadow gets moved back an extra NUMTREES!
					//tree[loop].shadow = loop+PASSED_GUY;
					tree[loop].passed = 1;
					tree[loop].on = loop+SHADOW_OFFSET;
					tree[loop].shadow = loop+PASSED_GUY;
					gOAM[loop+TREESTART].attribute0 = gOBJInfo[tree[loop].type].atr[0] | 160;
					// put the entry of this tree in front of the skiier
					// off the screen, and make another entry for it after
					// GUYNUM, so it will be drawn under the skiier
					gOAM[loop+SHADOW_OFFSET].attribute0 = gOBJInfo[tree[loop].type].atr[0]; 
					gOAM[loop+SHADOW_OFFSET].attribute1 = gOBJInfo[tree[loop].type].atr[1]; 
					gOAM[loop+SHADOW_OFFSET].attribute2 = gOBJInfo[tree[loop].type].atr[2];
					gOAM[loop+PASSED_GUY].attribute0 = gOBJInfo[tree[loop].type+1].atr[0]; 
					gOAM[loop+PASSED_GUY].attribute1 = gOBJInfo[tree[loop].type+1].atr[1]; 
					gOAM[loop+PASSED_GUY].attribute2 = gOBJInfo[tree[loop].type+1].atr[2];

					GBA_SpriteMove(tree[loop].on, tree[loop].x, tree[loop].y); // move tree
					if (tree[loop].type == NTREE) // its a normal tree
						GBA_SpriteMove(tree[loop].shadow, tree[loop].x-33, tree[loop].y+28); // move tree shadow
					else // its a pine tree
						GBA_SpriteMove(tree[loop].shadow, tree[loop].x-22, tree[loop].y+29); // move pine tree shadow
				}
				else if ((guyx < (tree[loop].x + 28) ) && // check to see if they hit the tree[loop]
					( guyx > (tree[loop].x - 15)) &&
					( guyy > (tree[loop].y + 24)) &&
					( guyy < (tree[loop].y + 31)) ) // if bgscroll is ever bigger than 4 than this will need to change
				{
					// display the crash guy as software sprite?!?
					int x, y;
					gOAM[GUYNUM].attribute0 = 160; // turn off real sprite
					for (y = 0; y < crash_HEIGHT; y++)  // draw the crashed guy
						for (x = 0; x < (crash_WIDTH/2); x++) // actual width 120
							PlotPixelTransparent(x+(guyx/2), y+guyy, crashData[y*(crash_WIDTH/2)+x]);
					gOAM[loop+SHADOW_OFFSET].attribute0 = COLOR_256 | TALL | 160; // turn off the tree shadow
					WaitForVblank();
					GBA_UpdateOAM(); // update change to sprite memory
					Flip(); // draw the screen
					Sleep(50); // pause the game for a second
					tree[loop].on = 0; // turn the tree off (if you hit a tree its removed (so you dont hit it again)
					gOAM[loop+TREESTART].attribute0 = COLOR_256 | SQUARE | 160; // turn tree OAM off
#ifndef INVINCIBLE
					lives--; // subtract from lives
#endif //INVINCIBLE
					gOAM[GUYNUM].attribute0 = gOBJInfo[NGUYF].atr[0] | guyy; // turn our skiier back on
				}

			} // end if tree[loop].on
			else // not on, so turn on! (tree[loop].on is 0)
			{
				rnum = RAND(60); // delay the tree after they've come on at least once
				if (rnum == NTREE || rnum == NPINE) 
				{
					tree[loop].x = RAND(200);
					tree[loop].y = 160;
					tree[loop].on = loop+TREESTART;
					tree[loop].shadow = loop+SHADOW_OFFSET;
					tree[loop].type = rnum;
					tree[loop].passed = 0;
					// put the tree either NTREE/NPINE in at this loop
					gOAM[loop+TREESTART].attribute0 = gOBJInfo[rnum].atr[0] | 160; 
					gOAM[loop+TREESTART].attribute1 = gOBJInfo[rnum].atr[1] | tree[loop].x;
					gOAM[loop+TREESTART].attribute2 = gOBJInfo[rnum].atr[2];
					// put the tree shadow either NTREES/NPINES in at this loop + SHADOWOFFSET
					gOAM[loop+SHADOW_OFFSET].attribute0 = (gOBJInfo[rnum+1].atr[0] | 160);
					gOAM[loop+SHADOW_OFFSET].attribute1 = (gOBJInfo[rnum+1].atr[1] | 240);	
					gOAM[loop+SHADOW_OFFSET].attribute2 = gOBJInfo[rnum+1].atr[2];
				}
			} // end else of if on
		} // end for 0...num sprites (trees)
	
	} // end while life > 0

	// Now they've died or Won so we need to tell them that
	EraseScreen();
	GBA_InitializeOAM(); // put sprites offscreen
	for(loop = 0; loop < 256; loop++)	// win/loss screen palette
		paletteMem[loop] = wendPalette[loop];	
	for(guyy = 0; guyy < 160; guyy++)		// I use guyx,y to loop since I dont want to make
		for(guyx = 0; guyx < 120; guyx++)	// another variable, plus we're done with it now (their dead)
			PlotPixel(guyx,guyy,wendData[guyy*120+guyx]);
	if (lives == 0)
	{
		// the wendData is actually the You win screen so if lives == 0, 
		// we need to overlay the you lose... text
		for (guyy = 0; guyy < lend_HEIGHT; guyy++)
			for (guyx = 0; guyx < (lend_WIDTH/2); guyx++) // actual width 120
			PlotPixel(guyx+28, guyy+12, lendData[guyy*(lend_WIDTH/2)+guyx]);
	}
	WaitForVblank();
	GBA_UpdateOAM(); 			// Update OAM
	Flip();
	WaitForStart();
} // end void DoGame(void)
