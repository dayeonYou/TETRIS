#define _CRT_SECURE_NO_WARNINGS
#include"tetr.h"

extern int downORpause;
extern int currentScore;
extern Block currentBlock;
extern int timeValueChange;
extern int changeBlock;
extern Block nextBlock;


int main() {
	int timeValue = ORIGINAL_TIMEVALUE; // 1초
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

	system("mode con: cols=80 lines=30"); // 열과 행

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

	initialization(); //새로운 block 생성까지 함 (currentSpace, currentBlock 있음) + nextBlockShape 틀 만듦 (+ 다음 블럭 방향, 타입 생성함)
	fileopen_rt(level); //bestScore 읽어오기
	newBlockDisplay();
	displayGame(level);
	startTime = GetTickCount64(); //시작 시각
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
				printf("게임이 종료되었습니다.\n");
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
					moveBlock(key); //tempSpace.row는 +1됨
					if (checkMove() == TRUE) {
						newBlockDisplay(); // checkMove()로 인해  currentSpace.row 바뀜 (+1)
						// --> 바뀐 currentSpace로 newBlockDisplay()에 의해 display에 있는 블럭들이 위치 바꿔감
					}
					else { //tempSpace는 moveBlock에 의해 바뀌었으나
						// currentSpace는 checkMove()의 결과 TRUE가 아니므로 바뀌지 않음.
						//newBlockDisplay(); 유무의 차이는? currentSpace가 바뀌지 않았으므로 newBlockDisplay()해도 display바뀌지 않음
						//saveBase(); //1초가 지나면 changed가 TRUE로 바뀌고 key는 down이고 checkMove()는 TRUE가 아니므로 saveBase하게 됨
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
			if (checkMove() == TRUE) { //temp를 current에 copy (움직일 수 있으므로)
				newBlockDisplay(); //display에 바뀐 or 새로운 블럭 추가
				displayGame(level);
				changed = FALSE;
			}
			else { //current 바꾸지 않음 (움직일 수 없으므로)
				if (key == KEY_DOWN) {
					if (currentBlock.type == NUM_BLOCK_TYPE - 1) {
						system("cls");
						initialization();
					}
					else {
						// base에 쌓인 블럭 저장시키기(바뀌지 않은 currentSpace와 currentBlock을 활용)
						saveBase();

						// 지울 라인 지우기
						removeLines(level, &(timeValue));

						switch (level) {
						case 1:
							if ((GetTickCount64() - startTime2) > 30000) { // 30초 넘었을 때마다 하나씩
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						case 2:
							if ((GetTickCount64() - startTime2) > 20000) { // 20초 넘었을 때마다 하나씩
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						case 3:
							if ((GetTickCount64() - startTime2) > 10000) { // 10초 넘었을 때마다 하나씩
								baseMakeBlock();
								startTime2 = GetTickCount64();
							}
							break;
						default:
							break;
						}

						// 새로운 블럭 생성
						makeNewBlock(TRUE);
					}

					// 생성한 새로운 블럭을 기준으로 끝났는지 확인
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