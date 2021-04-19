#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "linkedList.h"

typedef struct wfdrepo_t{
    char *fName;
    int numTokens;
    linkedlist_t *wfd;
    struct wfdrepo_t *next;
} wfdrepo_t;

void initRepo(wfdrepo_t *repo);
wfdrepo_t *insertRepo(wfdrepo_t *head, char* fName, linkedlist_t* wfd);
int repoSize(wfdrepo_t *repo);

wfdrepo_t *insertRepo(wfdrepo_t *head, char* fName, linkedlist_t* wfd) { //returns pointer to head of list
  wfdrepo_t *current = head;
  linkedlist_t *tmpWFD = malloc(sizeof(linkedlist_t));
  tmpWFD = wfd;
  char *tmpfName = malloc(strlen(fName)+1);
  strcpy(tmpfName, fName);
  if (current->fName!=NULL) {
    wfdrepo_t *new;
    new = malloc(sizeof(wfdrepo_t));
    initRepo(new);

    new->fName = tmpfName;
    new->numTokens = listCount(wfd);
    new->wfd = tmpWFD;
    new->next = current;
    return new;
  }
  else{
    current->fName = tmpfName;
    current->numTokens = listCount(wfd);
    current->wfd = tmpWFD;
    return current;
  }
}

void initRepo(wfdrepo_t *repo){
  repo->fName=NULL;
  repo->numTokens=0;
  repo->wfd = NULL;
  repo->next=NULL;
}

int repoSize(wfdrepo_t *repo){
  int size = 1;
  while (repo->next!=NULL) {
    size++;
    repo = repo->next;
  }
  return size;
}
