#include<stdlib.h>

#include "list.h"

typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy
#define queue_size list_size
#define queue_print list_print
int queue_enqueue(Queue *queue, const void *data);
int queue_dequeue(Queue *queue, const void **data);
void destroy(void *data);
