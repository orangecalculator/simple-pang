#include "Ball.h"
#include "Block.h"
#include "Pang.h"
#include <GL/glut.h>
#include <math.h>

/*
	Ball Trajectory
	Y = - (gravity / (2 * speedX ** 2)) * (X - speedX * speedY / gravity)
		+ (speedY ** 2 / (2 * gravity))
*/

Ball::Ball(double InitX, double InitY, double radius, bool goRight)
	: coord{ InitX, InitY }, peakcoord{ InitX, InitY },
		velocityX(goRight ? BallSpeedX : -BallSpeedX), radius(radius){

}

template<typename T>
static inline T square(T x) { return x * x; }

void Ball::setvelocity(double velX, double velY) {
	velocityX = velX;

	peakcoord[0] = coord[0] + velocityX * velY / BallGravity;
	peakcoord[1] = coord[1] + square(velY) / (2 * BallGravity);
}

double Ball::getcoordX() const {
	return coord[0];
}

double Ball::getcoordY() const {
	return coord[1];
}

double Ball::getvelocityX() const {
	return velocityX;
}

double Ball::getvelocityY() const {
	return getvelYatX(coord[0]);
}

void Ball::mirror(double ux, double uy, double uc) {
	//mirror against line ux * x + uy * y = uc

	const double absusq = square(ux) + square(uy);
	if (absusq < tol) {
		return;
	}

	double velocityY = getvelocityY();

	{
		const double diff = 2 * (ux * coord[0] + uy * coord[1] - uc) / absusq;
		coord[0] -= diff * ux;
		coord[1] -= diff * uy;
	}

	{
		const double diff = 2 * (ux * velocityX + uy * velocityY) / absusq;
		velocityX -= diff * ux;
		velocityY -= diff * uy;
		setvelocity(velocityX, velocityY);
	}
	
}

typedef struct VEC2D {
	double X;
	double Y;
} VEC2D;

static VEC2D mirrorVEC2D(double VX, double VY,
	double ux, double uy, double uc = 0.0) {
	//mirror against line ux * x + uy * y = uc

	const double absusq = square(ux) + square(uy);
	if (absusq < tol) {
		return VEC2D{ VX, VY };
	}

	const double diff = 2 * (ux * VX + uy * VY - uc) / absusq;
	VX -= diff * ux;
	VY -= diff * uy;

	return VEC2D{ VX, VY };
}

static double pointdistsq(double LX, double LY, double RX, double RY) {
	return square(LX - RX) + square(LY - RY);
}

static double pointlinedistsq(double PX, double PY, double LX, double LY, double RX, double RY) {
	/*
		test collision with a line segment
			from (LX, LY) to (RX, RY)

		reference: Steven Halim, Felix Halim, Competitive Programming 3
	*/

	const double DX = RX - LX;
	const double DY = RY - LY;

	const double SQABSD = square(DX) + square(DY);
	if (SQABSD < tol) {
		return pointdistsq(PX, PY, LX, LY);
	}

	//dot product
	const double u = ((PX - LX) * DX + (PY - LY) * DY) / SQABSD;

	if (u < 0) return pointdistsq(PX, PY, LX, LY);
	else if (u > 1) return pointdistsq(PX, PY, RX, RY);
	else {
		const double HX = LX + DX * u;
		const double HY = LY + DY * u;

		return pointdistsq(PX, PY, HX, HY);
	}
}

//static double pointlinemaxdistsq(double PX, double PY, double LX, double LY, double RX, double RY) {
//	return std::max(pointdistsq(PX, PY, LX, LY), pointdistsq(PX, PY, RX, RY));
//}

static double findinternaldiv(double PX, double PY, double SX, double SY, double EX, double EY, double distsq) {
	/*
		find the first point that has distance sqrt(distsq) with point (PX, PY)
		when starting from SX, SY and proceed toward EX, EY
	*/

	double Lp = 0.0;
	double Rp = 0.0;
	while (Rp - Lp > tol) {
		const double Mp = (Lp + Rp) / 2;
		const double MX = (1.0 - Mp) * SX + Mp * EX;
		const double MY = (1.0 - Mp) * SY + Mp * EY;

		if (distsq < pointlinedistsq(PX, PY, SX, SY, MX, MY))
			Rp = Mp;
		else
			Lp = Mp;
	}

	return Rp;

}

