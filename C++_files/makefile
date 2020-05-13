# set this variable to the director in which you saved the common files
commondir = ../common/
texLoaders =  $(commondir)LoadTexture.c $(commondir)readppm.c $(commondir)nanojpeg.c $(commondir)zpr.c $(commondir)LoadTextureCombo.c $(commondir)pnglite.c

all : main



main : Main.cpp Fundamentals.cpp Camera.cpp Object.cpp LightSource.cpp Book.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o main -I$(commondir) -I../common/Linux Main.cpp Fundamentals.cpp Camera.cpp Object.cpp LightSource.cpp Book.cpp -DGL_GLEXT_PROTOTYPES $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm -lstdc++



clean :
	rm main
