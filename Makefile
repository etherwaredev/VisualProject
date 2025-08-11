# Note: This makefile should be ran within the VisualProject folder
# Note: You should run vinit.o outside of the .out folder

OSNAME = $(shell uname)

ifeq ($(OSNAME), Linux)
	UtilizedCOM = gcc vinit.c -L/usr/include/ -L/usr/lib64 -lGL -lGLEW -lglfw -lpthread -lX11 -lXrandr -lXi -ldl -lm -o .out/vinit.o
endif

ifeq ($(OSNAME), Darwin)
	UtilizedCOM = gcc vinit.c -L/usr/local/include/ -L/usr/lib -framework OpenGL -lGLEW -lglfw -lpthread -ldl -lm -o .out/vinit.o
	sed -i "1,1s/460/410/" Shaders/Shader.vert
	sed -i "1,1s/460/410/" Shaders/Shader.geo
	sed -i "1,1s/460/410/" Shaders/Shader.frag
endif

vinit.c: vinit.c
	-mkdir .out
	$(UtilizedCOM)
