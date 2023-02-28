#define _CRT_SECURE_NO_WARNINGS

#include"tetr.h"

extern Point blockPattern[NUM_BLOCK_TYPE][NUM_BLOCK_DIR][NUM_BLOCK_POINT];
int nextBlockShape[SIZE_NEXT_ROW][SIZE_NEXT_COLUMN];
int base[SIZE_OVERALL_ROW][SIZE_OVERALL_COLUMN];
int previous[SIZE_OVERALL_ROW][SIZE_OVERALL_COLUMN];
int display[SIZE_OVERALL_ROW][SIZE_OVERALL_COLUMN];
int firstScreen[SIZE_FIRST_ROW][SIZE_FIRST_COLUMN];

int downORpause;
int currentScore = 0;
int timeValueChange;
int bestScore;

Point baseBlockSpace;
Block baseBlock;

Point currentSpace;
Point tempSpace;
Block currentBlock;
Block tempBlock;

Block nextBlock;

Point currentShadow;
Point tempShadow;

void initialization() {
	int i, k;
	//base setting
	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		base[i][0] = BLOCK_WALL;
		base[i][SIZE_OVERALL_COLUMN - 1] = BLOCK_WALL;
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			base[0][k] = BLOCK_WALL;
			base[SIZE_OVERALL_ROW - 1][k] = BLOCK_WALL;
		}
	}
	for (i = 1; i < SIZE_OVERALL_ROW - 1; i++) {
		for (k = 1; k < SIZE_OVERALL_COLUMN - 1; k++) {
			base[i][k] = BLOCK_EMPTY;
		}
	}
	//display setting
	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			display[i][k] = base[i][k];
		}
	}
	//previous setting
	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			previous[i][k] = BLOCK_EMPTY;
		}
	}
	makeNewBlock(TRUE);

	//nextBlockDisplay setting
	for (i = 0; i < SIZE_NEXT_ROW; i++) {
		nextBlockShape[i][0] = BLOCK_FENCE;
		nextBlockShape[i][SIZE_NEXT_COLUMN - 1] = BLOCK_FENCE;
		for (k = 0; k < SIZE_NEXT_COLUMN; k++) {
			nextBlockShape[0][k] = BLOCK_FENCE;
			nextBlockShape[SIZE_NEXT_ROW - 1][k] = BLOCK_FENCE;
		}
	}
	for (i = 1; i < SIZE_NEXT_ROW - 1; i++) {
		for (k = 1; k < SIZE_NEXT_COLUMN - 1; k++) {
			nextBlockShape[i][k] = BLOCK_EMPTY;
		}
	}
}

void changeBlockFunc() { // �� �ٲ� �� ������� �ٲ�� �ϱ� ���� �۾�
	int i, row, column;

	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = currentSpace.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = currentSpace.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		previous[row][column] = BLOCK_EMPTY;
	}
}

void displayGame(int level) { //������� �ٲٱ�

	int i, k;

	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			if (previous[i][k] != display[i][k]) {
				gotoxy(i, k * 2);
				switch (display[i][k]) {
				case BLOCK_EMPTY:
					printf("  ");
					break;
				case BLOCK_WALL:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("��");
					break;
				case BLOCK:
					switch (currentBlock.type) {
					case 0:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
						break;
					case 1:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
						break;
					case 2:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
						break;
					case 3:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
						break;
					case 4:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
						break;
					case 5:
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
						break;
					default:
						break;
					}
					printf("��");
					break;
				case BLOCK_SHADOW:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					printf("��");
					break;
				case BLOCK_STAR:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
					printf("��");
					break;
				default:
					break;
				}
			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(SIZE_OVERALL_ROW, 0);
	//printf("currentSpace : (%d,%d), currentBlock : (%d,%d)\n",currentSpace.row,currentSpace.column,currentBlock.type,currentBlock.dir);
	gotoxy(11, ((SIZE_OVERALL_COLUMN) * 2 + 5));
	printf("bestScore : %d\n", bestScore);
	gotoxy(12, ((SIZE_OVERALL_COLUMN) * 2 + 5));
	printf("currentScore : %d\n", currentScore);
	gotoxy(15, ((SIZE_OVERALL_COLUMN) * 2 + 5));
	printf("LEVEL : %d", level);

	//display�� --> previous�� copy
	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			previous[i][k] = display[i][k];
		}
	}

	//nextBlock display
	nextBlockDisplay();

}

