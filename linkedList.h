#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct linkedlist_t{
    char* word;
    int occurences;
    double frequency;
    struct linkedlist_t *next;

} linkedlist_t;

#endif
