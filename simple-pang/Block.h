#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block
{
private:
	double Left;
	double Right;

	double Up;
	double Down;

public:
	Block(double Left, double Right, double Up, double Down);

	double getLeft();
	double getRight();
	double getUp();
	double getDown();
};

#include <vector>
extern std::vector<Block> blocks;

#endif /* _BLOCK_H_ */