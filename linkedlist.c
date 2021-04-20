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

void initLinked(linkedlist_t *ll);
linkedlist_t *insertAlphabetically(linkedlist_t *ll, char *word, int occurences);
void displayLinked(linkedlist_t *ptr);
int listCount(linkedlist_t *ptr);
void updateFrequency(linkedlist_t *ptr);

/*int main(int argc, char const *argv[]) {
  linkedlist_t *list = malloc(sizeof(linkedlist_t));
  initLinked(list);
  insertAlphabetically(list->head, "zebra");
  insertAlphabetically(list->head, "apple");
  insertAlphabetically(list->head, "dog");
  insertAlphabetically(list->head, "cat");
  insertAlphabetically(list->head, "dandellion");
  displayLinked(list->head);
  return 0;
}*/

void initLinked(linkedlist_t *ll){ //initizialing a linked list
    ll->word = NULL;
    ll->occurences = 1;
    ll->frequency = 0.0;
    ll->next = NULL;
}

linkedlist_t *insertAlphabetically(linkedlist_t *ll, char *word, int occurences) {
    //check here
    linkedlist_t *current = ll;
    linkedlist_t *prev = NULL;
    if (ll->word != NULL) {
        int isHead = 1;
        while (current->word != NULL) {
            if(strcmp(current->word, word)==0){ //update occurences
              current->occurences+=occurences;
              return ll;
            }
            //create new node
            linkedlist_t *new = malloc(sizeof(linkedlist_t));
            initLinked(new);
            char *tmpData = malloc(strlen(word)+1);
            strcpy(tmpData, word);
            new->word=tmpData;
            if (strcmp(current->word, word)>0){ //insert alphabetically
                if(isHead==1){ //insert in front
                  new->next = ll;
                  return new;
                }
                else{
                  new->next=current;
                  prev->next=new;
                }
                return ll;
            }
            if(current->next==NULL){ //insert at the end
              current->next = new;
              return ll;
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
      return ll;
    }
    return ll;
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
        ptr->frequency = ((double)ptr->occurences/count);
        printf("%s\t%f\n", ptr->word, ptr->frequency);
    }
}
