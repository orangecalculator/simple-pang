#include <iostream>

#include "Pang.h"
#include "Texture.h"

using namespace std;

#pragma warning ( disable: 26812 )

static FIBITMAP* createBitMap(const char* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1) {
		DEBUG("Could not find image: %s - Aborting.\n", filename);
		exit(-1);
	}

	if (format == FIF_UNKNOWN) {
		DEBUG("Couldn't determine file format - attempting to get from file extension...\n");
		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format)) {
			DEBUG("Detected image format cannot be read!\n");
			exit(-1);
		}
	}

	// free memory by FreeImage_Unload
	//reference: https://stackoverflow.com/questions/21742092/freeimage-load-memory-leak
	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	if (bitsPerPixel == 32) {
		DEBUG("Source image has %d bits per pixel. Skipping conversion.\n", bitsPerPixel);
	}
	else {
		DEBUG("Source image has %d bits per pixel. Converting to 32-bit colour.\n", bitsPerPixel);

		FIBITMAP* bitmapsaved = bitmap;
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
		FreeImage_Unload(bitmapsaved);
	}

	return bitmap;
}

#pragma warning ( default: 26812 )

Texture::Texture(const std::string& filename) {
	image = createBitMap(filename.c_str());
	imageWidth = FreeImage_GetWidth(image);
	imageHeight = FreeImage_GetHeight(image);
	textureData = FreeImage_GetBits(image);

	//generateTexture()
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
		0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);

	FreeImage_Unload(image);
}

void Texture::draw() const {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
}