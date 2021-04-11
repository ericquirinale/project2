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

//all optional arguments required before regular args and at most once
void collectionPhase(){
  //set optionl arguments
  int i = 0;
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
  }

}
