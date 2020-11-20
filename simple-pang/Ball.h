#ifndef _BALL_H_
#define _BALL_H_

class Ball
{
private:
	double coord[2];

	double peakcoord[2];
	double velocityX;

	double radius;

	static inline double bouncespeed(double radius) { return radius / 4; }

public:
	Ball(double initcoordX, double initcoordY, double radius, bool goRight);

	double getcoordX() const;
	double getcoordY() const;
	double getvelocityX() const;
	double getvelocityY() const;

	double getYatX(double coordX) const;
	double getleftXatY(double coordY) const;
	double getrightXatY(double coordY) const;
	double getvelYatX(double coordX) const;

	void setvelocity(double velX, double velY);
	void mirror(double ux, double uy, double uc);

	bool collision(double posX, double posY) const;
	bool collision(double LX, double LY, double RX, double RY) const;

	void nextframe();
	void futurenextframe();

	void draw() const;
};

#endif /* _BALL_H_ */