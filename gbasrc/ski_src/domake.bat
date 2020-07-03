path=C:\devkitadv\bin

REM compiling with below line makes things run very slow
REM gcc  -o skiing.elf main.c mysprite.c sprite.c gfx.c -lm

gcc -c -O3 -mthumb -mthumb-interwork gfx.c
gcc -c -O3 -mthumb -mthumb-interwork sprite.c
gcc -c -O3 -mthumb -mthumb-interwork mysprite.c
gcc -c -O3 -mthumb -mthumb-interwork main.c

gcc -mthumb -mthumb-interwork -o skiing.elf main.o gfx.o mysprite.o sprite.o

objcopy -O binary skiing.elf skiing.gba

pause
