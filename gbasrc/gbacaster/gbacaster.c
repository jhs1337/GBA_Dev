//----------------------------------------------------------------
// gbacaster.c : main code for ray casting demo
//----------------------------------------------------------------
// Ray Casting 3D-Demo by Loirak Development
// http://www.loriak.com --- email:feedback@loirak.com
//----------------------------------------------------------------
// This is an adaptation of a Java applet made by F. Permadi,  
// part of his excellent ray-casting tutorial which is located at:
// URL:http://www.permadi.com/tutorial/raycast/index.html
//----------------------------------------------------------------
// Parts of this code like the headers, and stuff come from Pern
// Project. So thanks go to Pern Project, Lik-Sang, gbadev.org, 
// and all the rest of you. The community ROCKS!
//----------------------------------------------------------------
// Project length about 20 hours--- first WindowsGDI, then GBA
// mode3. The most time was spent working on checking the signs
// for the results of the distance and trig functions. For some
// reason I would always get the wrong sign, and so alot of checks
// and multiply by -1s are in the render function. After getting
// it to work, then I switched everything to mode4, hoping writing
// two pixels at one, it would go twice as fast. It probably did, 
// but it still wasn't fast enough so I put trig tables into
// a header file using a short visual c program. Then I made some
// small changes in the tables for 0,90,270,360. Every once in a 
// while you can still see a small glitch in the length at that 
// angle, but it is not really noticeable. Then I added simple 
// collision detection, Runs a couple frames a second, not fast, 
// but this is only a simple DEMO!

// Obvious speed improvements would be not using doubles, or 
// divides, and doing everything using fixed point math. Since 
// the GBA has no FPU unit, all double type code is handled by 
// the compiler, and emulated in code. 

// Things look fairly blocky becuase the grid size I used was 
// 64x64, it you changed this to 32, 32, the walls wouldn't be 
// so blocky, but this would involve changing quite a bit, like
// how the draw draws four lines at once, with 32 you would only
// draw two at once, and a whole bunch more, basically a rewrite. 

//----------------------------------------------------------------
// includes 
//----------------------------------------------------------------
#include "gba.h"		//from Eloist Wire cube Demo, defines the main GBA registers
#include "keypad.h"		//All the keypad definitions thanks to nokturn
#include "screenmode.h"	//screen mode stuff thanks to Dovoto
#include "trig.h"		//trig lookup tables
//#include "math.h"		//trig routines

//----------------------------------------------------------------
// constants and globals that are constant
//----------------------------------------------------------------
#define SCREENWIDTH				240
#define SCREENHEIGHT			160
#define GRIDWIDTH				64
#define GRIDHEIGHT				64
#define PLAY_LENGTH				(207.85)
#define W						1   // wall
#define O						0   // opening
#define PI						(3.14159)
#define SHORT_LINE				1200	// screenwidth/2 * 10
#define LONG_LINE				1800	// screenwidth/2 * 15
#define true					1
#define false					0

const u16 gPalette[] = { // 0..16                     			
	0x7203,	0x7205,	0x7208,	0x720A,	0x720D,	0x720F,	0x7212,	0x7214,
	0x2945,	0x2946,	0x2948,	0x294A,	0x294C,	0x294E,	0x6B5A,	0x56B5,};

//----------------------------------------------------------------
// this is the map feel free to change/englarge/shrink it
//----------------------------------------------------------------
#define MAPWIDTH				16
#define MAPHEIGHT				16

const char fMap[]=
{//     0         5         10        15
		W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W, // 0
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,W,O,O,O,W,
        W,O,O,O,O,O,W,O,W,O,O,W,O,O,O,W,
        W,O,O,O,O,O,W,O,W,W,O,W,O,O,O,W, // 5
        W,O,O,O,O,O,W,O,W,O,O,W,O,O,O,W,
        W,O,O,O,O,O,W,W,W,O,O,W,O,O,O,W,
        W,O,O,O,O,O,O,W,O,O,O,W,O,O,O,W,
        W,O,O,O,O,O,O,W,W,W,W,W,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W, // 10
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,O,O,O,O,O,O,O,O,O,O,O,O,O,O,W,
        W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W, // 15
};

//----------------------------------------------------------------
// global variables
//----------------------------------------------------------------

u32 nPlayerX;
u32 nPlayerY;
u32 nPlayerAngle;

int fKeyUp;		//bool fKeyUp=false;
int fKeyDown;	//bool fKeyDown=false;
int fKeyLeft;	//bool fKeyLeft=false;
int fKeyRight;	//bool fKeyRight=false;

