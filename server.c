/*
 * Copyright 2017 Yongjia Huang, Yuhan Ye,CMPUT379, University of Alberta, All Rights Reserved.
 * You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
 * You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
 */
// gcc client.c -o client -lncurses
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/timeb.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <netinet/in.h>
#include<pthread.h>
#include <signal.h>
#define START_Y 10
#define START_X 10

struct playerPosition{
    int exist;
    int boardsize;
    double updatePeriod;
    int MY_PORT;
    int seed;
    int new_sock;
    int id;
    int x;
    int y;
    char direction[2];
    char move;
    int fire; // if fire == 1 -> draw o on screen
    int o[4]; // contains x1,y1,x2,y2
    int score; // 0 in default
};

struct allplayer{
    struct playerPosition players[30];
    int currentIndex;
    int death[30];
};

pthread_mutex_t lock;
pthread_mutex_t lock1;
struct allplayer allplayers;
int num_client;
void clearfireo(struct playerPosition * currentplayer){
    currentplayer->fire = 0;

    currentplayer->o[0] = -1;
    currentplayer->o[1] = -1;
    currentplayer->o[2] = -1;
    currentplayer->o[3] = -1;


}
void *connection_handler(void *player1)
{
    //Get the socket descriptor
    struct playerPosition currentplayer;
    currentplayer = *(struct playerPosition *)player1;
    int nth = currentplayer.id;
    int sock = currentplayer.new_sock;


    struct timeb now;
    int period,recvreturn;
    period =(int) (allplayers.players[0].updatePeriod * 1000.0);


    allplayers.currentIndex = nth;
	while (1){



    allplayers.players[nth] = currentplayer;

    char move;

    pthread_mutex_lock(&lock1);
    send(sock,&allplayers,sizeof(struct allplayer),0);
    pthread_mutex_unlock(&lock1);
    	// Now we receive from the client,
    usleep(period);


    pthread_mutex_lock(&lock);
    recvreturn = recv(sock,&move,sizeof( char),0);
    pthread_mutex_unlock(&lock);

    if((recvreturn == 0)||(recvreturn == -1) ){
        allplayers.players[nth].exist = 0;
        puts("Client Exit");
        pthread_exit("");
    }


        // the switch statment below should check all constraint of all the clients,
        // and do the update
 switch(move){
//    case 'd':

        case '^':

            if(currentplayer.y > 1){
                currentplayer.y-=1;
                strcpy(currentplayer.direction,"^");
            }
            clearfireo(&currentplayer);
            break;
        case 'v':

            if((currentplayer.y) < (currentplayer.boardsize) ){
                currentplayer.y+=1;
                strcpy(currentplayer.direction,"v");
            }
            clearfireo(&currentplayer);
            break;
        case '<':

            if(currentplayer.x > 1){
                currentplayer.x-=1;
                strcpy(currentplayer.direction,"<");
            }
            clearfireo(&currentplayer);
            break;
        case '>':

            if((currentplayer.x) < (currentplayer.boardsize)){
                currentplayer.x+=1;
                strcpy(currentplayer.direction,">");
            }
            clearfireo(&currentplayer);
            break;

    // enable currentplayer.fire ( =1);
    // give o[4] to currentplayer
    // do check fire range is in boundary or not
    // also check if other clients current position is in o[4] or not
    // if yes,  currentplayer.death = id of the other player
    // currentplayer.score +=1


        case 'f':
            currentplayer.fire = 1;
            switch(currentplayer.direction[0]){
                case '^':
                    if(currentplayer.y==1){
                        ;
                    }
                    else if(currentplayer.y==2){
                        currentplayer.o[0] = currentplayer.x;
                        currentplayer.o[1] = 1;
                    }
                    else{
                        currentplayer.o[0] = currentplayer.x;
                        currentplayer.o[1] = currentplayer.y-1;
                        currentplayer.o[2] = currentplayer.x;
                        currentplayer.o[3] = currentplayer.y-2;
                    }
                    break;
                case 'v':
                    if(currentplayer.y == currentplayer.boardsize){
                        ;
                    }
                    else if(currentplayer.y == currentplayer.boardsize-1){
                        currentplayer.o[0] = currentplayer.x;
                        currentplayer.o[1] = currentplayer.boardsize-1;
                    }
                    else{

                        currentplayer.o[0] = currentplayer.x;
                        currentplayer.o[1] = currentplayer.y+1;
                        currentplayer.o[2] = currentplayer.x;
                        currentplayer.o[3] = currentplayer.y+2;
                    }
                    break;
                case '<':
                    if(currentplayer.x == 1){
                        ;
                    }
                    else if(currentplayer.x == 2){
                        currentplayer.o[0] = currentplayer.x-1;
                        currentplayer.o[1] = currentplayer.y;
                    }
                    else{
                        currentplayer.o[0] = currentplayer.x-1;
                        currentplayer.o[1] = currentplayer.y;
                        currentplayer.o[2] = currentplayer.x-2;
                        currentplayer.o[3] = currentplayer.y;
                    }
                    break;
                case '>':
                    if(currentplayer.x == currentplayer.boardsize){
                        ;
                    }
                    else if(currentplayer.x == currentplayer.boardsize-1){
                        currentplayer.o[0] = currentplayer.x+1;
                        currentplayer.o[1] = currentplayer.y;
                    }
                    else{
                        currentplayer.o[0] = currentplayer.x+1;
                        currentplayer.o[1] = currentplayer.y;
                        currentplayer.o[2] = currentplayer.x+2;
                        currentplayer.o[3] = currentplayer.y;
                    }
                    break;



            }
            // Here check whether bullet hit or not;
            // death contains the id of other player;
//            printf("bullet: %d,%d \t player1: %d %d\n",currentplayer.o[2],currentplayer.o[3],allplayers.players[0].x,allplayers.players[0].y);
            for (int i=0;i<=num_client;i++){
                    if (allplayers.players[i].exist == 1){
                        if(((allplayers.players[i].x == currentplayer.o[0]) && (allplayers.players[i].y == currentplayer.o[1]))
                           || ((allplayers.players[i].x == currentplayer.o[2]) && (allplayers.players[i].y == currentplayer.o[3]))){
                                 allplayers.death[i]=1;
                                 currentplayer.score++;
//                                 printf("player %d is death %d",i,allplayers.players[i].death);
                           }
                    }
            }




        break;
        default :
          clearfireo(&currentplayer);
    }


	}



}
int main(int argc, char * argv[])
{
  int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;


	int i = 0;
	for(i=0;i<30;i++){
        allplayers.death[i] = -1;
	}
//        if (pthread_mutex_init(&lock, NULL) != 0)
//    {
//        printf("\n mutex init has failed\n");
//        return 1;
//}
	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET;
	master.sin_addr.s_addr = inet_addr("127.0.0.1");
	master.sin_port = htons (atoi(argv[3]));

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

    puts("bind done");

    //Listen
   listen (sock, 5);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    while(1)
    {
        if (( snew = accept (sock, (struct sockaddr*) & from, & fromlength)) < 0)
        {
            perror("accept failed\n");
            exit(1);
        }
        puts("Connection accepted");
        //Reply to the client

        struct playerPosition currentplayer;
        pthread_t sniffer_thread;
        currentplayer.id = num_client;
        currentplayer.exist = 1;
        currentplayer.x = 2;
        currentplayer.y = 2;

        strcpy(currentplayer.direction,">");
        currentplayer.move = '?';
        currentplayer.boardsize = atoi(argv[1]);
        currentplayer.updatePeriod = atof(argv[2]);
        currentplayer.MY_PORT = atoi(argv[3]);
        currentplayer.seed = atoi(argv[4]);
        currentplayer.new_sock = snew;
        currentplayer.score = 0;

//        printf("thread ID %lu\n",currentplayer.threadID);

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler ,  (void*)&currentplayer) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        num_client++;
        puts("process end \n");

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
//        puts("Handler assigned");
    }



    return 0;
}





