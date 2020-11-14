#ifndef _BALL_H_
#define _BALL_H_

class Ball
{
private:
	double coord[2];

	double peakcoord[2];
	bool goRight;

	double radius;

	static inline double bouncespeed(double radius) { return radius; }

public:
	Ball(float initcoordX, float initcoordY, float radius, bool goRight);

	void nextframe();

	void draw() const;
};

#endif /* _BALL_H_ */