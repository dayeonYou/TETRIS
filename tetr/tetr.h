#ifndef _MY_TETRIS_
#define _MY_TETRIS_

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>

#define		TRUE					1
#define		FALSE					0

#define		SIZE_OVERALL_ROW		25
#define		SIZE_OVERALL_COLUMN		20
#define		SIZE_NEXT_ROW			8
#define		SIZE_NEXT_COLUMN		8

#define		SIZE_FIRST_ROW			25
#define		SIZE_FIRST_COLUMN		36

#define		ORIGINAL_TIMEVALUE		1000 //1초
#define		LEVEL_ONE_SCORE			500 // 500점 추가 될 때마다
#define		LEVEL_ONE_TIME			50 // 0.05초씩 감소
#define		LEVEL_TWO_SCORE			300 // 500점 추가 될 때마다
#define		LEVEL_TWO_TIME			50 // 0.05초씩 감소
#define		LEVEL_THREE_SCORE		200 // 500점 추가 될 때마다
#define		LEVEL_THREE_TIME		50 // 0.05초씩 감소


#define		KEY_UP					72
#define		KEY_DOWN				80
#define		KEY_RIGHT				77
#define		KEY_LEFT				75
#define		KEY_NOT					0
#define		KEY_SPACE				32
#define		KEY_PAUSE				'P' // 일시 중지, 다시 P를 누르면 시작
#define		KEY_RESET				'R' // 처음부터 다시
#define		KEY_STOP				'Q' //종료
#define		KEY_CHANGE				'C' //다른 블럭으로 바꾸기
#define		KEY_NEXT				'N' //NEXT BLOCK으로 바꾸기

#define		NUM_BLOCK_TYPE			6
#define		NUM_BLOCK_DIR			4
#define		NUM_BLOCK_POINT			4

#define		BLOCK					2
#define		BLOCK_EMPTY				0
#define		BLOCK_WALL				1
#define		BLOCK_SHADOW			3
#define		BLOCK_FENCE				4
#define		BLOCK_STAR				5
#define		BLOCK_VERTICAL			7
#define		BLOCK_HORIZONTAL		6

#define		DOWN					0
#define		PAUSE					1

#define		FILE_BESTSCORE1			"bestscore1.txt"
#define		FILE_BESTSCORE2			"bestscore2.txt"
#define		FILE_BESTSCORE3			"bestscore3.txt"

typedef struct _Point {
	int row;
	int column;
} Point;

typedef struct _Block {
	int type;
	int dir;
} Block;

typedef struct _Time {
	int hour;
	int min;
	int sec;
} Time;

void initialization();
void displayGame(int level);
void makeNewBlock(int nextChange);
void newBlockDisplay();
void gotoxy(int row, int column);
int checkTime(unsigned int time);
void moveBlock(int key);
int checkMove();
int getKey();
void saveBase();
void removeLines(int level, int* timeValue);
int finishCheck();
void makeShadow();
void changeBlockFunc();
int waitLineRemove(int time);
void fasterBlock(int level, int* timeValue);
void nextBlockDisplay();
void fileopen_rt(int level);
void fileopen_wt(int level);
void calculateTime(int ms);
void bombBlock();
void baseMakeBlock();
void makeFinishBlock();
int showScreen(int color);
int chooseLevel();
void initializeFirst();
void showScreen2();
void showHelp();

#endif