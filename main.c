#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <ctype.h>
#include "queue.h"
#include "linkedList.h"
#include "wfdRepo.h"

void collectionPhase();
void *directoryQueue();
void *fileQueue();
linkedlist_t WFD(FILE* f);

//global variables

//optional args
int directoryThreads = 1;
int fileThreads = 1;
int analysisThreads = 1;
char *fileSuffix = ".txt";

int argcG;
char **argvG;
queue_t dirQ;
queue_t fileQ;

wfdrepo_t wfdRepo;

int main(int argc, char const *argv[]) {
  //set global vars
  argvG = argv;
  argcG = argc;

  collectionPhase();
  return 0;
}

//all optional arguments required before regular args
void collectionPhase(){

  if (argcG==1) {
    printf("%s\n", "Error, no arguments");
    exit(EXIT_FAILURE);
  }

  //set optional arguments
  int i = 1;
  while (i < argcG && argvG[i][0] == '-') { //loop through optional arguments
    switch (argvG[i][1]) {
      case 'd':
        directoryThreads = atoi(argvG[i]+2);
        if(directoryThreads==0){
          perror("Error: ");
          exit(EXIT_FAILURE);
        }
        break;
      case 'f':
        fileThreads = atoi(argvG[i]+2);
        if(fileThreads==0){
          perror("Error: ");
          exit(EXIT_FAILURE);
        }
        break;
      case 'a':
        analysisThreads = atoi(argvG[i]+2);
        if(analysisThreads==0){
          perror("Error: ");
          exit(EXIT_FAILURE);
        }
        break;
      case 's':
        fileSuffix = argvG[i]+2;
        break;
      default :
        perror("Error: ");
        exit(EXIT_FAILURE);
    }
    i++;
  }

  init(&dirQ);
  init(&fileQ);
  initRepo(&wfdRepo);


  //create threads and start
  //pthread_t dirThreads[directoryThreads];
  //pthread_t fThreads[fileThreads];

  /*init barrier
  //pthread_barrier_init(&directoryFinished, NULL, fileThreads);

  for (size_t i = 0; i < directoryThreads; i++) {
    pthread_create(&dirThreads[i], NULL, directoryQueue, NULL);
  }

  for (size_t i = 0; i < fileThreads; i++) {
    pthread_create(&fThreads[i], NULL, fileQueue, NULL);
  }*/

  //enque arguments to proper queue
  struct stat arg;

  for (size_t k = 1; k < argcG; k++) { //loop through arguments
    if (stat(argvG[k], &arg)==0){
      if(S_ISREG(arg.st_mode)) { //if is file
        enqueue(&fileQ, argvG[k]);
      }
      else if(S_ISDIR(arg.st_mode)) { //if is directory
        if(argvG[k][0] != '.'){
          enqueue(&dirQ, argvG[k]);
        }
      }
    }
  }

  directoryQueue();

  //sleep(1);

  //Queue_done(&dirQ);

  /*join threads
  void *retval;
  for (size_t j = 0; j < directoryThreads; j++) {
    pthread_join(dirThreads[j], &retval);
  }

  for (size_t l = 0; l < fileThreads; l++) {
    pthread_join(fThreads[l], &retval);
  }*/


}

void *directoryQueue(){
  DIR *dir;
  struct dirent *dent;
  struct stat arg;
  char *dirName;
  char bSlash = '/';
  while(dirQ.count>0){ //while directorys to dequeue
    printf("%s", "display: ");
    display(dirQ.head);
    dirName = dequeue(&dirQ);
    /*if(!dirName){
      break;
    }*/
    char *filePath = malloc(strlen(dirName)+2);
    strcpy(filePath, dirName);
    strcpy(filePath, "/");
    dir = opendir(dirName);
    while((dent = readdir(dir)) != NULL){ //loop through directory
      filePath = (char *) realloc(filePath, strlen(dirName)+strlen(dent->d_name)+1);
      strcat(filePath, dent->d_name);
      if (stat(filePath, &arg)==0){
        if(S_ISREG(arg.st_mode)) { //if is file
          if(strcmp(dent->d_name+strlen(dent->d_name)-strlen(fileSuffix), fileSuffix) == 0){ //if correct suffix
            printf("%s\n%s\n", "Enqueued file: ", dent->d_name);
            enqueue(&fileQ, filePath); //add to file queue
          }
        }
        else if(S_ISDIR(arg.st_mode)) { //if is directory
          if(dent->d_name[0] != '.'){
            strncat(filePath, &bSlash, 1);
            printf("%s\n%s\n","Enqueued directory: ", filePath);
            enqueue(&dirQ, filePath); //add to directory queue
            printf("%s", "display2: ");
            display(dirQ.head);
          }
        }
      }
      strcpy(filePath, dirName);
    }
    free(filePath);
  }

  return 0;
}

void *fileQueue(){
    char *fileName;
    while(fileQ.count>0){ //while files to dequeue
      fileName = dequeue(&fileQ);
      FILE *fp = fopen(fileName, "r");
      linkedlist_t wfrequency;
      initLinked(&wfrequency);
      wfrequency = WFD(fp);
      insertRepo(&wfdRepo, fileName, &wfrequency);
    }
    return 0;
}

//need to create WFD Data Structure, WFD Repo
linkedlist_t WFD(FILE* f){// returns a Linked List for the WFD
  char *buf = malloc(sizeof(char)*1000);
  char *word = malloc(sizeof(char)*1000);
  char tmp;
  linkedlist_t wfd;

  while(fscanf(f, "%s", buf) == 1){ //checking each individual word
      for (size_t i = 0; i < strlen(buf); i++) {
        tmp = tolower(buf[i]);
        if(!ispunct(tmp)){ //checking if a letter is not a punctuation mark
        word[i] = tmp;
        }
      }
      insertAlphabetically(&wfd, word);
    }
    updateFrequency(&wfd);
    return wfd;
  }


/*int JSD(linkedlist_t wfd1, linkedlist_t wfd2){
  linkedlist_t meanF; //mean frequency linkedlist
}*/
