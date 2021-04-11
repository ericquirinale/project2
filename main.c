#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include "queue.c"

int main(int argc, char const *argv[]) {
  collectionPhase();
  return 0;
}

//all optional arguments required before regular args
void collectionPhase(){

  //set optional arguments
  int i = 1;
  int directoryThreads = 1;
  int fileThreads = 1;
  int analysisThreads = 1;
  char *fileSuffix = ".txt";
  while (argv[i][0] == '-') { //loop through optional arguments
    switch (argv[i][1]) {
      case 'd':
        directoryThreads = atoi(argv[i]+2);
        if(directoryThreads==0){
          perror("Error: ");
          return(-1);
        }
        break;
      case 'f':
        fileThreads = atoi(argv[i]+2);
        if(fileThreads==0){
          perror("Error: ");
          return(-1);
        }
        break;
      case 'a':
        analysisThreads = atoi(argv[i]+2);
        if(analysisThreads==0){
          perror("Error: ");
          return(-1);
        }
        break;
      case 's':
        fileSuffix = argv[i]+2;
        break;
      default :
        perror("Error: ");
        return(-1);
    }
    i++;
  }

  //create directory and file queues
  queue_t dirQ;
  queue_t fileQ;

  init(&dirQ);
  init(&fileQ);

  //enque arguments to proper queue
  struct stat arg;

  for (size_t k = i; k < argc; k++) { //loop through arguments
    if (stat(argv[k], &arg)==0){
      if(S_ISREG(arg.st_mode)) { //if is file
        enqueue(&fileQ, argv[k]);
      }
      else if(S_ISDIR(arg.st_mode)) { //if is directory
        if(argv[k][0] != '.'){
          enqueue(&dirQ, argv[k]);
        }
      }
    }
  }

  //create threads and start
  pthread_t dirThreads[directoryThreads];
  pthread_t fThreads[fileThreads];

  for (size_t i = 0; i < directoryThreads; i++) {
    pthread_create(&dirThreads[i], NULL, directoryQueue, NULL)
  }

  for (size_t i = 0; i < fileThreads; i++) {
    pthread_create(&fThreads[i], NULL, fileQueue, NULL)
  }

  //kill threads


}

void directoryQueue(){
  Dir *dir;
  struct dirent *dent;
  struct stat arg;
  char *dirName
  while(dequeue(dirQ, dirName)==0){ //while directorys to dequeue
    dir = opendir(dirName);
    while((dent = readdir(dir)) != NULL){ //loop through directory
      if (stat(dent->d_name, &arg)==0){
        if(S_ISREG(arg.st_mode)) { //if is file
          if(strcmp(dent->d_name+strlen(dent->d_name+)-strlen(fileSuffix), fileSuffix) == 0){ //if correct suffix
            enqueue(&fileQ, dent->d_name);
          }
        }
        else if(S_ISDIR(arg.st_mode)) { //if is directory
          if(dent->d_name[0] != '.'){
            enqueue(&dirQ, dent->d_name);
          }
        }
      }
    }
  }
}

void fileQueue(){
    char *fileName;
    while(dequeue(fileQ, dirName)==0){ //while files to dequeue
      FILE *fp = fopen("fileName", "r");
      linkedlist_t wfrequency = WFD(fp);
      //add to wfdrep
    }
}