bool Ball::collision(double posX, double posY) const {
	/*
		test collision with a point (posX, posY)
	*/

	return (pointdistsq(coord[0], coord[1], posX, posY) < square(radius));
}

bool Ball::collision(double LX, double LY, double RX, double RY) const {
	return (pointlinedistsq(coord[0], coord[1], LX, LY, RX, RY) < square(radius));
}

static inline double parabolaconst(double SpeedX) {
	return (BallGravity / (2 * square(SpeedX)));
}

double Ball::getYatX(double coordX) const {
	return -parabolaconst(velocityX) * square(coordX - peakcoord[0]) + peakcoord[1];
}

double Ball::getleftXatY(double coordY) const {
	if (peakcoord[1] < coordY)
		return -1.0;

	return peakcoord[0] - sqrt((peakcoord[1] - coordY) / parabolaconst(velocityX));
}

double Ball::getrightXatY(double coordY) const {
	if (peakcoord[1] < coordY)
		return -1.0;

	return peakcoord[0] + sqrt((peakcoord[1] - coordY) / parabolaconst(velocityX));
}

double Ball::getvelYatX(double coordX) const {
	const double coordtime = (coordX - peakcoord[0]) / velocityX;
	return -coordtime * BallGravity;
}

class FrameCalculator {
public:
	double leftframedelta;
	double nextframedelta;
	double ux, uy, uc;
	bool coercevx, coercevy;
	double vx, vy;

	FrameCalculator(double totalframedelta)
		: leftframedelta(totalframedelta) {
		initialize(leftframedelta);
	}

	void initialize(double leftframedelta) {
		nextframedelta = leftframedelta;
		ux = 0.0;
		uy = 0.0;
		uc = 0.0;
		coercevx = false;
		coercevy = false;
		vx = 0.0;
		vy = 0.0;
	}

	void apply() {
		leftframedelta -= nextframedelta;
		initialize(leftframedelta);
	}

	bool frameleft() const { return (leftframedelta > tol); }

	void tryupdate(double framedelta, double _ux, double _uy, double _uc,
					bool _coercevx, double _vx,
					bool _coercevy, double _vy) {
		if (framedelta <= tol)
			return;
		else if (framedelta < nextframedelta) {
			nextframedelta = framedelta;
			ux = _ux;
			uy = _uy;
			uc = _uc;
			coercevx = _coercevx;
			coercevy = _coercevy;
			vx = _vx;
			vy = _vy;
		}
	}

	double getnextframedelta() const { return nextframedelta; }
	double getux() const { return ux; }
	double getuy() const { return uy; }
	double getuc() const { return uc; }
	double getcoercevx() const { return coercevx; }
	double getcoercevy() const { return coercevy; }
	double getvx() const { return vx; }
	double getvy() const { return vy; }
};

void Ball::nextframe() {
	double framedelta = 1.0;
	FrameCalculator F(framedelta);

	while (F.frameleft()) {

		double candX, candY;
		double candframedelta;

		candY = GameFrameDown + radius;
		candX = getleftXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		F.tryupdate(candframedelta, 0.0, 1.0, candY,
			false, 0.0, true, bouncespeed(radius));

		candY = GameFrameDown + radius;
		candX = getrightXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		F.tryupdate(candframedelta, 0.0, 1.0, candY,
			false, 0.0, true, bouncespeed(radius));

		candY = GameFrameUp - radius;
		candX = getleftXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelY = getvelYatX(candX);
			if (candvelY > 0.0)
				candvelY = -candvelY;
			F.tryupdate(candframedelta, 0.0, 1.0, candY,
				false, 0.0, true, candvelY);
		}

		candY = GameFrameUp - radius;
		candX = getrightXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelY = getvelYatX(candX);
			if (candvelY > 0.0)
				candvelY = -candvelY;
			F.tryupdate(candframedelta, 0.0, 1.0, candY,
				false, 0.0, true, candvelY);
		}

		candX = GameFrameRight - radius;
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelX = velocityX;
			if (candvelX > 0.0)
				candvelX = -candvelX;
			F.tryupdate(candframedelta, 1.0, 0.0, candX,
				true, candvelX, false, 0.0);
		}

		candX = GameFrameLeft + radius;
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelX = velocityX;
			if (candvelX < 0.0)
				candvelX = -candvelX;
			F.tryupdate(candframedelta, 1.0, 0.0, candX,
				true, candvelX, false, 0.0);
		}

		coord[0] += velocityX * F.getnextframedelta();
		coord[1] = getYatX(coord[0]);

		//mirror(F.getux(), F.getuy(), F.getuc());
		if (F.getcoercevx() || F.getcoercevy())
			setvelocity(F.getcoercevx() ? F.getvx() : velocityX,
				F.getcoercevy() ? F.getvy() : getvelocityY());
		F.apply();
	}
}

