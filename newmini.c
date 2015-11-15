/*****************************************************************************
 * Copyright (C) Tejaswa Gavankar.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<unistd.h>
#include"new.h"

/*Yet to do:
->scoring mechanism and storage
->randomness of make2 and anyblank?
->UI input screen of mode2
->WASD i/p functionality for mode2
*/

#define DELAY 100000

int turn = 1;
int board[9]={2, 2, 2, 2, 2, 2, 2, 2, 2};
int move1;
int mode;
char name[2][20];
int main(){
	/*start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	*/initscr();
	/*curs_set(FALSE);
	bkgd(COLOR_PAIR(1));*/

	mvprintw(1, 52, "Welcome to Tic-Tac-Toe\n");
/*	printw("1 | 2 | 3\n");
	printw("---------\n");
	printw("4 | 5 | 6\n");
	printw("---------\n");
	printw("7 | 8 | 9\n");
	printw("\n");*/	
	
	char yes; 

	mvprintw(5, 41, "Single Player(Press S) or MultiPlayer(Press M)?");
	refresh();
	scanw("%c", &yes);
	noecho();
	if((yes == 'M') || (yes == 'm')){
		mode = 2;
		system("clear");		
		player2();
	}
	else if((yes == 'S') || (yes == 's')){ //Single player move flow
		mode = 1;
		mvprintw(7, 43, "Do you want to make the first move? (Y/N)");
		mvprintw(8, 34, "(Press 'h' during your turn if you need help in making a move)");
		refresh();
		scanw("%c", &yes);
		noecho();
		if((yes == 'Y') || (yes == 'y')){
			playermove();
			move1 = 1;
		}
		else if((yes == 'N') || (yes == 'n')){
			pcmove();
			move1 = 0;
		}
		else{ 
			printw("Invalid Input");
			refresh();
			getch();
			endwin();
			exit(1);
		}		
	
		for( ; turn <= 9; ){
			if(move1 == 1){
				if(turn % 2 == 1)
					playermove();
				else pcmove();
			}
			else if(move1 == 0){
			
				if(turn % 2 == 1)
					pcmove();
				else playermove();
			}
		}
	}
	else{ 
		printw("Invalid Input\n");
		refresh();
		getch();
		endwin();
	//	system("clear");	
	//	printw("Type again");		
	//	main();			
		exit(1);
	}	
}

void go(int spot){
	spot = spot - 1;
	if ((spot > -1) && (spot < 9)){ 
		if(board[spot] == 2){
			if(turn % 2 == 1)
				board[spot] = 3;
			else
				board[spot] = 5;	
		if(mode == 1)
			showboard();
		else
			show2xboard(spot);
	}
	else
		if(((turn % 2 == 1) && (move1 = 1)) || ((turn % 2 == 0) && (move1 = 0)))
			playermove();
	}
	turn++;
}

void showboard(){
	char graph[9];
	int i;
	for(i = 0; i < 9; i++){
		if(board[i] == 3){
			graph[i] = 'X';
		}
		else if(board[i] == 2){
			graph[i] = ' ';
		}
		else graph[i] = 'O';
	}
	clear();
	mvprintw(13, 57, "%c | %c | %c\n", graph[0], graph[1], graph[2]);
	mvprintw(14, 56, "------------\n");
	mvprintw(15, 57, "%c | %c | %c\n", graph[3], graph[4], graph[5]);
	mvprintw(16, 56, "------------\n");
	mvprintw(17, 57, "%c | %c | %c\n\n", graph[6], graph[7], graph[8]);
	refresh();
	usleep(DELAY);
 	
	if(turn > 4){	
		if(checkwin() == 1){
			winpage();
			exit(1);
		}
		else{ 
			if(turn == 9) {
				curs_set(FALSE);
				mvprintw(25, 55, "It's a draw !! \n");
				getch();
				endwin();
				exit(1);
			}
		}
	}
	
	
}

