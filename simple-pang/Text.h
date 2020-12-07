#ifndef _TEXT_H_
#define _TEXT_H_

#include <array>
#include <string>
#include <GL/glut.h>

class Text
{
private:
	std::array<double, 3> color;
	std::array<double, 2> position;
	void* font;

public:
	Text(const std::array<double, 2>& position = std::array<double, 2>{0.0, 0.0},
		const std::array<double, 3>& color = std::array<double, 3>{1.0, 1.0, 1.0},
		void * font = GLUT_BITMAP_8_BY_13);

	void setColor(double r, double g, double b);
	void setPosition(double x, double y);
	void setFont(void* font);

	const double* getColor() const;
	const double* getPosition() const;
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