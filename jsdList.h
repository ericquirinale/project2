#ifndef JSDLIST_H_
#define JSDLIST_H_

typedef struct jsdlist_t{
    char* fpath1;
    char* fpath2;
    int combinedCount;
    double jsd;
    struct jsdlist_t *next;

} jsdlist_t;

void initJSD(jsdlist_t *list);
jsdlist_t *insertJsd(jsdlist_t *head, char* fpath1, char* fpath2, int count1, int count2, double jsd);
void displayJSD(jsdlist_t *ptr);

#endif
