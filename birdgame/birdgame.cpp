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
#define GRAVITY 1

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipePos[2]; //
int gapPos[2]; //gap[k] == m : the height of top part of pipe with index k is m
int pipeFlag[2]; //pipe[k] == 1  : pipe with index k is currently present on screen
char bird[BIRD_HEIGHT][BIRD_WIDTH] = { '/','-','-','O','\\',' ',
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
	gotoxy(WIN_WIDTH + 8, 14);cout << "Press 1";
	while (1) {
		if (_kbhit()) {
			char k = _getch();
			if (k == '1')break;
		}
	}

}
void updateScore() {
	highscore = max(highscore, score);
	gotoxy(WIN_WIDTH + 7, 5);cout << "Score: " << score << endl;
	gotoxy(WIN_WIDTH + 8, 10);cout << "Best: " << highscore;
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

void ClearTerminalText() {
	gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);cout << "   " << endl;
	gotoxy(10, 3);cout << "                                               " << endl;
	gotoxy(10, 4);cout << "                                               " << endl;;
	gotoxy(10, 5);cout << "                                               " << endl;
	gotoxy(10, 6);cout << "                                               " << endl;
	gotoxy(10, 7);cout << "                                               " << endl;

	gotoxy(10, 9);cout << "                             " << endl;
	gotoxy(10, 10);cout << "                             " << endl;;
	gotoxy(10, 11);cout << "                             " << endl;
	gotoxy(10, 12);cout << "                             " << endl;

	gotoxy(10, 14);cout << "                    " << endl;
	gotoxy(10, 15);cout << "                    "  << endl;;
	gotoxy(10, 16);cout << "                    "  << endl;
	gotoxy(10, 17);cout << "                    "  << endl;

	gotoxy(10, 19);cout << "                           " << endl;
	gotoxy(10, 20);cout << "                           " << endl;;
	gotoxy(10, 21);cout << "                           " << endl;
	gotoxy(10, 22);cout << "                           " << endl;
}
void InitialiseTerminal() {
	system("cls");
	score = 0;

	drawBorder();
	birdPos = 6;
	drawBird();

	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;
	genPipe(0);
	updateScore();
	drawPipe(0);

	gotoxy(WIN_WIDTH + 6, 4);cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 6);cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 9);cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 11);cout << "-----------";

	//font name : small
	gotoxy(10,3);cout <<  " ___ _                       ___ _        _    " << endl;
	gotoxy(10, 4);cout << "| __| |__ _ _ __ _ __ _  _  | _ |_)_ _ __| |   " << endl;;
	gotoxy(10, 5);cout << "| _|| / _` | '_ \\ '_ \\ || | | _ \\ | '_/ _` |" << endl;
	gotoxy(10, 6);cout << "|_| |_\\__,_| .__/ .__/\\_, | |___/_|_| \\__,_|" << endl;
	gotoxy(10, 7);cout << "           |_|  |_|   |__/                  " << endl;
	
	gotoxy(10, 9);cout <<  "  _     ___ _            _   " << endl;
	gotoxy(10, 10);cout << " / |   / __| |_ __ _ _ _| |_ " << endl;;
	gotoxy(10,11);cout <<  " | |_  \\__ \\  _/ _` | '_|  _|" << endl;
	gotoxy(10, 12);cout << " |_(_) |___/\\__\\__,_|_|  \\__|" << endl;

	gotoxy(10, 14);cout << "  ___       _   ___ " << endl;
	gotoxy(10, 15);cout << " |_  )     /_\\ |_ _|" << endl;;
	gotoxy(10, 16);cout << "  / / _   / _ \\ | | " << endl;
	gotoxy(10, 17);cout << " /___(_) /_/ \\_\\___|" << endl;

	gotoxy(10, 19);cout << "  ____     ___       _ _   " << endl;
	gotoxy(10, 20);cout << " |__ /    / _ \\ _  _(_) |_ " << endl;;
	gotoxy(10, 21);cout << "  |_ \\_  | (_) | || | |  _|" << endl;
	gotoxy(10, 22);cout << " |___(_)  \\__\\\_\\_,_|_|\\__|" << endl;

}

void play(const bool AIMode) {
	ClearTerminalText();
	eraseBird();
	while (1) {
		birdPos += GRAVITY; //DO NOT CHANGE position in loop. AI relies on this.

		if (AIMode) {
			char ch = AI();
			if (ch == 32)birdPos -= BIRD_JUMP_HEIGHT; 
		}
		else {
			if (_kbhit()) {
				char ch = _getch();
				if (ch == 32) { //space bar
					birdPos -= BIRD_JUMP_HEIGHT; //bird jumps 3 units up
				}
				if (ch == 27) { //escape key to exit game
					break;
				}
			}
		}

		drawBird();
		drawPipe(0);
		drawPipe(1);

		if (collision() == 1) {
			gameover();
			return;
		}

		if (AIMode)Sleep(10);
		else Sleep(75);

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
int main(){
	hidecursor(); //add to loop in play() if screen is resized.
	srand((unsigned)time(NULL));
	InitialiseTerminal();

	//TESTING: REMOVE
	//drawBorder();
	//drawBird();
	//genPipe(0);
	//pipeFlag[0] = 1;
	//pipeFlag[1] = 0;
	//pipePos[0] = pipePos[1] = 4;
	//drawPipe(0);
	//erasePipe(0);
	//gotoxy( 0, SCREEN_HEIGHT + 1);
	//system("pause");

	do {
		InitialiseTerminal();
		gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);
			char op = _getche();
			if (op == '1') play(0);
			else if (op == '2') play(1);
			else if (op == '3') break;

	} while (1);

	return 0;
}
