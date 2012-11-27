#include<stdlib.h>
#include<stdio.h>

#include "queue.h"
int main(){
    int *x;
    int *y;
    Queue *queue;
    queue = (Queue *) malloc(sizeof(Queue));
    queue_init(queue, destroy);
    x = malloc(1);
    *x = 100;
    queue_enqueue(queue, (const void *) x);
    queue_print(queue);

    y = malloc(1);
    *y = 600;

    queue_enqueue(queue, (const void *) y);
    queue_print(queue);

    x = malloc(1);
    *x = 200;

    queue_enqueue(queue, (const void *) x);
    queue_print(queue);
    x = malloc(1);
    *x = 300;
    queue_enqueue(queue, (const void *) x);
    queue_print(queue);
    
    queue_dequeue(queue, (const void **) &x);
    queue_print(queue);

  
    printf("head: %d\n", *x);
    queue_dequeue(queue, (const void **) &x);
    queue_print(queue);
    printf("head: %d\n", *x);
    printf("size before destroy: %d\n", queue_size(queue));
    queue_destroy(queue);
    printf("size after destroy: %d\n", queue_size(queue));
}


