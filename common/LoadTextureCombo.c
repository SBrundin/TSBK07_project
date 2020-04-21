// Loader of TGA, PPM, PNG and JPEGTest of four texture loaders.
// Has the entire LoadTGA, readppm, pnglite and nanojpeg copied into a single file.
// Their respective copyright notices are retained in the file, see below!

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"

#include "LoadTextureCombo.h"

//#include "LoadTGA.h"
//#include "readppm.h"
//#include "pnglite.h"
//#include "nanojpeg.h"

#include "zlib.h"
//#else
//#include "zlite.h"
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





// LoadTGA By Ingemar Ragnemalm

#ifndef __cplusplus
	#ifndef true
	#define true 1
	#endif

	#ifndef false
	#define false 0
	#endif

	#ifndef bool
	#define bool char
	#endif
#endif

typedef struct TextureData		// Create A Structure for .tga loading.
{
	GLubyte	*imageData;			// Image Data (Up To 32 Bits)
	GLuint	bpp;				// Image Color Depth In Bits Per Pixel.
	GLuint	width;				// Image Width
	GLuint	height;				// Image Height
//	GLuint	w;				// Image Width "raw"
//	GLuint	h;				// Image Height "raw"
	GLuint	texID;				// Texture ID Used To Select A Texture
//	GLfloat	texWidth, texHeight;
} TextureData, *TextureDataPtr;					// Structure Name

bool LoadTGATexture(char *filename, TextureData *texture);
void LoadTGATextureSimple(char *filename, GLuint *tex);
void LoadTGASetMipmapping(bool active);
bool LoadTGATextureData(char *filename, TextureData *texture);

// Constants for SaveTGA
#define	TGA_ERROR_FILE_OPEN				-5
#define TGA_ERROR_READING_FILE			-4
#define TGA_ERROR_INDEXED_COLOR			-3
#define TGA_ERROR_MEMORY				-2
#define TGA_ERROR_COMPRESSED_FILE		-1
#define TGA_OK							 0

// Save functions
int SaveDataToTGA(char			*filename,
			 short int		width,
			 short int		height,
			 unsigned char	pixelDepth,
			 unsigned char	*imageData);
void SaveTGA(TextureData *tex, char *filename);
void SaveFramebufferToTGA(char *filename, GLint x, GLint y, GLint w, GLint h);















// TGA loader, by Ingemar 2009, based on "tga.c" and some other sources.
// Updated 2013 to use modern mipmapping
// 130423: Added LoadTGATextureData
// 130905: Fixed a bug in LoadTGATextureSimple
// 140212: Fixed a bug in loading of uncompressed images.
// 140520: Supports grayscale images (loads to red channel).
// 141007: Added SaveTGA. (Moved from the obsolete LoadTGA2.)
// 160302: Uses fopen_s on Windows, as suggested by Jesper Post. Should reduce warnings a bit.
// 170220: Changed fopen_s to "rb". This made it fail for some textures.
// 170331: Cleaned up a bit to remove warnings.
// 170419: Fixed a bug that prevented monochrome images from loading.
// 180216: calloc is better than malloc when you may have undefined data in parts of the texture!
// 190416: Skipped testing the last four bytes of the 12-byte heading. That is the origin of the
// image which is usually 0. It is now ignored.
// 191112: Revised: Corrected the "flipped" support. Removed the system for loading
// textures 1:1 with power of 2 size. It mostly causes confusion.

// NOTE: LoadTGA does NOT support all TGA variants! You may need to re-save your TGA
// with different settings to find a suitable format.

static bool gMipmap = true;

// Note that turning mimpapping on and off refers to the loading stage only.
// If you want to turn off mipmapping later, use
// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

void LoadTGASetMipmapping(bool active)
{
	gMipmap = active;
}

