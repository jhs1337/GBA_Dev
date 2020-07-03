path=C:\devkitadv\bin

gcc  -o gbacaster.elf gbacaster.c -lm

rem gcc -c -mthumb -mthumb-interwork gbacaster.c
rem gcc -mthumb -mthumb-interwork -o gbacaster.elf gbacaster.o

objcopy -O binary gbacaster.elf gbacaster.gba

pause


