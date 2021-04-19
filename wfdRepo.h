#ifndef WFDREPO_H_
#define WFDREPO_H_

typedef struct wfdrepo_t{
    char *fName;
    int numTokens;
    linkedlist_t *wfd;
    struct wfdrepo_t *next;
} wfdrepo_t;

void insertRepo(wfdrepo_t *head, char* fName, linkedlist_t* wfd);
void initRepo(wfdrepo_t *repo);
void displayWFD(wfdrepo_t *ptr);
int repoSize(wfdrepo_t *repo);

#endif
