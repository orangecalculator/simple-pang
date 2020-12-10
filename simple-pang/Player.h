#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Ball.h"

class Player {
private:
	double coord[2];
	double harpoon;

	int life;
	long lastCollision;

public:
	Player();

	void setCoord(double x, double y);
	void setHarpoon(double harpoon);
	void setLife(int lifeNumber);

	const double* getCoord() const;
	double getHarpoon() const;

	int getLife() const;

	void useHarpoon();
	void move(double dx, double dy);
	void nextframe();

	void checkcollision(const Ball&);
	void launch();
	void draw() const;
};

#endif