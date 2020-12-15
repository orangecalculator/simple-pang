#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <vector>
#include "Ball.h"

class Block
{
public:
	typedef double (*BOUNCEFUNCTION)(double originalspeed, const Ball&);

private:
	double Left;
	double Right;

	double Up;
	double Down;

	Material mtl;

public:
	Block(double Left, double Right, double Up, double Down);

	double getLeft() const;
	double getRight() const;
	double getUp() const;
	double getDown() const;

	virtual BOUNCEFUNCTION getBounceLeft() const;
	virtual BOUNCEFUNCTION getBounceRight() const;
	virtual BOUNCEFUNCTION getBounceUp() const;
	virtual BOUNCEFUNCTION getBounceDown() const;

	virtual void draw() const;
};

class OuterFrameBlock : public Block {
public:
	OuterFrameBlock(double Left, double Right, double Up, double Down);

	virtual BOUNCEFUNCTION getBounceDown() const;

	virtual void draw() const;
};

extern std::vector<Block*> blocks;

#endif /* _BLOCK_H_ */