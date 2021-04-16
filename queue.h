#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct node{
    char *data;
    struct node *next;
} node;

typedef struct {
	node *head;;
	node *tail;
  int activeThreads;
	int open;
	int count;
  int done;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
	//pthread_cond_t write_ready;
} queue_t;

int init(queue_t *Q);
int destroy(queue_t *Q);
int enqueue(queue_t *Q, char *item);
char *dequeue(queue_t *Q);
int qclose(queue_t *Q);
int isempty(queue_t *q);
void display(node *head);
//void Queue_done(queue_t *Q);

#endif