u16* FrontBuffer = (u16*)0x6000000;
u16* BackBuffer = (u16*)0x600A000;
u16* videoBuffer;
u16* paletteMem = (u16*)0x5000000;	//PalMemory is 256 16 bit BGR values starting at 0x5000000

//----------------------------------------------------------------
// function prototypes 
//----------------------------------------------------------------
void drawBackground(void);		// draw the background (ceiling and floor) basically clears screen.
void renderWalls();				// draw the walls (drawn on top of background)
void WaitForVsync(void);		// waits for the vsync
void updateKeyVars();			// checks KEYS register and updates our four globals bools
void Flip(void);				// switch witch buffer is being displayed
void move(int moveup);			// moves the character back/forward checks for collision
s32 absint(s32 d);				// returns absolute value of int
s32 distAngle(s32 s1, s32 s2);  // return positive distance between to angles 

//----------------------------------------------------------------
// main --- gba entry point
//----------------------------------------------------------------

int main(int argc, char* argv[])
{
	char x; 
	char changed = 0;

	SetMode(MODE_4 | BG2_ENABLE ); //set mode 4

	for(x = 0; x < 16; x++)	// palette 
		paletteMem[x] = gPalette[x];	

	nPlayerX = 96;		// one and half squares in 
	nPlayerY = 128;		// two squares down
	nPlayerAngle = 0;	// angled to the right

	Flip();
	drawBackground();
	renderWalls(); // draw to the screen
	Flip();

	while(true)
	{
		// process the movement
		if (fKeyLeft)
		{
			nPlayerAngle += 10;
			if (nPlayerAngle >= 360) nPlayerAngle = 0;
			changed = 1;
		}

		else if (fKeyRight)
		{
			if (nPlayerAngle < 10) nPlayerAngle = 350;
			else nPlayerAngle -= 10;
			changed = 1;
		}

		if (fKeyUp)
		{
			move(1); // checks for collisions before moving
			//nPlayerX += (u32) (10 * tableCOS[nPlayerAngle]);
			//nPlayerY -= (u32) (10 * tableSIN[nPlayerAngle]);
			changed = 1;
		}
		else if (fKeyDown)
		{
			move(0); // checks for collisions before moving
			//nPlayerX -= (u32) (10 * tableCOS[nPlayerAngle]);
			//nPlayerY += (u32) (10 * tableSIN[nPlayerAngle]);
			changed = 1;
		}


		if (changed) // only redraw if they push a key and change the screen
		{
			changed = 0;
			drawBackground(); // draw background
			renderWalls(); // draw to the screen
			WaitForVsync();
			Flip(); // flip it to screen
		}
		updateKeyVars();
	}

	return 0;
}

void move(int moveup) // check for collision detection and move our guy along
{
	double deltaX;
	double deltaY;
	if (moveup)
	{
		deltaX = (tableCOS[nPlayerAngle] * 15);
		deltaY = -1*(tableSIN[nPlayerAngle] * 15);
	}
	else
	{
		deltaX = -1*(tableCOS[nPlayerAngle] * 15);
		deltaY = (tableSIN[nPlayerAngle] * 15);
	}
	
	int newX = (int)(nPlayerX + deltaX + (8 * abs(deltaX) / deltaX));
	int newY = (int)(nPlayerY + deltaY + (8 * abs(deltaY) / deltaY));
	
	if (fMap[(newX/64) + MAPWIDTH * (newY/64)] == O) {
		nPlayerX += (int) deltaX;
		nPlayerY += (int) deltaY;
	} else if (fMap[(newX/64) + MAPWIDTH * (nPlayerY/64)] == O) {
		nPlayerX += (int) deltaX;
	} else if (fMap[(nPlayerX/64) + MAPWIDTH * (newY/64)] == O) {
		nPlayerY += (int) deltaY;
	}
}

void WaitForVsync(void)
{
	//lets get rid of htat inline asm we used in arm
	/*
		__asm 
		{
			mov 	r0, #0x4000006   //0x4000006 is vertical trace counter; when it hits 160					 //160 the vblanc starts
			scanline_wait:	       	//the vertical blank period has begun. done in asm just 
									//because:)
			ldrh	r1, [r0]
			cmp	r1, #160
			bne 	scanline_wait
		}			
	*/
	while(REG_VCOUNT<160);
}

