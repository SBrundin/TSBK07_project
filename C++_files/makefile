# set this variable to the director in which you saved the common files
commondir = ../common/

all : main


main : Main.cpp Fundamentals.cpp Camera.cpp Object.cpp AndreasClass.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux Main.cpp Fundamentals.cpp Camera.cpp Object.cpp AndreasClass.cpp -DGL_GLEXT_PROTOTYPES $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++


clean :
	rm main
