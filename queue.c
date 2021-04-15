#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//linkedList implementations
typedef struct node{
    char *data;
    struct node *next;
} node;

extern int directoryThreads;
//unbounded queue for file and directory queue
typedef struct {
	node *head;;
	node *tail;
  int activeThreads;
	int open;
	int count;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
	//pthread_cond_t write_ready;
} queue_t;

int isempty(queue_t *q);
void display(node *head);

int init(queue_t *Q)
{
	Q->count = 0;
	Q->head = NULL;
	Q->tail = NULL;
	Q->open = 1;
  Q->activeThreads = directoryThreads;
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

  printf("%s", "Enqueued: ");
  display(head->data);

	return 0;
}


char *dequeue(queue_t *Q)
{
	pthread_mutex_lock(&Q->lock); //lock queue

  if(isempty(Q)){
    Q->activeThreads--;
    if(Q->activeThreads == 0){
      pthread_mutex_unlock(&Q->lock);
  		return NULL;
    }
    while (isempty(Q) && Q->activeThreads>0) {
  		pthread_cond_wait(&Q->read_ready, &Q->lock);
  	}
    if (isempty(Q)){
  		pthread_mutex_unlock(&Q->lock);
  		return NULL;
  	}
    Q->activeThreads++;
  }

  printf("%s", "Dequeued: ");
  display(Q->head);
  char *item = Q->head->data; //segfault here

  if(Q->count>1){
    Q->head = Q->head->next;
  }
  else{
    Q->head = NULL;
  }
  Q->count--;

	pthread_mutex_unlock(&Q->lock);

	return item;
}

int isempty(queue_t *q)
{
    return (q->tail == NULL);
}

void display(node *head)
{
    if(head == NULL)
    {
        printf("NULL\n");
    }
    else
    {
        printf("%s\t", head->data);
        display(head->next);
    }
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
