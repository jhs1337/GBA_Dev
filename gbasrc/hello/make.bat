set path=C:\devkitadv\bin;%path%
gcc -o hello.elf hello.c -lm
objcopy -O binary hello.elf hello.gba
pause