#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h>
#include <cassert>

#define SCREEN_WIDTH 90 //full window width. Index : [0, SCREEN_WIDTH-1]
#define SCREEN_HEIGHT 26 // full window height. Index : [0, SCREEN_HEIGHT-1]
#define WIN_WIDTH 70  // size of screen where game is played. contains bird and pipes
#define MENU_WIDTH 20 // right window width MENU_WIDTH = SCREEN_WIDTH - WIN_WIDTH
#define GAP_SIZE 7 // gap size in each pipe. must be greater than bird height.
#define PIPE_DIF 45 //distance between pipes
#define BIRD_WIDTH 6 //bird occupies 6 cells horizontally
#define BIRD_HEIGHT 2 //bird occupies 2 cells vertically
#define BIRD_JUMP_HEIGHT 3 //bird can jump 3 units at once

#define GAP_WALL_BIRD 2 //gap size horizontally between left wall and left part of bird. Keep at 2.
#define PIPE_SPEED 2 // pipe moves 2 units towards bird every frame
#define PIPE_WIDTH 3 //pipe is 3 units wide

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipePos[2]; //
int gapPos[2]; //gap[k] == m : the height of top part of pipe with index k is m
int pipeFlag[2]; //pipe[k] == 1  : pipe with index k is currently present on screen
char bird[BIRD_HEIGHT][BIRD_WIDTH] = { '/','-','-','o','\\',' ',
					'|','_','_','_',' ','>' };
int birdPos = 6; //Coordinates (row,col) of top left corner of bird = (birdPos, GAP_WALL_BIRD). Bird is always in column 1.
int score = 0;
int highscore = -1;

void gotoxy(int col, int row){
	CursorPosition.X = col;
	CursorPosition.Y = row;
	SetConsoleCursorPosition(console, CursorPosition);
}
void drawBorder() {
	
	for (int i = 0; i < SCREEN_WIDTH; i++) { 
		gotoxy(i, 0); cout << "#"; //draw top border
		gotoxy(i, SCREEN_HEIGHT-1); cout << "#"; //draw bottom border
	}

	for (int i = 1; i < SCREEN_HEIGHT-1; i++) {
		gotoxy(0, i); cout << "|";  //draw left border
		gotoxy(SCREEN_WIDTH-1, i); cout << "|";  //draw right border
	}
	for (int i = 1; i < SCREEN_HEIGHT-1; i++) { 
		gotoxy(WIN_WIDTH, i); cout << "|"; //draw separating line for right window
	}

}

void genPipe(int ind) {
	gapPos[ind] = 3 + rand() % 14;
}
void drawPipe(int ind) {
	if (pipeFlag[ind] == true) {
		for (int i = 0; i < gapPos[ind]; i++) { //draw top part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "###";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) { //draw bottom part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "###";
		}
	}
}
void erasePipe(int ind) {
 	if (pipeFlag[ind] == true) {
		for (int i = 1; i < gapPos[ind]+1; i++) { //erase top part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i); cout << "   ";
		}
		for (int i = 1+gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) { //erase bottom part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i); cout << "   ";
		}
	}
}

void drawBird() {
	for (int i = 0; i < BIRD_HEIGHT; i++) {
		for (int j = 0; j < BIRD_WIDTH; j++) {
			int row = i + birdPos;
			int col = j + GAP_WALL_BIRD;
			//draw only part of bird which is valid. This is important when collision occurs.
			if (col >= 0 && row >= 0 && col < SCREEN_WIDTH && row < SCREEN_HEIGHT) { 
				gotoxy(col, row); cout << bird[i][j];
			}
		}
	}
}
void eraseBird() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + birdPos); cout << " ";
		}
	}
}

