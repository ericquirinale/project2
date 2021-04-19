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

} jsdlist_t;


void initJSD(jsdlist_t *list){ //initizialing a linked list
    list->fpath1 = NULL;
    list->fpath2 = NULL;
    list->combinedCount = 0;
    list->jsd = 0.0;
    list->next = NULL;
}

jsdlist_t *insertJsd(jsdlist_t *head, char* fpath1, char* fpath2, int count1, int count2, double jsd) { //returns pointer to head of list
  jsdlist_t *current = head;
  if (current->fpath1!=NULL) { //if not empty
    jsdlist_t *new;
    jsdlist_t *prev;
    new = malloc(sizeof(jsdlist_t));
    initJSD(new);
    current->fpath1 = fpath1;
    current->fpath2 = fpath2;
    current->combinedCount = count1+count2;
    current->jsd = jsd;
    while(current->combinedCount > new->combinedCount && current->next!=NULL){ //insert in decreasing order pf combinedCount
      prev = current;
      current=current->next;
    }
    new->next = current;
    prev->next=new;
    return new;
  }
  else{ // if empty
    current->fpath1 = fpath1;
    current->fpath2 = fpath2;
    current->combinedCount = count1+count2;
    current->jsd = jsd;
    return current;
  }
}

void displayJSD(jsdlist_t *ptr) {
    for (; ptr != NULL; ptr = ptr->next) {
        printf("%f\t%s\t%s\n", ptr->jsd, ptr->fpath1, ptr->fpath2);
    }
}