void nextBlockSet() { //nextBlock�� Ÿ�԰� ������ ���� nextBlockShape�� block �߰��ϱ� 
	//==> newBlockDisplay�� �ϴ� ���� �����(��Ȯ�� ��ġ�� ��Ȯ�� �� �߰�).
	// newBlockDisplay = display�� currentBlock �߰� / nextBlockSet = nextBlockShape�� nextBlock �߰�
	int i, k, row, column;
	for (i = 1; i < SIZE_NEXT_ROW - 1; i++) {
		for (k = 1; k < SIZE_NEXT_COLUMN - 1; k++) {
			nextBlockShape[i][k] = BLOCK_EMPTY;
		}
	}
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = 3 + blockPattern[nextBlock.type][nextBlock.dir][i].row;
		column = 3 + blockPattern[nextBlock.type][nextBlock.dir][i].column;
		nextBlockShape[row][column] = BLOCK;
	}
}

void nextBlockDisplay() {

	int i, k, row = 1;
	
	gotoxy(row, ((SIZE_OVERALL_COLUMN) * 2 + 5));
	printf("��NEXT BLOCK��");
	gotoxy(++row, ((SIZE_OVERALL_COLUMN) * 2 + 5));
	for (i = 0; i < SIZE_NEXT_ROW; i++) {
		for (k = 0; k < SIZE_NEXT_COLUMN; k++) {
			gotoxy(i+2, ((SIZE_OVERALL_COLUMN) * 2) + 5 + (k*2));
			switch (nextBlockShape[i][k]) {
			case BLOCK_FENCE:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("��");//��
				break;
			case BLOCK:
				switch (nextBlock.type) {
				case 0:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
					break;
				case 1:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
					break;
				case 2:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
					break;
				case 3:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
					break;
				case 4:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
					break;
				case 5:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
					break;
				default:
					break;
				}
				printf("��");
				break;
			case BLOCK_EMPTY:
				printf("  ");
				break;
			default:
				break;
			}
		}
		gotoxy(++row, ((SIZE_OVERALL_COLUMN) * 2) + 5);
	}
}

void makeNewBlock(int nextChange) {
	static int newBlockNum = 0;
	static int resetBlockNum = 0;
	//BLOCK ����
	srand((unsigned)time(NULL));
	if (newBlockNum == 0) {
		currentBlock.type = rand() % (NUM_BLOCK_TYPE - 1);
		currentBlock.dir = rand() % NUM_BLOCK_DIR;
	}
	else {
		currentBlock = nextBlock;
	}

	nextBlock.type = rand() % NUM_BLOCK_TYPE;
	nextBlock.dir = rand() % NUM_BLOCK_DIR;

	if (nextBlock.type == NUM_BLOCK_TYPE - 1) {
		if (resetBlockNum > 15) {
			resetBlockNum = 0;
		}
		else {
			nextBlock.type = rand() % (NUM_BLOCK_TYPE - 1);
		}
	}

	newBlockNum++;

	//BLOCK ��ġ �Է�
	if (nextChange == TRUE) { //KEY_NEXT�� ���� ���� ��ġ �ȹٲ㵵 ��
		currentSpace.row = 1;
		currentSpace.column = SIZE_OVERALL_COLUMN / 2;
		resetBlockNum++;
	}
	/*
	gotoxy(SIZE_OVERALL_ROW +3,0);
	printf("                             ");
	gotoxy(SIZE_OVERALL_ROW +3,0);
	printf("resetBlockNum = %d\n",resetBlockNum);
	*/
}

