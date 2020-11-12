#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player{
private:
	float coord[2];
	float harpoon;

	bool harpoonvalid() const;

public:
	Player();
	
	void setCoord(float x, float y);
	void setHarpoon(float harpoon);

	const float* getCoord() const;
	float getHarpoon() const;

	void move(float dx, float dy);
	void nextframe();

	void launch();
	void draw() const;
};

#endif