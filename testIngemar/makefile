# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab1-1


lab1-1 : lab1-1.cpp  LightHandler.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
		gcc -Wall -o main -I$(commondir) -I../common/Linux lab1-1.cpp LightHandler.cpp -DGL_GLEXT_PROTOTYPES $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++

clean :
	rm lab1-1
