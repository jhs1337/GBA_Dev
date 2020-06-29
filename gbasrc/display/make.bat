path=C:\devkitadv\bin

gcc  -o display.elf display.c

objcopy -O binary display.elf display.gba

pause


