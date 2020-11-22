
#include "Ball.h"
#include "Block.h"
#include "Pang.h"
#include <GL/glut.h>
#include <math.h>

/*
	Ball Trajectory
	Y = - (gravity / (2 * speedX ** 2)) * (X - speedX * speedY / gravity) ** 2
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

double Ball::getpeakcoordX() const {
	return peakcoord[0];
}

double Ball::getpeakcoordY() const {
	return peakcoord[1];
}

double Ball::getvelocityX() const {
	return velocityX;
}

double Ball::getvelocityY() const {
	return getvelYatX(coord[0]);
}

double Ball::getradius() const {
	return radius;
}

double Ball::getbouncespeed() const {
	constexpr double BallSizeScale = BallMaxSize / 8.0;
	
	const double x = radius / BallSizeScale;
	
	// f(x) = Max * (1 - exp(- x / scale))
	return BallMaxSpeedY * (1 - 1 / (1.0 + x + 0.5 * square(x)));
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
	double Rp = 1.0;
	while (Rp - Lp > tol) {
		const double Mp = (Lp + Rp) / 2;
		const double MX = (1.0 - Mp) * SX + Mp * EX;
		const double MY = (1.0 - Mp) * SY + Mp * EY;

		if (distsq + tol < pointlinedistsq(PX, PY, SX, SY, MX, MY))
			Lp = Mp;
		else
			Rp = Mp;
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
	const Ball& B;
	double leftframedelta;
	double nextframedelta;
	double ux, uy, uc;
	double vx, vy;
	bool change;

	FrameCalculator(const Ball& B, double totalframedelta)
		: B(B), leftframedelta(totalframedelta),
		change(false), ux(0.0), uy(0.0), uc(0.0), vx(0.0), vy(0.0) {
		initialize(leftframedelta);
	}

	void initialize(double leftframedelta) {
		nextframedelta = leftframedelta;
		change = false;
		//ux = 0.0;
		//uy = 0.0;
		//uc = 0.0;
		//vx = B.getvelocityX();
		//vy = B.getvelYatX(B.getcoordX() + B.getvelocityX() * nextframedelta);
	}

	void apply() {
		leftframedelta -= nextframedelta;
		initialize(leftframedelta);
	}

	bool frameleft() const { return (leftframedelta > tol); }

	void tryupdate(double framedelta, double _ux, double _uy, double _uc,
					double _vx, double _vy) {
		if (framedelta < -tol)
			;
		else if (framedelta < nextframedelta) {
			nextframedelta = framedelta;
			change = true;
			ux = _ux;
			uy = _uy;
			uc = _uc;
			vx = _vx;
			vy = _vy;
		}
	}

	void considerVertical(double lineX, double ulim, double dlim) {
		if (B.getcoordX() < lineX) {
			const double candX = lineX - B.getradius();

			if (!(B.getcoordX() > candX
				|| B.getcoordX() + B.getvelocityX() * nextframedelta > candX))
				return;
			else {
				const double candY = B.getYatX(candX);
				if (!(dlim + tol < candY && candY < ulim - tol))
					return;
			}

			DEBUG("Vertical Line Left Hit Detected at %11.4g\n", lineX);

			const double framecount = (candX - B.getcoordX()) / B.getvelocityX();

			double nextvelocityX = B.getvelocityX();
			if (nextvelocityX > 0.0)
				nextvelocityX = -nextvelocityX;

			tryupdate(framecount, 1.0, 0.0, candX,
				nextvelocityX, B.getvelYatX(candX));
		}
		else {
			const double candX = lineX + B.getradius();

			if (!(B.getcoordX() < candX
				|| B.getcoordX() + B.getvelocityX() * nextframedelta < candX))
				return;
			else {
				const double candY = B.getYatX(candX);
				if (!(dlim + tol < candY && candY < ulim - tol))
					return;
			}

			DEBUG("Vertical Line Right Hit Detected at %11.4g\n", lineX);

			const double framecount = (candX - B.getcoordX()) / B.getvelocityX();

			double nextvelocityX = B.getvelocityX();
			if (nextvelocityX < 0.0)
				nextvelocityX = -nextvelocityX;

			tryupdate(framecount, 1.0, 0.0, candX,
				nextvelocityX, B.getvelYatX(candX));
		}
	}

	void considerHorizontal(double lineY, double llim, double rlim) {
		if (B.getcoordY() < lineY) {
			const double candY = lineY - B.getradius();

			if (B.getpeakcoordY() <= candY)
				return;
			else if (B.getcoordY() > candY)
				;
			else if (B.getcoordX() < B.getpeakcoordX() &&
				B.getleftXatY(candY)
				< B.getcoordX() + B.getvelocityX() * nextframedelta)
				;
			else if (B.getcoordX() >= B.getpeakcoordX() &&
				B.getrightXatY(candY)
				> B.getcoordX() + B.getvelocityX() * nextframedelta)
				;
			else
				return;

			double framedelta;
			double candX;
			if (B.getcoordX() < B.getpeakcoordX()) {
				framedelta = (B.getleftXatY(candY) - B.getcoordX()) / B.getvelocityX();
				candX = B.getleftXatY(candY);
			}
			else {
				framedelta = (B.getrightXatY(candY) - B.getcoordX()) / B.getvelocityX();
				candX = B.getrightXatY(candY);
			}

			if (!(llim + tol < candX && candX < rlim - tol))
				return;

			DEBUG("Horizontal Line Bottom Hit Detected at %11.4g\n", lineY);

			double nextvelocityY = B.getvelYatX(candX);
			if (nextvelocityY > 0.0)
				nextvelocityY = -nextvelocityY;

			tryupdate(framedelta, 0.0, 1.0, candY,
				B.getvelocityX(), nextvelocityY);
		}
		else {
			const double candY = lineY + B.getradius();
			double candX;
			double framedelta;

			if (B.getcoordY() < candY)
				;
			else if (B.getvelocityX() < 0.0 &&
				B.getleftXatY(candY)
			> B.getcoordX() + B.getvelocityX() * nextframedelta)
				;
			else if (B.getvelocityX() >= 0.0 &&
				B.getrightXatY(candY)
				< B.getcoordX() + B.getvelocityX() * nextframedelta)
				;
			else
				return;

			if (B.getvelocityX() < 0.0) {
				framedelta = (B.getleftXatY(candY) - B.getcoordX()) / B.getvelocityX();
				candX = B.getleftXatY(candY);
			}
			else {
				framedelta = (B.getrightXatY(candY) - B.getcoordX()) / B.getvelocityX();
				candX = B.getrightXatY(candY);
			}

			if (!(llim + tol < candX && candX < rlim - tol))
				return;

			DEBUG("Horizontal Line Up Hit Detected at %11.4g\n", lineY);

			tryupdate(framedelta, 0.0, 1.0, candY,
				B.getvelocityX(), B.getbouncespeed());
		}
	}

	void considerPoint(double PX, double PY) {
		
		const double nextX = B.getcoordX() + nextframedelta * B.getvelocityX();
		const double nextY = B.getYatX(nextX);

		if (B.collision(PX, PY))
			;
		if (pointlinedistsq(PX, PY, B.getcoordX(), B.getcoordY(), nextX, nextY)
			< square(B.getradius()))
			;
		else
			return;

		DEBUG("Edge Hit Detected at %11.4g %11.4g\n", PX, PY);

		const double framedelta = nextframedelta
			* findinternaldiv(PX, PY,
				B.getcoordX(), B.getcoordY(), nextX, nextY,
				square(B.getradius()));

		const double candX = B.getcoordX() + framedelta * B.getvelocityX();
		const double candY = B.getYatX(candX);

		double ux = candX - PX;
		double uy = candY - PY;
		const double udot = square(ux) + square(uy);
		if (udot > tol) {
			const double udotsqrt = sqrt(udot);
			ux /= udotsqrt;
			uy /= udotsqrt;
		}

		VEC2D V = VEC2D{ B.getvelocityX(), B.getvelYatX(candX) };
		
		if (V.X * ux + V.Y * uy < 0.0) {
			V = mirrorVEC2D(V.X, V.Y, ux, uy);
		}

		const double uc = ux * (PX + ux * B.getradius())
			+ uy * (PY + uy * B.getradius());

		tryupdate(framedelta, ux, uy, uc,
				V.X, V.Y);
	}

	double getnextframedelta() const { return nextframedelta; }
	bool getchange() const { return change; }
	double getux() const { return ux; }
	double getuy() const { return uy; }
	double getuc() const { return uc; }
	double getvx() const { return vx; }
	double getvy() const { return vy; }
};

void Ball::nextframe() {
	double framedelta = 1.0;
	FrameCalculator F(*this, framedelta);

	while (F.frameleft()) {

		for (const Block& B : blocks) {
			F.considerHorizontal(B.getDown(), B.getLeft(), B.getRight());
			F.considerHorizontal(B.getUp(), B.getLeft(), B.getRight());
			F.considerVertical(B.getLeft(), B.getUp(), B.getDown());
			F.considerVertical(B.getRight(), B.getUp(), B.getDown());

			F.considerPoint(B.getLeft(), B.getUp());
			F.considerPoint(B.getLeft(), B.getDown());
			F.considerPoint(B.getRight(), B.getDown());
			F.considerPoint(B.getRight(), B.getUp());
		}

		if (F.getnextframedelta() < -tol)
			DEBUG("negative framedelta detected\n");

		coord[0] += velocityX * F.getnextframedelta();
		coord[1] = getYatX(coord[0]);

		if(F.getchange()){
			mirror(F.getux(), F.getuy(), F.getuc());
			setvelocity(F.getvx(), F.getvy());

			F.apply();

			F.initialize(tol);

			coord[0] += velocityX * F.getnextframedelta();
			coord[1] = getYatX(coord[0]);

			DEBUG("\n");

		}

		F.apply();
	}

	//DEBUG("Ball Trajectory %11.4g %11.4g from peak %11.4g %11.4g\n", coord[0], coord[1], peakcoord[0], peakcoord[1]);
}

void Ball::futurenextframe() {

}

void Ball::draw() const {
	glPushMatrix();

	glTranslated(coord[0], coord[1], 0.0f);
	glutSolidSphere(radius, BallSlice, BallStack);

	glPopMatrix();
}

std::vector<Ball> balls;