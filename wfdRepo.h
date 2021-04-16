#ifndef WFDREPO_H_
#define WFDREPO_H_

typedef struct wfdrepo_t{
    char *fName;
    int numTokens;
    linkedlist_t *wfd;
    struct wfdrepo_t *next;
} wfdrepo_t;

wfdrepo_t *insertRepo(wfdrepo_t *head, char* fName, linkedlist_t* wfd);
void initRepo(wfdrepo_t *repo);

#endif
