#include "Text.h"

Text::Text(const std::array<double, 2>& position,
	const std::array<double, 3>& color,
	void * font) :
	position(position), color(color), font(font) {

}

void Text::setColor(double r, double g, double b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Text::setPosition(double x, double y) {
	position[0] = x;
	position[1] = y;
}

void Text::setFont(void* _font) {
	font = _font;
}

const double* Text::getColor() const {
	return color.data();
}

const double* Text::getPosition() const {
	return position.data();
}

void* Text::getFont() const {
	return font;
}

void Text::draw(const std::string& s) {
	glColor3f(color[0], color[1], color[2]);
	glRasterPos2f(position[0], position[1]);
	for (const char& c : s) {
		glutBitmapCharacter(font, c);
	}
}