#include "Ball.h"
#include "Pang.h"
#include <GL/glut.h>

/*
	Ball Trajectory
	Y = - (gravity / (2 * speedX ** 2)) * (X - speedX * speedY / gravity)
		+ (speedY ** 2 / (2 * gravity))
*/

Ball::Ball(double InitX, double InitY, double radius, bool goRight)
	: coord{ InitX, InitY }, peakcoord{ InitX, InitY }, goRight(goRight), radius(radius){

}

template<typename T>
static inline T square(T x) { return x * x; }

bool Ball::collision(double posX, double posY) const {
	/*
		test collision with a point (posX, posY)
	*/

	return (square(coord[0] - posX) + square(coord[1] - posY) < square(radius));
}

bool Ball::collision(double LX, double LY, double RX, double RY) const {
	/*
		test collision with a line segment
			from (LX, LY) to (RX, RY)

		reference: Steven Halim, Felix Halim, Competitive Programming 3
	*/

	const double DX = RX - LX;
	const double DY = RY - LY;
	
	const double SQABSD = square(DX) + square(DY);
	if (SQABSD < tol) {
		return collision(LX, LY);
	}

	//dot product
	const double u = ((coord[0] - LX) * DX + (coord[1] - LY) * DY) / SQABSD;

	if (u < 0) return collision(LX, LY);
	else if (u > 1) return collision(RX, RY);
	else {
		const double PX = LX + DX * u;
		const double PY = LY + DY * u;

		return collision(PX, PY);
	}

}


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