void Ball::futurenextframe() {
	double framedelta = 1.0;
	FrameCalculator F(framedelta);

	while (F.frameleft()) {

		double candX, candY;
		double candframedelta;

		candY = GameFrameDown + radius;
		candX = getleftXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		F.tryupdate(candframedelta, 0.0, 1.0, candY,
			false, 0.0, true, bouncespeed(radius));

		candY = GameFrameDown + radius;
		candX = getrightXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		F.tryupdate(candframedelta, 0.0, 1.0, candY,
			false, 0.0, true, bouncespeed(radius));

		candY = GameFrameUp - radius;
		candX = getleftXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelY = getvelYatX(candX);
			if (candvelY > 0.0)
				candvelY = -candvelY;
			F.tryupdate(candframedelta, 0.0, 1.0, candY,
				false, 0.0, true, candvelY);
		}

		candY = GameFrameUp - radius;
		candX = getrightXatY(candY);
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelY = getvelYatX(candX);
			if (candvelY > 0.0)
				candvelY = -candvelY;
			F.tryupdate(candframedelta, 0.0, 1.0, candY,
				false, 0.0, true, candvelY);
		}

		candX = GameFrameRight - radius;
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelX = velocityX;
			if (candvelX > 0.0)
				candvelX = -candvelX;
			F.tryupdate(candframedelta, 1.0, 0.0, candX,
				true, candvelX, false, 0.0);
		}

		candX = GameFrameLeft + radius;
		candframedelta = (candX - coord[0]) / velocityX;
		{
			double candvelX = velocityX;
			if (candvelX < 0.0)
				candvelX = -candvelX;
			F.tryupdate(candframedelta, 1.0, 0.0, candX,
				true, candvelX, false, 0.0);
		}

		for (const double edgeX : {GameFrameLeft, GameFrameRight})
			for (const double edgeY : {GameFrameUp, GameFrameDown}) {
			const double nextX = coord[0] + F.getnextframedelta() * velocityX;
			const double nextY = getYatX(nextX);
			if (pointlinedistsq(edgeX, edgeY,
				coord[0], coord[1], nextX, nextY) < square(radius)) {
				candframedelta = F.getnextframedelta() * findinternaldiv(edgeX, edgeY,
					coord[0], coord[1], nextX, nextY, square(radius));
				
				candX = coord[0] + candframedelta * velocityX;
				candY = getYatX(candX);

				VEC2D V = mirrorVEC2D(velocityX, getvelYatX(candX),
					-(candY - edgeY), (candX - edgeX));

				//check direction
				const double signX = (candX > edgeX ? 1.0 : -1.0);
				const double signY = (candY > edgeY ? 1.0 : -1.0);

				if (V.X * signX < 0 && V.Y * signY < 0) {
					V.X = -V.X;
					V.Y = -V.Y;
				}

				F.tryupdate(candframedelta, 0.0, 0.0, 0.0,
					true, V.X, true, V.Y);
			}
		}

		coord[0] += velocityX * F.getnextframedelta();
		coord[1] = getYatX(coord[0]);

		//mirror(F.getux(), F.getuy(), F.getuc());
		if (F.getcoercevx() || F.getcoercevy())
			setvelocity(F.getcoercevx() ? F.getvx() : velocityX,
				F.getcoercevy() ? F.getvy() : getvelocityY());
		F.apply();
	}
}

void Ball::draw() const {
	glPushMatrix();

	glTranslated(coord[0], coord[1], 0.0f);
	glutSolidSphere(radius, BallSlice, BallStack);

	glPopMatrix();
}