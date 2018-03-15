/*
 * Copyright 2017 Yongjia Huang,Yuhan Ye CMPUT379, University of Alberta, All Rights Reserved.
 * You may use distribut, or modify this code under terms and conditions of the ode of Student Behavior at University of Alberta
 * You may find a copy of the license in this project. Otherwise please contact jajayongjia@gmail.com
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include<signal.h>

int currentLen = 0;
int storedLen = 0;

struct file {


 char *fileName;
 char *fileType;
 char *modifiedTime;
 ino_t ino;

};

void getM(char *fileName,struct file *CurrentFindFiles);
void getFiles(char *diraction,struct file *CurrentFindFiles,struct file *LastFindFiles);
void printOutPut(struct file *CurrentFindFiles,struct file *LastFindFiles);
void storeScanedFiles(struct file *CurrentFindFiles,struct file *LastFindFiles);
void sigUserHandler(int usignal);
void sigIntHandler(int isignal);
void printTime(time_t time);
void getM(char *fileName, struct file *CurrentFindFiles) {

    struct stat inputFile;
    char buff1[50];

    struct tm * timeinfo1;

    stat(fileName, &inputFile);
    timeinfo1 = localtime (&(inputFile.st_ctime));
	strftime(buff1, 50, "%Y %b %d %T ", timeinfo1);
    CurrentFindFiles->modifiedTime = malloc(sizeof(char) * 50 +1);
    strcpy(CurrentFindFiles->modifiedTime,buff1);
    CurrentFindFiles->ino = inputFile.st_ino;
     // printf("%s\n", CurrentFindFiles->modifiedTime);

}


//---------------------------------
// change to switch.
char* changeType(int a){
 char *returnValue;
 if(a == 8){
  returnValue = "+";

 }
 else if (a == 4){
  returnValue = "d";
 }
 else{
  returnValue = "o";
 }
 return returnValue;
}
//---------------------------------


void getFiles(char *diraction,struct file *CurrentFindFiles,struct file *LastFindFiles){
   DIR *dir;
   struct dirent *entry;
   int i = 0;
   currentLen = 0;
   char pathToFile[50];

   if ((dir = opendir(diraction)) == NULL){
      printf("%s\n","Does not exisit, byebye! ---ending program." );
      exit(0);
}
   else {
   while ((entry = readdir(dir)) != NULL){
    if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")){


         CurrentFindFiles[i].fileName = malloc(sizeof(char) * 50 +1);
         strcpy(CurrentFindFiles[i].fileName,entry->d_name);

         CurrentFindFiles[i].fileType = malloc(sizeof(char) * 50 +1);
         strcpy(CurrentFindFiles[i].fileType,changeType(entry->d_type));

         strcpy(pathToFile,diraction);
         strcat(pathToFile,"/");
         strcat(pathToFile,CurrentFindFiles[i].fileName);

         getM(pathToFile, &CurrentFindFiles[i]);
         i++;

    }

    currentLen = i;
   }


   closedir(dir);
  }

}



void printOutPut(struct file *CurrentFindFiles,struct file *LastFindFiles){




	int find ;
	//find = -1  means the file is deleted
	// find = -2 means the file is modified.
	for(int i = 0; i<storedLen; i++){

		find = -1;
		for(int n = 0; n<currentLen; n++){



			if (!strcmp(LastFindFiles[i].fileName , CurrentFindFiles[n].fileName)){
				find =i;

				if (strcmp(LastFindFiles[i].modifiedTime,CurrentFindFiles[n].modifiedTime)){
					find = -2;
				}

				if (LastFindFiles[i].ino != CurrentFindFiles[n].ino){
					find = -1;
				}

			}
		}

		if (find == -1){
			printf("- %s\n",LastFindFiles[i].fileName);
		}else if (find == -2){
			printf("* %s\n",LastFindFiles[i].fileName );
		}
	}






   for(int i = 0; i<currentLen; i++){
   	    find = -1;


		for(int n = 0; n<storedLen; n++){


			if (!strcmp(CurrentFindFiles[i].fileName,LastFindFiles[n].fileName)){

				find =i;

				if (LastFindFiles[n].ino != CurrentFindFiles[i].ino){
					find = -1;
				}
			}
		}

		if (find == -1){
			printf("%s %s\n",CurrentFindFiles[i].fileType,CurrentFindFiles[i].fileName);
		}
	}




}

void storeScanedFiles(struct file *CurrentFindFiles,struct file *LastFindFiles){
	for (int i = 0; i<currentLen; i++){


		LastFindFiles[i].fileName = malloc(sizeof(char) * 50 +1);
        strcpy(LastFindFiles[i].fileName,CurrentFindFiles[i].fileName);


        LastFindFiles[i].fileType = malloc(sizeof(char) * 50 +1);
        strcpy(LastFindFiles[i].fileType,CurrentFindFiles[i].fileType);


        LastFindFiles[i].modifiedTime = malloc(sizeof(char) * 50 +1);
        strcpy(LastFindFiles[i].modifiedTime,CurrentFindFiles[i].modifiedTime);

	    LastFindFiles[i].ino = CurrentFindFiles[i].ino;

	    free(CurrentFindFiles[i].fileName);
	    free(CurrentFindFiles[i].fileType);
	    free(CurrentFindFiles[i].modifiedTime);



	}

}

static int f = 0;
void signalHandler(int rsignal){
	switch(rsignal){
		case SIGUSR1:
			f=1;
			break;
		case SIGINT:
			printf("\n byebye! ---ending program.\n");
			exit(0);
			break;
		default:
			printf("catch a signal\n");

	}
}


void printTime(time_t time){
	 struct tm timeinfo;
	 char buff[100];
	 timeinfo = *localtime(&time);
	 strftime(buff, sizeof buff, "%a %b %d %T %Z %Y", &timeinfo);
	 printf("%s\n", buff);
}

int main(int argc, char *argv[])
{

	 double period;
	 char *path;
	 double diff_t;
	 time_t startTime;
	 time_t endTime;
	 time_t sigtime;

	 struct file CurrentFindFiles[1000];
	 struct file LastFindFiles[1000];


	 period = atof(argv[1]);
	 path = argv[2];

	 startTime = time(NULL);


	while(1){

	    printTime(startTime);

	    getFiles(path,CurrentFindFiles,LastFindFiles);
	    printOutPut(CurrentFindFiles,LastFindFiles);
	    // do check file
	    storedLen = currentLen;
     	storeScanedFiles(CurrentFindFiles,LastFindFiles);


	 do{


	 	// printf("|");
		  endTime = time(NULL);
		  diff_t = difftime(startTime, endTime);
		  signal(SIGUSR1,signalHandler);

	 	if(f){
			f=0;
			sigtime = time(NULL);
	 		printTime(sigtime);
	    	getFiles(path,CurrentFindFiles,LastFindFiles);
	    	printOutPut(CurrentFindFiles,LastFindFiles);
	    	// do check file
	    	storedLen = currentLen;
     	 	storeScanedFiles(CurrentFindFiles,LastFindFiles);

	 	}

		signal(SIGINT,signalHandler);


	 } while(diff_t != -period);

	 startTime = endTime;
	 }




	 return 0;
}
