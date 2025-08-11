# Note: This makefile should be ran within the VisualProject folder
# Note: You should run vinit.o outside of the .out folder

OSNAME = $(shell uname)

ifeq ($(OSNAME), Linux)
	UtilizedCOM = gcc vinit.c -L/usr/include/ -L/usr/lib64 -lGL -lGLEW -lglfw -lpthread -lX11 -lXrandr -lXi -ldl -lm -o .out/vinit.o
endif

ifeq ($(OSNAME), Darwin)
	UtilizedCOM = gcc vinit.c -L/usr/local/include/ -L/usr/lib -framework OpenGL -lGLEW -lglfw -lpthread -ldl -lm -o .out/vinit.o
endif

vinit.c: vinit.c
	-mkdir .out
	$(UtilizedCOM)

ifeq ($(OSNAME), Darwin)
	# Note: Sed requires suffix arguments for -i on MacOS
	sed -i=.bak "1,1s/460/410/" Shaders/Shader.vert
	sed -i=.bak "1,1s/460/410/" Shaders/Shader.geo
	sed -i=.bak "1,1s/460/410/" Shaders/Shader.frag
endif
