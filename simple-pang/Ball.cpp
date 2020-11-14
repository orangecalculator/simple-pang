#include "Ball.h"
#include "Pang.h"
#include <GL/glut.h>
#include <math.h>

/*
	Ball Trajectory
	Y = - (gravity / (2 * speedX ** 2)) * (X - speedX * speedY / gravity)
		+ (speedY ** 2 / (2 * gravity))
*/

Ball::Ball(float InitX, float InitY, float radius, bool goRight)
	: coord{ InitX, InitY }, peakcoord{ InitX, InitY }, goRight(goRight), radius(radius){

}

template<typename T>
static inline T square(T x) { return x * x; }

static inline double getlocationY(double coordX, double peakX, double peakY) {
	constexpr double parabolaconst = (BallGravity / (2 * BallSpeedX * BallSpeedX));
	
	return -parabolaconst * square(coordX - peakX) + peakY;
}

static inline double mirror(double location, double mirror) {
	return 2 * mirror - location;
}

void Ball::nextframe() {

	constexpr double parabolaconst = (BallGravity / (2 * BallSpeedX * BallSpeedX));
	const double baseY = GameFrameDown + radius;

	//update coord
	coord[0] += (goRight ? BallSpeedX : -BallSpeedX);
	coord[1] = baseY + getlocationY(coord[0], peakcoord[0], peakcoord[1] - baseY);

	//bounce detection
	while (coord[1] < baseY - tol) {
		const double sign = (goRight ? 1.0 : -1.0);
		const double bouncecoordX = peakcoord[0]
			+ sign * sqrt((peakcoord[1] - baseY) / parabolaconst);

		peakcoord[0] = bouncecoordX + sign * BallSpeedX * bouncespeed(radius) / BallGravity;
		peakcoord[1] = baseY + square(bouncespeed(radius)) / (2 * BallGravity);

		coord[1] = baseY + getlocationY(coord[0], peakcoord[0], peakcoord[1] - baseY);
	}

	//mirror until frame in bound
	while (true) {

		double mirrorcoord;
		if (coord[0] < GameFrameLeft + radius - tol) {
			mirrorcoord = GameFrameLeft + radius;
		}
		else if (GameFrameRight - radius + tol < coord[0]) {
			mirrorcoord = GameFrameRight - radius;
		}
		else
			break;

		coord[0] = mirror(coord[0], mirrorcoord);
		peakcoord[0] = mirror(peakcoord[0], mirrorcoord);
		goRight = !goRight;
	}
}

void Ball::draw() const {
	glPushMatrix();

	glTranslated(coord[0], coord[1], 0.0f);
	glutSolidSphere(radius, BallSlice, BallStack);

	glPopMatrix();
}