void newBlockDisplay() {
	int i, row, column, k;

	//base�� display�� copy
	for (i = 0; i < SIZE_OVERALL_ROW; i++) {
		for (k = 0; k < SIZE_OVERALL_COLUMN; k++) {
			display[i][k] = base[i][k];
		}
	}
	makeShadow();
	// ���� ��ġ�� ���� ������ display�� block ��ġ ��Ű��
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		//i=0
		//i=1
		//i=2
		//i=3
		row = currentSpace.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		//row = 1 + 0
		//row = 1 + 1
		//row = 1 + 1
		//row = 1 + 2
		column = currentSpace.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		//column = 5 + 1
		//column = 5 + 0
		//column = 5 + 1
		//column = 5 + 1
		display[row][column] = BLOCK;
		//[1][6]
		//[2][5]
		//[2][6]
		//[3][6]
	}
	nextBlockSet();
}

int getKey() {
	int key;
	key = _getch();
	if (key == 224) {
		downORpause = DOWN;
		key = _getch();
	}
	else downORpause = PAUSE;

	return key;
}

void gotoxy(int row, int column) {

	static HANDLE h = NULL;
	COORD c = { column, row };

	if (!h) {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	SetConsoleCursorPosition(h, c);
}

int checkTime(unsigned int time) {
	unsigned int newTime;
	static unsigned int oldTime;
	newTime = GetTickCount64();
	if ((newTime - oldTime) > time) {
		oldTime = newTime;
		return TRUE;
	}
	return FALSE;
}

void moveBlock(int key) {
	tempBlock = currentBlock;
	//�ݵ�� �־�� �� : ������? current�� �ٲ� ������ �׿� ���� temp�� �ٲ���� �׶��׶� move�� �������� üũ ����
	tempSpace = currentSpace;
	switch (key) {
	case KEY_UP:
		if (tempBlock.dir != 3) tempBlock.dir += 1;
		else tempBlock.dir = 0;
		break;
	case KEY_DOWN:
		tempSpace.row += 1;
		break;
	case KEY_RIGHT:
		tempSpace.column += 1;
		break;
	case KEY_LEFT:
		tempSpace.column -= 1;
		break;
	default:
		break;
	}
}

int checkMove() {
	// CANNOT MOVE --> base�� previous�� copy, FALSE�� return
	int i, row, column;
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = tempSpace.row + blockPattern[tempBlock.type][tempBlock.dir][i].row;
		column = tempSpace.column + blockPattern[tempBlock.type][tempBlock.dir][i].column;
		if (base[row][column] != BLOCK_EMPTY) {
			return FALSE;
		}
	}
	currentSpace = tempSpace;
	currentBlock = tempBlock;

	return TRUE;
}

void saveBase() {
	int i, row, column;
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = currentSpace.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = currentSpace.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		base[row][column] = BLOCK;
	}

}
// ���� ���� �ִ��� Ȯ��
int checkRemove(int row) {
	int i;
	for (i = 1; i < SIZE_OVERALL_COLUMN - 1; i++) {
		if (base[row][i] == BLOCK_EMPTY) {
			return FALSE;
		}
	}
	return TRUE;
}

