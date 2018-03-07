/*
 * Copyright 2017 Yongjia Huang, Yuhan Ye,CMPUT379, University of Alberta, All Rights Reserved.
 * You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
 * You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
 */

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
    int death; // 1 if this player is hit
    int score; // 0 in default
};

struct allplayer{
    struct playerPosition players[30];
    int currentIndex;
};

pthread_mutex_t lock;
struct allplayer allplayers;
struct timeb currentTime;
struct timeb t_start,t_current,readtime;
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
    int period;
    period =(int) (allplayers.players[0].updatePeriod * 1000.0);
//    period = 1000;


    allplayers.currentIndex = nth;
	while (1){
//    printf("a%d",allplayers.players[1].id);

    ftime(&t_current);
    while ((int) (1000.0 * (t_current.time - t_start.time)
        + (t_current.millitm - t_start.millitm)) < period){
        ftime(&t_current);
        }
    t_start = t_current;
//    printf("b%d",allplayers.players[1].id);

    allplayers.players[nth] = currentplayer;
    char move;

    printf("ready to send");
    send(sock,&allplayers,sizeof(struct allplayer),0);
    	// Now we receive from the client,
    printf("ready to recv");
    recv(sock,&move,sizeof( char),0);
    printf("||recved||");

        // the switch statment below should check all constraint of all the clients,
        // and do the update
 switch(move){
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
    // if yes,  other_player.death = 1
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



        break;
        default :
          clearfireo(&currentplayer);
    }
	}

    puts("connection down");
    pthread_exit("");

}
int main(int argc, char * argv[])
{

	int	sock, snew, fromlength, number, outnum,i,valread;
	int client_socket[30] , max_clients = 30, sd , max_sd,activity;
	struct	sockaddr_in	master;
	pthread_t thread_id;
	ftime(&t_start);

	char buffer[1025];
	fd_set readfds;


        if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 1;
    }

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

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

	fromlength = sizeof (master);

	puts("Waiting for connections ...");

    while(1){
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(sock, &readfds);
        max_sd = sock;

         //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
 //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }


                //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(sock, &readfds))
        {
            if ((snew = accept(sock, (struct sockaddr *)&master, (socklen_t*)&fromlength))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , snew , inet_ntoa(master.sin_addr) , ntohs(master.sin_port));
            puts("Connection accepted");


            pthread_t thread1;
            struct playerPosition currentplayer;
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = snew;
                    printf("Adding to list of sockets as %d\n" , i);
            currentplayer.id = i;
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

                    break;
                }
            }
            ftime(&currentTime);
            printf("thread ID %d",(int)thread1);
        	if( pthread_create( &thread1 , NULL ,  connection_handler , (void*)&currentplayer) < 0)
     		   {
            perror("could not create thread");
            return 1;

      		  }

        }
        puts("ss");

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&master , (socklen_t*)&fromlength);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(master.sin_addr) , ntohs(master.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                    allplayers.players[i].exist = 0;
                }

            }
        }

}

pthread_mutex_destroy(&lock);
 return 0;
}
