#include "Block.h"
#include <GL/glut.h>

Block::Block(double Left, double Right, double Up, double Down)
	: Left(Left), Right(Right), Up(Up), Down(Down) {

}

double Block::getLeft() const {
	return Left;
}

double Block::getRight() const {
	return Right;
}

double Block::getUp() const {
	return Up;
}

double Block::getDown() const {
	return Down;
}

void Block::draw() const {
	glBegin(GL_QUADS);

	glVertex2d(Left, Up);
	glVertex2d(Left, Down);
	glVertex2d(Right, Down);
	glVertex2d(Right, Up);

	glEnd();
}

std::vector<Block> blocks;