int removeALine(int row) {
	int i, k;

	if (checkRemove(row) == TRUE) {
		for (i = row; i > 1; i--) {
			for (k = 1; k < SIZE_OVERALL_COLUMN - 1; k++) {
				base[i][k] = base[i - 1][k];
			}
		}
		return TRUE;
	}
	return FALSE;
}
// 1�϶��� ��� �ؾ��ϴ°�??
void removeLines(int level, int* timeValue) {
	int i;
	int tempScore;

	int levelValue;

	tempScore = currentScore;
	timeValueChange = FALSE;
	for (i = SIZE_OVERALL_ROW - 2; i > 1; i--) {
		if (removeALine(i) == TRUE) {
			//base�� display�� copy (base�� ������ ������ ������)
			currentScore += 100; // �� �ٿ� 100����
			fileopen_wt(level); //currentScore�� bestScore�� ��
			//-> currentScore�� �� Ŀ���� ����bestScore�� currentScore�� bestScore�ν� ����

			switch (level) {
			case 1:
				levelValue = LEVEL_ONE_SCORE;
				break;
			case 2:
				levelValue = LEVEL_TWO_SCORE;
				break;
			case 3:
				levelValue = LEVEL_THREE_SCORE;
				break;
			default:
				break;
			}

			if ((currentScore % levelValue) == 0) {
				timeValueChange = TRUE;
				//baseMakeBlock();
			}
			fasterBlock(level, timeValue);
			newBlockDisplay();
			displayGame(level);
			i++; // ������ ������ ���ο� ���������Ƿ� �ٽ� ������ ���� Ȯ��
			waitLineRemove(500); //0.5�� ����Ʈ

		}
	}
}

void fasterBlock(int level, int* timeValue) {
	int levelScore, levelTime;
	switch (level) {
	case 1:
		levelScore = LEVEL_ONE_SCORE;
		levelTime = LEVEL_ONE_TIME;
		break;
	case 2:
		levelScore = LEVEL_TWO_SCORE;
		levelTime = LEVEL_TWO_TIME;
		break;
	case 3:
		levelScore = LEVEL_THREE_SCORE;
		levelTime = LEVEL_THREE_TIME;
		break;
	default:
		break;
	}
	if (((currentScore % levelScore) == 0) && (timeValueChange == TRUE) && (currentScore != 0) && ((*timeValue) > 100)) {
		(*timeValue) -= levelTime;
		timeValueChange = FALSE;

		gotoxy(SIZE_OVERALL_ROW + 2, 0);
		printf("#�ӵ��� �������ϴ�!#\n");
		printf("%d\n", *(timeValue));
	}
}

int waitLineRemove(int time) {
	while (1) {
		if (checkTime(time) == TRUE) {
			break;
		}
	}
	return 0;
}
//�� �������� �˷������
int finishCheck() {
	int i, row, column;
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = currentSpace.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = currentSpace.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		if (base[row][column] == BLOCK) {
			if (base[1][SIZE_OVERALL_COLUMN / 2] != BLOCK && base[1][SIZE_OVERALL_COLUMN / 2 + 1] != BLOCK) {
				makeFinishBlock();
			}
			return TRUE;
		}
		else {
			//display[row][column] = BLOCK;
		}
	}
	return FALSE;
}

int checkShadowMove() {
	int i, row, column;
	for (i = 0; i < NUM_BLOCK_POINT; i++) { // currentBlock���� Ȯ���ϱ�
		row = tempShadow.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = tempShadow.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		if (base[row][column] != BLOCK_EMPTY) {
			return FALSE;
		}
	}
	currentShadow = tempShadow;
	return TRUE;
}

void makeShadow() {// �� ���� or Ÿ�� �ٲ� ��, ��ġ �ٲ� ��
	int row, column, i;
	currentShadow = currentSpace;
	tempShadow = currentShadow;

	// ��ġ ��� -->  ���� �Ʒ������� �ϵ�, ���� �� ���� ������� Ȯ�� �� ��
	while (1) {
		tempShadow.row++;
		if (checkShadowMove() == FALSE) {
			break;
		}
	}
	// display�� ��ġ�� �׸��� �־��ֱ�
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = currentShadow.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = currentShadow.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		display[row][column] = BLOCK_SHADOW;
	}
	// shadow�� block�� �������� ���� ó��

}

