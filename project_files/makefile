# set this variable to the director in which you saved the common files


CCC = g++ -std=c++98 -W -Wall -lGL -lm -lGLU -DGL_GLEXT_PROTOTYPES
CCC2 = gcc -std=c99 -W -Wall -lGL -lm -DGL_GLEXT_PROTOTYPES

commondir = ../common/
commonfiles =  $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c

all : main

main : main.cc $(commonfiles)
	gcc -Wall -o main -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES main.cc $(commonfiles) -lXt -lX11 -lGL -lm

clean :
	rm main
