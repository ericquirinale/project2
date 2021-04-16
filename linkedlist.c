#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct linkedlist_t{
    char* word;
    int occurences;
    double frequency;
    struct linkedlist_t *next;

} linkedlist_t;


void initLinked(linkedlist_t *ll){ //initizialing a linked list
    ll->word = NULL;
    ll->occurences = 1;
    ll->frequency = 0.0;
    ll->next = NULL;
}

void insertAlphabetically(linkedlist_t *ll, char *word) {
    linkedlist_t *current = ll;
    //check here
    linkedlist_t *prev = NULL;
    if (ll->word != NULL) {
        while (current->word != NULL) {
            if(strcmp(current->word, word)==0){
              current->occurences++;
              return;
            }
            if (strcmp(current->word, word)>0) {
                linkedlist_t *new = malloc(sizeof(linkedlist_t));
                initLinked(new);
                new->word = word;
                new->next = current;
                //check here
                if (prev != NULL) {
                    prev->next = new;
                }
                return;
            }
            if(current->next==NULL){
              linkedlist_t *new = malloc(sizeof(linkedlist_t));
              initLinked(new);
              new->word = word;
              current->next = new;
              return;
            }
            //check here
            prev = current;
            current = current->next;
        }
    }
    else{
      ll->word=word;
    }
}

void displayLinked(linkedlist_t *ptr) {
    for (; ptr != NULL; ptr = ptr->next) {
        printf("%s\n", ptr->word);
    }
}

int listCount(linkedlist_t *ptr) {
    int count = 0;
    for (; ptr != NULL; ptr = ptr->next) {
        count += ptr->occurences;
    }
    return count;
}

void updateFrequency(linkedlist_t *ptr) {
    int count = listCount(ptr);
    for (; ptr != NULL; ptr = ptr->next) {
        ptr->frequency = (ptr->occurences/count);
    }
}
