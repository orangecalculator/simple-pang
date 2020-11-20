#include "Block.h"

Block::Block(double Left, double Right, double Up, double Down)
	: Left(Left), Right(Right), Up(Up), Down(Down) {

}

double Block::getLeft() {
	return Left;
}

double Block::getRight() {
	return Right;
}

double Block::getUp() {
	return Up;
}

double Block::getDown() {
	return Down;
}

std::vector<Block> blocks;