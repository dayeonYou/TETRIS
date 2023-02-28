#define _CRT_SECURE_NO_WARNINGS
#include"tetr.h"

extern int downORpause;
extern int currentScore;
extern Block currentBlock;
extern int timeValueChange;
extern int changeBlock;
extern Block nextBlock;


int main() {
	int timeValue = ORIGINAL_TIMEVALUE; // 1��
	int changed = FALSE;
	int key;
	int temptype;
	int level = 1;
	int color;
	unsigned int endTime;
	unsigned int startTime;
	unsigned int startTime2;
	char checkHelp;

	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);

	system("mode con: cols=80 lines=30"); // ���� ��

	srand(time(NULL));
	while (1) {
		while (1) {
			color = rand() % 16;
			if (color != 7) break;
		}
		if (showScreen(color) == 1) {
			checkHelp = _getch();
			system("cls");
			if (checkHelp == 'A') {
				while (_kbhit() == 0) {
					showHelp();
				}
				_getch();
				system("cls");
			}
			level = chooseLevel();
			break;
		}
		Sleep(500);
	}

	//showScreen2();
	system("cls");
	//getch();

	initialization(); //���ο� block �������� �� (currentSpace, currentBlock ����) + nextBlockShape Ʋ ���� (+ ���� �� ����, Ÿ�� ������)
	fileopen_rt(level); //bestScore �о����
	newBlockDisplay();
	displayGame(level);
	startTime = GetTickCount64(); //���� �ð�
	startTime2 = startTime;


	while (1) {
		if (checkTime(timeValue) == TRUE) {
			key = KEY_DOWN;
			changed = TRUE;
		}
		if (_kbhit()) {
			key = getKey();
			if (key == KEY_STOP) {
				gotoxy(SIZE_OVERALL_ROW + 2, 0);
				printf("������ ����Ǿ����ϴ�.\n");
				return 0;
			}
			else if (key == KEY_RESET) {
				system("cls");
				currentScore = 0;
				timeValue = ORIGINAL_TIMEVALUE;
				initialization();
				fileopen_rt(level);
				newBlockDisplay();
				displayGame(level);
				continue;
			}
			else if (key == KEY_PAUSE) {
				if (downORpause == PAUSE) {
					while (1) {
						if (_kbhit()) {
							key = getKey();
							if (key == KEY_PAUSE && downORpause == PAUSE) {
								break;
							}
						}
					}
				}
				else changed = TRUE;
			}
			else if (key == KEY_CHANGE) {
				while (1) {
					temptype = currentBlock.type;
					currentBlock.type = rand() % NUM_BLOCK_TYPE;
					if (temptype != currentBlock.type) {
						break;
					}
				}
				changeBlockFunc();
				newBlockDisplay();
				displayGame(level);
				changed = FALSE;
			}
			else if (key == KEY_NEXT) {
				makeNewBlock(FALSE);
				changeBlockFunc();
				newBlockDisplay();
				displayGame(level);
				changed = FALSE;
			}
			else if (key == KEY_SPACE) {
				while (1) {
					key = KEY_DOWN;
					moveBlock(key); //tempSpace.row�� +1��
					if (checkMove() == TRUE) {
						newBlockDisplay(); // checkMove()�� ����  currentSpace.row �ٲ� (+1)
						// --> �ٲ� currentSpace�� newBlockDisplay()�� ���� display�� �ִ� ������ ��ġ �ٲ㰨
					}
					else { //tempSpace�� moveBlock�� ���� �ٲ������
						// currentSpace�� checkMove()�� ��� TRUE�� �ƴϹǷ� �ٲ��� ����.
						//newBlockDisplay(); ������ ���̴�? currentSpace�� �ٲ��� �ʾ����Ƿ� newBlockDisplay()�ص� display�ٲ��� ����
						//saveBase(); //1�ʰ� ������ changed�� TRUE�� �ٲ�� key�� down�̰� checkMove()�� TRUE�� �ƴϹǷ� saveBase�ϰ� ��
						displayGame(level);
						changed = TRUE;
						break;
					}
				}
			}
			else changed = TRUE;
		}
		if (changed == TRUE) {
			moveBlock(key);
			if (checkMove() == TRUE) { //temp�� current�� copy (������ �� �����Ƿ�)
				newBlockDisplay(); //display�� �ٲ� or ���ο� �� �߰�
				displayGame(level);
				changed = FALSE;
			}
			else { //current �ٲ��� ���� (������ �� �����Ƿ�)
				if (key == KEY_DOWN) {
					if (currentBlock.type == NUM_BLOCK_TYPE - 1) {
						system("cls");
						initialization();
					}
					else {
						// base�� ���� �� �����Ű��(�ٲ��� ���� currentSpace�� currentBlock�� Ȱ��)
						saveBase();

						// ���� ���� �����
						removeLines(level, &(timeValue));

						switch (level) {
						case 1:
							if ((GetTickCount64() - startTime2) > 30000) { // 30�� �Ѿ��� ������ �ϳ���
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						case 2:
							if ((GetTickCount64() - startTime2) > 20000) { // 20�� �Ѿ��� ������ �ϳ���
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						case 3:
							if ((GetTickCount64() - startTime2) > 10000) { // 10�� �Ѿ��� ������ �ϳ���
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						default:
							break;
						}

						// ���ο� �� ����
						makeNewBlock(TRUE);
					}

					// ������ ���ο� ���� �������� �������� Ȯ��
					if (finishCheck() == TRUE) {
						displayGame(level);
						fileopen_rt(level);
						endTime = GetTickCount64();
						gotoxy(SIZE_OVERALL_ROW + 2, 0);
						calculateTime((endTime - startTime));
						break;
					}
					newBlockDisplay();
					displayGame(level);
					changed = FALSE;

				}
			}
		}
	}
	return 0;
}