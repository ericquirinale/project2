#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct jsdlist_t{
    char* fpath1;
    char* fpath2;
    int combinedCount;
    double jsd;
    struct jsdlist_t *next;
    struct jsdlist_t *head;

} jsdlist_t;


void initJSD(jsdlist_t *list){ //initizialing a linked list
    list->fpath1 = NULL;
    list->fpath2 = NULL;
    list->combinedCount = 0;
    list->jsd = 0.0;
    list->next = NULL;
    list->head = list;
}

jsdlist_t *insertJsd(jsdlist_t *list, char* fpath1, char* fpath2, int count1, int count2, double jsd) { //returns pointer to head of list
  jsdlist_t *current = list->head;
  if (current->fpath1!=NULL) { //if not empty
    jsdlist_t *new = malloc(sizeof(jsdlist_t));
    initJSD(new);
    jsdlist_t *prev;
    char *fData1 = malloc(strlen(fpath1)+1);
    char *fData2 = malloc(strlen(fpath2)+1);
    strcpy(fData1, fpath1);
    strcpy(fData2, fpath2);
    new->fpath1=fData1;
    new->fpath2=fData2;
    new->combinedCount=count1+count2;
    new->jsd=jsd;
    int isHead = 1;
    while(current!=NULL){ //insert in decreasing order pf combinedCount
      if (current->combinedCount < new->combinedCount) {
        if(isHead==1){ //insert in front
          new->next = list->head;
          list->head = new;
        }
        else{
          new->next=current;
          prev->next=new;
        }
        return list->head;
      }
      if(current->next==NULL){
        current->next = new;
        return list->head;
      }
      isHead=0;
      prev = current;
      current=current->next;
    }
    return list->head;
  }
  else{ // if empty
    current->fpath1 = fpath1;
    current->fpath2 = fpath2;
    current->combinedCount = count1+count2;
    current->jsd = jsd;
    return list->head;
  }
}

void displayJSD(jsdlist_t *ptr) {
    for (; ptr != NULL; ptr = ptr->next) {
        printf("%f\t%s\t%s\n", ptr->jsd, ptr->fpath1, ptr->fpath2);
    }
}
