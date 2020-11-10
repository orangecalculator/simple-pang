#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player{
private:
	float coord[2];

public:
	Player();
	
	void setCoord(float x, float y);

	const float* getCoord() const;

	void move(float dx, float dy);

	void draw() const;
};

#endif