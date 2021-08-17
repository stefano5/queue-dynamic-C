#ifndef _QUEUE_C
#define _QUEUE_C

#include <stdio.h>
#include <stdlib.h>

#ifndef _LIST_C

#define CHAR        0
#define INT         1
#define FLOAT       2
#define DOUBLE      3
#define STRING      4
#define VOID_GP     5

#define TRUE        1
#define FALSE       0

#endif

#ifndef MEX_EMPTY_QUEUE
    #define MEX_EMPTY_QUEUE      "Empty queue\n"
#endif

#define CHECK_IF_IS_EMPTY   if (q->front == NULL) { printf(MEX_EMPTY_QUEUE); return; }
#define CHECK_IF_IS_EMPTY_  if (q.front == NULL) { printf(MEX_EMPTY_QUEUE); return; }

//Queue structure
typedef struct {
    void *data;
    uint8_t type;
} basic_type_queue;

typedef struct node {
	basic_type_queue info;
	struct node *next;
} node_queue;

typedef struct {
	node_queue *front, *rear;
    uint16_t size;                  // you don't need to increase it
} Queue;


//###################################################
//                      Queue standard function
//###################################################
void makeNullQueue(Queue *q) {
	q->front = q->rear = NULL;
    q->size=0;
}

int emptyQueue(Queue q) {
	return q.front == NULL;
}

void enQueue(Queue *q, basic_type_queue elem) {
	node_queue *temp;
	temp = (node_queue*)malloc(sizeof(node_queue));
    q->size++;
	if (temp != NULL) {
		temp->info = elem;
		temp->next = NULL;
		if (q->front == NULL) {
            q->front = q->rear = temp;
        } else {
			q->rear->next = temp;
			q->rear = temp;
		}
	}
}

void deQueue(Queue *q) {
    node_queue *temp;
    CHECK_IF_IS_EMPTY;
    q->size--;
    temp = q->front->next;
    if (q->front->info.data != NULL)
        free(q->front->info.data);
    free(q->front);
    q->front = temp;
    if (q->front == NULL) 
        q->rear = NULL;
}

//###################################################
//                  Implemented function
//###################################################


int getSizeQueue(Queue q) {
    return q.size;
}


/*
 * this function don't remove item from queue, it just will visit your data
 * */
void getNextElem(Queue *q) {
    node_queue *temp;
    CHECK_IF_IS_EMPTY;

    temp = q->front->next;
    q->front = temp;
    if (q->front == NULL) 
        q->rear = NULL;
}

/*
 * Remove all item from the list
 * */
void clearAllQueue(Queue *q) {
    CHECK_IF_IS_EMPTY;

    while (!emptyQueue(*q))
        deQueue(q);
}

/*
 * Get front from the queue. Front must exist, you must check it before call this function
 *
 * */
basic_type_queue* getFront(Queue q) {
    if (!emptyQueue(q)) 
        return &q.front->info;
    fprintf(stderr, "FATAL ERROR - CHECK EMPTY QUEUE BEFOR CALL 'getFront(Queue q) FUNCTION. library: %s - line: %d \n", __FILE__, __LINE__);
    fprintf(stderr, "ABORT\n");
    exit(EXIT_FAILURE);
}

/*
 * Get stored data from the queue's front
 * This function is better (safer) than 'getFront'
 *
 * */
void* getFrontData(Queue q) {
    if (emptyQueue(q)) {
        printf(MEX_EMPTY_QUEUE);
        return NULL;
    }
   return (void*)q.front->info.data;
}

/**
 * Print stored data. If 'basic_type_queue' is a primitive type, you won't need to know which is it
 * If 'basic_type_queue' isn't a primitive type will be print its pointer
 *
 * */
void print_data_default_type_queue(basic_type_queue elem) {
    switch (elem.type) {
        case CHAR:
            printf("%c\n", *(char*)elem.data);
            break;
        case INT:
            printf("%d\n", *(int*)elem.data);
            break;
        case FLOAT:
            printf("%.2f\n", *(float*)elem.data);
            break;
        case DOUBLE:
            printf("%.4f\n", *(double*)elem.data);
            break;
        case STRING:
            printf("%s\n", (char*)elem.data);
            break;
        default: 
        case VOID_GP:
            printf("%p", (void*)elem.data);
            break;
    }
}

/*
 * Print all item of the queue, or its pointers
 * */
void printQueue(Queue *q) {
    CHECK_IF_IS_EMPTY;

    while (!emptyQueue(*q)) {
        print_data_default_type_queue(*getFront(*q));
        deQueue(q);
    }
}


//###################################################
//                  High level function. Use callback function
//###################################################

/*
 * This function allow you to get value from queue then remove it from queue
 * */
void getValueFromQueue_rm(Queue *q, void *arg, void(*genericFunction)  (basic_type_queue, void*)) {
    CHECK_IF_IS_EMPTY;

    while (!emptyQueue(*q)) {
        genericFunction(*getFront(*q), arg);
        deQueue(q);
    }
}


/*
 * This function allow you to get pointer from queue than remove it from queue
 * NB: after called this function the queue will be clear (safety)
 * */
void getPointerFromQueue_rm(Queue *q, void *arg, void(*genericFunction) (basic_type_queue *, void*)) {
    CHECK_IF_IS_EMPTY;

    while (!emptyQueue(*q)) {
        genericFunction(getFront(*q), arg);
        deQueue(q);
    }
}


/*
 * This function allow you to scan the queue without remove anything from it.
 * It needs 'clearAllQueue' function before closing your app to avoid leaks
 * */
void getValueFromQueue(Queue q, void *arg, void(*genericFunction)(basic_type_queue, void*)) {
    CHECK_IF_IS_EMPTY_; //_ because here you have q, not *q

    while (!emptyQueue(q)) {
        genericFunction(*getFront(q), arg);
        getNextElem(&q);
    }
}

/*
 * This function allow you to get pointer from queue, but that function don't remove nothing
 * You must call clearAllQueue function in order to clear the memory
 *
 * */
void getPointerFromQueue(Queue *q, void *arg, void(*genericFunction)(basic_type_queue*, void*)) {
    CHECK_IF_IS_EMPTY;
    node_queue *temp = q->front;
    while (!emptyQueue(*q)) {
        genericFunction(getFront(*q), arg);
        getNextElem(q);
    }
    q->front = temp;
}


#endif
