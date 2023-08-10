#include "queue.h"


// Initialize the queue
int init(thread_queue* q) {
    q->num_th = 0;
    q->head = NULL;
    q->tail = NULL;
    return 0;
}

// Enqueue a thread to the end of the queue
void enqueue(thread_queue* q, uthread* t) {
    q_node *new_node = (q_node*)malloc(sizeof(q_node));
    new_node->thread = t;
    new_node->next = NULL;
    if (q->num_th == 0) {
        q->head = new_node;
        q->tail = new_node;
    }
    else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    q->num_th++;
}

// Dequeue the first thread in the queue
uthread* dequeue(thread_queue* q) {
    if (q->num_th == 0) {
        return NULL;
    }
    q_node *node = q->head;
    q->head = node->next;
    if (q->num_th == 1) {
        q->tail = NULL;
    }
    q->num_th--;
    uthread *t = node->thread;
    free(node);
    return t;
}

// Get a thread from the queue by its tid
uthread* get_node(thread_queue* q, uthread_t tid) {
    q_node *node = q->head;
    while (node != NULL) {
        if (node->thread->user_id == tid) {
            // printf("get_node called\n");
            return node->thread;
        }
        node = node->next;
    }
    return NULL;
}
// Get a thread from the queue by its tid
void print_all_threads(thread_queue* q) {
    q_node *node = q->head;
    while (node != NULL) {
        printf("thread userid =%d,thread status =%d\n",node->thread->user_id,node->thread->status);
        node = node->next;
    }

}

// Check if the queue is empty
int isempty(thread_queue *q) {
    return (q->num_th == 0);
}

  
