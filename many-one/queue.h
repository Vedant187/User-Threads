#include "uthread.h"


//structure of node for queue
typedef struct q_node{
    struct uthread* thread;
    struct q_node* next;
}q_node;

//structure for global queue of threads
typedef struct thread_queue{
    int num_th;
    q_node *head;
    q_node *tail;
}thread_queue;


// queue prototypes
int init(thread_queue* q);
void enqueue(thread_queue* q, uthread* t);
uthread* dequeue(thread_queue* q);
uthread* get_node(thread_queue* q, uthread_t tid);
int isempty(thread_queue *q);
void print_all_threads(thread_queue *q);
// uthread *get_custom_node(thread_queue *q, pid_t tid);
