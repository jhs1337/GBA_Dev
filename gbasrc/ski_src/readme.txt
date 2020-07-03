Skiing Advance ----------------------------------------------

A simple mode 4 game that was made for the Gameboy Advance
development competition of Feb 2002. We put it together in 
a period of about 18 hours, since we started on the day 
before the entries were due. Oh well, after we finished. 
I went back through the code and broke it up into 
multiple C files making it easier to read. Then I've 
also fixed a few bugs in collision detection, and some
other things. You can read more about this in the comments
at the top of the main.c file. 

Instructions.
Ski down the mountain and avoid running into any trees.
You can move left, and right while skiing. You have three 
lives, and your score is kept in the bottom left. 

I have included the dsp files for Visual C++ if you want
to use that as your development environment. When compiled 
the project runs the DOMAKE.BAT that is included.

If you dont have or want to use VisualC++ as your IDE, then
you can simply use DOMAKE.BAT, which is included. You can 
double click in from a windows browser, or run it from a 
dos command prompt. 

About the graphics. The tool pcx2gba.exe, and pcx2sprite.exe
included is made by Dovoto. It was part of the Pern tutorial 
he wrote. It has a few quircks like the name of the C structure
containing the data from the PCX for some reason cuts off
the first letter of the PCX filename, however the resulting
header file still has the right name. Wierd, anyhow the
resulting header files I use to build were hand edited a
little to correct these things, and also some small objects
were combined into one header file, via copy and paste.
You should be able to figure it out. 

Comments or Questions, then goto www.loirak.com or you
can email Ben Rhoades at benr@loirak.com

