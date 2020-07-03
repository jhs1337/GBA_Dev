//--------------------------------------------------------------------------------------------
// MYSPRITE.H 
//--------------------------------------------------------------------------------------------
#ifndef MYSPRITE_H
#define MYSPRITE_H

#include "gba.h"

//--------------------------------------------------------------------------------------------
//	DEFINES 
//--------------------------------------------------------------------------------------------
// In game there are two structures for holding the attributes of the hud (numbers/head) and
// the objects (guy sprites/tree sprites)
#define		NGUYF	0	// guy front
#define		NGUYL	1	// guy left....
#define		NGUYR	2	// guy right
#define		NGUYC	3	// eliminated we were using too many sprites
#define		NSIGN	4	// eliminated we were using too many sprites
#define		NTREE	5	// tree
#define		NTREES	6	// tree shadow
#define		NPINE	7	// pine tree
#define		NPINES	8	// pine tree shadow
#define		NMAX	9	

#define		HUDHEAD 10	// digits are 0..9, therefore head is 10
#define		HUDMAX	11	// 0..9+head sprite
#define		HUDY	150	// y coord of the starting y for the hud
#define		HUDX1	12	// x coord of first digit in hud
#define		HUDX2	20	// x coord of second digit in hud
#define		HUDX3	28	// x coord of third digit in hud
#define		HUDX4	200	// x coord of first head in hud
#define		HUDX5	210	// x coord of second head in hud
#define		HUDX6	220	// x coord of third head in hud

#define		TREESTART		6	// 0..TREESTART
								// Three for numbers on HUD, and three for heads on status bar
#define		TREEMAX			16	// TREESTART...TREEMAX	for the trees (10 trees)
#define		GUYNUM			17	// for our skiier (treemax+1)
#define		SHADOW_OFFSET	18	// GUYNUM+1....(TREESTART-TREEMAX+GUYNUM) for the shadow of the trees
#define		PASSED_GUY		28	// if the tree doesnt collide with our skiier, we need to draw
								// it after the skiier is done see DoGame() for more info
#define		NUMTREES		10	// TREEMAX-TREESTART

typedef struct tagSEntry		// structure for storing standard attributes about sprites
{
	u16 atr[3];
} SEntry;

typedef struct sOBJTree
{
	int x; // x coords of tree
	int y; // y coords of tree
	int type; // which one of NSOMETHING this is (NPINE or NTREE)
	int on; // 0 if off, gOAM[on] represents where this sprite is!, if on > 0;
	int shadow; // gOAM[shadow] is where the shadow is, if on !=0;
	int passed; // 0 if not passed skiier, 1 if tree is past skiier
} OBJTree;

//--------------------------------------------------------------------------------------------
//	Function Prototypes
//--------------------------------------------------------------------------------------------
void GBA_LoadSprites();
void GBA_LoadOBJPalette();

#endif
