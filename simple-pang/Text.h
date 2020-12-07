#ifndef _TEXT_H_
#define _TEXT_H_

#include <array>
#include <string>
#include <GL/glut.h>

class Text
{
private:
	std::array<float, 3> color;
	std::array<float, 2> position;
	void* font;

public:
	Text(const std::array<float, 2>& position);
	Text(const std::array<float, 3>& color,
		const std::array<float, 2>& position,
		void * font);

	void setColor(float r, float g, float b);
	void setPosition(float x, float y);
	void setFont(void* font);

	const float* getColor() const;
	const float* getPosition() const;
	void* getFont() const;

	void draw(const std::string&);
};

/*
* 
* font can be one of the followings
* 
GLUT_BITMAP_8_BY_13 - default
GLUT_BITMAP_8_BY_15
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18
* 
*/

#endif /* _TEXT_H_ */