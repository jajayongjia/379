/*
 * Copyright 2017 Yongjia Huang, Yuhan Ye,CMPUT379, University of Alberta, All Rights Reserved.
 * You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
 * You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include<pthread.h>

#define START_Y 10
#define START_X 10


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
void *connection_handler(void *player1)
{
    //Get the socket descriptor
    struct playerPosition *currentPlayer;
    currentPlayer = malloc(sizeof(struct playerPosition));
    currentPlayer = (struct playerPosition *) player1;
    int sock = *(currentPlayer->new_sock);

    send(sock,currentPlayer,sizeof(struct playerPosition),0);
        // Now we receive from the client, we specify that we would like 11 bytes
    while (recv(sock,currentPlayer,sizeof(struct playerPosition),0)){

        // the switch statment below should check all constraint of all the clients,
        // and do the update

    switch(currentPlayer->move){
        case '^':

            if(currentPlayer->y > 1){
                currentPlayer->y-=1;
                strcpy(currentPlayer->direction,"^");
            }
            break;
        case 'v':

            if((currentPlayer->y) < (currentPlayer->boardsize-2) ){
                currentPlayer->y+=1;
                strcpy(currentPlayer->direction,"v");
            }
            break;
        case '<':

            if(currentPlayer->x > 1){
                currentPlayer->x-=1;
                strcpy(currentPlayer->direction,"<");
            }
            break;
        case '>':

            if((currentPlayer->x) < (currentPlayer->boardsize-2)){
                currentPlayer->x+=1;
                strcpy(currentPlayer->direction,">");
            }
            break;
    }

        send(sock,currentPlayer,sizeof(struct playerPosition),0);
    }
    free(currentPlayer);

    pthread_exit("");
}
int main(int argc, char * argv[])
{
    int sock, snew, fromlength, number, outnum;

    struct  sockaddr_in master, from;
    pthread_t thread_id;
    // char c[11];



    int i = 0;

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

    listen (sock, 5);
    fromlength = sizeof (from);
    struct playerPosition *player1;
    player1 = malloc(sizeof(struct playerPosition));
    player1->new_sock = malloc(1);
    player1->x = 2;
    player1->y = 2;
    strcpy(player1->direction,">");
    player1->move = '?';
    player1->boardsize = atoi(argv[1]);
    player1->updatePeriod = atof(argv[2]);
    player1->MY_PORT = atoi(argv[3]);
    player1->seed = atoi(argv[4]);

    /*******************/
    while (snew = accept (sock, (struct sockaddr*) & from, & fromlength))
    {

        puts("Connection accepted");

        pthread_t thread1;
        *(player1->new_sock) = snew;

        if( pthread_create( &thread1 , NULL ,  connection_handler , (void*) player1) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread1 , NULL);
        puts("Handler assigned");
    }

    if (snew < 0)
    {
        perror("accept failed");
        return 1;
    }
    /*******************/





    close (snew);
}

