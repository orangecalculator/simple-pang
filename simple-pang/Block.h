#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <vector>

class Block
{
private:
	double Left;
	double Right;

	double Up;
	double Down;

public:
	Block(double Left, double Right, double Up, double Down);

	double getLeft() const;
	double getRight() const;
	double getUp() const;
	double getDown() const;

	void draw() const;
};

extern std::vector<Block> blocks;

#endif /* _BLOCK_H_ */