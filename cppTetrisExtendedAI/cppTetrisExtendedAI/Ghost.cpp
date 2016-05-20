#include "Ghost.h"

void Ghost::gen(const Block& block, const Board& board) {
	setPosX(block.getPos().x);
	setPosY(block.getPos().y + 4);
	setPosZ(block.getPos().z);

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (block.getBlock(y, x) != 0) {
				//if (block.getBlockShape(getPos().z, y, x)!=0) {
				setBlock(y, x, getPos().z + 1);
			}
			else {
				setBlock(y, x, 0);
			}
		}
	}

	findPos(block, board);

}

void Ghost::findPos(const Block& block, const Board& board) {
	setPosX(block.getPos().x);
	setPosZ(block.getPos().z);

	while (!collision(*this, board)) {
		moveDown();
	}
	moveUp("up");
	show(block);

}

bool Ghost::collision(const Block& block, const Board& board) {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (board.getBoard(getPos().y + y, getPos().x + x) != 0 &&
				getBlock(y, x) != 0) {
				return true;
			}
		}
	}

	return false;
}

void Ghost::show(const Block& block) const {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if ((getPos().y > block.getPos().y + 3) && getBlock(y, x)) {

				printXY(getPos().x + x + startPos.X, getPos().y + startPos.Y + y, "H", getPos().z + 4);
			}
		}
	}
}

void Ghost::setStartPos(int _y, int _x) {
	startPos.Y = _y;
	startPos.X = _x;


}