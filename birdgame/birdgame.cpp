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
#define PIPE_SPEED 2 // pipe moves 2 units towards bird every frame
#define PIPE_WIDTH 3 //pipe is 3 units wide

#define GAP_WALL_BIRD 2 //gap size horizontally between left wall and left part of bird. Keep at 2.
#define BIRD_WIDTH 6 //bird occupies 6 cells horizontally
#define BIRD_HEIGHT 2 //bird occupies 2 cells vertically
#define BIRD_JUMP_HEIGHT 3 //bird can jump 3 units at once
#define GRAVITY 1 //bird falls 1 unit every frame

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipePos[2]; //distance of left end of pipe from right window
int gapPos[2]; //gap[k] == m : the height of top part of pipe with index k is m
bool pipeFlag[2]; //pipe[k] == 1  : pipe with index k is currently present on screen
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
		gotoxy(i, 0); std::cout << "="; //draw top border
		gotoxy(i, SCREEN_HEIGHT-1); std::cout << "="; //draw bottom border
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i); std::cout << "|";  //draw left border
		gotoxy(SCREEN_WIDTH-1, i); std::cout << "|";  //draw right border
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) { 
		gotoxy(WIN_WIDTH, i); std::cout << "|"; //draw separating line for right window
	}

}

void genPipe(int ind) {
	gapPos[ind] = 3 + rand() % 14;
}
void drawPipe(int ind) {
	if (pipeFlag[ind] == true) {
		for (int i = 0; i < gapPos[ind]; i++) { //draw top part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); std::cout << "###";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 2; i++) { //draw bottom part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); std::cout << "###";
		}
	}
}
void erasePipe(int ind) {
 	if (pipeFlag[ind] == true) {
		for (int i = 1; i < gapPos[ind]+1; i++) { //erase top part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i); std::cout << "   ";
		}
		for (int i = 1+gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) { //erase bottom part of pipe
			gotoxy(WIN_WIDTH - pipePos[ind], i); std::cout << "   ";
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
				gotoxy(col, row); std::cout << bird[i][j];
			}
		}
	}
}
void eraseBird() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + birdPos); std::cout << " ";
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
	gotoxy(WIN_WIDTH + 8, 14);std::cout << "Press 1";
	while (1) {
		if (_kbhit()) {
			char k = _getch();
			if (k == '1')break;
		}
	}

}
void updateScore() {
	highscore = max(highscore, score);
	gotoxy(WIN_WIDTH + 7, 5);std::cout << "Score: " << score << std::endl;
	gotoxy(WIN_WIDTH + 8, 10);std::cout << "Best: " << highscore;
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
	gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT);std::cout << "   " << std::endl;
	gotoxy(10, 3);std::cout << "                                               " << std::endl;
	gotoxy(10, 4);std::cout << "                                               " << std::endl;
	gotoxy(10, 5);std::cout << "                                               " << std::endl;
	gotoxy(10, 6);std::cout << "                                               " << std::endl;
	gotoxy(10, 7);std::cout << "                                               " << std::endl;

	gotoxy(10, 9);std::cout << "                             " << std::endl;
	gotoxy(10, 10);std::cout << "                             " << std::endl;
	gotoxy(10, 11);std::cout << "                             " << std::endl;
	gotoxy(10, 12);std::cout << "                             " << std::endl;

	gotoxy(10, 14);std::cout << "                    " << std::endl;
	gotoxy(10, 15);std::cout << "                    "  << std::endl;
	gotoxy(10, 16);std::cout << "                    "  << std::endl;
	gotoxy(10, 17);std::cout << "                    "  << std::endl;

	gotoxy(10, 19);std::cout << "                           " << std::endl;
	gotoxy(10, 20);std::cout << "                           " << std::endl;
	gotoxy(10, 21);std::cout << "                           " << std::endl;
	gotoxy(10, 22);std::cout << "                           " << std::endl;
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

	gotoxy(WIN_WIDTH + 6, 4);std::cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 6);std::cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 9);std::cout << "-----------";
	gotoxy(WIN_WIDTH + 6, 11);std::cout << "-----------";

	//font name : small
	gotoxy(10,3);std::cout <<  " ___ _                       ___ _        _    " << std::endl;
	gotoxy(10, 4);std::cout << "| __| |__ _ _ __ _ __ _  _  | _ |_)_ _ __| |   " << std::endl;
	gotoxy(10, 5);std::cout << "| _|| / _` | '_ \\ '_ \\ || | | _ \\ | '_/ _` |" << std::endl;
	gotoxy(10, 6);std::cout << "|_| |_\\__,_| .__/ .__/\\_, | |___/_|_| \\__,_|" << std::endl;
	gotoxy(10, 7);std::cout << "           |_|  |_|   |__/                  " << std::endl;
	
	gotoxy(10, 9);std::cout <<  "  _     ___ _            _   " << std::endl;
	gotoxy(10, 10);std::cout << " / |   / __| |_ __ _ _ _| |_ " << std::endl;
	gotoxy(10,11);std::cout <<  " | |_  \\__ \\  _/ _` | '_|  _|" << std::endl;
	gotoxy(10, 12);std::cout << " |_(_) |___/\\__\\__,_|_|  \\__|" << std::endl;

	gotoxy(10, 14);std::cout << "  ___       _   ___ " << std::endl;
	gotoxy(10, 15);std::cout << " |_  )     /_\\ |_ _|" << std::endl;
	gotoxy(10, 16);std::cout << "  / / _   / _ \\ | | " << std::endl;
	gotoxy(10, 17);std::cout << " /___(_) /_/ \\_\\___|" << std::endl;

	gotoxy(10, 19);std::cout << "  ____     ___       _ _   " << std::endl;
	gotoxy(10, 20);std::cout << " |__ /    / _ \\ _  _(_) |_ " << std::endl;
	gotoxy(10, 21);std::cout << "  |_ \\_  | (_) | || | |  _|" << std::endl;
	gotoxy(10, 22);std::cout << " |___(_)  \\__\\\_\\_,_|_|\\__|" << std::endl;

}

void play(const bool AIMode) {
	ClearTerminalText();
	eraseBird();
	while (1) {
		birdPos += GRAVITY; //DO NOT CHANGE position of this line in loop. AI relies on this.

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

		if (AIMode)Sleep(50);
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
