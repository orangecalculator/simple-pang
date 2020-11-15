#ifndef _BALL_H_
#define _BALL_H_

class Ball
{
private:
	double coord[2];

	double peakcoord[2];
	bool goRight;

	double radius;

	static inline double bouncespeed(double radius) { return radius / 4; }

public:
	Ball(double initcoordX, double initcoordY, double radius, bool goRight);

	bool collision(double posX, double posY) const;
	bool collision(double LX, double LY, double RX, double RY) const;

	void nextframe();

	void draw() const;
};

#endif /* _BALL_H_ */