# set this variable to the director in which you saved the common files


CCC = g++ -std=c++98 -W -Wall -lGL -lm -lGLU -DGL_GLEXT_PROTOTYPES
CCC2 = gcc -std=c99 -W -Wall -lGL -lm -DGL_GLEXT_PROTOTYPES

commondir = ../common/

all : main

main : main.cc $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES main.cc $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


clean :
	rm main
