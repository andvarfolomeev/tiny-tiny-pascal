nasm -fwin32 ../tests/generator_out.asm -o ../tests/generator_out.obj
c:/mingw/bin/gcc.exe -m32 -g ../tests/generator_out.obj -o ../tests/generator_out.exe
../tests/generator_out.exe