#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct linkedlist_t{
    char* word;
    int occurences;
    double frequency;
    struct linkedlist_t *next;

} linkedlist_t;

void initLinked(linkedlist_t *ll);
linkedlist_t *insertAlphabetically(linkedlist_t *ll, char *word);
void displayLinked(linkedlist_t *ptr);
int listCount(linkedlist_t *ptr);
void updateFrequency(linkedlist_t *ptr);

#endif
