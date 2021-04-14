#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//linkedList implementations
typedef struct node{
    char *data;
    struct node *next;
} node;

//unbounded queue for file and directory queue
typedef struct {
	node *head;
	node *tail;
	int open;
	int count;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
	//pthread_cond_t write_ready;
} queue_t;

int isempty(queue_t *q);

int init(queue_t *Q)
{
	Q->count = 0;
	Q->head = NULL;
	Q->tail = NULL;
	Q->open = 1;
	pthread_mutex_init(&Q->lock, NULL);
	pthread_cond_init(&Q->read_ready, NULL);
	//pthread_cond_init(&Q->write_ready, NULL);

	return 0;
}

int destroy(queue_t *Q)
{
	pthread_mutex_destroy(&Q->lock);
	pthread_cond_destroy(&Q->read_ready);
	//pthread_cond_destroy(&Q->write_ready);

	return 0;
}


// add item to end of queue
// if the queue is full, block until space becomes available
int enqueue(queue_t *Q, char *item){
	pthread_mutex_lock(&Q->lock);

	/*while (Q->count == QSIZE && Q->open) {
		pthread_cond_wait(&Q->write_ready, &Q->lock);
	}*/
	if (!Q->open) {
		pthread_mutex_unlock(&Q->lock);
		return -1;
	}

	//unsigned i = Q->head + Q->count;
	//if (i >= QSIZE) i -= QSIZE;

	node *tmp;
	tmp = malloc(sizeof(node));
	tmp->data = item;
	tmp->next = NULL;
	if(!isempty(Q)){
		Q->tail->next = tmp;
		Q->tail = tmp;
	}
	else{
		Q->head = Q->tail = tmp;
	}

	Q->count++;

	pthread_cond_signal(&Q->read_ready);

	pthread_mutex_unlock(&Q->lock);

	return 0;
}


char *dequeue(queue_t *Q)
{

	pthread_mutex_lock(&Q->lock);

	while (Q->count == 0 && Q->open) {
		pthread_cond_wait(&Q->read_ready, &Q->lock);
	}

	if (Q->count == 0) {
		pthread_mutex_unlock(&Q->lock);
		return NULL;
	}

	node *tmp;
  char *item = Q->head->data;
  tmp = Q->head;
  Q->head = Q->head->next;
  Q->count--;
  free(tmp);

	/**item = *Q->data[Q->head]; //causing segfault
	--Q->count;
	++Q->head;
	if (Q->head == QSIZE) Q->head = 0;

	//pthread_cond_signal(&Q->write_ready);*/

	pthread_mutex_unlock(&Q->lock);

	return item;
}

int isempty(queue_t *q)
{
    return (q->tail == NULL);
}

int qclose(queue_t *Q)
{
	pthread_mutex_lock(&Q->lock);
	Q->open = 0;
	pthread_cond_broadcast(&Q->read_ready);
	//pthread_cond_broadcast(&Q->write_ready);
	pthread_mutex_unlock(&Q->lock);

	return 0;
}
