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

void draw(int y,int x,struct playerPosition *player,WINDOW* window,int height,int width);

void drawScreen(WINDOW* win,int height,int width);

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

	initscr();
	cbreak();
	int height,width,start_y,start_x;
	height=width=12;
	start_y = start_x =10;
	WINDOW* win = newwin(height,width,start_y,start_x);
	refresh();
	
	drawScreen(win,height,width);
	while(1){
    	player = malloc(sizeof(struct playerPosition));

	recv(s,player,sizeof(struct playerPosition),0);
	
	wclear(win);
	
	drawScreen(win,height,width);
	draw(player->y,player->x,player,win,height,width);

	send(s,player,sizeof(struct playerPosition),0);

   // printf("\nplayer id: %d\nx: %d \ny: %d\n",player->id,player->x,player->y);
	
	

    free(player);
	}
	close (s);
}

void drawScreen(WINDOW* win,int height,int width){
	char lr = '|';
	char tb = '-';
	box(win,(int)lr,(int)tb);
	refresh();
	wrefresh(win);

	mvwprintw(win,0,0,"+");
	mvwprintw(win,0,height-1,"+");
	mvwprintw(win,width-1,0,"+");
	mvwprintw(win,width-1,height-1,"+");
	wrefresh(win);
}

// void draw(int y,int x,struct playerPosition *player,WINDOW* win,int height, int width){
// 	char *direction,*p;
// 	direction = malloc(sizeof(char)*5);
	
// 	strcpy(direction,"?");

// 	mvwprintw(win,y,x,direction);
// 	wrefresh(win);
	
// 	keypad(win,true);
// 	int c = wgetch(win);
// 	switch(c){
// 		case KEY_UP:
// 			if(player->y>1)
// 			player->y-=1;
// 			*p = "^";
// 			break;
// 		case KEY_DOWN:
// 			if(player->y<height-2)
// 			player->y+=1;
// 			*p = "v";
// 			break;
// 		case KEY_LEFT:
// 			if(player->x>1)
// 			player->x-=1;
// 			*p="<";
// 			break;
// 		case KEY_RIGHT:
// 			if(player->x<width-2)
// 			player->x+=1;
// 			*p = ">";
// 			break;	
// 	}

// 	wrefresh(win);
// 	mvwprintw(win,y,x,direction);
// 	wrefresh(win);
// 	free(direction);
// }
void draw(int y,int x,struct playerPosition *player,WINDOW* win,int height, int width){
	char *direction;
	direction = malloc(sizeof(char)*5);
	
	strcpy(direction,"?");

	mvwprintw(win,y,x,direction);
	wrefresh(win);
	
	keypad(win,true);
	int c = wgetch(win);
	switch(c){
		case KEY_UP:
			if(player->y>1)
			player->y-=1;
				strcpy(direction,"^");
			break;
		case KEY_DOWN:
			if(player->y<height-2)
			player->y+=1;
				strcpy(direction,"v");
			break;
		case KEY_LEFT:
			if(player->x>1)
			player->x-=1;
				strcpy(direction,"<");
			break;
		case KEY_RIGHT:
			if(player->x<width-2)
			player->x+=1;
				strcpy(direction,">");
			break;	
	}

	wrefresh(win);
	mvwprintw(win,y,x,direction);
	wrefresh(win);
	free(direction);
}