void fileopen_rt(int level) {
	// bestScore ���� Ȥ�� bestScore ���Ͽ��� fscanf�� �о�ͼ� bestScore�� �����ϱ�
	FILE* fp;
	switch (level) {
	case 1:
		fp = fopen(FILE_BESTSCORE1, "rt");
		break;
	case 2:
		fp = fopen(FILE_BESTSCORE2, "rt");
		break;
	case 3:
		fp = fopen(FILE_BESTSCORE3, "rt");
		break;
	default:
		fp = fopen(FILE_BESTSCORE1, "rt");
		break;
	}
	if (fp == NULL) { // ������ ������ = ������ �� ���� ���� ���� : bestScore = 0;
		bestScore = 0;
		//printf("������ ������ �ʽ��ϴ�.\n");
		return;
	}
	else { // ���� ���� = bestScore ���� --> �о�ͼ� �����ϱ�
		fscanf(fp, "%d", &bestScore);
	}
	fclose(fp);
	return;
}

void fileopen_wt(int level) {
	// currentScore�� bestScore ���ؼ� currentScore�� �� ũ�� currentScore�� bestScore �ٲ������
	if (currentScore > bestScore) {
		FILE* fp;
		switch (level) {
		case 1:
			fp = fopen(FILE_BESTSCORE1, "wt");
			break;
		case 2:
			fp = fopen(FILE_BESTSCORE2, "wt");
			break;
		case 3:
			fp = fopen(FILE_BESTSCORE3, "wt");
			break;
		default:
			fp = fopen(FILE_BESTSCORE1, "wt");
			break;
		}
		if (fp == NULL) {
			//printf("������ ������ �ʽ��ϴ�.\n");
			return;
		}
		fprintf(fp, "%d", currentScore); //currentScore�� ���Ͽ� bestScore�� ����("wt"��� �̹Ƿ� �� ����� ���������)
		//printf("��BESTSCORE��");
		fclose(fp);
	}
	return;
}

void calculateTime(int ms) {
	Time passTime;
	passTime.sec = ms / 1000;
	passTime.hour = passTime.sec / 3600;
	if (passTime.hour != 0) passTime.sec = passTime.sec % 3600;
	passTime.min = passTime.sec / 60;
	if (passTime.min != 0) passTime.sec = passTime.sec % 60;
	printf("%d �ð� %d �� %d �ʰ� �ɷȽ��ϴ�.\n", passTime.hour, passTime.min, passTime.sec);
}

int baseMakeCheck(Point tempBaseSpace) {
	int i, row, column;
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = tempBaseSpace.row + blockPattern[baseBlock.type][baseBlock.dir][i].row;
		column = tempBaseSpace.column + blockPattern[baseBlock.type][baseBlock.dir][i].column;
		if (base[row][column] != BLOCK_EMPTY) {
			return FALSE;
		}
	}
	return TRUE;
}

void baseMakeBlock() {
	int i;
	int row, column;
	Point tempBaseSpace;

	srand((unsigned)time(NULL));
	baseBlock.type = (rand() % (NUM_BLOCK_TYPE - 1));
	baseBlock.dir = (rand() % (NUM_BLOCK_DIR));
	tempBaseSpace = baseBlockSpace;
	for (tempBaseSpace.row = SIZE_OVERALL_ROW - 2; tempBaseSpace.row > 0; (tempBaseSpace.row)--) {
		for (tempBaseSpace.column = 1; tempBaseSpace.column < SIZE_OVERALL_COLUMN - 1; (tempBaseSpace.column)++) {
			if (baseMakeCheck(tempBaseSpace) == TRUE) {
				baseBlockSpace = tempBaseSpace;
				break;
			}
		}
		if (baseMakeCheck(tempBaseSpace) == TRUE) break;
	}
	// display�� ��ġ�� �� �־��ֱ�
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = baseBlockSpace.row + blockPattern[baseBlock.type][baseBlock.dir][i].row;
		column = baseBlockSpace.column + blockPattern[baseBlock.type][baseBlock.dir][i].column;
		base[row][column] = BLOCK;
		display[row][column] = BLOCK;
		previous[row][column] = BLOCK;
		gotoxy(row, column * 2);
		printf("��");
	}
}

