vinit.c:
	# Note: This make file should be ran within the VisualProject folder
	# Note: You should run vinit.o outside of the .out folder
	-mkdir .out
	gcc vinit.c -L/usr/include/ -L/usr/lib64 -lGL -lGLEW -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lm -o .out/vinit.o
