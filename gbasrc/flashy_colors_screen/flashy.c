/* hello.c - Gameboy Advance Tutorial - Loirak Development */
#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))

int main()
{
	char x,y;
	int i;
	unsigned short* Screen = (unsigned short*)0x6000000;
	*(unsigned long*)0x4000000 = 0x403; // mode3, bg2 on

	while(1){
	// clear screen, and draw a blue back ground
	for(x = 0; x<240;x++)   //loop through all x
	{
		for(y = 0; y<160; y++)  //loop through all y
		{
			Screen[x+y*240] = RGB16(0,0,31);
		}
	}
	//for(i=0; i<1000; i++){;}

for(x = 0; x<240;x++)   //loop through all x
	{
		for(y = 0; y<160; y++)  //loop through all y
		{
			Screen[x+y*240] = RGB16(0,31,0);
		}
	}
	//for(i=0; i<1000; i++){;}

for(x = 0; x<240;x++)   //loop through all x
	{
		for(y = 0; y<160; y++)  //loop through all y
		{
			Screen[x+y*240] = RGB16(31,0,0);
		}
	}
	//for(i=0; i<1000; i++){;}

for(x = 0; x<240;x++)   //loop through all x
	{
		for(y = 0; y<160; y++)  //loop through all y
		{
			Screen[x+y*240] = RGB16(31,31,0);
		}
	}
	//for(i=0; i<1000; i++){;}

for(x = 0; x<240;x++)   //loop through all x
	{
		for(y = 0; y<160; y++)  //loop through all y
		{
			Screen[x+y*240] = RGB16(31,31,31);
		}
	}
	for(i=0; i<1000; i++){;}

	// /* draw a white HI on the background
	// for(x = 20; x<=60; x+=15)
		//for(y = 30; y<50; y++)
			//Screen[x+y*240] = RGB16(0,0,0);
	//for (x = 20; x < 35; x++)
		//Screen[x+40*240] = RGB16(0,0,0);

	}	//loop forever
}
