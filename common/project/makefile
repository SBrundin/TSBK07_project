# set this variable to the director in which you saved the common files

CPP = g++ -std=c++98 -W -Wall -lGL -lm -DGL_GLEXT_PROTOTYPES

CCC2 = gcc -std=c99 -W -Wall -lGL -lm -DGL_GLEXT_PROTOTYPES
commondir = ../common/



main : $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
				$(CPP) -o main main.o -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11

main.o : main.cc
			$(CPP) -c main.cc



clean :
	rm main.o