int make2(){
	if((board[4]) == 2)
		return 5;
	else if((board[1]) == 2)
		return 2;
	else if((board[3]) == 2)
		return 4;
	else if((board[5]) == 2)
		return 6;
	else if((board[7]) == 2)
		return 8;
}
//!! RANDOM has to be returned out of 1,3,5,7...? HOW ??
//Why is it giving a typecasting error??

int posswin(int value){
	int check, i;
	
	if(value == 1)
		check = 18; //i.e. posswin(X)
	else check = 50;

	for(i = 0; i < 9; i = i + 3){
		if(board[i] * board[i+1] * board[i+2] == check){	
			if(board[i] == 2) //Is there any shorter way to check 3 LHS with same RHS and return the match?
				return i;
			else if(board[i+1] == 2)
				return (i+1);
			else if(board[i+2] == 2)
				return (i+2);
		}
	}

	for(i = 0; i < 3; i = i + 1){
		if(board[i] * board[i+3] * board[i+6] == check){
			if(board[i] == 2)
				return i;
			else if(board[i+3] == 2)
				return (i+3);
			else if(board[i+6] == 2)
				return (i+6);
		}
	}
	
	if(board[0] * board[4] * board[8] == check){
		if(board[0] == 2)
			return 0;
		else if(board[4] == 2)
			return 4;
		else if(board[8] == 2)
			return 8;
	}

	else if(board[2] * board[4] * board[6] == check){
		if(board[2] == 2)
			return 2;
		else if(board[4] == 2)
			return 4;
		else if(board[6] == 2)
			return 6;
	}

	else return 10; //For win not possible
}


