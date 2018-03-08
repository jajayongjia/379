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
#include <string.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <time.h>
#include <sys/timeb.h>
#define START_Y 10
#define START_X 10
/* ---------------------------------------------------------------------
   This is a sample client program for the number server. The client and
   the server need to run on the same machine.
   --------------------------------------------------------------------- */

struct playerPosition{
    int exist;    // 1 if this player is enabled
    int boardsize;  // user input boardsize
    double updatePeriod; // user input updateperiod
    int MY_PORT;    // user input my_port
    int new_sock;   // sock id
    int id;          // player id
    int x;           // player position x
    int y;            // player position y
    char direction[2];  // player direction
    char move;          // player's input
    int fire; // if fire == 1 -> draw o on screen
    int o[4]; // contains x1,y1,x2,y2
    int score; // 0 in default
};


// This structure is the message pass from server to clients
struct allplayer{
    struct playerPosition players[30];  // contains 30 clients, Max number of client is 30
    int currentIndex;                   // whenever a message is passed to client, it will tell its client its ID
    int death[30];                      // death contains which client  is dead
};
char changePosition(WINDOW* win,int period);

void drawScreen(WINDOW* win,struct playerPosition player[30],int current,char *move,int death[30]);


int main(int argc, char * argv[])
{

	int	s;
    struct allplayer allplayers;
	struct	sockaddr_in	server;
	int ID;
	int a=0;
	int updatep;
	struct	hostent		*host;

/* Connect to the server */
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


    WINDOW *win;
    char move;
    curs_set(0);




/** After successfully connect to the server, client enters in the while loop below
 ** This while loop support client constantly recv and sand data from and to the server
 ** Client recv a struct allplayer from the server and print the latest map on screen
 ** Client make move and send a char message to the server
 ** This char message contains only one size of char type data
 ** and contains the last user input if this client did not died
 ** or 'd' if client is died  **/

	while(1){


        recv(s,&allplayers,sizeof(struct allplayer),0);


        // init the win screen if it is the first entry of the loop
	    if (a==0){
            ID = allplayers.currentIndex;
            win = newwin(allplayers.players[ID].boardsize+2,allplayers.players[ID].boardsize+2,START_Y,START_X);
            refresh();
             updatep =(int) (allplayers.players[ID].updatePeriod * 1000.0);
         }
        a=1;
        move = ' ';

        // After recving the latest data, draw them on screen.
		drawScreen(win,allplayers.players,ID,&move,allplayers.death);

		// check whether this client is died or not
        if(move == 'd'){
            goto end;
        }
        refresh();

         // program will ask for client's move five times, and each time has a period of updateperiod/5
         char moves[5];
	     int i = 0;
         for (i=0;i<5;i++){
            moves[i] = changePosition(win,updatep/5);
         }

 	     // get the latest user valid input
 	     // if no valid input provided, return '?' instead 
         for (i = 4;i>=0;i--){
            if(moves[i]!=-1){
                move = moves[i];
                break;
            }
            move = '?';
         }


         // handle death 
        if (move == 'd'){
                allplayers.death[ID] = 1;
                drawScreen(win,allplayers.players,ID,&move,allplayers.death);
                    goto end;
        }

		send(s,&move,sizeof(char),0);
	}
end:
    close (s);
    sleep(2);
     endwin();
}



// This Function draw game map on screen.
void drawScreen(WINDOW* win,struct playerPosition player[30],int current,char *move,int death[30]){
	wclear(win);
	char lr = '|';
	char tb = '-';
	box(win,(int)lr,(int)tb);
	refresh();
	wrefresh(win);


	mvwprintw(win,0,0,"+");
	mvwprintw(win,0,player[0].boardsize+1,"+");
	mvwprintw(win,player[0].boardsize+1,0,"+");
	mvwprintw(win,player[0].boardsize+1,player[0].boardsize+1,"+");
	for(int i=0;i<30;i++){
        if (player[i].exist == 1){

            mvwprintw(win,player[i].y,player[i].x,player[i].direction);
            if(player[i].fire ==1 ){
                for(int j = 0 ; j < 3;){
                    if ((player[i].o[j] !=-1)&&(player[i].o[j+1] !=-1)){
                        mvwprintw(win,player[i].o[j+1],player[i].o[j],"o");

                        }
                        j=j+2;
            }

        }
	}
    }

    //print information after the player is dead
    if(death[current]==1){
        wclear(win);
        printw("Score : %d",player[current].score);
        printw("You Lose!\n");
        refresh();
        printw("Wait 2 Seconds to exit\n");
        refresh();
        *move = 'd';
    }
	wrefresh(win);

}
//This function gets the user input and return the action as a char
char changePosition(WINDOW* win,int period){
	keypad(win,true);
    wtimeout(win,period);
    noecho();
	int c = wgetch(win);
	//user input:i     action:move up
	//user input:j     action:move left
	//user input:k     action:move down
	//user input:l     action:move right
	//user input:' '   action:fire
	//user input:x     action:quit the game

		switch(c){
		case 'i':
			return '^';

			break;
		case 'k':

			return 'v';
			break;
		case 'j':

			return '<';
			break;
		case 'l':
			return '>';
			break;
		case ' ':
            return 'f';
            break;
        case 'x':
            return 'd';
            break;
        default:
            return -1;
            break;



	}


}

