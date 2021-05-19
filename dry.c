
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t 
{
 int x;
 struct node_t *next;
} *Node;

typedef enum 
{
 SUCCESS=0,
 MEMORY_ERROR,
 UNSORTED_LIST,
 NULL_ARGUMENT,
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code);

Node copyNode(Node node);
Node lastNodeInList(Node list);
void destroyList(Node list);
bool listsValid(Node list1, Node list2, ErrorCode error_code);
int minNode(Node list1, Node list2);



Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code)
{
    if(listsValid(list1,list2,error_code) == false)
    {
        return NULL;
    }
    int total_list_size = getListLength(list1)+getListLength(list2);
    Node head_node=NULL;
    Node curr_node_from_1 = list1;
    Node curr_node_from_2 = list2;
    if(list1->x >= list2->x)
    {
        head_node = copyNode(list1);
        curr_node_from_1= curr_node_from_1->next;
    }
    else
    {
        head_node = copyNode(list2);
        curr_node_from_2= curr_node_from_2->next;
    }
    if(head_node==NULL)
    {
        *error_code = MEMORY_ERROR;
        return NULL;
    }
    for(int i = 0; i<total_list_size-1 ; i++)
    {
        Node next_node_in_new_list = malloc(sizeof(*next_node_in_new_list));
        if(next_node_in_new_list == NULL)
        {
            destroyList(head_node);
            *error_code=MEMORY_ERROR;
            return NULL;
        }
        int min_node = minNode(curr_node_from_1,curr_node_from_2);
        if(min_node = 0 )
        {
            lastNodeInList(head_node)->next = curr_node_from_1;
            curr_node_from_1 = curr_node_from_1->next;
        }
        else
        {
            lastNodeInList(head_node)->next = curr_node_from_2;
            curr_node_from_2 = curr_node_from_2->next;
        }
    }
    *error_code = SUCCESS;
    return head_node;

}


void destroyList(Node list)
{
    while(list->next!=NULL)
    {
        Node temp = list;
        list = list->next;
        free(temp);
    }
}


Node copyNode(Node node)
{
    Node copy = malloc(sizeof(*copy));
    if(copy == NULL)
    {
        return NULL;
    }
    copy->x = node->x;
    return copy;
}


Node lastNodeInList(Node list)
{
    while(list->next!=NULL)
    {
        list = list->next;
    }
    return list;
}


int minNode(Node list1, Node list2)  
{
    if (list1 ==NULL)
    {
        return 1;
    }
    if (list2 == NULL)
    {
        return 0;
    }
    if (list1->x<list2->x)
    {
        return 0;
    }
    return 1;
}


bool listsValid(Node list1, Node list2, ErrorCode* error_code)
{
    if(list1==NULL||list2==NULL)
    {
        *error_code= NULL_ARGUMENT;
        return false;
    }
    if (isListSorted(list1)==false||isListSorted(list2)==false)
    {
        *error_code==UNSORTED_LIST;
        return false;
    }
    return true;
    
}



/*
Node getCurrentNode(Node list, int node_index);
bool listsValid(Node list1, Node list2, ErrorCode* error_code);
int minNode(Node list1, Node list2);
Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code)           
{
    if(listsValid(list1,list2,error_code)==false)
    {
        return NULL;
    }                                                                          
    int list_tracker = minNode(list1,list2);
    Node return_node = NULL;
    if(list_tracker==0)
    {
        return_node = list1;
    }
    else
    {
        return_node = list2;
    }
    Node current_node_from_list1 = list1;
    Node current_node_from_list2 = list2;
    for (int i=0;i<(getListLength(list1)+getListLength(list2));i++)
    {
        if(list_tracker==0)
        {
            list_tracker=minNode(current_node_from_list1->next,current_node_from_list2);
            if(list_tracker==0)
            {
                current_node_from_list1=current_node_from_list1->next;
            }
            else
            {
                current_node_from_list1->next=current_node_from_list2;
                current_node_from_list2=current_node_from_list2->next;
            }
        }
        else 
        {
            list_tracker=minNode(current_node_from_list1->next,current_node_from_list2);
            if(list_tracker==0)
            {
                current_node_from_list2->next=current_node_from_list1;
                current_node_from_list2=current_node_from_list2->next;
            }
            else
            {
                current_node_from_list2=current_node_from_list2->next;
            }
        }
    }
    *error_code = SUCCESS;
    return return_node;
}
*/
