#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "queue.c"

void insert_int(Queue *queue, int valueToAdd);
void insert_char(Queue *queue, char valueToAdd);
void insert_float(Queue *queue, float valueToAdd);
void insert_str(Queue *queue, char* valueToAdd);
void print_from_value(basic_type_queue elem, void *arg);
void print_from_pointer(basic_type_queue *elem, void *arg);
void change_value(basic_type_queue *elem, void *arg);

int main() {
    //Declare new queue
    Queue queue;

    //Initialize it. Do not forget this function
    makeNullQueue(&queue);    

    //Insert random value with different type
    for (int i=0; i<5; i++)
        insert_int(&queue, i);
    
    for (float i=10.0; i<10.5; i+=0.1)
        insert_float(&queue, i);

    for (int i=0; i<5; i++)
        insert_char(&queue, 'a' + i);

    insert_str(&queue, "hello queue, i'm a string");

    printf("First function: print all item\n");
    getValueFromQueue(queue, NULL, print_from_value);

    printf("size = %d\n", getSizeQueue(queue));
    
    printf("Let's make some changes, i.e. multiply all integer by 10, leave the rest unchanged\n");
    int multiplicator = 10;     //Define a multiplicator to give to your callback function (change_value)
    getPointerFromQueue(&queue, (void*)&multiplicator, change_value);

    //Then print the queue with new value
    getPointerFromQueue(&queue, NULL, print_from_pointer);
    
    //Now we must free the memory allocated. Have many function to do it:
    //
    //If you would see all item than delete it:
    //getValueFromQueue_rm(queue, NULL, print_from_pointer);
    //getPointerFromQueue_rm(&queue, NULL, print_from_pointer);
    //
    //If you don't want print any item but you would just cleare the queue:
    //clearAllQueue(&queue);

    printf("\n");
    printf("Suppose we would just free memory without print anything, then call 'clearAllQueue'\n");
    clearAllQueue(&queue);

    printf("Note that call now same get[pointer/value]FromQueue(..) return the error message below shown. You can set this message defining in your source file the constant MEX_EMPTY_QUEUE like simple string (BEFORE #include <queue.c> obv)\n");
    getPointerFromQueue_rm(&queue, NULL, print_from_pointer);
    
    exit(EXIT_SUCCESS);
}

void insert_int(Queue *queue, int valueToAdd) {
    basic_type_queue newElem;
    
    int *newData = (int*)malloc(sizeof(int));
    *newData  = valueToAdd;

    newElem.type = INT;
    newElem.data = (void*)newData;
    
    enQueue(queue, newElem);
}

void insert_char(Queue *queue, char valueToAdd) {
    basic_type_queue newElem;
    
    char *newData = (char*)malloc(sizeof(char));
    *newData  = valueToAdd;

    newElem.type = CHAR;
    newElem.data = (void*)newData;
    
    enQueue(queue, newElem);
}

void insert_float(Queue *queue, float valueToAdd) {
    basic_type_queue newElem;
    
    float *newData = (float*)malloc(sizeof(float));
    *newData  = valueToAdd;

    newElem.type = FLOAT;
    newElem.data = (void*)newData;
    
    enQueue(queue, newElem);
}

void insert_str(Queue *queue, char* valueToAdd) {
    basic_type_queue newElem;
    
    char *newData = (char*)malloc(strlen(valueToAdd) + 1);

    for (int i=0; i<strlen(valueToAdd) + 1; i++) newData[i] = '\0';
    strcpy(newData, valueToAdd);

    newElem.type = STRING;
    newElem.data = (void*)newData;
    
    enQueue(queue, newElem);
}

void print_from_value(basic_type_queue elem, void *arg) {
    //char elem = *(char*)elem.data); elem++;   <= local change
    //int elem = *(int*)elem.data); and go on...
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

void print_from_pointer(basic_type_queue *elem, void *arg) {
    //char elem = *(char*)elem->data);  <= global change
    //int elem = *(int*)elem->data); and go on...
    switch (elem->type) {
        case CHAR:
            printf("%c\n", *(char*)elem->data);
            break;
        case INT:
            //(*(int*)elem->data)++;      //<= tyo to uncomment
            printf("++%d\n", *(int*)elem->data);
            break;
        case FLOAT:
            printf("%.2f\n", *(float*)elem->data);
            break;
        case DOUBLE:
            printf("%.4f\n", *(double*)elem->data);
            break;
        case STRING:
            printf("%s\n", (char*)elem->data);
            break;
        default: 
        case VOID_GP:
            printf("%p", (void*)elem->data);
            break;
    }

}

void change_value(basic_type_queue *elem, void *arg) {
    int multiplicator = *(int*)arg;
    if (elem->type == INT) {
        int *val = (int*)elem->data;
        (*val) *= multiplicator;
    }
}
