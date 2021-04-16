#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct wfdrepo_t{
    char* fName;
    int numTokens;
    linkedlist_t* wfd;
    struct wfdrepo_t *next;
} wfdrepo_t;

void push(node_t * head, char* fName, int numTokens, linkedlist_t* wfd) {
    wfdrepo_t* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (wfdrepo_t *) malloc(sizeof(wfdrepo_t));
    current->next->fName = fName;
    current->next->numTokens = numTokens;
    current->next->wfd = wfd;
    current->next->next = NULL;
}