int playermove(){

	int slot = -1;
	int yes;
	clear();
	showboard();	
	refresh();
	curs_set(FALSE);
	yes = getch();
	if((yes == 'h') || (yes == 'H')) {
		pcmove();
		return 0;
	}
	else{ 
	curs_set(TRUE);
//	printw("Enter cell number of your move");
//	scanw("%d", &slot);
//	Have value of slot //
	move(13, 57);
	keypad(stdscr, TRUE);
	int a = 0;
	
	if((mode == 2) && (turn % 2 == 1)){
		while(a != ' ') {
			a = getch();
			int nowx, nowy;
			getyx(stdscr, nowy, nowx);
		
			if(a == 'w' && nowy == 13) {
				move(13, nowx);
				if(nowx == 57)
					slot = 1;
				else if(nowx == 61)
					slot = 2;
				else if(nowx == 65)
					slot = 3;
			}
			else if(a == 'w' && nowy == 15) {
				move(13, nowx);
				if(nowx == 57)
					slot = 1;
				else if(nowx == 61)
					slot = 2;
				else if(nowx == 65)
					slot = 3;
			}
			else if(a == 'w' && nowy == 17) {
				move(15, nowx);
				if(nowx == 57)
					slot = 4;
				else if(nowx == 61)
					slot = 5;
				else if(nowx == 65)
					slot = 6;
			}
			else if(a == 's' && nowy == 13) {
				move(15, nowx);
				if(nowx == 57)
					slot = 4;
				else if(nowx == 61)
					slot = 5;
				else if(nowx == 65)
					slot = 6;
			}
			else if(a == 's' && nowy == 15) {
				move(17, nowx);
				if(nowx == 57)
					slot = 7;
				else if(nowx == 61)
					slot = 8;
				else if(nowx == 65)
					slot = 9;
			}
			else if(a == 's' && nowy == 17) {
				move(17, nowx);
				if(nowx == 57)
					slot = 7;
				else if(nowx == 61)
					slot = 8;
				else if(nowx == 65)
					slot = 9;
			}
			else if(a == 'd' && nowx == 57) {
				move(nowy, 61);
				if(nowy == 13)
					slot = 2;
				else if(nowy == 15)
					slot = 5;
				else if(nowy == 17)
					slot = 8;
			} 
			else if(a == 'd' && nowx == 61) {
				move(nowy, 65);
				if(nowy == 13)
					slot = 3;
				else if(nowy == 15)
					slot = 6;
				else if(nowy == 17)
					slot = 9;
			} 
			else if(a == 'd' && nowx == 65) {
				move(nowy, 65);
				if(nowy == 13)
					slot = 3;
				else if(nowy == 15)
					slot = 6;
				else if(nowy == 17)
					slot = 9;
			} 
			else if(a == 'a' && nowx == 57) { 
				move(nowy, 57);
				if(nowy == 13)
					slot = 1;
				else if(nowy == 15)
					slot = 4;
				else if(nowy == 17)
					slot = 7;
			} 
			else if(a == 'a' && nowx == 61) {
				move(nowy, 57);
				if(nowy == 13)
					slot = 1;
				else if(nowy == 15)
					slot = 4;
				else if(nowy == 17)
					slot = 7;
			} 
			else if(a == 'a' && nowx == 65) {
				move(nowy, 61);
				if(nowy == 13)
					slot = 2;
				else if(nowy == 15)
					slot = 5;
				else if(nowy == 17)
					slot = 8;
			}
		}	
	}

	else{
		while(a != '\n') {
		a = getch();
		int nowx, nowy;
		getyx(stdscr, nowy, nowx);
		
		
			if(a == KEY_UP && nowy == 13) {
				move(13, nowx);
				if(nowx == 57)
					slot = 1;
				else if(nowx == 61)
					slot = 2;
				else if(nowx == 65)
					slot = 3;
			}
			else if(a == KEY_UP && nowy == 15) {
				move(13, nowx);
				if(nowx == 57)
					slot = 1;
				else if(nowx == 61)
					slot = 2;
				else if(nowx == 65)
					slot = 3;
			}
			else if(a == KEY_UP && nowy == 17) {
				move(15, nowx);
				if(nowx == 57)
					slot = 4;
				else if(nowx == 61)
					slot = 5;
				else if(nowx == 65)
					slot = 6;
			}
			else if(a == KEY_DOWN && nowy == 13) {
				move(15, nowx);
				if(nowx == 57)
					slot = 4;
				else if(nowx == 61)
					slot = 5;
				else if(nowx == 65)
					slot = 6;
			}
			else if(a == KEY_DOWN && nowy == 15) {
				move(17, nowx);
				if(nowx == 57)
					slot = 7;
				else if(nowx == 61)
					slot = 8;
				else if(nowx == 65)
					slot = 9;
			}
			else if(a == KEY_DOWN && nowy == 17) {
				move(17, nowx);
				if(nowx == 57)
					slot = 7;
				else if(nowx == 61)
					slot = 8;
				else if(nowx == 65)
					slot = 9;
			}
			else if(a == KEY_RIGHT && nowx == 57) {
				move(nowy, 61);
				if(nowy == 13)
					slot = 2;
				else if(nowy == 15)
					slot = 5;
				else if(nowy == 17)
					slot = 8;
			} 
			else if(a == KEY_RIGHT && nowx == 61) {
				move(nowy, 65);
				if(nowy == 13)
					slot = 3;
				else if(nowy == 15)
					slot = 6;
				else if(nowy == 17)
					slot = 9;
			} 
			else if(a == KEY_RIGHT && nowx == 65) {
				move(nowy, 65);
				if(nowy == 13)
					slot = 3;
				else if(nowy == 15)
					slot = 6;
				else if(nowy == 17)
					slot = 9;
			} 
			else if(a == KEY_LEFT && nowx == 57) { 
				move(nowy, 57);
				if(nowy == 13)
					slot = 1;
				else if(nowy == 15)
					slot = 4;
				else if(nowy == 17)
					slot = 7;
			} 
			else if(a == KEY_LEFT && nowx == 61) {
				move(nowy, 57);
				if(nowy == 13)
					slot = 1;
				else if(nowy == 15)
					slot = 4;
				else if(nowy == 17)
					slot = 7;
			} 
			else if(a == KEY_LEFT && nowx == 65) {
				move(nowy, 61);
				if(nowy == 13)
					slot = 2;
				else if(nowy == 15)
					slot = 5;
				else if(nowy == 17)
					slot = 8;
			}
		}
	}
	if(slot == -1)
		slot = 1;	
	go(slot);
	}
}

