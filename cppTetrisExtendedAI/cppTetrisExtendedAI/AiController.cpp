#include "AiController.h"

AiController::AiController() {
	Controller();
	board.setBoardStart(2, 40);
	block.setCoordStartPos(2, 40);

	targetPos.x = 5; targetPos.y = 1; targetPos.rotation = 0; targetPos.rotation = 0;
	aispeed = 0.1;
}

void AiController::playGame() {
	block.randomizeShape();
	while (key != QUIT) {
		//timeControl(); //key�Է¹޴°� �������
		//������ Ű�Է� �ȹ����ϱ� ���� �ð���� ����

		//switch (key)
		//{
		//case RIGHT:
		//	block.moveRight();
		//	if (collision())
		//		block.moveLeft();
		//	break;
		//case LEFT:
		//	block.moveLeft();
		//	if (collision())
		//		block.moveRight();
		//	break;
		//case UP:
		//	block.moveUp();
		//	if (collision()) {
		//		block.moveUp(); block.moveUp(); block.moveUp();
		//	}
		//	break;
		//case DOWN:
		//	block.moveDown();
		//	if (collision())
		//		stack();
		//	break;
		//case SPACE:
		//	while (!collision()) {
		//		block.moveDown();
		//	}stack();
		//	break;
		//default:
		//	break;
		//}
		targetPos = findIdealPosition();
		moveToBestPos();

		board.writeBlockOnBoard(block);
		board.drawBoard();
		//block.drawNextBlock();-> block.randomizeNextBlock�� ������ �Ź��׸��°ͺ��� �װ� �� ȿ�����̶�
		board.eraseBlockOffBoard(block);
	}
}

void AiController::animationEffect(const double _time) {
	board.writeBlockOnBoard(block);
	board.drawBoard();
	board.eraseBlockOffBoard(block);
	Sleep((DWORD)_time*speed);
}

void AiController::moveToBestPos() {
	int i;
	block.setPosY(1); block.setPosX(5);
	for (i = 0; i < targetPos.rotation; i++) {
		block.moveUp();
		if (collision())
			nxt = QUIT;
		animationEffect(4);
	}
	while (block.getPos().x != targetPos.x) {
		if (block.getPos().x > targetPos.x) {
			block.moveLeft();
			if (collision())
				nxt = QUIT;
			animationEffect(2);
		}
		else if (block.getPos().x < targetPos.x) {
			block.moveRight();
			if (collision())
				nxt = QUIT;
			animationEffect(2);
		}
	}
	animationEffect(2);
	while (!collision()) {
		animationEffect(0.5);
		block.moveDown();
	}
	stack();
	return;
}

PosAi AiController::findIdealPosition() {
	int firstX = block.getPos().x, firstY = block.getPos().y;
	PosAi position[4] = { 0, };
	int highestScore = 0, plusScore = 0, minusScore = 0, totalScore = 0;
	int rotation = 0, bestRotation = 0;
	int i = 0;

	for (rotation = 0; rotation < 4; rotation++) {
		for (block.setPosX(0), i = 0; 1; block.moveRight()) {
			block.setPosY(1);

			if (i == 0 && collision()) {
				block.setPosX(1); i++;
			}

			if (collision()) {
				break;
			}

			while (!collision()) {
				block.moveDown();
			}
			block.moveUp("up");
			board.writeBlockOnBoard(block);
			plusScore = 7 * getIdealPoint() + 5 * getClearLinePoint() + 10 * getAdjacentPoint();
			minusScore = 17 * getMinusPoint();
			totalScore = plusScore - minusScore;
			board.eraseBlockOffBoard(block);

			if (highestScore <= totalScore) {
				highestScore = totalScore;
				position[rotation].x = block.getPos().x;
				position[rotation].y = block.getPos().y;
				position[rotation].rotation = rotation;
				position[rotation].score = totalScore;
			}
		}

		block.moveUp();
	}
	highestScore = 0;

	for (rotation = 0; rotation < 4; rotation++) {

		if (position[rotation].score >= highestScore) {
			highestScore = position[rotation].score;
			bestRotation = rotation;
		}

	}

	block.setPosX(firstX); block.setPosX(firstY);
	return position[bestRotation];
}

int AiController::getIdealPoint() {
	int boardIdealPoint = 0;
	int x = 0, y = 0;
	
	for (y = 2; y < board.Y_LEN - 1; y++) {
		for (x = 1; x < board.X_LEN - 1; x++) {

			if (board.getBoard(y, x) != 0) {
				boardIdealPoint += y;
			}

		}
	}
	
	return boardIdealPoint;
}

int AiController::getAdjacentPoint() {
	int adjacentPoint = 0;
	int x = 0, y = 0;

	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {

			if (block.getBlock(y, x) != 0) {

				if (board.getBoard(block.getPos().y + y, block.getPos().x + x - 1) != 0) {
					adjacentPoint += block.getPos().y + y;
				}

				if (board.getBoard(block.getPos().y + y, block.getPos().x + x + 1) != 0) {
					adjacentPoint += block.getPos().y + y;

				}
			}

		}
	}
	return adjacentPoint;
}

int AiController::getMinusPoint() {
	int boardMinusPoint = 0;
	int x = 0, y = 0;
	int height = 0;

	
	//for (x = block.getPos().x; x < block.getPos().x + 4; x++) {
	//	
	//	if (block.getBlock(0,x)||block.getBlock(1, x)||block.getBlock(2, x)||block.getBlock(3,x)) {
	//		for (y = 2; board.getBoard(y, x) == 0; y++) {
	//			height = y;
	//		}

	//		for (y = board.Y_LEN - 1; y > height; y--) {

	//			if (board.getBoard(y, x) == 0) {
	//				
	//				boardMinusPoint += board.Y_LEN - y + 10;
	//			}
	//		}
	//	}

	//}//making new formular

	for (x = 1; x < board.X_LEN -1; x++) {
	
		for (y = 2; board.getBoard(y, x) == 0; y++) {
			height = y;
		}

		for (y = board.Y_LEN - 1; y > height; y--) {

			if (board.getBoard(y, x) == 0) {
				
				boardMinusPoint += y;
			}
		}

	}//stable formular

	return boardMinusPoint;
}

int AiController::getClearLinePoint() {
	int clearLinePoint = 0;
	int x = 0, y = 0;
	int count = 0;
	
	for (y = block.getPos().y; y < block.getPos().y + 4; y++) {
		for (x = 1, count = 0; x < board.X_LEN - 1; x++) {

			if (board.getBoard(y, x) != 0) {
				count++;
			}
		}

		if (count == board.X_LEN - 2) {
			clearLinePoint += y*(board.X_LEN - 2);
		}
	}
	
	//		if (count == board.X_LEN - 3) {}
	
	return clearLinePoint;
}

//@Override