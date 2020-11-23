#include "Player.h"
#include "Pang.h"
#include <GL/glut.h>

constexpr double HarpoonINVALID = -1.0;

Player::Player() : coord{ 0.0f, GameFrameDown }, harpoon(HarpoonINVALID) {

}

bool Player::harpoonvalid() const {
	return (harpoon >= GameFrameDown - tol);
}

void Player::setCoord(double x, double y) {
	coord[0] = x;
	coord[1] = y;
}

void Player::setHarpoon(double _harpoon) {
	harpoon = _harpoon;
}

const double* Player::getCoord() const {
	return coord;
}

double Player::getHarpoon() const {
	return harpoon;
}

void Player::useHarpoon() {
	harpoon = HarpoonINVALID;
}

void Player::move(double dx, double dy) {
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
	glColor4d(0.0f, 1.0f, 0.0f, 0.0f);
	glVertex2d(coord[0], coord[1]);
	glEnd();

	if (harpoonvalid()) {
		DEBUG("drawing harpoon x %5.2f y %5.2f\n", coord[0], harpoon);

		glBegin(GL_LINES);
		glColor4d(0.0f, 1.0f, 1.0f, 0.0f);
		glVertex2d(coord[0], coord[1]);
		glVertex2d(coord[0], harpoon);
		glEnd();

		glBegin(GL_POINTS);
		glColor4d(0.0f, 1.0f, 0.0f, 0.0f);
		glVertex2d(coord[0], harpoon);
		glEnd();
	}

	glPointSize(1);

	glPopMatrix();
}