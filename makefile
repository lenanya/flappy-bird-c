all: main main.exe

main: main.c
	clang main.c -o main -lraylib -lm -static

main.exe: main.c
	i686-w64-mingw32-gcc main.c -o main.exe -Iraylib/include -Lraylib/lib -lraylib -lwinmm -lopengl32 -lgdi32

