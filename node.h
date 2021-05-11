#ifndef NODE_H_
#define NODE_H_
#include <stdlib.h>

typedef struct Node_t* Node;

typedef enum 
{
    NODE_SUCCESS =0,
    NODE_NULL_ARG
} NodeResult;

typedef void *Data;


Node nodeCreate(Data data);

Node nodeGetNext(Node node);

Data nodeGetData(Node node);

NodeResult nodeDestroy(Node node);

NodeResult nodeLink(Node first, Node second);

NodeResult nodeUpdate(Node node, Data data);

Data nodeGetData(Node node);


#endif 
