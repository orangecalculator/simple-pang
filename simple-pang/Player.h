#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Ball.h"
#include "Block.h"

class Player {
private:
	double coord[2];
	double harpoon;

	int life;
	long lastCollision;

public:
	Player();

	void init();

	void setCoord(double x, double y);
	void setHarpoon(double harpoon);
	void setLife(int lifeNumber);

	const double* getCoord() const;
	double getHarpoon() const;

	int getLife() const;

	void useHarpoon();
	void move(double dx, double dy);
	void nextframe(double framedelta);

	void checkcollision(const Ball&);
	bool checkHarpooncollision(const Ball&);
	bool checkHarpooncollision(const Block&);
	void launch();
	void draw() const;
};

#endif