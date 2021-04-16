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
       // printf("%s\n", "hi22");
    ll->word = NULL;
    ll->occurences = 1;
    ll->frequency = 0.0;
    ll->next = NULL;
     // printf("%s\n", "hi33");
}

void insertAlphabetically(linkedlist_t *ll, char *word) {
    if(ll->word==NULL){
      ll->word=word;
    }
    else{
      for (; ll!=NULL; ll=ll->next) {
        if(strcmp(ll->word, word)==0){
          ll->occurences++;
          break;
        }
        else if (strcmp(ll->next->word, word)<0||ll->next==NULL) {
          linkedlist_t *new = malloc(sizeof(linkedlist_t));
          new->word = word;
          new->next = ll->next;
          ll->next = new;
          break;
        }
      }
    }
}

void displayLinked(linkedlist_t *ptr) {

    for (; ptr != NULL; ptr = ptr->next) {
        printf("%s\n", ptr->word);
    }
}

int main(void) {
   // printf("%s\n", "hi");
    linkedlist_t *root = malloc(sizeof(linkedlist_t));
    initLinked(root);

    insertAlphabetically(root, "apple");
    insertAlphabetically(root, "cat");
    insertAlphabetically(root, "bananna");
    insertAlphabetically(root, "cat");

    displayLinked(root);

    return 0;
}
