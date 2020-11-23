#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player {
private:
	double coord[2];
	double harpoon;

	bool harpoonvalid() const;

public:
	Player();

	void setCoord(double x, double y);
	void setHarpoon(double harpoon);

	const double* getCoord() const;
	double getHarpoon() const;

	void useHarpoon();
	void move(double dx, double dy);
	void nextframe();

	void launch();
	void draw() const;
};

#endif