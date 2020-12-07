#include "Player.h"
#include "Pang.h"
#include "PangIO.h"
#include "Ball.h"
#include <GL/glut.h>

constexpr double HarpoonINVALID = -1.0;

Player::Player() : coord{ 0.0f, GameFrameDown }, harpoon(HarpoonINVALID),
life(LifeInit), lastCollision(-2 * CollideDelay) {

}

static bool harpoonvalid(double harpoon) {
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

int Player::getLife() const {
	return life;
}

void Player::useHarpoon() {
	harpoon = HarpoonINVALID;
}

void Player::move(double dx, double dy) {
	if (!harpoonvalid(harpoon)) {
		coord[0] += dx;
		coord[1] += dy;
	}
}

void Player::nextframe() {
	if (harpoonvalid(harpoon)) {
		DEBUG("harpoon proceed at %5.2f\n", harpoon);

		harpoon += HarpoonSpeed;
		if (harpoon > GameFrameUp)
			harpoon = HarpoonINVALID;
	}
}

void Player::checkcollision(const Ball & B) {
	if (lastCollision + CollideDelay >= getFrameCount())
		return;

	if (B.collision(getCoord()[0] - PlayerCollideBoxSize, getCoord()[1] + PlayerCollideBoxSize,
		getCoord()[0] - PlayerCollideBoxSize, getCoord()[1] - PlayerCollideBoxSize)
		|| B.collision(getCoord()[0] - PlayerCollideBoxSize, getCoord()[1] - PlayerCollideBoxSize,
			getCoord()[0] + PlayerCollideBoxSize, getCoord()[1] - PlayerCollideBoxSize)
		|| B.collision(getCoord()[0] + PlayerCollideBoxSize, getCoord()[1] - PlayerCollideBoxSize,
			getCoord()[0] + PlayerCollideBoxSize, getCoord()[1] + PlayerCollideBoxSize)
		|| B.collision(getCoord()[0] + PlayerCollideBoxSize, getCoord()[1] + PlayerCollideBoxSize,
			getCoord()[0] - PlayerCollideBoxSize, getCoord()[1] + PlayerCollideBoxSize)
		) {
		life--;
		lastCollision = getFrameCount();
	}
}

void Player::launch() {
	if (!harpoonvalid(harpoon)) {
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

	if (harpoonvalid(harpoon)) {
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