bool collision() { 
	//if top part of bird touches top part of screen
	if (birdPos <= 0) { 
		return 1;
	}
	//if bottom part of bird touches bottom of screen
	if (birdPos + BIRD_HEIGHT - 1 >= SCREEN_HEIGHT - 1) { 
		return 1;
	}
	//if bird hits top part of pipe 0 before entering pipe
	//if you want to see bird enter pipe when collision occurs replace GAP_WALL_BIRD + BIRD_WIDTH with GAP_WALL_BIRD + BIRD_WIDTH - 1
	if (birdPos + BIRD_HEIGHT - 1 <= gapPos[0] && GAP_WALL_BIRD + BIRD_WIDTH >= WIN_WIDTH-pipePos[0]) {
 		return 1;
	}
	//if bird hits bottom part of pipe 0 before entering pipe
	if (birdPos + BIRD_HEIGHT - 1 >= gapPos[0]+GAP_SIZE && GAP_WALL_BIRD + BIRD_WIDTH >= WIN_WIDTH - pipePos[0]) {
 		return 1;
	}
	//bird in pipe gap. Must take PIPE_WIDTH into account
	if (pipePos[0] >= WIN_WIDTH-BIRD_WIDTH) {
		if (birdPos <= gapPos[0] || birdPos + BIRD_HEIGHT - 1 >= gapPos[0] + GAP_SIZE) {
			return 1;
		}
	}
	return 0;
}
void gameover() {
	highscore = max(highscore, score);
	//Erase right window
	gotoxy(WIN_WIDTH + 5, 2);cout << "           ";

	gotoxy(WIN_WIDTH + 6, 2);cout << "GAME OVER!";
	gotoxy(WIN_WIDTH + 6, 8);cout << "HighScore: " << highscore;
	gotoxy(WIN_WIDTH + 8, 10);cout << "Press 1";
	while (1) {
		if (_kbhit()) {
			char k = _getch();
			if (k == '1')break;
		}
	}

}
void updateScore() {
	gotoxy(WIN_WIDTH + 7, 5);cout << "Score: " << score << endl;
}

void instructions() {

	system("cls");
	cout << "Instructions";
	cout << "\n----------------";
	cout << "\n Press spacebar to make bird fly";
	cout << "\n\nPress any key to go back to menu";
	_getch();
}

char AI() {
	//if bird above top part, do nothing
	if (birdPos + BIRD_HEIGHT - 1 <= gapPos[0] ) {
		return 1;
	}
	//if bird below top part of pipe 0
	if (birdPos + BIRD_HEIGHT - 1 >= gapPos[0] + GAP_SIZE) {
		if (birdPos - BIRD_JUMP_HEIGHT > gapPos[0]) { 
			return 32;
		}
	}
	return 0;
}
void play() {

	birdPos = 6;
	score = 0;
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;

	//system("cls");
	drawBorder();
	genPipe(0);
	updateScore();

	gotoxy(WIN_WIDTH + 5, 2);cout << "FLAPPY BIRD";
	gotoxy(WIN_WIDTH + 6, 4);cout << "----------";
	gotoxy(WIN_WIDTH + 6, 6);cout << "----------";


	//gotoxy(10, 5);cout << "Press any key to start";
	//_getch();
	//gotoxy(10, 5);cout << "                      ";

	while (1) {
		birdPos += 1; //gravity effect : bird falls 1 unit down every frame

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 32) { //space bar
				birdPos -= BIRD_JUMP_HEIGHT; //bird jumps 3 units up
			}
			if (ch == 27) { //escape key to exit game
				break;
			}
		}
		//char ch = AI();
		//if (ch == 32)birdPos -= BIRD_JUMP_HEIGHT; 

		drawBird();
		drawPipe(0);
		drawPipe(1);

		if (collision() == 1) {
			gameover();
			return;
		}

		//Sleep(100);
		Sleep(50);
		eraseBird();
		erasePipe(0);
		erasePipe(1);

		if (pipeFlag[0] == 1)
			pipePos[0] += PIPE_SPEED;

		if (pipeFlag[1] == 1)
			pipePos[1] += PIPE_SPEED;

		if (pipePos[0] >= 40 && pipePos[0] < 42) {
			pipeFlag[1] = 1;
			pipePos[1] = 4;
			genPipe(1);
		}
		if (pipePos[0] > 68) {
			score++;
			updateScore();
			pipeFlag[1] = 0;
			pipePos[0] = pipePos[1];
			gapPos[0] = gapPos[1];
		}

	}

}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
int main()
{
	hidecursor(); //add to loop in play() if screen is resized.
	srand((unsigned)time(NULL));
	//TESTING: REMOVE
	drawBorder();
	drawBird();
	genPipe(0);
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;
	drawPipe(0);
	erasePipe(0);
	gotoxy( 0, SCREEN_HEIGHT + 1);
	system("pause");

	do {
		system("cls");
		//gotoxy(10, 5); cout << " -------------------------- ";
		//gotoxy(10, 6); cout << " |      Flappy Bird       | ";
		//gotoxy(10, 7); cout << " --------------------------";
		//gotoxy(10, 9); cout << "1. Start Game";
		//gotoxy(10, 10); cout << "2. AI Mode";
		//gotoxy(10, 11); cout << "3. Instructions";
		//gotoxy(10, 12); cout << "4. Quit";
		//gotoxy(10, 14); cout << "Select option: ";
		//char op = _getche();
		char op = '1';

		if (op == '1') play();
		else if (op == '2') instructions();
		else if (op == '3') break;

	} while (1);

	return 0;
}
