

build:
	clang main.c -o main -I. -I../src -L. -L../release/linux -lraylib -lglfw -lm -lpthread -ldl -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -g
	# clang main.c -o main -lraylib -lm -g

run: build
	./main data