void pcmove(){
	switch(turn){	
		case 1:
			go(5);
			break;

		case 2:
			if(board[4] == 3)
				go(1);
			else if(board[4] == 2)
				go(5);
			break;

		case 3:
			if(board[8] == 2)
				go(9);
			else go(3);
			break;
	
		case 4:
			if(posswin(1) != 10)
				go((posswin(1))+1);
			else if(board[1] == 3 && board[5] == 3)
				go(3);
			else if(board[1] == 3 && board[3] == 3)
				go(1);
			else if(board[5] == 3 && board[7] == 3)
				go(9);
			else if(board[3] == 3 && board[7] == 3)
				go(7);			
			else
				go(make2());
			break;
	
		case 5:
			if(posswin(1) != 10)
				go((posswin(1))+1);
			else if(posswin(0) != 10)
				go((posswin(0))+1);
			else if(board[6] == 2)
				go(7);
			else go(4);
			break;
	
		case 6:
			if(posswin(0) != 10)
				go((posswin(0))+1);
			else if(posswin(1) != 10)
				go((posswin(1))+1);
			else(go(make2()));
			break;
	
		case 7:	
			if(posswin(1) != 10)
				go((posswin(1))+1);
			else if(posswin(0) != 10)
				go((posswin(0))+1);
			else go(anyblank()+1);
			break;
		
		case 8:
			if(posswin(0) != 10)
				go((posswin(0))+1);
			else if(posswin(1) != 10)
				go((posswin(1))+1);
			else go(anyblank()+1);
			break;
		
		case 9:
			if(posswin(1) != 10)
				go((posswin(1))+1);
			else if(posswin(0) != 10)
				go((posswin(0))+1);
			else go(anyblank()+1);
			break;                                                    
	}
}	
	
int anyblank(){
	int i;
	for(i = 0; i < 9; i++){
		if(board[i] == 2)
			return i;
	}
}
//Finds all the empty spaces and "randomly" gives one back?

int checkwin(){
	int check, i;

	if(turn % 2 == 1)
		check = 27; //If the move made was just odd, then for X to win, 3*3*3=27
	else check = 125;

	for(i = 0; i < 9; i = i + 3){
		if(board[i] * board[i+1] * board[i+2] == check)	
			return 1;
	}

	for(i = 0; i < 3; i = i + 1){
		if(board[i] * board[i+3] * board[i+6] == check)
			return 1;
	}
	
	if(board[0] * board[4] * board[8] == check)
			return 1;

	if(board[2] * board[4] * board[6] == check)
			return 1;

	else return 0;
}

void winpage(){
	
	if(move1 = 1)
		if (turn % 2 == 1)
			mvprintw(23, 58, "You Won!!");
		else
			mvprintw(23, 58, "PC Won!!");
	if(move1 = 0)
		if (turn % 2 == 0)
			mvprintw(23, 58, "You Won!!");
		else
			mvprintw(23, 58, "PC Won!!");
	printw("\n\n");
	
	mvprintw(29, 16, "-----------------------------------------GAME OVER------------------------------------------\n");	
	getch();
	refresh();
	endwin();
	exit(1);
}




