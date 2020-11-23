#include "Pang.h"
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
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);

	glVertex2d(Left, Up);
	glVertex2d(Left, Down);
	glVertex2d(Right, Down);
	glVertex2d(Right, Up);

	glEnd();
}

OuterFrameBlock::OuterFrameBlock(double Left, double Right, double Up, double Down)
	: Block(Left, Right, Up, Down) {

}

void OuterFrameBlock::draw() const {
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex2d(getLeft(), getUp());
	glVertex2d(getLeft(), getDown());
	glVertex2d(getRight(), getDown());
	glVertex2d(getRight(), getUp());

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//bounce functions

static double SimpleBounce(double originalspeed, const Ball&) {
	return -originalspeed;
}

static double BounceOnGround(double, const Ball& B) {
	constexpr double BallSizeScale = BallMaxSize / 8.0;

	const double x = B.getradius() / BallSizeScale;

	// f(x) = Max * (1 - exp(- x / scale))
	return BallMaxSpeedY * (1 - 1 / (1.0 + x + 0.5 * square(x)));
}

Block::BOUNCEFUNCTION Block::getBounceLeft() const {
	return SimpleBounce;
}

Block::BOUNCEFUNCTION Block::getBounceRight() const {
	return SimpleBounce;
}

Block::BOUNCEFUNCTION Block::getBounceUp() const {
	return SimpleBounce;
}

Block::BOUNCEFUNCTION Block::getBounceDown() const {
	return SimpleBounce;
}

Block::BOUNCEFUNCTION OuterFrameBlock::getBounceDown() const {
	return BounceOnGround;
}

std::vector<Block*> blocks;