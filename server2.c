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

#define	MY_PORT	7777

struct playerPosition{
    int *new_sock;
	int id;
	int x;
	int y;
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

        send(sock,currentPlayer,sizeof(struct playerPosition),0);
//     printf("Before recieving from client\n--------------------------\n");
//	printf("Character array c has the following byte values:\n");
//	printf("sock %d \nplayer id: %d\nx: %d \ny: %d\n",*(currentPlayer->new_sock),currentPlayer->id,currentPlayer->x,currentPlayer->y);

	}
//	printf("\nAfter receiving from client\n-------------------------\n");
//	// printf("Printing character array c as a string is: %s\n",c);
//	printf("Character array c has the following byte values:\n");
//printf("sock %d \nplayer id: %d\nx: %d \ny: %d\n",*(currentPlayer->new_sock),currentPlayer->id,currentPlayer->x,currentPlayer->y);
    free(currentPlayer);

    pthread_exit("");
}
int main(int argc, char * argv[])
{
	int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;
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
	master.sin_port = htons (MY_PORT);

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen (sock, 5);
	fromlength = sizeof (from);
    struct playerPosition *player1;
    player1 = malloc(sizeof(struct playerPosition));
     player1->new_sock = malloc(1);
    player1->x = 1;
    player1->y = 1;

    /*******************/
    while (snew = accept (sock, (struct sockaddr*) & from, & fromlength))
    {
//        printf("sock %d \nplayer id: %d\nx: %d \ny: %d\n",*(player1->new_sock),player1->id,player1->x,player1->y);
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
