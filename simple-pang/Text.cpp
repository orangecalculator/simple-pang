#include "Text.h"

Text::Text(const std::array<float, 2>& position) : Text({ 1.0f, 1.0f, 1.0f }, position, GLUT_BITMAP_8_BY_13) {

}

Text::Text(const std::array<float, 3>& color,
	const std::array<float, 2>& position,
	void * font) :
	color(color), position(position), font(font) {

}

void Text::setColor(float r, float g, float b) {
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void Text::setPosition(float x, float y) {
	position[0] = x;
	position[1] = y;
}

void Text::setFont(void* _font) {
	font = _font;
}

const float* Text::getColor() const {
	return color.data();
}

const float* Text::getPosition() const {
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