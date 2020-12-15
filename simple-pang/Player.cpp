#include "Player.h"
#include "Pang.h"
#include "PangIO.h"
#include "Ball.h"
#include "Texture.h"

#include <GL/glut.h>

#include <iostream>
using namespace std;

constexpr double HarpoonINVALID = -1.0;

Player::Player() : coord{ 0.0f, 0.1f + GameFrameDown }, harpoon(HarpoonINVALID),
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

void  Player::setLife(int lifeNumber) {
	life = lifeNumber;
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

bool Player::checkHarpooncollision(const Ball& B) {
	if (!harpoonvalid(getHarpoon()))
		return false;

	return B.collision(getCoord()[0], getCoord()[1], getCoord()[0], getHarpoon());
}

void Player::launch() {
	if (!harpoonvalid(harpoon)) {
		harpoon = GameFrameDown;
	}
}

void drawRope(double x, double y1, double y2){
	glEnable(GL_TEXTURE_2D);

	static Texture rImage("rope.png");
	rImage.draw();

	glBegin(GL_QUADS);

	float xsize = 0.01;
	glTexCoord2f(0, 0); glVertex3f(x - xsize, y1 , 0.0);
	glTexCoord2f(0, 1); glVertex3f(x - xsize, y2, 0.0);
	glTexCoord2f(1, 1); glVertex3f(x + xsize, y2, 0.0);
	glTexCoord2f(1, 0); glVertex3f(x + xsize, y1, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawHarpoonEnd(double x, double y) {
	glEnable(GL_TEXTURE_2D);

	static Texture rImage("harpoon_end.png");
	rImage.draw();

	glBegin(GL_QUADS);

	float xsize = 0.01;
	float ysize = 0.01;
	glTexCoord2f(0, 0); glVertex3f(x - xsize, y, 0.0);
	glTexCoord2f(0, 1); glVertex3f(x - xsize, y + 2 * ysize, 0.0);
	glTexCoord2f(1, 1); glVertex3f(x + xsize, y + 2 * ysize, 0.0);
	glTexCoord2f(1, 0); glVertex3f(x + xsize, y, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void Player::draw() const {
	glPushMatrix();

	if (harpoonvalid(harpoon)) {
		DEBUG("drawing harpoon x %5.2f y %5.2f\n", coord[0], harpoon);

		drawRope(coord[0], coord[1], harpoon);
		drawHarpoonEnd(coord[0], harpoon);
	}

	

	glPopMatrix();
}