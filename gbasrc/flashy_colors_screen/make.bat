set path=C:\devkitadv\bin;%path%
gcc -o flashy.elf flashy.c -lm
objcopy -O binary flashy.elf flashy.gba
pause