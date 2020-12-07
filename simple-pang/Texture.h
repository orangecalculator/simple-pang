#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <FreeImage.h>
#include <GL/glut.h>

class Texture
{
private:
	GLuint textureID;

	FIBITMAP* image;
	int imageWidth;
	int imageHeight;
	GLubyte* textureData;

public:
	Texture(const std::string& filename);
	~Texture();

	void draw() const;
};

#endif /* _TEXTURE_H_ */