void initializeFirst() {
	int i, k;

	//================
	for (i = 0; i < SIZE_FIRST_ROW; i++) {
		for (k = 0; k < SIZE_FIRST_COLUMN; k++) {
			firstScreen[i][k] = BLOCK_EMPTY;
			firstScreen[0][k] = BLOCK_HORIZONTAL;
			firstScreen[SIZE_FIRST_ROW - 1][k] = BLOCK_HORIZONTAL;
			firstScreen[i][0] = BLOCK_VERTICAL;
			firstScreen[i][SIZE_FIRST_COLUMN - 1] = BLOCK_VERTICAL;
		}
	}
	//================

	//T
	for (k = 4; k < 9; k++) firstScreen[9][k] = BLOCK;
	for (i = 10; i < 16; i++) firstScreen[i][6] = BLOCK;
	//E
	for (k = 10; k < 14; k++) firstScreen[11][k] = BLOCK;
	for (k = 10; k < 14; k++) firstScreen[14][k] = BLOCK;
	for (k = 10; k < 14; k++) firstScreen[17][k] = BLOCK;
	for (i = 12; i < 17; i++) firstScreen[i][10] = BLOCK;
	//T
	for (k = 15; k < 20; k++) firstScreen[5][k] = BLOCK;
	for (i = 6; i < 12; i++) firstScreen[i][17] = BLOCK;
	//R
	for (k = 20; k < 24; k++) firstScreen[10][k] = BLOCK;
	for (k = 20; k < 24; k++) firstScreen[13][k] = BLOCK;
	for (i = 11; i < 18; i++) firstScreen[i][20] = BLOCK;
	for (i = 11; i < 13; i++) firstScreen[i][23] = BLOCK;
	for (i = 16; i < 18; i++) firstScreen[i][23] = BLOCK;
	firstScreen[14][21] = BLOCK;
	firstScreen[15][22] = BLOCK;
	//I
	for (k = 25; k < 28; k++) firstScreen[13][k] = BLOCK;
	for (k = 25; k < 28; k++) firstScreen[20][k] = BLOCK;
	for (i = 14; i < 20; i++) firstScreen[i][26] = BLOCK;
	//S
	for (k = 29; k < 33; k++) firstScreen[7][k] = BLOCK;
	for (k = 29; k < 33; k++) firstScreen[10][k] = BLOCK;
	for (k = 29; k < 33; k++) firstScreen[13][k] = BLOCK;
	for (i = 8; i < 10; i++) firstScreen[i][29] = BLOCK;
	for (i = 11; i < 13; i++) firstScreen[i][32] = BLOCK;

}

int showScreen(int color) {
	int i, k;
	initializeFirst();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	for (i = 0; i < SIZE_FIRST_ROW; i++) {
		for (k = 0; k < SIZE_FIRST_COLUMN; k++) {
			gotoxy(i, k * 2);
			switch (firstScreen[i][k]) {
			case BLOCK_HORIZONTAL:
				printf("��");
				break;
			case BLOCK_VERTICAL:
				printf("��");
				break;
			case BLOCK_EMPTY:
				printf("  ");
				break;
			default:
				break;
			}
		}
	}
	gotoxy(SIZE_FIRST_ROW + 2, 0);
	printf("���� ������ �ʿ��ϴٸ� A��ư��, ");
	gotoxy(SIZE_FIRST_ROW + 3, 0);
	printf("������ �����Ϸ��� A��ư�� ������ ��ư�� �����ֽʽÿ�.");

	for (i = 1; i < SIZE_FIRST_ROW - 1; i++) {
		for (k = 1; k < SIZE_FIRST_COLUMN - 1; k++) {
			if (firstScreen[i][k] == BLOCK) {
				gotoxy(i, k * 2);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
				printf("��");
				if (_kbhit()) {
					return 1;
				}
				Sleep(50);
			}
		}
	}
	return 0;
}

