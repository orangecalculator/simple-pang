#include <GL/glut.h>
#include "Player.h"

#define NDEBUG
#include "Pang.h"

constexpr float HarpoonINVALID = -1.0f;

Player::Player() : coord{0.0f, GameFrameDown}, harpoon(HarpoonINVALID) {

}

bool Player::harpoonvalid() const {
	return (harpoon >= GameFrameDown - tol);
}

void Player::setCoord(float x, float y) {
	coord[0] = x;
	coord[1] = y;
}

void Player::setHarpoon(float _harpoon) {
	harpoon = _harpoon;
}

const float* Player::getCoord() const{
	return coord;
}

float Player::getHarpoon() const {
	return harpoon;
}

void Player::move(float dx, float dy) {
	if (!harpoonvalid()) {
		coord[0] += dx;
		coord[1] += dy;
	}
}

void Player::nextframe() {
	if (harpoonvalid()) {
		DEBUG("harpoon proceed at %5.2f\n", harpoon);

		harpoon += HarpoonSpeed;
		if (harpoon > GameFrameUp)
			harpoon = HarpoonINVALID;
	}
}

void Player::launch() {
	if (!harpoonvalid()) {
		harpoon = GameFrameDown;
	}
}

void Player::draw() const {
	glPushMatrix();

	//to be implemented
	glPointSize(5);
	
	glBegin(GL_POINTS);
	glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
	glVertex2f(coord[0], coord[1]);
	glEnd();

	if (harpoonvalid()) {
		DEBUG("drawing harpoon x %5.2f y %5.2f\n", coord[0], harpoon);

		glBegin(GL_LINES);
		glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
		glVertex2f(coord[0], coord[1]);
		glVertex2f(coord[0], harpoon);
		glEnd();

		glBegin(GL_POINTS);
		glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
		glVertex2f(coord[0], harpoon);
		glEnd();
	}

	glPointSize(1);
	
	glPopMatrix();
}