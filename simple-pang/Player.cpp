#include <GL/glut.h>
#include "Player.h"

Player::Player() : coord{} {

}

void Player::setCoord(float x, float y) {
	coord[0] = x;
	coord[1] = y;
}

const float* Player::getCoord() const{
	return coord;
}

void Player::move(float dx, float dy) {
	coord[0] += dx;
	coord[1] += dy;
}

void Player::draw() const {
	glPushMatrix();

	glTranslatef(coord[0], coord[1], 0.0f);

	//to be implemented
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 0.1f);
	glEnd();
	glPointSize(1);
	
	glPopMatrix();
}