void updateKeyVars()
{
	//keypad register is a 16 bit register at 0x40000130;
	//key bits are cleared when a key is pressed.  
	//keys are defined in keypad.h
	fKeyUp = fKeyDown = fKeyLeft = fKeyRight = 0;

	if(!((*KEYS) & KEY_UP))
	{
		fKeyUp = 1;
	}
	if(!((*KEYS) & KEY_DOWN))
	{
		fKeyDown = 1;
	}
	if(!((*KEYS) & KEY_LEFT))
	{
		fKeyLeft=1;
	}
	if(!((*KEYS) & KEY_RIGHT))
	{
		fKeyRight=1;
	}
	/*if(!((*KEYS) & KEY_A))
	{
	}
	if(!((*KEYS) & KEY_B))
	{
	}
	if(!((*KEYS) & KEY_R))
	{
	}
	if(!((*KEYS) & KEY_L))
	{
	}*/
}

// draw the background, which ends up being the ceiling and floor
// the walls are drawn directly on top of this
void drawBackground(void)
{
	u16* destBuffer =videoBuffer;
	u8 loop;
	for (loop = 0; loop < 8; loop++)
	{
		u16 val = (loop << 8) + loop;
		u16* finalAdr=destBuffer+SHORT_LINE;
		while(destBuffer<finalAdr)
			*destBuffer++=val;
	}
	for (loop = 8; loop < 14; loop++)
	{
		u16 val = (loop<<8) + loop;
		u16* finalAdr=destBuffer+LONG_LINE;
		while(destBuffer<finalAdr)
			*destBuffer++=val;
	}
}


// below are two small helper functions used by renderWalls()
s32 absint(s32 d)
{
	if (d < 0) d *= -1;
	return d;
}

s32 distAngle(s32 s1, s32 s2)
{
	s32 temp;
	if (s2 >= s1)
		temp = s2 - s1;
	else
		temp = s1 - s2;
	if (temp > 30) temp -= 360;
	return absint(temp);
}

//----------------------------------------------------
// The coordinate system for this render method
//----------------------------------------------------
//                       90 deg, y--
//                           |
//                           |
//                 QUAD2     |     QUAD1
//                           |
//                           |
//                           |
// x--, 180 deg --------------------------- 0 deg, x++
//                           |
//                           |
//                 QUAD3     |     QUAD4
//                           |
//                           |
//                           |
//                      270 deg, y++
//----------------------------------------------------