bool LoadTGATextureData(char *filename, TextureData *texture)	// Loads A TGA File Into Memory
{
	GLuint i;
	GLubyte
		TGAuncompressedheader[12]={ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Uncompressed TGA Header
		TGAcompressedheader[12]={ 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Compressed TGA Header
		TGAuncompressedbwheader[12]={ 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Uncompressed grayscale TGA Header
		TGAcompressedbwheader[12]={ 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Compressed grayscale TGA Header
		actualHeader[12],	// Used To Compare TGA Header
		header[6];		// First 6 Useful Bytes From The Header
	GLuint bytesPerPixel,		// Holds Number Of Bytes Per Pixel Used In The TGA File
		imageSize,		// Used To Store The Image Size When Setting Aside Ram
		temp;			// Temporary Variable
	long rowSize, stepSize, bytesRead;
//	long w, h;
	GLubyte *rowP;
	int err;
	GLubyte rle;
	int b;
	long row, rowLimit;
	GLubyte pixelData[4];

	// Nytt f�r flipping-st�d 111114
	char flipped;
	long step;

	// It seems Windows/VS doesn't like fopen any more, but fopen_s is not on the others.
	FILE *file;
	#if defined(_WIN32)
		fopen_s(&file, filename, "rb");
	#else
		file = fopen(filename, "rb"); // rw works everywhere except Windows?
	#endif
//	FILE *file = fopen(filename, "rb");			// Open The TGA File
	err = 0;
	if (file == NULL) err = 1;				// Does File Even Exist?
	else if (fread(actualHeader, 1, sizeof(actualHeader), file) != sizeof(actualHeader)) err = 2; // Are There 12 Bytes To Read?
	else if (
	// Edit 2019: Ignore the origin!
				(memcmp(TGAuncompressedheader, actualHeader, sizeof(TGAuncompressedheader)-4) != 0) &&
				(memcmp(TGAcompressedheader, actualHeader, sizeof(TGAcompressedheader)-4) != 0) &&
				(memcmp(TGAuncompressedbwheader, actualHeader, sizeof(TGAuncompressedheader)-4) != 0) &&
				(memcmp(TGAcompressedbwheader, actualHeader, sizeof(TGAcompressedheader)-4) != 0)
			)
			{
				err = 3; // Does The Header Match What We Want?
				for (i = 0; i < 12; i++)
					printf("%d ", actualHeader[i]);
				printf("\n");
			}
	else if (fread(header, 1, sizeof(header), file) != sizeof(header)) err = 4; // If So Read Next 6 Header Bytes

	if (err != 0)
	{
		switch (err)
		{
			case 1: printf("could not open file %s\n", filename); break;
			case 2: printf("could not read header of %s\n", filename); break;
			case 3: printf("unsupported format in %s\n", filename); break;
			case 4: printf("could not read file %s\n", filename); break;
		}

		if(file == NULL)		// Did The File Even Exist? *Added Jim Strong*
			return false;
		else
		{
			fclose(file);		// If Anything Failed, Close The File
			return false;
		}
	}
	texture->width  = header[1] * 256 + header[0];	// Determine The TGA Width (highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];	// Determine The TGA Height (highbyte*256+lowbyte)
	if (texture->width <= 0 ||	// Is The Width Less Than Or Equal To Zero
	texture->height <= 0 ||		// Is The Height Less Than Or Equal To Zero
	(header[4] != 24 && header[4] != 32 && header[4] != 8))			// Is The TGA 24 or 32 Bit?
	{
		fclose(file);		// If Anything Failed, Close The File
		return false;
	}
	flipped = (header[5] & 32) != 0; // Testa om flipped

//	w = 1;
//	while (w < texture->width) w = w << 1;
//	h = 1;
//	while (h < texture->height) h = h << 1;
//	texture->texWidth = (GLfloat)texture->width / w;
//	texture->texHeight = (GLfloat)texture->height / h;


	texture->bpp = header[4];		// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp/8;		// Divide By 8 To Get The Bytes Per Pixel
//	imageSize = w * h * bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	imageSize = texture->width * texture->height * bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	rowSize	= texture->width * bytesPerPixel;	// Image memory per row
//	stepSize = w * bytesPerPixel;		// Memory per row
	stepSize = texture->width * bytesPerPixel;		// Memory per row
	texture->imageData = (GLubyte *)calloc(1, imageSize);	// Reserve Memory To Hold The TGA Data
	if (texture->imageData == NULL)				// Does The Storage Memory Exist?
	{
		fclose(file);
		return false;
	}

	// Set rowP and step depending on direction
	// Inverted this 120131, since a texture came out wrong.
	// I am still not sure this is OK.
	// Inverted again 191112 because this
	if (flipped)
	{
		step = -stepSize;
		rowP = &texture->imageData[imageSize - stepSize];
		row = 0 + (texture->height -1) * stepSize;
	}
	else
	{
		step = stepSize;
		rowP = &texture->imageData[0];
		row = 0;
	}

	if (actualHeader[2] == 2 || actualHeader[2] == 3) // uncompressed
	{
		for (i = 0; i < texture->height; i++)
		{
			bytesRead = fread(rowP, 1, rowSize, file);
			rowP += step;
			if (bytesRead != rowSize)
			{
				free(texture->imageData);	// If So, Release The Image Data
				fclose(file);			// Close The File
				return false;			// Return False
			}
		}
	}
	else
	{ // compressed
//		row = 0 + (texture->height -1) * stepSize;
		i = row;
		rowLimit = row + rowSize;
		do
		{
			bytesRead = fread(&rle, 1, 1, file);
			if (rle < 128)
			{ // rle+1 raw pixels
				bytesRead = fread(&texture->imageData[i], 1, (rle+1)*bytesPerPixel, file);
				i += bytesRead;
				if (bytesRead == 0)
					i = imageSize;
			}
			else
			{ // range of rle-127 pixels with a color that follows
				bytesRead = fread(&pixelData, 1, bytesPerPixel, file);
				do
				{
					for (b = 0; b < bytesPerPixel; b++)
						texture->imageData[i+b] = pixelData[b];
					i += bytesPerPixel;
					rle = rle - 1;
				} while (rle > 127);
			}
			if (i >= rowLimit)
			{
				row = row + step; // - stepSize;
				rowLimit = row + rowSize;
				i = row;
			}
		} while (i < imageSize);
	}

	if (bytesPerPixel >= 3) // if not monochrome
	for (i = 0; i < (int)(imageSize); i += bytesPerPixel)	// Loop Through The Image Data
	{		// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];		// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;	// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	fclose (file);

//texture->w = w;
//texture->h = h;

	return true;				// Texture loading Went Ok, Return True
}

bool LoadTGATexture(char *filename, TextureData *texture)	// Loads A TGA File Into Memory and creates texture object
{
	char ok;
	GLuint type = GL_RGBA;		// Set The Default GL Mode To RBGA (32 BPP)

	ok = LoadTGATextureData(filename, texture);	// Loads A TGA File Into Memory
	if (!ok)
		return false;

	// Build A Texture From The Data
	glGenTextures(1, &texture->texID);			// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, texture->texID);		// Bind Our Texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	if (texture->bpp == 8)						// Was The TGA 8 Bits? Should be grayscale then.
	{
		type=GL_RED;			// If So Set The 'type' To GL_RED
	}
	if (texture->bpp == 24)						// Was The TGA 24 Bits?
	{
		type=GL_RGB;			// If So Set The 'type' To GL_RGB
	}
//	glTexImage2D(GL_TEXTURE_2D, 0, type, texture->w, texture->h, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);
	glTexImage2D(GL_TEXTURE_2D, 0, type, texture->width, texture->height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	if (gMipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filtered
	}

	return true;				// Texture Building Went Ok, Return True
}

void LoadTGATextureSimple(char *filename, GLuint *tex) // If you really only need the texture object.
{
	TextureData texture;
	memset(&texture, 0, sizeof(texture)); // Bug fix 130905.

	if (LoadTGATexture(filename, &texture))
	{
		if(texture.imageData != NULL)
			free(texture.imageData);
		*tex = texture.texID;
	}
	else
		*tex = 0;
}


// saves an array of pixels as a TGA image
// Was tgaSave, found in some reusable code.
// Limitation: Can NOT save with transparency! Only RGB, not RGBA!
// But it should! Why not?
int SaveDataToTGA(char			*filename,
			 short int		width,
			 short int		height,
			 unsigned char	pixelDepth,
			 unsigned char	*imageData)
{
	unsigned char cGarbage = 0, mode,aux;
	int i, w, ix;
	FILE *file;
	char /*GLubyte*/ TGAuncompressedheader[12]={ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// Uncompressed TGA Header

// open file and check for errors
	file = fopen(filename, "w");
	if (file == NULL)
		return(TGA_ERROR_FILE_OPEN);

// compute image type: 2 for RGB(A), 3 for greyscale
	mode = pixelDepth / 8;
// NOT YET IMPLEMENTED
//	if ((pixelDepth == 24) || (pixelDepth == 32))
//		type = 2;
//	else
//		type = 3;

// write the header
	fwrite(&TGAuncompressedheader, 12, 1, file);
	fwrite(&width, sizeof(short int), 1, file);
	fwrite(&height, sizeof(short int), 1, file);
	fwrite(&pixelDepth, sizeof(unsigned char), 1, file);

	fwrite(&cGarbage, sizeof(unsigned char), 1, file);

// convert the image data from RGB(a) to BGR(A)
	if (mode >= 3)
	for (i=0; i < width * height * mode ; i+= mode)
	{
		aux = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = aux;
	}

// save the image data
	w = 1;
	while (w < width) w = w << 1;
//	bytesPerPixel = pixelDepth/8;
//	row = width * bytesPerPixel;

// Write one row at a time
	for (i = 0; i < height; i++)
	{
		ix = i * w * mode;
		fwrite(&imageData[ix], sizeof(unsigned char), width * mode, file);
	}

	fclose(file);
// release the memory
	free(imageData);

	return(TGA_OK);
}

// Save a TextureData
// Problem: Saves upside down!
void SaveTGA(TextureData *tex, char *filename)
{
	SaveDataToTGA(filename, tex->width, tex->height,
			tex->bpp, tex->imageData);
}

void SaveFramebufferToTGA(char *filename, GLint x, GLint y, GLint w, GLint h)
{
	int err;
	void *buffer = malloc(h*w*3);
	glReadPixels(x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
	err = SaveDataToTGA(filename, w, h,
			3*8,  (unsigned char *)buffer);
//	free(buffer); already done
//	printf("SaveDataToTGA returned %d\n", err);
}














// NanoJPEG -- KeyJ's Tiny Baseline JPEG Decoder
// version 1.3.5 (2016-11-14)
// Copyright (c) 2009-2016 Martin J. Fiedler <martin.fiedler@gmx.net>
// published under the terms of the MIT license
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.


///////////////////////////////////////////////////////////////////////////////
// DOCUMENTATION SECTION                                                     //
// read this if you want to know what this is all about                      //
///////////////////////////////////////////////////////////////////////////////

// INTRODUCTION
// ============
//
// This is a minimal decoder for baseline JPEG images. It accepts memory dumps
// of JPEG files as input and generates either 8-bit grayscale or packed 24-bit
// RGB images as output. It does not parse JFIF or Exif headers; all JPEG files
// are assumed to be either grayscale or YCbCr. CMYK or other color spaces are
// not supported. All YCbCr subsampling schemes with power-of-two ratios are
// supported, as are restart intervals. Progressive or lossless JPEG is not
// supported.
// Summed up, NanoJPEG should be able to decode all images from digital cameras
// and most common forms of other non-progressive JPEG images.
// The decoder is not optimized for speed, it's optimized for simplicity and
// small code. Image quality should be at a reasonable level. A bicubic chroma
// upsampling filter ensures that subsampled YCbCr images are rendered in
// decent quality. The decoder is not meant to deal with broken JPEG files in
// a graceful manner; if anything is wrong with the bitstream, decoding will
// simply fail.
// The code should work with every modern C compiler without problems and
// should not emit any warnings. It uses only (at least) 32-bit integer
// arithmetic and is supposed to be endianness independent and 64-bit clean.
// However, it is not thread-safe.


// COMPILE-TIME CONFIGURATION
// ==========================
//
// The following aspects of NanoJPEG can be controlled with preprocessor
// defines:
//
// _NJ_EXAMPLE_PROGRAM     = Compile a main() function with an example
//                           program.
// _NJ_INCLUDE_HEADER_ONLY = Don't compile anything, just act as a header
//                           file for NanoJPEG. Example:
//                               #define _NJ_INCLUDE_HEADER_ONLY
//                               #include "nanojpeg.c"
//                               int main(void) {
//                                   njInit();
//                                   // your code here
//                                   njDone();
//                               }
// NJ_USE_LIBC=1           = Use the malloc(), free(), memset() and memcpy()
//                           functions from the standard C library (default).
// NJ_USE_LIBC=0           = Don't use the standard C library. In this mode,
//                           external functions njAlloc(), njFreeMem(),
//                           njFillMem() and njCopyMem() need to be defined
//                           and implemented somewhere.
// NJ_USE_WIN32=0          = Normal mode (default).
// NJ_USE_WIN32=1          = If compiling with MSVC for Win32 and
//                           NJ_USE_LIBC=0, NanoJPEG will use its own
//                           implementations of the required C library
//                           functions (default if compiling with MSVC and
//                           NJ_USE_LIBC=0).
// NJ_CHROMA_FILTER=1      = Use the bicubic chroma upsampling filter
//                           (default).
// NJ_CHROMA_FILTER=0      = Use simple pixel repetition for chroma upsampling
//                           (bad quality, but faster and less code).


// API
// ===
//
// For API documentation, read the "header section" below.

// Changes by Ingemar:
// The demo was removed, and changed to a loader that is in LoadTecture.c.
// The header was put in a separate file.
// No other changes were made. In particular, the license above is unchanged.
// nj_result_t: Result codes for njDecode().

typedef enum _nj_result {
    NJ_OK = 0,        // no error, decoding successful
    NJ_NO_JPEG,       // not a JPEG file
    NJ_UNSUPPORTED,   // unsupported format
    NJ_OUT_OF_MEM,    // out of memory
    NJ_INTERNAL_ERR,  // internal error
    NJ_SYNTAX_ERROR,  // syntax error
    __NJ_FINISHED,    // used internally, will never be reported
} nj_result_t;


///////////////////////////////////////////////////////////////////////////////
// CONFIGURATION SECTION                                                     //
// adjust the default settings for the NJ_ defines here                      //
///////////////////////////////////////////////////////////////////////////////

#ifndef NJ_USE_LIBC
    #define NJ_USE_LIBC 1
#endif

#ifndef NJ_USE_WIN32
  #ifdef _MSC_VER
    #define NJ_USE_WIN32 (!NJ_USE_LIBC)
  #else
    #define NJ_USE_WIN32 0
  #endif
#endif

#ifndef NJ_CHROMA_FILTER
    #define NJ_CHROMA_FILTER 1
#endif


///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION SECTION                                                    //
// you may stop reading here                                                 //
///////////////////////////////////////////////////////////////////////////////

#ifndef _NJ_INCLUDE_HEADER_ONLY

#ifdef _MSC_VER
    #define NJ_INLINE static __inline
    #define NJ_FORCE_INLINE static __forceinline
#else
    #define NJ_INLINE static inline
    #define NJ_FORCE_INLINE static inline
#endif

#if NJ_USE_LIBC
    #include <stdlib.h>
    #include <string.h>
    #define njAllocMem malloc
    #define njFreeMem  free
    #define njFillMem  memset
    #define njCopyMem  memcpy
#elif NJ_USE_WIN32
    #include <windows.h>
    #define njAllocMem(size) ((void*) LocalAlloc(LMEM_FIXED, (SIZE_T)(size)))
    #define njFreeMem(block) ((void) LocalFree((HLOCAL) block))
    NJ_INLINE void njFillMem(void* block, unsigned char value, int count) { __asm {
        mov edi, block
        mov al, value
        mov ecx, count
        rep stosb
    } }
    NJ_INLINE void njCopyMem(void* dest, const void* src, int count) { __asm {
        mov edi, dest
        mov esi, src
        mov ecx, count
        rep movsb
    } }
#else
    extern void* njAllocMem(int size);
    extern void njFreeMem(void* block);
    extern void njFillMem(void* block, unsigned char byte, int size);
    extern void njCopyMem(void* dest, const void* src, int size);
#endif

typedef struct _nj_code {
    unsigned char bits, code;
} nj_vlc_code_t;

typedef struct _nj_cmp {
    int cid;
    int ssx, ssy;
    int width, height;
    int stride;
    int qtsel;
    int actabsel, dctabsel;
    int dcpred;
    unsigned char *pixels;
} nj_component_t;

typedef struct _nj_ctx {
    nj_result_t error;
    const unsigned char *pos;
    int size;
    int length;
    int width, height;
    int mbwidth, mbheight;
    int mbsizex, mbsizey;
    int ncomp;
    nj_component_t comp[3];
    int qtused, qtavail;
    unsigned char qtab[4][64];
    nj_vlc_code_t vlctab[4][65536];
    int buf, bufbits;
    int block[64];
    int rstinterval;
    unsigned char *rgb;
} nj_context_t;

static nj_context_t nj;

static const char njZZ[64] = { 0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18,
11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35,
42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45,
38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63 };

NJ_FORCE_INLINE unsigned char njClip(const int x) {
    return (x < 0) ? 0 : ((x > 0xFF) ? 0xFF : (unsigned char) x);
}

#define W1 2841
#define W2 2676
#define W3 2408
#define W5 1609
#define W6 1108
#define W7 565

NJ_INLINE void njRowIDCT(int* blk) {
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;
    if (!((x1 = blk[4] << 11)
        | (x2 = blk[6])
        | (x3 = blk[2])
        | (x4 = blk[1])
        | (x5 = blk[7])
        | (x6 = blk[5])
        | (x7 = blk[3])))
    {
        blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
        return;
    }
    x0 = (blk[0] << 11) + 128;
    x8 = W7 * (x4 + x5);
    x4 = x8 + (W1 - W7) * x4;
    x5 = x8 - (W1 + W7) * x5;
    x8 = W3 * (x6 + x7);
    x6 = x8 - (W3 - W5) * x6;
    x7 = x8 - (W3 + W5) * x7;
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2);
    x2 = x1 - (W2 + W6) * x2;
    x3 = x1 + (W2 - W6) * x3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;
    blk[0] = (x7 + x1) >> 8;
    blk[1] = (x3 + x2) >> 8;
    blk[2] = (x0 + x4) >> 8;
    blk[3] = (x8 + x6) >> 8;
    blk[4] = (x8 - x6) >> 8;
    blk[5] = (x0 - x4) >> 8;
    blk[6] = (x3 - x2) >> 8;
    blk[7] = (x7 - x1) >> 8;
}

NJ_INLINE void njColIDCT(const int* blk, unsigned char *out, int stride) {
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;
    if (!((x1 = blk[8*4] << 8)
        | (x2 = blk[8*6])
        | (x3 = blk[8*2])
        | (x4 = blk[8*1])
        | (x5 = blk[8*7])
        | (x6 = blk[8*5])
        | (x7 = blk[8*3])))
    {
        x1 = njClip(((blk[0] + 32) >> 6) + 128);
        for (x0 = 8;  x0;  --x0) {
            *out = (unsigned char) x1;
            out += stride;
        }
        return;
    }
    x0 = (blk[0] << 8) + 8192;
    x8 = W7 * (x4 + x5) + 4;
    x4 = (x8 + (W1 - W7) * x4) >> 3;
    x5 = (x8 - (W1 + W7) * x5) >> 3;
    x8 = W3 * (x6 + x7) + 4;
    x6 = (x8 - (W3 - W5) * x6) >> 3;
    x7 = (x8 - (W3 + W5) * x7) >> 3;
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2) + 4;
    x2 = (x1 - (W2 + W6) * x2) >> 3;
    x3 = (x1 + (W2 - W6) * x3) >> 3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;
    *out = njClip(((x7 + x1) >> 14) + 128);  out += stride;
    *out = njClip(((x3 + x2) >> 14) + 128);  out += stride;
    *out = njClip(((x0 + x4) >> 14) + 128);  out += stride;
    *out = njClip(((x8 + x6) >> 14) + 128);  out += stride;
    *out = njClip(((x8 - x6) >> 14) + 128);  out += stride;
    *out = njClip(((x0 - x4) >> 14) + 128);  out += stride;
    *out = njClip(((x3 - x2) >> 14) + 128);  out += stride;
    *out = njClip(((x7 - x1) >> 14) + 128);
}

#define njThrow(e) do { nj.error = e; return; } while (0)
#define njCheckError() do { if (nj.error) return; } while (0)

static int njShowBits(int bits) {
    unsigned char newbyte;
    if (!bits) return 0;
    while (nj.bufbits < bits) {
        if (nj.size <= 0) {
            nj.buf = (nj.buf << 8) | 0xFF;
            nj.bufbits += 8;
            continue;
        }
        newbyte = *nj.pos++;
        nj.size--;
        nj.bufbits += 8;
        nj.buf = (nj.buf << 8) | newbyte;
        if (newbyte == 0xFF) {
            if (nj.size) {
                unsigned char marker = *nj.pos++;
                nj.size--;
                switch (marker) {
                    case 0x00:
                    case 0xFF:
                        break;
                    case 0xD9: nj.size = 0; break;
                    default:
                        if ((marker & 0xF8) != 0xD0)
                            nj.error = NJ_SYNTAX_ERROR;
                        else {
                            nj.buf = (nj.buf << 8) | marker;
                            nj.bufbits += 8;
                        }
                }
            } else
                nj.error = NJ_SYNTAX_ERROR;
        }
    }
    return (nj.buf >> (nj.bufbits - bits)) & ((1 << bits) - 1);
}

NJ_INLINE void njSkipBits(int bits) {
    if (nj.bufbits < bits)
        (void) njShowBits(bits);
    nj.bufbits -= bits;
}

NJ_INLINE int njGetBits(int bits) {
    int res = njShowBits(bits);
    njSkipBits(bits);
    return res;
}

NJ_INLINE void njByteAlign(void) {
    nj.bufbits &= 0xF8;
}

static void njSkip(int count) {
    nj.pos += count;
    nj.size -= count;
    nj.length -= count;
    if (nj.size < 0) nj.error = NJ_SYNTAX_ERROR;
}

NJ_INLINE unsigned short njDecode16(const unsigned char *pos) {
    return (pos[0] << 8) | pos[1];
}

static void njDecodeLength(void) {
    if (nj.size < 2) njThrow(NJ_SYNTAX_ERROR);
    nj.length = njDecode16(nj.pos);
    if (nj.length > nj.size) njThrow(NJ_SYNTAX_ERROR);
    njSkip(2);
}

NJ_INLINE void njSkipMarker(void) {
    njDecodeLength();
    njSkip(nj.length);
}

NJ_INLINE void njDecodeSOF(void) {
    int i, ssxmax = 0, ssymax = 0;
    nj_component_t* c;
    njDecodeLength();
    njCheckError();
    if (nj.length < 9) njThrow(NJ_SYNTAX_ERROR);
    if (nj.pos[0] != 8) njThrow(NJ_UNSUPPORTED);
    nj.height = njDecode16(nj.pos+1);
    nj.width = njDecode16(nj.pos+3);
    if (!nj.width || !nj.height) njThrow(NJ_SYNTAX_ERROR);
    nj.ncomp = nj.pos[5];
    njSkip(6);
    switch (nj.ncomp) {
        case 1:
        case 3:
            break;
        default:
            njThrow(NJ_UNSUPPORTED);
    }
    if (nj.length < (nj.ncomp * 3)) njThrow(NJ_SYNTAX_ERROR);
    for (i = 0, c = nj.comp;  i < nj.ncomp;  ++i, ++c) {
        c->cid = nj.pos[0];
        if (!(c->ssx = nj.pos[1] >> 4)) njThrow(NJ_SYNTAX_ERROR);
        if (c->ssx & (c->ssx - 1)) njThrow(NJ_UNSUPPORTED);  // non-power of two
        if (!(c->ssy = nj.pos[1] & 15)) njThrow(NJ_SYNTAX_ERROR);
        if (c->ssy & (c->ssy - 1)) njThrow(NJ_UNSUPPORTED);  // non-power of two
        if ((c->qtsel = nj.pos[2]) & 0xFC) njThrow(NJ_SYNTAX_ERROR);
        njSkip(3);
        nj.qtused |= 1 << c->qtsel;
        if (c->ssx > ssxmax) ssxmax = c->ssx;
        if (c->ssy > ssymax) ssymax = c->ssy;
    }
    if (nj.ncomp == 1) {
        c = nj.comp;
        c->ssx = c->ssy = ssxmax = ssymax = 1;
    }
    nj.mbsizex = ssxmax << 3;
    nj.mbsizey = ssymax << 3;
    nj.mbwidth = (nj.width + nj.mbsizex - 1) / nj.mbsizex;
    nj.mbheight = (nj.height + nj.mbsizey - 1) / nj.mbsizey;
    for (i = 0, c = nj.comp;  i < nj.ncomp;  ++i, ++c) {
        c->width = (nj.width * c->ssx + ssxmax - 1) / ssxmax;
        c->height = (nj.height * c->ssy + ssymax - 1) / ssymax;
        c->stride = nj.mbwidth * c->ssx << 3;
        if (((c->width < 3) && (c->ssx != ssxmax)) || ((c->height < 3) && (c->ssy != ssymax))) njThrow(NJ_UNSUPPORTED);
        if (!(c->pixels = (unsigned char*) njAllocMem(c->stride * nj.mbheight * c->ssy << 3))) njThrow(NJ_OUT_OF_MEM);
    }
    if (nj.ncomp == 3) {
        nj.rgb = (unsigned char*) njAllocMem(nj.width * nj.height * nj.ncomp);
        if (!nj.rgb) njThrow(NJ_OUT_OF_MEM);
    }
    njSkip(nj.length);
}

NJ_INLINE void njDecodeDHT(void) {
    int codelen, currcnt, remain, spread, i, j;
    nj_vlc_code_t *vlc;
    static unsigned char counts[16];
    njDecodeLength();
    njCheckError();
    while (nj.length >= 17) {
        i = nj.pos[0];
        if (i & 0xEC) njThrow(NJ_SYNTAX_ERROR);
        if (i & 0x02) njThrow(NJ_UNSUPPORTED);
        i = (i | (i >> 3)) & 3;  // combined DC/AC + tableid value
        for (codelen = 1;  codelen <= 16;  ++codelen)
            counts[codelen - 1] = nj.pos[codelen];
        njSkip(17);
        vlc = &nj.vlctab[i][0];
        remain = spread = 65536;
        for (codelen = 1;  codelen <= 16;  ++codelen) {
            spread >>= 1;
            currcnt = counts[codelen - 1];
            if (!currcnt) continue;
            if (nj.length < currcnt) njThrow(NJ_SYNTAX_ERROR);
            remain -= currcnt << (16 - codelen);
            if (remain < 0) njThrow(NJ_SYNTAX_ERROR);
            for (i = 0;  i < currcnt;  ++i) {
                register unsigned char code = nj.pos[i];
                for (j = spread;  j;  --j) {
                    vlc->bits = (unsigned char) codelen;
                    vlc->code = code;
                    ++vlc;
                }
            }
            njSkip(currcnt);
        }
        while (remain--) {
            vlc->bits = 0;
            ++vlc;
        }
    }
    if (nj.length) njThrow(NJ_SYNTAX_ERROR);
}

NJ_INLINE void njDecodeDQT(void) {
    int i;
    unsigned char *t;
    njDecodeLength();
    njCheckError();
    while (nj.length >= 65) {
        i = nj.pos[0];
        if (i & 0xFC) njThrow(NJ_SYNTAX_ERROR);
        nj.qtavail |= 1 << i;
        t = &nj.qtab[i][0];
        for (i = 0;  i < 64;  ++i)
            t[i] = nj.pos[i + 1];
        njSkip(65);
    }
    if (nj.length) njThrow(NJ_SYNTAX_ERROR);
}

NJ_INLINE void njDecodeDRI(void) {
    njDecodeLength();
    njCheckError();
    if (nj.length < 2) njThrow(NJ_SYNTAX_ERROR);
    nj.rstinterval = njDecode16(nj.pos);
    njSkip(nj.length);
}

static int njGetVLC(nj_vlc_code_t* vlc, unsigned char* code) {
    int value = njShowBits(16);
    int bits = vlc[value].bits;
    if (!bits) { nj.error = NJ_SYNTAX_ERROR; return 0; }
    njSkipBits(bits);
    value = vlc[value].code;
    if (code) *code = (unsigned char) value;
    bits = value & 15;
    if (!bits) return 0;
    value = njGetBits(bits);
    if (value < (1 << (bits - 1)))
        value += ((-1) << bits) + 1;
    return value;
}

NJ_INLINE void njDecodeBlock(nj_component_t* c, unsigned char* out) {
    unsigned char code = 0;
    int value, coef = 0;
    njFillMem(nj.block, 0, sizeof(nj.block));
    c->dcpred += njGetVLC(&nj.vlctab[c->dctabsel][0], NULL);
    nj.block[0] = (c->dcpred) * nj.qtab[c->qtsel][0];
    do {
        value = njGetVLC(&nj.vlctab[c->actabsel][0], &code);
        if (!code) break;  // EOB
        if (!(code & 0x0F) && (code != 0xF0)) njThrow(NJ_SYNTAX_ERROR);
        coef += (code >> 4) + 1;
        if (coef > 63) njThrow(NJ_SYNTAX_ERROR);
        nj.block[(int) njZZ[coef]] = value * nj.qtab[c->qtsel][coef];
    } while (coef < 63);
    for (coef = 0;  coef < 64;  coef += 8)
        njRowIDCT(&nj.block[coef]);
    for (coef = 0;  coef < 8;  ++coef)
        njColIDCT(&nj.block[coef], &out[coef], c->stride);
}

NJ_INLINE void njDecodeScan(void) {
    int i, mbx, mby, sbx, sby;
    int rstcount = nj.rstinterval, nextrst = 0;
    nj_component_t* c;
    njDecodeLength();
    njCheckError();
    if (nj.length < (4 + 2 * nj.ncomp)) njThrow(NJ_SYNTAX_ERROR);
    if (nj.pos[0] != nj.ncomp) njThrow(NJ_UNSUPPORTED);
    njSkip(1);
    for (i = 0, c = nj.comp;  i < nj.ncomp;  ++i, ++c) {
        if (nj.pos[0] != c->cid) njThrow(NJ_SYNTAX_ERROR);
        if (nj.pos[1] & 0xEE) njThrow(NJ_SYNTAX_ERROR);
        c->dctabsel = nj.pos[1] >> 4;
        c->actabsel = (nj.pos[1] & 1) | 2;
        njSkip(2);
    }
    if (nj.pos[0] || (nj.pos[1] != 63) || nj.pos[2]) njThrow(NJ_UNSUPPORTED);
    njSkip(nj.length);
    for (mbx = mby = 0;;) {
        for (i = 0, c = nj.comp;  i < nj.ncomp;  ++i, ++c)
            for (sby = 0;  sby < c->ssy;  ++sby)
                for (sbx = 0;  sbx < c->ssx;  ++sbx) {
                    njDecodeBlock(c, &c->pixels[((mby * c->ssy + sby) * c->stride + mbx * c->ssx + sbx) << 3]);
                    njCheckError();
                }
        if (++mbx >= nj.mbwidth) {
            mbx = 0;
            if (++mby >= nj.mbheight) break;
        }
        if (nj.rstinterval && !(--rstcount)) {
            njByteAlign();
            i = njGetBits(16);
            if (((i & 0xFFF8) != 0xFFD0) || ((i & 7) != nextrst)) njThrow(NJ_SYNTAX_ERROR);
            nextrst = (nextrst + 1) & 7;
            rstcount = nj.rstinterval;
            for (i = 0;  i < 3;  ++i)
                nj.comp[i].dcpred = 0;
        }
    }
    nj.error = __NJ_FINISHED;
}

#if NJ_CHROMA_FILTER

#define CF4A (-9)
#define CF4B (111)
#define CF4C (29)
#define CF4D (-3)
#define CF3A (28)
#define CF3B (109)
#define CF3C (-9)
#define CF3X (104)
#define CF3Y (27)
#define CF3Z (-3)
#define CF2A (139)
#define CF2B (-11)
#define CF(x) njClip(((x) + 64) >> 7)

NJ_INLINE void njUpsampleH(nj_component_t* c) {
    const int xmax = c->width - 3;
    unsigned char *out, *lin, *lout;
    int x, y;
    out = (unsigned char*) njAllocMem((c->width * c->height) << 1);
    if (!out) njThrow(NJ_OUT_OF_MEM);
    lin = c->pixels;
    lout = out;
    for (y = c->height;  y;  --y) {
        lout[0] = CF(CF2A * lin[0] + CF2B * lin[1]);
        lout[1] = CF(CF3X * lin[0] + CF3Y * lin[1] + CF3Z * lin[2]);
        lout[2] = CF(CF3A * lin[0] + CF3B * lin[1] + CF3C * lin[2]);
        for (x = 0;  x < xmax;  ++x) {
            lout[(x << 1) + 3] = CF(CF4A * lin[x] + CF4B * lin[x + 1] + CF4C * lin[x + 2] + CF4D * lin[x + 3]);
            lout[(x << 1) + 4] = CF(CF4D * lin[x] + CF4C * lin[x + 1] + CF4B * lin[x + 2] + CF4A * lin[x + 3]);
        }
        lin += c->stride;
        lout += c->width << 1;
        lout[-3] = CF(CF3A * lin[-1] + CF3B * lin[-2] + CF3C * lin[-3]);
        lout[-2] = CF(CF3X * lin[-1] + CF3Y * lin[-2] + CF3Z * lin[-3]);
        lout[-1] = CF(CF2A * lin[-1] + CF2B * lin[-2]);
    }
    c->width <<= 1;
    c->stride = c->width;
    njFreeMem((void*)c->pixels);
    c->pixels = out;
}

NJ_INLINE void njUpsampleV(nj_component_t* c) {
    const int w = c->width, s1 = c->stride, s2 = s1 + s1;
    unsigned char *out, *cin, *cout;
    int x, y;
    out = (unsigned char*) njAllocMem((c->width * c->height) << 1);
    if (!out) njThrow(NJ_OUT_OF_MEM);
    for (x = 0;  x < w;  ++x) {
        cin = &c->pixels[x];
        cout = &out[x];
        *cout = CF(CF2A * cin[0] + CF2B * cin[s1]);  cout += w;
        *cout = CF(CF3X * cin[0] + CF3Y * cin[s1] + CF3Z * cin[s2]);  cout += w;
        *cout = CF(CF3A * cin[0] + CF3B * cin[s1] + CF3C * cin[s2]);  cout += w;
        cin += s1;
        for (y = c->height - 3;  y;  --y) {
            *cout = CF(CF4A * cin[-s1] + CF4B * cin[0] + CF4C * cin[s1] + CF4D * cin[s2]);  cout += w;
            *cout = CF(CF4D * cin[-s1] + CF4C * cin[0] + CF4B * cin[s1] + CF4A * cin[s2]);  cout += w;
            cin += s1;
        }
        cin += s1;
        *cout = CF(CF3A * cin[0] + CF3B * cin[-s1] + CF3C * cin[-s2]);  cout += w;
        *cout = CF(CF3X * cin[0] + CF3Y * cin[-s1] + CF3Z * cin[-s2]);  cout += w;
        *cout = CF(CF2A * cin[0] + CF2B * cin[-s1]);
    }
    c->height <<= 1;
    c->stride = c->width;
    njFreeMem((void*) c->pixels);
    c->pixels = out;
}

#else

NJ_INLINE void njUpsample(nj_component_t* c) {
    int x, y, xshift = 0, yshift = 0;
    unsigned char *out, *lin, *lout;
    while (c->width < nj.width) { c->width <<= 1; ++xshift; }
    while (c->height < nj.height) { c->height <<= 1; ++yshift; }
    out = (unsigned char*) njAllocMem(c->width * c->height);
    if (!out) njThrow(NJ_OUT_OF_MEM);
    lin = c->pixels;
    lout = out;
    for (y = 0;  y < c->height;  ++y) {
        lin = &c->pixels[(y >> yshift) * c->stride];
        for (x = 0;  x < c->width;  ++x)
            lout[x] = lin[x >> xshift];
        lout += c->width;
    }
    c->stride = c->width;
    njFreeMem((void*) c->pixels);
    c->pixels = out;
}

#endif

NJ_INLINE void njConvert(void) {
    int i;
    nj_component_t* c;
    for (i = 0, c = nj.comp;  i < nj.ncomp;  ++i, ++c) {
        #if NJ_CHROMA_FILTER
            while ((c->width < nj.width) || (c->height < nj.height)) {
                if (c->width < nj.width) njUpsampleH(c);
                njCheckError();
                if (c->height < nj.height) njUpsampleV(c);
                njCheckError();
            }
        #else
            if ((c->width < nj.width) || (c->height < nj.height))
                njUpsample(c);
        #endif
        if ((c->width < nj.width) || (c->height < nj.height)) njThrow(NJ_INTERNAL_ERR);
    }
    if (nj.ncomp == 3) {
        // convert to RGB
        int x, yy;
        unsigned char *prgb = nj.rgb;
        const unsigned char *py  = nj.comp[0].pixels;
        const unsigned char *pcb = nj.comp[1].pixels;
        const unsigned char *pcr = nj.comp[2].pixels;
        for (yy = nj.height;  yy;  --yy) {
            for (x = 0;  x < nj.width;  ++x) {
                register int y = py[x] << 8;
                register int cb = pcb[x] - 128;
                register int cr = pcr[x] - 128;
                *prgb++ = njClip((y            + 359 * cr + 128) >> 8);
                *prgb++ = njClip((y -  88 * cb - 183 * cr + 128) >> 8);
                *prgb++ = njClip((y + 454 * cb            + 128) >> 8);
            }
            py += nj.comp[0].stride;
            pcb += nj.comp[1].stride;
            pcr += nj.comp[2].stride;
        }
    } else if (nj.comp[0].width != nj.comp[0].stride) {
        // grayscale -> only remove stride
        unsigned char *pin = &nj.comp[0].pixels[nj.comp[0].stride];
        unsigned char *pout = &nj.comp[0].pixels[nj.comp[0].width];
        int y;
        for (y = nj.comp[0].height - 1;  y;  --y) {
            njCopyMem(pout, pin, nj.comp[0].width);
            pin += nj.comp[0].stride;
            pout += nj.comp[0].width;
        }
        nj.comp[0].stride = nj.comp[0].width;
    }
}

void njInit(void) {
    njFillMem(&nj, 0, sizeof(nj_context_t));
}

void njDone(void) {
    int i;
    for (i = 0;  i < 3;  ++i)
        if (nj.comp[i].pixels) njFreeMem((void*) nj.comp[i].pixels);
    if (nj.rgb) njFreeMem((void*) nj.rgb);
    njInit();
}

nj_result_t njDecode(const void* jpeg, const int size) {
    njDone();
    nj.pos = (const unsigned char*) jpeg;
    nj.size = size & 0x7FFFFFFF;
    if (nj.size < 2) return NJ_NO_JPEG;
    if ((nj.pos[0] ^ 0xFF) | (nj.pos[1] ^ 0xD8)) return NJ_NO_JPEG;
    njSkip(2);
    while (!nj.error) {
        if ((nj.size < 2) || (nj.pos[0] != 0xFF)) return NJ_SYNTAX_ERROR;
        njSkip(2);
        switch (nj.pos[-1]) {
            case 0xC0: njDecodeSOF();  break;
            case 0xC4: njDecodeDHT();  break;
            case 0xDB: njDecodeDQT();  break;
            case 0xDD: njDecodeDRI();  break;
            case 0xDA: njDecodeScan(); break;
            case 0xFE: njSkipMarker(); break;
            default:
                if ((nj.pos[-1] & 0xF0) == 0xE0)
                    njSkipMarker();
                else
                    return NJ_UNSUPPORTED;
        }
    }
    if (nj.error != __NJ_FINISHED) return nj.error;
    nj.error = NJ_OK;
    njConvert();
    return nj.error;
}

int njGetWidth(void)            { return nj.width; }
int njGetHeight(void)           { return nj.height; }
int njIsColor(void)             { return (nj.ncomp != 1); }
unsigned char* njGetImage(void) { return (nj.ncomp == 1) ? nj.comp[0].pixels : nj.rgb; }
int njGetImageSize(void)        { return nj.width * nj.height * nj.ncomp; }

#endif // _NJ_INCLUDE_HEADER_ONLY















// Based on the demo that comes with nanojpeg
void * loadjpg(char* filename, int *width, int *height, int *bpp)
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





/*  pnglite.h - Interface for pnglite library
	Copyright (c) 2007 Daniel Karling

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	   distribution.

	Daniel Karling
	daniel.karling@gmail.com
 */


/*
	Enumerations for pnglite.
	Negative numbers are error codes and 0 and up are okay responses.
*/

enum
{
	PNG_DONE				= 1,
	PNG_NO_ERROR			= 0,
	PNG_FILE_ERROR			= -1,
	PNG_HEADER_ERROR		= -2,
	PNG_IO_ERROR			= -3,
	PNG_EOF_ERROR			= -4,
	PNG_CRC_ERROR			= -5,
	PNG_MEMORY_ERROR		= -6,
	PNG_ZLIB_ERROR			= -7,
	PNG_UNKNOWN_FILTER		= -8,
	PNG_NOT_SUPPORTED		= -9,
	PNG_WRONG_ARGUMENTS		= -10
};

/*
	The five different kinds of color storage in PNG files.
*/

enum
{
	PNG_GREYSCALE			= 0,
	PNG_TRUECOLOR			= 2,
	PNG_INDEXED				= 3,
	PNG_GREYSCALE_ALPHA		= 4,
	PNG_TRUECOLOR_ALPHA		= 6
};

/*
	Typedefs for callbacks.
*/

typedef unsigned (*png_write_callback_t)(void* input, size_t size, size_t numel, void* user_pointer);
typedef unsigned (*png_read_callback_t)(void* output, size_t size, size_t numel, void* user_pointer);
typedef void (*png_free_t)(void* p);
typedef void * (*png_alloc_t)(size_t s);

typedef struct
{
	void*					zs;				/* pointer to z_stream */
	png_read_callback_t		read_fun;
	png_write_callback_t	write_fun;
	void*					user_pointer;

	unsigned char*			png_data;
	unsigned				png_datalen;

	unsigned				width;
	unsigned				height;
	unsigned char			depth;
	unsigned char			color_type;
	unsigned char			compression_method;
	unsigned char			filter_method;
	unsigned char			interlace_method;
	unsigned char			bpp;
} png_t;

/*  pnglite.c - pnglite library
    For conditions of distribution and use, see copyright notice in pnglite.h
*/
#define DO_CRC_CHECKS 1
//#define USE_ZLIB 1

// Modified #include by Ingemar, assuming zlib installed.
//#ifdef USE_ZLIB
#include "zlib.h"
//#else
//#include "zlite.h"
//#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "pnglite.h"


static png_alloc_t png_alloc;
static png_free_t png_free;

static size_t file_read(png_t* png, void* out, size_t size, size_t numel)
{
	size_t result;
	if(png->read_fun)
	{
		result = png->read_fun(out, size, numel, png->user_pointer);
	}
	else
	{
		if(!out)
		{
			result = fseek(png->user_pointer, (long)(size*numel), SEEK_CUR);
		}
		else
		{
			result = fread(out, size, numel, png->user_pointer);
		}
	}

	return result;
}

static size_t file_write(png_t* png, void* p, size_t size, size_t numel)
{
	size_t result;

	if(png->write_fun)
	{
		result = png->write_fun(p, size, numel, png->user_pointer);
	}
	else
	{
		result = fwrite(p, size, numel, png->user_pointer);
	}

	return result;
}

static int file_read_ul(png_t* png, unsigned *out)
{
	unsigned char buf[4];

	if(file_read(png, buf, 1, 4) != 4)
		return PNG_FILE_ERROR;

	*out = (buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3];

	return PNG_NO_ERROR;
}

static int file_write_ul(png_t* png, unsigned in)
{
	unsigned char buf[4];

	buf[0] = (in>>24) & 0xff;
	buf[1] = (in>>16) & 0xff;
	buf[2] = (in>>8) & 0xff;
	buf[3] = (in) & 0xff;

	if(file_write(png, buf, 1, 4) != 4)
		return PNG_FILE_ERROR;

	return PNG_NO_ERROR;
}


static unsigned get_ul(unsigned char* buf)
{
	unsigned result;
	unsigned char foo[4];

	memcpy(foo, buf, 4);

	result = (foo[0]<<24) | (foo[1]<<16) | (foo[2]<<8) | foo[3];

	return result;
}

static unsigned set_ul(unsigned char* buf, unsigned in)
{
	buf[0] = (in>>24) & 0xff;
	buf[1] = (in>>16) & 0xff;
	buf[2] = (in>>8) & 0xff;
	buf[3] = (in) & 0xff;

	return PNG_NO_ERROR;
}

int png_init(png_alloc_t pngalloc, png_free_t pngfree)
{
	if(pngalloc)
		png_alloc = pngalloc;
	else
		png_alloc = &malloc;

	if(pngfree)
		png_free = pngfree;
	else
		png_free = &free;

	return PNG_NO_ERROR;
}

static int png_get_bpp(png_t* png)
{
	int bpp;

	switch(png->color_type)
	{
	case PNG_GREYSCALE:
		bpp = 1; break;
	case PNG_TRUECOLOR:
		bpp = 3; break;
	case PNG_INDEXED:
		bpp = 1; break;
	case PNG_GREYSCALE_ALPHA:
		bpp = 2; break;
	case PNG_TRUECOLOR_ALPHA:
		bpp = 4; break;
	default:
		return PNG_FILE_ERROR;
	}

	bpp *= png->depth/8;

	return bpp;
}

static int png_read_ihdr(png_t* png)
{
	unsigned length;
#if DO_CRC_CHECKS
	unsigned orig_crc;
	unsigned calc_crc;
#endif
	unsigned char ihdr[13+4];		 /* length should be 13, make room for type (IHDR) */

	file_read_ul(png, &length);

	if(length != 13)
	{
		printf("%d\n", length);
		return PNG_CRC_ERROR;
	}

	if(file_read(png, ihdr, 1, 13+4) != 13+4)
		return PNG_EOF_ERROR;
#if DO_CRC_CHECKS
	file_read_ul(png, &orig_crc);

	calc_crc = crc32(0L, 0, 0);
	calc_crc = crc32(calc_crc, ihdr, 13+4);

	if(orig_crc != calc_crc)
		return PNG_CRC_ERROR;
#else
	file_read_ul(png);
#endif

	png->width = get_ul(ihdr+4);
	png->height = get_ul(ihdr+8);
	png->depth = ihdr[12];
	png->color_type = ihdr[13];
	png->compression_method = ihdr[14];
	png->filter_method = ihdr[15];
	png->interlace_method = ihdr[16];

	if(png->color_type == PNG_INDEXED)
		return PNG_NOT_SUPPORTED;

	if(png->depth != 8 && png->depth != 16)
		return PNG_NOT_SUPPORTED;

	if(png->interlace_method)
		return PNG_NOT_SUPPORTED;

	return PNG_NO_ERROR;
}

static int png_write_ihdr(png_t* png)
{
	unsigned char ihdr[13+4];
	unsigned char *p = ihdr;
	unsigned crc;

	file_write(png, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 1, 8);

	file_write_ul(png, 13);

	*p = 'I';			p++;
	*p = 'H';			p++;
	*p = 'D';			p++;
	*p = 'R';			p++;
	set_ul(p, png->width);	p+=4;
	set_ul(p, png->height);	p+=4;
	*p = png->depth;			p++;
	*p = png->color_type;	p++;
	*p = 0;				p++;
	*p = 0;				p++;
	*p = 0;				p++;

	file_write(png, ihdr, 1, 13+4);

	crc = crc32(0L, 0, 0);
	crc = crc32(crc, ihdr, 13+4);

	file_write_ul(png, crc);

	return PNG_NO_ERROR;
}

void png_print_info(png_t* png)
{
	printf("PNG INFO:\n");
	printf("\twidth:\t\t%d\n", png->width);
	printf("\theight:\t\t%d\n", png->height);
	printf("\tdepth:\t\t%d\n", png->depth);
	printf("\tcolor:\t\t");

	switch(png->color_type)
	{
	case PNG_GREYSCALE:			printf("greyscale\n"); break;
	case PNG_TRUECOLOR:			printf("truecolor\n"); break;
	case PNG_INDEXED:			printf("palette\n"); break;
	case PNG_GREYSCALE_ALPHA:	printf("greyscale with alpha\n"); break;
	case PNG_TRUECOLOR_ALPHA:	printf("truecolor with alpha\n"); break;
	default:					printf("unknown, this is not good\n"); break;
	}

	printf("\tcompression:\t%s\n",	png->compression_method?"unknown, this is not good":"inflate/deflate");
	printf("\tfilter:\t\t%s\n",		png->filter_method?"unknown, this is not good":"adaptive");
	printf("\tinterlace:\t%s\n",	png->interlace_method?"interlace":"no interlace");
}

int png_open_read(png_t* png, png_read_callback_t read_fun, void* user_pointer)
{
	char header[8];
	int result;

	png->read_fun = read_fun;
	png->write_fun = 0;
	png->user_pointer = user_pointer;

	if(!read_fun && !user_pointer)
		return PNG_WRONG_ARGUMENTS;

	if(file_read(png, header, 1, 8) != 8)
		return PNG_EOF_ERROR;

	if(memcmp(header, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8) != 0)
		return PNG_HEADER_ERROR;

	result = png_read_ihdr(png);

	png->bpp = (unsigned char)png_get_bpp(png);

	return result;
}

int png_open_write(png_t* png, png_write_callback_t write_fun, void* user_pointer)
{
	png->write_fun = write_fun;
	png->read_fun = 0;
	png->user_pointer = user_pointer;

	if(!write_fun && !user_pointer)
		return PNG_WRONG_ARGUMENTS;

	return PNG_NO_ERROR;
}

int png_open(png_t* png, png_read_callback_t read_fun, void* user_pointer)
{
	return png_open_read(png, read_fun, user_pointer);
}

int png_open_file_read(png_t *png, const char* filename)
{
	FILE* fp = fopen(filename, "rb");

	if(!fp)
		return PNG_FILE_ERROR;

	return png_open_read(png, 0, fp);
}

int png_open_file_write(png_t *png, const char* filename)
{
	FILE* fp = fopen(filename, "wb");

	if(!fp)
		return PNG_FILE_ERROR;

	return png_open_write(png, 0, fp);
}

int png_open_file(png_t *png, const char* filename)
{
	return png_open_file_read(png, filename);
}

int png_close_file(png_t* png)
{
	fclose(png->user_pointer);

	return PNG_NO_ERROR;
}

static int png_init_deflate(png_t* png, unsigned char* data, int datalen)
{
	z_stream *stream;
	png->zs = png_alloc(sizeof(z_stream));

	stream = png->zs;

	if(!stream)
		return PNG_MEMORY_ERROR;

	memset(stream, 0, sizeof(z_stream));

	if(deflateInit(stream, Z_DEFAULT_COMPRESSION) != Z_OK)
		return PNG_ZLIB_ERROR;

	stream->next_in = data;
	stream->avail_in = datalen;

	return PNG_NO_ERROR;
}

static int png_init_inflate(png_t* png)
{
//#if USE_ZLIB
	z_stream *stream;
	png->zs = png_alloc(sizeof(z_stream));
//#else
//	zl_stream *stream;
//	png->zs = png_alloc(sizeof(zl_stream));
//#endif

	stream = png->zs;

	if(!stream)
		return PNG_MEMORY_ERROR;



//#if USE_ZLIB
	memset(stream, 0, sizeof(z_stream));
	if(inflateInit(stream) != Z_OK)
		return PNG_ZLIB_ERROR;
//#else
//	memset(stream, 0, sizeof(zl_stream));
//	if(z_inflateInit(stream) != Z_OK)
//		return PNG_ZLIB_ERROR;
//#endif

	stream->next_out = png->png_data;
	stream->avail_out = png->png_datalen;

	return PNG_NO_ERROR;
}

static int png_end_deflate(png_t* png)
{
	z_stream *stream = png->zs;

	if(!stream)
		return PNG_MEMORY_ERROR;

	deflateEnd(stream);

	png_free(png->zs);

	return PNG_NO_ERROR;
}

static int png_end_inflate(png_t* png)
{
//#if USE_ZLIB
	z_stream *stream = png->zs;
//#else
//	zl_stream *stream = png->zs;
//#endif

	if(!stream)
		return PNG_MEMORY_ERROR;

//#if USE_ZLIB
	if(inflateEnd(stream) != Z_OK)
//#else
//	if(z_inflateEnd(stream) != Z_OK)
//#endif
	{
		printf("ZLIB says: %s\n", stream->msg);
		return PNG_ZLIB_ERROR;
	}

	png_free(png->zs);

	return PNG_NO_ERROR;
}

static int png_inflate(png_t* png, char* data, int len)
{
	int result;
//#if USE_ZLIB
	z_stream *stream = png->zs;
//#else
//	zl_stream *stream = png->zs;
//#endif

	if(!stream)
		return PNG_MEMORY_ERROR;

	stream->next_in = (unsigned char*)data;
	stream->avail_in = len;

//#if USE_ZLIB
	result = inflate(stream, Z_SYNC_FLUSH);
//#else
//	result = z_inflate(stream);
//#endif

	if(result != Z_STREAM_END && result != Z_OK)
	{
		printf("%s\n", stream->msg);
		return PNG_ZLIB_ERROR;
	}

	if(stream->avail_in != 0)
		return PNG_ZLIB_ERROR;

	return PNG_NO_ERROR;
}

static int png_deflate(png_t* png, char* outdata, int outlen, int *outwritten)
{
	int result;

	z_stream *stream = png->zs;


	if(!stream)
		return PNG_MEMORY_ERROR;

	stream->next_out = (unsigned char*)outdata;
	stream->avail_out = outlen;

	result = deflate(stream, Z_SYNC_FLUSH);

	*outwritten = outlen - stream->avail_out;

	if(result != Z_STREAM_END && result != Z_OK)
	{
		printf("%s\n", stream->msg);
		return PNG_ZLIB_ERROR;
	}

	return result;
}

static int png_write_idats(png_t* png, unsigned char* data)
{
	unsigned char *chunk;
	unsigned long written;
	unsigned long crc;
	unsigned size = png->width * png->height * png->bpp + png->height;

	(void)png_init_deflate;
	(void)png_end_deflate;
	(void)png_deflate;

	chunk = png_alloc(size);
	memcpy(chunk, "IDAT", 4);

	written = size;
	compress(chunk+4, &written, data, size);

	crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, chunk, written+4);
	set_ul(chunk+written+4, crc);
	file_write_ul(png, written);
	file_write(png, chunk, 1, written+8);
	png_free(chunk);

	file_write_ul(png, 0);
	file_write(png, "IEND", 1, 4);
	crc = crc32(0L, (const unsigned char *)"IEND", 4);
	file_write_ul(png, crc);

	return PNG_NO_ERROR;
}

static int png_read_idat(png_t* png, unsigned firstlen)
{
	unsigned type = 0;
	char *chunk;
	int result;
	unsigned length = firstlen;
	unsigned old_len = length;

#if DO_CRC_CHECKS
	unsigned orig_crc;
	unsigned calc_crc;
#endif

	chunk = png_alloc(firstlen);

	result = png_init_inflate(png);

	if(result != PNG_NO_ERROR)
	{
		png_end_inflate(png);
		png_free(chunk);
		return result;
	}

	do
	{
		if(file_read(png, chunk, 1, length) != length)
		{
			png_end_inflate(png);
			png_free(chunk);
			return PNG_FILE_ERROR;
		}

#if DO_CRC_CHECKS
		calc_crc = crc32(0L, Z_NULL, 0);
		calc_crc = crc32(calc_crc, (unsigned char*)"IDAT", 4);
		calc_crc = crc32(calc_crc, (unsigned char*)chunk, length);

		file_read_ul(png, &orig_crc);

		if(orig_crc != calc_crc)
		{
			result = PNG_CRC_ERROR;
			break;
		}
#else
		file_read_ul(png);
#endif

		result = png_inflate(png, chunk, length);

		if(result != PNG_NO_ERROR) break;

		file_read_ul(png, &length);

		if(length > old_len)
		{
			png_free(chunk);
			chunk = png_alloc(length);
			old_len = length;
		}

		if(file_read(png, &type, 1, 4) != 4)
		{
			result = PNG_FILE_ERROR;
			break;
		}

	}while(type == *(unsigned int*)"IDAT");

	if(type == *(unsigned int*)"IEND")
		result = PNG_DONE;

	png_free(chunk);
	png_end_inflate(png);

	return result;
}

static int png_process_chunk(png_t* png)
{
	int result = PNG_NO_ERROR;
	unsigned type;
	unsigned length;

	file_read_ul(png, &length);

	if(file_read(png, &type, 1, 4) != 4)
		return PNG_FILE_ERROR;

	if(type == *(unsigned int*)"IDAT")	/* if we found an idat, all other idats should be followed with no other chunks in between */
	{
		png->png_datalen = png->width * png->height * png->bpp + png->height;
		png->png_data = png_alloc(png->png_datalen);

		if(!png->png_data)
			return PNG_MEMORY_ERROR;

		return png_read_idat(png, length);
	}
	else if(type == *(unsigned int*)"IEND")
	{
		return PNG_DONE;
	}
	else
	{
		file_read(png, 0, 1, length + 4);		/* unknown chunk */
	}

	return result;
}

static void png_filter_sub(int stride, unsigned char* in, unsigned char* out, int len)
{
	int i;
	unsigned char a = 0;

	for(i = 0; i < len; i++)
	{
		if(i >= stride)
			a = out[i - stride];

		out[i] = in[i] + a;
	}
}

static void png_filter_up(int stride, unsigned char* in, unsigned char* out, unsigned char* prev_line, int len)
{
	int i;

	if(prev_line)
    {
        for(i = 0; i < len; i++)
            out[i] = in[i] + prev_line[i];
    }
    else
        memcpy(out, in, len);
}

static void png_filter_average(int stride, unsigned char* in, unsigned char* out, unsigned char* prev_line, int len)
{
	int i;
	unsigned char a = 0;
	unsigned char b = 0;
	unsigned int sum = 0;

	for(i = 0; i < len; i++)
	{
		if(prev_line)
			b = prev_line[i];

		if(i >= stride)
			a = out[i - stride];

		sum = a;
		sum += b;

		out[i] = (char)(in[i] + sum/2);
	}
}

static unsigned char png_paeth(unsigned char a, unsigned char b, unsigned char c)
{
	int p = (int)a + b - c;
	int pa = abs(p - a);
	int pb = abs(p - b);
	int pc = abs(p - c);

	int pr;

	if(pa <= pb && pa <= pc)
		pr = a;
	else if(pb <= pc)
		pr = b;
	else
		pr = c;

	return (char)pr;
}

static void png_filter_paeth(int stride, unsigned char* in, unsigned char* out, unsigned char* prev_line, int len)
{
	int i;
	unsigned char a;
	unsigned char b;
	unsigned char c;

	for(i = 0; i < len; i++)
	{
		if(prev_line && i >= stride)
		{
			a = out[i - stride];
			b = prev_line[i];
			c = prev_line[i - stride];
		}
		else
		{
			if(prev_line)
				b = prev_line[i];
			else
				b = 0;

			if(i >= stride)
				a = out[i - stride];
			else
				a = 0;

			c = 0;
		}

		out[i] = in[i] + png_paeth(a, b, c);
	}
}

static int png_filter(png_t* png, unsigned char* data)
{


	return PNG_NO_ERROR;
}

static int png_unfilter(png_t* png, unsigned char* data)
{
	unsigned i;
	unsigned pos = 0;
	unsigned outpos = 0;
	unsigned char *filtered = png->png_data;

	int stride = png->bpp;

	while(pos < png->png_datalen)
	{
		unsigned char filter = filtered[pos];

		pos++;

		if(png->depth == 16)
		{
			for(i = 0; i < png->width * stride; i+=2)
			{
				*(short*)(filtered+pos+i) = (filtered[pos+i] << 8) | filtered[pos+i+1];
			}
		}

		switch(filter)
		{
		case 0: /* none */
			memcpy(data+outpos, filtered+pos, png->width * stride);
			break;
		case 1: /* sub */
			png_filter_sub(stride, filtered+pos, data+outpos, png->width * stride);
			break;
		case 2: /* up */
			if(outpos)
				png_filter_up(stride, filtered+pos, data+outpos, data + outpos - (png->width*stride), png->width*stride);
			else
				png_filter_up(stride, filtered+pos, data+outpos, 0, png->width*stride);
			break;
		case 3: /* average */
			if(outpos)
				png_filter_average(stride, filtered+pos, data+outpos, data + outpos - (png->width*stride), png->width*stride);
			else
				png_filter_average(stride, filtered+pos, data+outpos, 0, png->width*stride);
			break;
		case 4: /* paeth */
			if(outpos)
				png_filter_paeth(stride, filtered+pos, data+outpos, data + outpos - (png->width*stride), png->width*stride);
			else
				png_filter_paeth(stride, filtered+pos, data+outpos, 0, png->width*stride);
			break;
		default:
			return PNG_UNKNOWN_FILTER;
		}

		outpos += png->width * stride;
		pos += png->width * stride;
	}

	return PNG_NO_ERROR;
}

int png_get_data(png_t* png, unsigned char* data)
{
	int result = PNG_NO_ERROR;

	while(result == PNG_NO_ERROR)
	{
		result = png_process_chunk(png);
	}

	if(result != PNG_DONE)
	{
		png_free(png->png_data);
		return result;
	}

	result = png_unfilter(png, data);

	png_free(png->png_data);

	return result;
}

int png_set_data(png_t* png, unsigned width, unsigned height, char depth, int color, unsigned char* data)
{
	int i;
	unsigned char *filtered;
	png->width = width;
	png->height = height;
	png->depth = depth;
	png->color_type = color;
	png->bpp = png_get_bpp(png);

	filtered = png_alloc(width * height * png->bpp + height);

	for(i = 0; i < png->height; i++)
	{
		filtered[i*png->width*png->bpp+i] = 0;
		memcpy(&filtered[i*png->width*png->bpp+i+1], data + i * png->width*png->bpp, png->width*png->bpp);
	}

	png_filter(png, filtered);
	png_write_ihdr(png);
	png_write_idats(png, filtered);

	png_free(filtered);
	return PNG_NO_ERROR;
}


char* png_error_string(int error)
{
	switch(error)
	{
	case PNG_NO_ERROR:
		return "No error";
	case PNG_FILE_ERROR:
		return "Unknown file error.";
	case PNG_HEADER_ERROR:
		return "No PNG header found. Are you sure this is a PNG?";
	case PNG_IO_ERROR:
		return "Failure while reading file.";
	case PNG_EOF_ERROR:
		return "Reached end of file.";
	case PNG_CRC_ERROR:
		return "CRC or chunk length error.";
	case PNG_MEMORY_ERROR:
		return "Could not allocate memory.";
	case PNG_ZLIB_ERROR:
		return "zlib reported an error.";
	case PNG_UNKNOWN_FILTER:
		return "Unknown filter method used in scanline.";
	case PNG_DONE:
		return "PNG done";
	case PNG_NOT_SUPPORTED:
		return "The PNG is unsupported by pnglite, too bad for you!";
	case PNG_WRONG_ARGUMENTS:
		return "Wrong combination of arguments passed to png_open. You must use either a read_function or supply a file pointer to use.";
	default:
		return "Unknown error.";
	};
}











// READPPM
// By Ingemar Ragnemalm
// based on a ppm reader by Edward Angel,
// Added support for binary PPM (2002?)
// 2006: Fixed a bug that caused some files to end prematurely
// and added a PPM writer
// for making screen shots and other output (i.e. GPGPU uses)
// Added writeppm. (Writes in ASCII format only.)
// 2009: Added readppmtexture. (A writer for a texture would also be possible when needed.)
// Portable #include.
// 2010: Fixed a stupid bug that made non-square textures fail.
// Still crashes on non-power-of-two.
// 2010, march: Even more bug fixes, and now it is OK (for power of two).
// Checks for power of two to avoid crashes.
// 2010-03-15 A fix in writeppm

// CUDA friendly version

int writeppm(char *filename, int width, int height, unsigned char *data)
{
	FILE *fp;
	int error = 1;
	int i, h, v;

	if (filename != NULL)
	{
		fp = fopen(filename,"w");

		if (fp != NULL)
		{
			// Write PPM file
			// Header
			fprintf(fp, "P3\n");
			fprintf(fp, "# written by Ingemars PPM writer\n");
			fprintf(fp, "%d %d\n", width, height);
			fprintf(fp, "%d\n", 255); // range

			// Data
			for (v = height-1; v >=0; v--)
			{
				for (h = 0; h < width; h++)
				{
					i = (width*v + h)*3; // assumes rgb, not rgba
					fprintf(fp, "%d %d %d ", data[i], data[i+1], data[i+2]);
				}
				fprintf(fp, "\n"); // range
			}

			if (fwrite("\n",sizeof(char),1,fp) == 1)
				error = 0; // Probable success
			fclose(fp);
		}
	}
	return(error);
}



unsigned char *readppm(char *filename, int *width, int *height)
{
	FILE *fd;
	int  k;
	char c;
	int i,j;
	char b[100];
	int red, green, blue;
	long numbytes;
	int n;
	int m;
	unsigned char *image;

	fd = fopen(filename, "rb");
	if (fd == NULL)
	{
		printf("Could not open %s\n", filename);
		return NULL;
	}
	c = getc(fd);
	if (c=='P' || c=='p')
		c = getc(fd);

	if (c == '3')
	{
		printf("%s is a PPM file (plain text version)\n", filename);

		// NOTE: This is not very good PPM code! Comments are not allowed
		// except immediately after the magic number.
		c = getc(fd);
		if (c == '\n' || c == '\r') // Skip any line break and comments
		{
			c = getc(fd);
			while(c == '#')
			{
				fscanf(fd, "%[^\n\r] ", b);
				printf("%s\n",b);
				c = getc(fd);
			}
			ungetc(c,fd);
		}
		fscanf(fd, "%d %d %d", &n, &m, &k);

		printf("%d rows  %d columns  max value= %d\n",n,m,k);

		numbytes = n * m * 3;
		image = (unsigned char *) malloc(numbytes);
		if (image == NULL)
		{
			printf("Memory allocation failed!\n");
			return NULL;
		}
		for(i=m-1;i>=0;i--) for(j=0;j<n;j++) // Important bug fix here!
		{ // i = row, j = column
			fscanf(fd,"%d %d %d",&red, &green, &blue );
			image[(i*n+j)*3]=red * 255 / k;
			image[(i*n+j)*3+1]=green * 255 / k;
			image[(i*n+j)*3+2]=blue * 255 / k;
		}
	}
	else
	if (c == '6')
	{
		printf("%s is a PPM file (raw version)!\n", filename);

		c = getc(fd);
		if (c == '\n' || c == '\r') // Skip any line break and comments
		{
			c = getc(fd);
			while(c == '#')
			{
				fscanf(fd, "%[^\n\r] ", b);
				printf("%s\n",b);
				c = getc(fd);
			}
			ungetc(c,fd);
		}
		fscanf(fd, "%d %d %d", &n, &m, &k);
		printf("%d rows  %d columns  max value= %d\n",m,n,k);
		c = getc(fd); // Skip the last whitespace

		numbytes = n * m * 3;
		image = (unsigned char *) malloc(numbytes);
		if (image == NULL)
		{
			printf("Memory allocation failed!\n");
			return NULL;
		}
		// Read and re-order as necessary
		for(i=m-1;i>=0;i--) for(j=0;j<n;j++) // Important bug fix here!
		{
			image[(i*n+j)*3+0]=getc(fd);
			image[(i*n+j)*3+1]=getc(fd);
			image[(i*n+j)*3+2]=getc(fd);
		}
	}
	else
	{
		printf("%s is not a PPM file!\n", filename);
		return NULL;
	}

	printf("read image\n");

	*height = m;
	*width = n;
	return image;
}









GLuint LoadTexture(char *filename, char dorepeat)
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
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
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
//			case PNG_GREYSCALE:
//				fmt = GL_LUMINANCE;break; // Untested!
			case PNG_TRUECOLOR:
				fmt = GL_RGB;break;
	// PNG_INDEXED ignored
//			case PNG_GREYSCALE_ALPHA:
//				fmt = GL_LUMINANCE_ALPHA;break; // Untested!
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