void player2(){
	int i, odd, j;
	char name1[20], name2[20];
	char name[2][20];	
	//echo();
	for(i = 0; i < 2; i++){	
		mvprintw((3+(3*i)), 50, "Player %d, What's your name?\n", i+1);
		scanw("%s", &name[i]);
		mvprintw(4+(3*i), 50, "Player %d is - %s\n", i + 1, name[i]);
	}
	strcpy(name1, name[0]);
	strcpy(name2, name[1]);	
	
	mvprintw(10, 50, "Multiplayer rules are as follow: \n");
	
	mvprintw(13, 10, "%s's latest move on this side", name1);
	mvprintw(15, 10, "Navigate with W-A-S-D key set");
	mvprintw(17, 10, "'Space' for selecting");
	
	mvprintw(13, 76, "%s's latest move on this side", name2);
	mvprintw(15, 76, "Navigate with regular arrow keys");
	mvprintw(17, 76, "'Enter' key for selecting");

	mvprintw(19, 50, "Press any key to continue");	
	getch();
	system("clear");
	while(turn <= 9){
		if((turn % 2) == 1)
			mvprintw(8, 50, "Hey, %s, Play your move", name1);
		else 
			mvprintw(8, 50, "Hey, %s, Play your move", name2);
		getch();
		playermove();
		turn--;
		if(turn > 4){	
			if(checkwin() == 1){
				victorydisp(name1, name2);
			}
		}
		else{ 
			if(turn == 9){
				curs_set(FALSE);
				mvprintw(25, 55, "It's a draw !! \n");
				getch();
				endwin();
			}
		}
		turn++;
	}
}

void show2xboard(int changeloc){
	
	char graph[9];
	int i;
	for(i = 0; i < 9; i++){
		if(board[i] == 3){
			graph[i] = 'X';
		}
		else if(board[i] == 2){
			graph[i] = ' ';
		}
		else graph[i] = 'O';
	}
	char hold = graph[changeloc];
	clear();

	if(turn % 2 == 1){
		
		mvprintw(13, 37, "%c | %c | %c\n", graph[0], graph[1], graph[2]);
		mvprintw(14, 36, "------------\n");
		mvprintw(15, 37, "%c | %c | %c\n", graph[3], graph[4], graph[5]);
		mvprintw(16, 36, "------------\n");
		mvprintw(17, 37, "%c | %c | %c\n\n", graph[6], graph[7], graph[8]);
	
		graph[changeloc] = ' ';
	
		mvprintw(13, 77, "%c | %c | %c\n", graph[0], graph[1], graph[2]);
		mvprintw(14, 76, "------------\n");
		mvprintw(15, 77, "%c | %c | %c\n", graph[3], graph[4], graph[5]);
		mvprintw(16, 76, "------------\n");
		mvprintw(17, 77, "%c | %c | %c\n\n", graph[6], graph[7], graph[8]);
	}
	
	else{
		
		graph[changeloc] = ' ';
		
		mvprintw(13, 37, "%c | %c | %c\n", graph[0], graph[1], graph[2]);
		mvprintw(14, 36, "------------\n");
		mvprintw(15, 37, "%c | %c | %c\n", graph[3], graph[4], graph[5]);
		mvprintw(16, 36, "------------\n");
		mvprintw(17, 37, "%c | %c | %c\n\n", graph[6], graph[7], graph[8]);
	
		graph[changeloc] = hold; 	
	
		mvprintw(13, 77, "%c | %c | %c\n", graph[0], graph[1], graph[2]);
		mvprintw(14, 76, "------------\n");
		mvprintw(15, 77, "%c | %c | %c\n", graph[3], graph[4], graph[5]);
		mvprintw(16, 76, "------------\n");
		mvprintw(17, 77, "%c | %c | %c\n\n", graph[6], graph[7], graph[8]);
	}
		
	refresh();
	usleep(DELAY);
}

void victorydisp(char *name1, char *name2){
	if(turn % 2 == 1)
		mvprintw(23, 58, "%s Won!!", name1);
	else
		mvprintw(23, 58, "%s Won!!", name2);
	printw("\n\n");
	
	mvprintw(31, 16, "-----------------------------------------GAME OVER------------------------------------------\n");	
	getch();
	refresh();
	endwin();
	exit(1);
}






