// draw the walls
void renderWalls()
{
	u32		loop;		// loop over every fourth line for the screen 0, 4, 8, ... 236
	s16		curAngle;
	s32		gridX;		// grid coordinates for the map
	s32		gridY;		// grid coordinates for the map
	u16*	destBuffer = videoBuffer; // points to where we draw to
	u8		x,y;		//counters to loop through all x and y positions on the screen
	
	//double	radian;		// angle in radians
	double	horzLength; // length to wall along horizontal route
	double	vertLength; // length to wall along vertical route
	double*	minLength;	// pointer which will point to smaller of above two
	u32		lineLength;	// the length of the line we will draw to the screen

	char darkgray = 0;	// draw vertical as darkgray, and horzontal as lightgray

	double	fdistNextX; // the distance between each test coordinate .
	double	fdistNextY;
	int		ndistNextX; 
	int		ndistNextY;

	int		horzY;		// the test coordinates of the vertical/horizonatal test
	double  horzX;
	int		vertX;
	double	vertY; 

	curAngle = nPlayerAngle + 30;		// start left 30 degrees of playerAngle
	if (curAngle >= 360) curAngle -= 360;

	for (loop = 0; loop < SCREENWIDTH; loop+=4) // 4 = SCREENWIDTH / 64 (TILEHEIGHT)
	{
		// calculate the horizontal distance
		if (curAngle == 0 || curAngle == 180)
		{
			// there is no horizontal wall at this angle
			// so set the distance to something really large
			horzLength = 9999999.00;
		}
		else // check which direction ray is facing (up or down)
		{
			if (curAngle < 180) // facing up 
			{
				horzY = (nPlayerY/64) * 64; 
				ndistNextY = -64;
				double amountChange = ((s32) (horzY - nPlayerY) ) * tableINVTAN[curAngle];
				if (curAngle < 90 || curAngle > 270) // facing right
				{
					if (amountChange < 0) amountChange *= -1; // shoud be pos
				}
				else 
				{
					if (amountChange > 0) amountChange *= -1; // should be neg
				}
				horzX = nPlayerX + amountChange; 
				horzY--;
			}
			else // facing down 
			{
				horzY = (nPlayerY/64) * 64 + 64; 
				ndistNextY = 64; 
				double amountChange = ((s32)(horzY - nPlayerY)) * tableINVTAN[curAngle];
				if (curAngle < 90 || curAngle > 270) // facing right
				{
					if (amountChange < 0) amountChange *= -1; // should be pos
				}
				else 
				{
					if (amountChange > 0) amountChange *= -1; // should be neg
				}
				horzX = nPlayerX + amountChange;
			}
			fdistNextX = (64.00 * tableINVTAN[curAngle]);
			// at 90/270 the fdistNextX should get very small, should be zero
			if ( (curAngle < 90) || (curAngle>270) )
			{
				if (fdistNextX < 0) fdistNextX *= -1;		// positive distance to next block
			}
			else
			{
				if (fdistNextX > 0) fdistNextX *= -1;		// negative distance to next block
			}

			while (true)
			{
				gridX = (s32)(horzX / 64);
				gridY = (s32)(horzY / 64);
				if (gridX >= MAPWIDTH || gridY >= MAPHEIGHT || gridX < 0 || gridY < 0)
				{
					horzLength = 9999999.00;
					break;
				}
				else if (fMap[gridX+gridY*MAPHEIGHT])
				{
					horzLength = (horzX - nPlayerX) * tableINVCOS[curAngle];
					break;
				}
				horzX += fdistNextX;
				horzY += ndistNextY;
			}
		}
		// caclulate the vertical distance
		if (curAngle == 90 || curAngle == 270)
		{
			// there is no vertical wall at this angle
			// so set the distance to something really large
			vertLength = 9999999.00;
		}
		else // check which direction ray is facing left or right
		{
			if (curAngle < 90 || curAngle > 270) // facing right
			{
				vertX = (nPlayerX/64) * 64 + 64;
				ndistNextX = 64;
				double amountChange = tableTAN[curAngle]*((s32)(vertX-nPlayerX));
				if (curAngle < 180) // facing up 
				{
					if (amountChange > 0) amountChange *= -1; // should be neg
				}
				else
				{
					if (amountChange < 0) amountChange *= -1; // should be pos
				}
				vertY = nPlayerY + amountChange; 
			}
			else // facing left
			{
				vertX = (nPlayerX/64) * 64; 
				ndistNextX = -64;			
				double amountChange = tableTAN[curAngle]*((s32)(vertX-nPlayerX));
				if (curAngle < 180) // facing up 
				{
					if (amountChange > 0) amountChange *= -1; // should be neg
				}
				else
				{
					if (amountChange < 0) amountChange *= -1; // should be pos
				}
				vertY = nPlayerY + amountChange; 
				vertX--;
			}
			fdistNextY = 64.00 * tableTAN[curAngle]; 
			if (curAngle < 180) 
			{
				if (fdistNextY > 0) fdistNextY *= -1;	// make it negative (y--) for rays facing up
			}
			else
			{
				if (fdistNextY < 0) fdistNextY *= -1;	// make it positive (y++) for rays facing down
			}

			while (true)
			{
				gridX = (s32)(vertX / 64);
				gridY = (s32)(vertY / 64);
				if (gridX >= MAPWIDTH || gridY >= MAPHEIGHT || gridX < 0 || gridY < 0)
				{
					vertLength = 9999999.00;
					break;
				}
				else if (fMap[gridX+gridY*MAPHEIGHT])
				{
					vertLength = (vertY - nPlayerY) * tableINVSIN[curAngle];
					break;
				}
				vertX += ndistNextX;
				vertY += fdistNextY;
			}
		}

		// correct for negative values distance is always an absolute value
		if (vertLength < 0) vertLength *= -1; 
		if (horzLength < 0) horzLength *= -1;

		// determine which is smaller
		if (vertLength < horzLength)
		{
			minLength = &vertLength;
			darkgray = 1;
		}
		else
		{
			darkgray = 0;
			minLength = &horzLength;
		}

		// fix the distortion
		(*minLength) = (*minLength) * tableCOS[distAngle(curAngle, nPlayerAngle)];

		// figure out how long line is
		lineLength = absint((s32)((64.00 / *minLength) * PLAY_LENGTH)   );

		// find where to start drawing the line
		int end = (80 - lineLength/2);
		int start;
		if (end < 0)
		{
			end = 160;
			start = 0;
		}
		else
		{
			start = end;
			end += lineLength;
		}

		// draw the line
		u32 where = loop/2 + start*120;
		if (darkgray)
		{
			for(y = start; y<end; y++)  //loop through all y
			{
					destBuffer[where] = 0x0f0f;
					destBuffer[where+1] = 0x0f0f;
					where += 120;
			}
		}
		else
		{
			for(y = start; y<end; y++)  //loop through all y
			{
					destBuffer[where] = 0x0e0e;
					destBuffer[where+1] = 0x0e0e;
					where += 120;
			}
		}

		curAngle -= 1; // change angle one degree to the right
		if (curAngle < 0) curAngle += 360;

	} // end for loop 0..240 by 4

} // end renderWalls

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
