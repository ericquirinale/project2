#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct {
	char *data;
	unsigned count;
	unsigned head;
	int open;
	pthread_mutex_t lock;
	pthread_cond_t read_ready;
	//pthread_cond_t write_ready;
} queue_t;

int init(queue_t *Q);
int destroy(queue_t *Q);
int enqueue(queue_t *Q, char *item);
int dequeue(queue_t *Q, char *item);
int qclose(queue_t *Q);

#endif
