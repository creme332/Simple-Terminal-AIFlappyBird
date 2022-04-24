#include<iostream>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90 //full window width
#define SCREEN_HEIGHT 26 // full window height
#define WIN_WIDTH 70  // size of playing screen
#define MENU_WIDTH 20 // right window width MENU_WIDTH = SCREEN_WIDTH - WIN_WIDTH
#define GAP_SIZE 7 // gap size in 1 pipe
#define PIPE_DIF 45 //distance between pipes
#define BIRD_WIDTH 6 //bird occupies 6 cells horizontally
#define PIPE_WIDTH 3 //pipe is 3 units wide

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int pipePos[2]; //
int gapPos[2];
int pipeFlag[2];
char bird[2][6] = { '/','-','-','o','\\',' ',
					'|','_','_','_',' ','>' };
int birdPos = 6; //Coordinates of top left corner of bird = (0, birdPos)
int score = 0;
int highscore = -1;

void gotoxy(int col, int row){
	CursorPosition.X = col;
	CursorPosition.Y = row;
	SetConsoleCursorPosition(console, CursorPosition);
}
void drawBorder() {
	
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		gotoxy(i, 0); cout << "#";
		gotoxy(i, SCREEN_HEIGHT); cout << "#";
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(0, i); cout << "|";
		gotoxy(SCREEN_WIDTH, i); cout << "|";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		gotoxy(WIN_WIDTH, i); cout << "|";
	}
	gotoxy(0, SCREEN_HEIGHT); cout << "|";
	gotoxy(SCREEN_WIDTH, SCREEN_HEIGHT); cout << "|";
	gotoxy(WIN_WIDTH, SCREEN_HEIGHT); cout << "|";



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
		for (int i = 0; i < gapPos[ind]; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "   ";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++) {
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1); cout << "   ";
		}
	}
}

void drawBird() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + birdPos); cout << bird[i][j];
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
	if (birdPos <= 1) { //if bird touches top part of screen
		return 1;
	}
	if (birdPos > SCREEN_HEIGHT - 2) { //if bird touches bottom of screen
		return 1;
	}
	if (pipePos[0] >= WIN_WIDTH-BIRD_WIDTH) { //bird in pipe gap
		if (birdPos<gapPos[0] || birdPos >gapPos[0] + GAP_SIZE) {
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

void play() {

	birdPos = 6;
	score = 0;
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;

	system("cls");
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

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 32) { //space bar
				birdPos -= 3; //bird jumps 3 units up
			}
			if (ch == 27) { //escape key to exit game
				break;
			}
		}

		drawPipe(0);
		drawPipe(1);
		drawBird();

		if (collision() == 1) {
			gameover();
			return;
		}

		Sleep(100);
		eraseBird();
		erasePipe(0);
		erasePipe(1);
		birdPos += 1; //gravity effect : bird falls 1 unit down 

		if (pipeFlag[0] == 1)
			pipePos[0] += 2;

		if (pipeFlag[1] == 1)
			pipePos[1] += 2;

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
	//drawBird();

	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;
	drawPipe(0);
	gotoxy( 0, SCREEN_HEIGHT + 1);
	system("pause");

	do {
		system("cls");
		//gotoxy(10, 5); cout << " -------------------------- ";
		//gotoxy(10, 6); cout << " |      Flappy Bird       | ";
		//gotoxy(10, 7); cout << " --------------------------";
		//gotoxy(10, 9); cout << "1. Start Game";
		//gotoxy(10, 10); cout << "2. Instructions";
		//gotoxy(10, 11); cout << "3. Quit";
		//gotoxy(10, 13); cout << "Select option: ";
		//char op = _getche();
		char op = '1';

		if (op == '1') play();
		else if (op == '2') instructions();
		else if (op == '3') break;

	} while (1);

	return 0;
}
