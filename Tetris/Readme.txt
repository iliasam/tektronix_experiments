You can copy "tetris.app" and "tetris.o" to a Floppy disk 
and start tetris.app from oscilloscope Save/Recall - SETUP menu.

--------------------------------------------

tetris.o was build using:
- Binutils from here: https://gist.github.com/WillSams/f592f9d494b51119945440f7e91079b0 (build with key --target=m68k-unknown-aout)
[GNU assembler version 2.41 (x86_64-w64-mingw32)]

- GCC: m68k-elf-gcc4.8.0 - https://gnutoolchains.com/m68k-elf/

--------------------------------------------

Compiling:
C:\sysgcc\m68k-elf\bin\m68k-elf-gcc.exe D:\{path_to_file}\tetris.c -o D:\{path_to_file}\tetris.s -S  

Remove ".section" words from "tetris.s" file.
Replace ".rodata" with ".data" in  "tetris.s" file.

Assembler in MSYS2:
cd /opt/m68k/bin/
./m68k-unknown-aout-as.exe tetris.s