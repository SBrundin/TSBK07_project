// Loader of TGA, PPM, PNG and JPEGTest of four texture loaders.
// Needs LoadTGA, readppm, pnglite and nanojpeg.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

#include "LoadTGA.h"
#include "readppm.h"
#include "pnglite.h"
#include "nanojpeg.h"

// Based on the demo that comes with nanojpeg
void * loadjpg(char* const filename, int *width, int *height, int *bpp)
{
    int size;
    char *buf;
    FILE *f;

    f = fopen(filename, "rb");
    if (!f)
    {
        printf("Error opening the input file.\n");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    size = (int) ftell(f);
    buf = (char*) malloc(size);
    fseek(f, 0, SEEK_SET);
    size = (int) fread(buf, 1, size, f);
    fclose(f);

    njInit();
    if (njDecode(buf, size))
    {
        free((void*)buf);
        printf("Error decoding the input file.\n");
        return 1;
    }
    free((void*)buf);

    if (!njIsColor)
    {
    }

    *width = njGetWidth();
    *height = njGetHeight();
    void *data = malloc(njGetImageSize());
    *bpp = njGetImageSize() / njGetHeight() / njGetWidth();

//    // Flip the data!
//	int line = njGetImageSize() / njGetHeight();
//	int nlines = njGetImageSize() / line;
//	void *input = njGetImage();
//	for (int i = 0; i < nlines; i++)
//	{
//	    memcpy(&data[line * i], &input[line * (nlines - i - 1)], line);
//	}
// The flipping responsability is moved to the caller.

    memcpy(data, njGetImage(), njGetImageSize());
    njDone();

    return data;
}


GLuint LoadTexture(char const *filename, char dorepeat)
{
	GLuint texID;
	char *ending = &filename[strlen(filename) - 4];
	char *data;
	int height, width, bpp;
	GLint fmt;
	printf("%s\n", ending);

	// Create texture reference and set parameters
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	if (dorepeat)
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	}

	// JPEG (using nanojpeg)
	if (strcmp(ending, ".jpg") == 0 || strcmp(ending, ".JPG") == 0 || strcmp(ending, "jpeg") == 0 || strcmp(ending, "JPEG") == 0)
	{
		data = loadjpg(filename, &width, &height, &bpp);
		fmt = GL_RGB;
		bpp = 3;
	}

	// PNG (using pnglite)
	if (strcmp(ending, ".png") == 0 || strcmp(ending, ".PNG") == 0)
	{
		png_t pnginfo;
		int err = png_init(NULL, NULL);

		err = png_open_file_read(&pnginfo, filename);
		if (err != 0)
		{
			printf("png_open_file_read error = %d\n", err);
			return 0;
		}
	//	png_print_info(&pnginfo);

		data = (unsigned char *)malloc(pnginfo.width * pnginfo.height * pnginfo.bpp);
		err = png_get_data(&pnginfo, data);
		if (err != 0)
			printf("png_get_data error = %d\n", err);

		width = pnginfo.width;
		height = pnginfo.height;
		bpp = pnginfo.bpp;

		switch (pnginfo.color_type)
		{
			case PNG_GREYSCALE:
				fmt = GL_LUMINANCE;break; // Untested!
			case PNG_TRUECOLOR:
				fmt = GL_RGB;break;
	// PNG_INDEXED ignored
			case PNG_GREYSCALE_ALPHA:
				fmt = GL_LUMINANCE_ALPHA;break; // Untested!
			case PNG_TRUECOLOR_ALPHA:
	//			printf("PNG_TRUECOLOR_ALPHA\n");
				fmt = GL_RGBA;break;
			default:
				fmt = GL_RGBA;
		}
	}

	// PPM
	if (strcmp(ending, ".ppm") == 0 || strcmp(ending, ".PPM") == 0)
	{
		data = readppm(filename, &width, &height);
		bpp = 3;

		printf("read ppm with size %d %d\n", width, height);
		// ALready flipped?
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE, data);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		return texID;
	}

	// TGA
	if (strcmp(ending, ".tga") == 0 || strcmp(ending, ".TGA") == 0)
	{
		LoadTGATextureSimple(filename, &texID);
		// Should already be flipped correctly
		return texID;
	}

	// flip!
	int line = width * bpp;
	int nlines = height;
	void *flippeddata = malloc(width * height * bpp);
	for (int i = 0; i < nlines; i++)
	{
	    memcpy(&flippeddata[line * i], &data[line * (nlines - i - 1)], line);
	}

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, fmt, GL_UNSIGNED_BYTE, flippeddata);
	glGenerateMipmap(GL_TEXTURE_2D);

	free(flippeddata);
	free(data);

	return texID;
}
