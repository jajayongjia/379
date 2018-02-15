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
#include <strings.h>
#include <arpa/inet.h>
#include <ncurses.h>


#define	 MY_PORT  7777

/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */
struct playerPosition{
    int *new_sock;
	int id;
	int x;
	int y;
};
int draw(int y,int x);

int main()
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
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons (MY_PORT);

	if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}

	while(1){
    player = malloc(sizeof(struct playerPosition));

	recv(s,player,sizeof(struct playerPosition),0);

	player->x  +=1;
	player->y +=1;
	send(s,player,sizeof(struct playerPosition),0);

    printf("\nplayer id: %d\nx: %d \ny: %d\n",player->id,player->x,player->y);

    free(player);
	}
	close (s);
}


int draw(int y,int x){
	raw();
	int height,width,start_y,start_x;
	double xin = ((double)x) /100.0;
	double yin = ((double)y) /100.0;

	height=width=10;
	start_y = start_x =10;
	WINDOW* win = newwin(height,width,start_y,start_x);
	refresh();
	char lr = '|';
	char tb = '-';
	box(win,(int)lr,(int)tb);

	mvwprintw(win,0,0,"+");
	mvwprintw(win,0,9,"+");
	mvwprintw(win,9,0,"+");
	mvwprintw(win,9,9,"+");
	mvwprintw(win,yin,xin,">");

	wrefresh(win);
//	getch();
	endwin();

	return 0;
}
