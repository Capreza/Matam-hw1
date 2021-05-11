#include "node.h"
#include <stdio.h>
#include <stdlib.h>

struct Node_t
{
    Data data;
    Node next;
};

Node nodeCreate(Data data)
{
    Node node = malloc(sizeof(*node));
    if(node == NULL)
    {
        return NULL;
    }
    
    
    node->next = NULL;
    
    return node;
}

NodeResult nodeDestroy(Node node)
{
    if (node == NULL)
    {
        return NODE_NULL_ARG;
    }
    free(node);
    return NODE_SUCCESS;
}

Node nodeGetNext(Node node)
{
    if(node == NULL)
    {
        return NULL;
    }
    return node->next;
}

NodeResult nodeLink(Node first, Node second)
{
    if ((first == NULL) || (second == NULL))
    {
        return NODE_NULL_ARG;
    }
    first->next = second;
    return NODE_SUCCESS;
}

Data nodeGetData(Node node)
{
    if (node == NULL)
    {
        return NULL;
    }
    return node->data;
}

NodeResult nodeUpdate(Node node, Data data)
{
    if(node==NULL||data==NULL)
    {
        return NODE_NULL_ARG;
    }
    node->data=data;
    return NODE_SUCCESS;
}