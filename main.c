#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <ctype.h>
#include <math.h>
#include "queue.h"
#include "linkedList.h"
#include "wfdRepo.h"
#include "jsdList.h"

void collectionPhase();
void *directoryQueue();
void *fileQueue();
void analysisPhase(wfdrepo_t *repo);
linkedlist_t *WFD(FILE* f);
double JSD(linkedlist_t *wfd1, linkedlist_t *wfd2);
double logBase2(double x);

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
  fileQueue();
  analysisPhase(&wfdRepo);

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

void analysisPhase(wfdrepo_t *repo){
wfdrepo_t *tmp = repo->next;
if (tmp==NULL) {
  perror("Only one file found");
}
int size = repoSize(repo);
jsdlist_t *jsdArray = malloc(sizeof(jsdlist_t));
double jsd;
initJSD(jsdArray);
while (repo->next!=NULL) {
  while (tmp!=NULL) {
    jsd = JSD(repo->wfd, tmp->wfd);
    jsdArray = insertJsd(jsdArray, repo->fName, tmp->fName, repo->numTokens, tmp->numTokens, jsd);
    tmp = tmp->next;
  }
  repo = repo->next;
  tmp = repo->next;
}
displayJSD(jsdArray);
}


void *directoryQueue(){
  DIR *dir;
  struct dirent *dent;
  struct stat arg;
  char *dirName;
  char bSlash = '/';
  while(dirQ.count>0){ //while directorys to dequeue
    dirName = dequeue(&dirQ);
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
            enqueue(&fileQ, filePath); //add to file queue
          }
        }
        else if(S_ISDIR(arg.st_mode)) { //if is directory
          if(dent->d_name[0] != '.'){
            strncat(filePath, &bSlash, 1);
            enqueue(&dirQ, filePath); //add to directory queue
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
    linkedlist_t *wptr = malloc(sizeof(linkedlist_t));
    initLinked(wptr);
    while(fileQ.count>0){ //while files to dequeue
      fileName = dequeue(&fileQ);
      //printf("%s\n", fileName);
      FILE *fp = fopen(fileName, "r");
      wptr = WFD(fp);
      insertRepo(&wfdRepo, fileName, wptr);
    }
    return 0;
}

//need to create WFD Data Structure, WFD Repo
linkedlist_t *WFD(FILE* f){// returns a Linked List for the WFD
  char buf[1000];
  char word[1000];
  memset(buf, 0, sizeof(buf));
  memset(word, 0, sizeof(word));
  char *wordPtr = word;
  int tmpCount = 0;
  linkedlist_t *head = malloc(sizeof(linkedlist_t));
  initLinked(head);

  while(fscanf(f, "%s", buf) == 1){ //checking each individual word
      for (size_t i = 0; i < strlen(buf); i++) {
        if(!ispunct(buf[i])){ //checking if a letter is not a punctuation mark
        word[tmpCount] = tolower(buf[i]);
        tmpCount++;
        }
      }
      tmpCount = 0;
      head = insertAlphabetically(head, wordPtr);

      memset(buf, 0, sizeof(buf));
      memset(word, 0, sizeof(word));
    }
    updateFrequency(head);
    return head;
  }

 double JSD(linkedlist_t *wfd1, linkedlist_t *wfd2){
  linkedlist_t *head = malloc(sizeof(linkedlist_t));
  initLinked(head); //mean frequency linkedlist

  linkedlist_t *tmp1 = wfd1;
  linkedlist_t *tmp2 = wfd2;
  double jsd = 0.0;
  double kld1 = 0.0;
  double kld2 = 0.0;

  while(tmp1->next!=NULL){ //create head
    for (size_t i = 0; i < tmp1->occurences; i++) {
      head = insertAlphabetically(head, tmp1->word);
    }
    tmp1 = tmp1->next;
  }
  for (size_t i = 0; i < tmp1->occurences; i++) {
    head = insertAlphabetically(head, tmp1->word);
  }
  tmp1 = wfd1; //reset temp
  while(tmp2->next!=NULL){ //create head
    for (size_t i = 0; i < tmp2->occurences; i++) {
      head = insertAlphabetically(head, tmp2->word);
    }
    tmp2 = tmp2->next;
  }
  for (size_t i = 0; i < tmp2->occurences; i++) {
    head = insertAlphabetically(head, tmp2->word);
  }

  printf("%s\n", "MEANF: ");

   updateFrequency(head);

   tmp1 = wfd1;
   tmp2 = wfd2;
   linkedlist_t *headTmp = head;

   while(tmp1->word != NULL){
     if(strcmp(head->word, tmp1->word) == 0){ //the found the right word
       kld1 += (tmp1->frequency * logBase2(tmp1->frequency/head->frequency));
       if (tmp1->next==NULL) {
         break;
       }
       tmp1 = tmp1->next;
       head = head->next;

     }
     else{
       head = head->next;
     }
   }


   head = headTmp;

   while(tmp2->word != NULL){
     if(strcmp(head->word, tmp2->word) == 0){
       kld2 += (tmp2->frequency * logBase2(tmp2->frequency/head->frequency));
       if (tmp1->next==NULL) {
         break;
       }
       tmp2 = tmp2->next;
       head = head->next;
     }
     else{
       head = head->next;
     }
   }

  jsd = sqrt((0.5 * kld1) + (0.5 * kld2));
  return jsd;
}

double logBase2(double x){ //calculates log base 2 of a function
  return log(x) / log(2.0);
}
