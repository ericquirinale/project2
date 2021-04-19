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
    struct linkedlist_t *head;

} linkedlist_t;

void initLinked(linkedlist_t *ll);
linkedlist_t *insertAlphabetically(linkedlist_t *ll, char *word);
void displayLinked(linkedlist_t *ptr);
int listCount(linkedlist_t *ptr);
void updateFrequency(linkedlist_t *ptr);

int main(int argc, char const *argv[]) {
  linkedlist_t *list = malloc(sizeof(linkedlist_t));
  initLinked(list);
  insertAlphabetically(list, "zebra");
  insertAlphabetically(list, "apple");
  insertAlphabetically(list, "dog");
  insertAlphabetically(list, "cat");
  insertAlphabetically(list, "dandellion");
  displayLinked(list);
  return 0;
}

void initLinked(linkedlist_t *ll){ //initizialing a linked list
    ll->word = NULL;
    ll->occurences = 1;
    ll->frequency = 0.0;
    ll->next = NULL;
    ll->head = ll;
}

linkedlist_t *insertAlphabetically(linkedlist_t *ll, char *word) {
    linkedlist_t *current = ll;
    //check here
    linkedlist_t *prev = NULL;
    if (ll->word != NULL) {
        int isHead = 1;
        while (current->word != NULL) {
            if(strcmp(current->word, word)==0){
              current->occurences++;
              return ll->head;
            }
            if (strcmp(current->word, word)>0){
                linkedlist_t *new = malloc(sizeof(linkedlist_t));
                initLinked(new);
                char *tmpData = malloc(strlen(word)+1);
                strcpy(tmpData, word);
                new->word=tmpData;
                new->next = current;
                if(isHead==1){
                  ll->head = new;
                }
                //check here
                if (prev != NULL) {
                    prev->next = new;
                }
                return ll->head;
            }
            if(current->next==NULL){
              linkedlist_t *new = malloc(sizeof(linkedlist_t));
              initLinked(new);
              char *tmpData = malloc(strlen(word)+1);
              strcpy(tmpData, word);
              new->word=tmpData;
              current->next = new;
              return ll->head;
            }
            //check here
            prev = current;
            current = current->next;
            isHead=0;
        }

    }
    else{
      char *tmpData = malloc(strlen(word)+1);
      strcpy(tmpData, word);
      ll->word=tmpData;
      return ll->head;
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