int chooseLevel() {
	int level;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(10, 5);
	printf("������ �Է��ϼ���.(LEVEL 1, LEVEL 2, LEVEL 3)");
	gotoxy(11, 5);
	printf("�߸� �Է��Ͽ��� ��� 'LEVEL 1'�� �����մϴ�.");
	gotoxy(13, 5);
	printf("�������� ���� ���� �ӵ��� �ٸ��ϴ�.");
	gotoxy(15, 5);
	printf("���� : ");
	scanf("%d", &level);


	if (level == 1 || level == 2 || level == 3)	return level;
	else {
		level = 1;
		return level;
	}
}

void showScreen2() {
	int color;
	srand(time(NULL));
	while (1) {
		color = rand() % 16;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);	if (_kbhit()) break;
		gotoxy(3, 0); printf("     �����������������������������"); Sleep(100); if (_kbhit()) break;
		gotoxy(4, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(5, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(6, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(7, 0); printf("     ��                         �ڡڡڡ�           �ڡڡ�  ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(8, 0); printf("     ��  �ڡڡڡ�      �ڡڡڡ� ��    ��  �ڡڡ�   ��      ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(9, 0); printf("     ��     ��   �ڡڡ�   ��    �ڡڡڡ�    ��     �ڡڡ�  ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(10, 0); printf("     ��     ��   ��       ��    ��          ��         ��  ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(11, 0); printf("     ��     ��   �ڡڡ�   ��    �ڡ�        ��     �ڡڡ�  ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(12, 0); printf("     ��     ��   ��       ��    ��  ��    �ڡڡ�           ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(13, 0); printf("     ��          �ڡڡ�         ��    ��                   ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(14, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(15, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(16, 0); printf("     ��                                                    ��"); Sleep(100); if (_kbhit()) break;
		gotoxy(17, 0); printf("     �����������������������������"); Sleep(100); if (_kbhit()) break;
		gotoxy(20, 20); printf("input any key...");
	}

}

void showHelp() {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(10, 5);
	printf("����Ű UP : ���� ���� �ٲٱ�");
	gotoxy(11, 5);
	printf("������ ����Ű : ���� ��ġ �ٲٱ�");
	gotoxy(13, 5);
	printf("'Q' : ���� �׸��α�");
	gotoxy(14, 5);
	printf("'P' : ���� ��� ���߱�, �ٽ� ������ �����մϴ�.");
	gotoxy(15, 5);
	printf("'R' : ���� �ٽ� �����ϱ�");
	gotoxy(16, 5);
	printf("'N' : ���� ���� ���� ������ �ٲٱ�");
	gotoxy(17, 5);
	printf("'C' : ���� ���� �ٸ� ������ �ٲٱ�");
}

int checkBombBlock() {
	int i, k;
	int count = 0;
	for (i = SIZE_OVERALL_ROW - 1; i > 0; i--) {
		for (k = 1; k < SIZE_OVERALL_COLUMN; k++) {
			if (base[i][k] == BLOCK) {
				count++;
				if (i < SIZE_OVERALL_ROW / 2) {
					return TRUE;
				}
			}
		}
	}
	if (count >= 80) return TRUE;
	return FALSE;
}

void bombBlock() {//����-->��������� �󵵷� �� �� ������ ����? ��� �Լ� ��ġ���� ȣ������?
	int i, k;
	for (i = (currentSpace.row - 2); i < (currentSpace.row + 3); i++) {
		for (k = (currentSpace.column - 2); k < (currentSpace.column + 4); k++) {
			if (base[i][k] == BLOCK) {
				base[i][k] = BLOCK_EMPTY;
			}
		}
	}

}

void makeFinishBlock() {
	int i;
	int row, column;
	currentSpace.row--;
	for (i = 0; i < NUM_BLOCK_POINT; i++) {
		row = currentSpace.row + blockPattern[currentBlock.type][currentBlock.dir][i].row;
		column = currentSpace.column + blockPattern[currentBlock.type][currentBlock.dir][i].column;
		if (base[row][column] == BLOCK_EMPTY) {
			gotoxy(row, column * 2);
			printf("��");
		}
	}
}