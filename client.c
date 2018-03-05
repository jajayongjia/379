/*
 * Copyright 2017 Yongjia Huang, Yuhan Ye,CMPUT379, University of Alberta, All Rights Reserved.
 * You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
 * You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
 */


#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <time.h>

#define START_Y 10
#define START_X 10
/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

struct playerPosition{
    int boardsize;
    double updatePeriod;
    int MY_PORT;
    int seed;
    int *new_sock;
    int id;
    int x;
    int y;
    char direction[2];
    char move;
};

void changePosition(struct playerPosition *player,WINDOW* win);

void drawScreen(WINDOW* win,struct playerPosition *player);


int main(int argc, char * argv[])
{

	int	s, number;
    struct playerPosition *player;
	struct	sockaddr_in	server;

	struct	hostent		*host;

	host = gethostbyname ("localhost");



	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}


	s = socket (AF_INET, SOCK_STREAM, 0);

	if (s < 0) {
		perror ("Client: cannot open socket");
		exit (1);
	}

	bzero (&server, sizeof (server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons (atoi(argv[2]));
	if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}

	initscr();
	cbreak();
	int a = 0;


 WINDOW *win;
	while(1){
    	player = malloc(sizeof(struct playerPosition));

		recv(s,player,sizeof(struct playerPosition),0);
		if (a==0){
            win = newwin(player->boardsize,player->boardsize,START_Y,START_X);
        }
        a=1;
		// full contents with x, y , direction
		player->move = '?';
		drawScreen(win,player);
        unsigned int reTime = time(0) +1;
         while (time(0)<reTime){

		 	changePosition(player,win);
		 }

        // player only contains new move direction
		send(s,player,sizeof(struct playerPosition),0);

    	free(player);
	}
	close (s);
}

void drawScreen(WINDOW* win,struct playerPosition *player){
	wclear(win);
	char lr = '|';
	char tb = '-';
	box(win,(int)lr,(int)tb);
	refresh();
	wrefresh(win);

	mvwprintw(win,0,0,"+");
	mvwprintw(win,0,player->boardsize-1,"+");
	mvwprintw(win,player->boardsize-1,0,"+");
	mvwprintw(win,player->boardsize-1,player->boardsize-1,"+");
	strcat(player->direction,"\0");
	mvwprintw(win,player->y,player->x,player->direction);
	wrefresh(win);
}


void changePosition(struct playerPosition *player,WINDOW* win){
	keypad(win,true);

	int c = wgetch(win);
	switch(c){
		case KEY_UP:


			player->move='^';

			break;
		case KEY_DOWN:

			player->move='v';
			break;
		case KEY_LEFT:

			player->move='<';
			break;
		case KEY_RIGHT:

			player->move='>';
			break;
	}




}




