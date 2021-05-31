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

Node createNode(int x);
void destroyList(Node ptr);
Node advanceList(Node new_list, Node list);
Node mergeLists(Node list1, Node list2, Node new_list);
bool listsValid(Node list1, Node list2, ErrorCode error_code);


Node createNode(int x)
{
     Node new = malloc(sizeof(*new));
     if (new == NULL)
     {
          return NULL;
     }
     new->x = x;
     new->next = NULL;
     return new;
}


bool listsValid(Node list1, Node list2, ErrorCode* error_code)
{
    if (list1 == NULL || list2 == NULL)
    {
        *error_code = NULL_ARGUMENT;
        return false;
    }
    if (isListSorted(list1) == false || isListSorted(list2) == false)
    {
        *error_code = UNSORTED_LIST;
        return false;
    }
    return true;
}


void destroyList(Node ptr)
{
     while(ptr)
     {
          Node to_delete = ptr;
          ptr = ptr->next;
          free(to_delete);
     }
}

Node advanceList(Node new_list, int x)
{
     Node new_node = createNode(x);
     if (new_node == NULL) 
     {
          return NULL;
     }
     new_list->next = new_node;
     return new_node;
}

Node mergeLists(Node list1, Node list2)
{
     bool list_started = false;
     int num_to_add;
     Node result_ptr = NULL, result_list = NULL;
     while((list1 != NULL) && (list2 != NULL)) 
     {
          if (list1->x < list2->x)
          {
               num_to_add = list1->x;
               list1 = list1->next;
          }
          else
          {
               num_to_add = list2->x;
               list2 = list2->next;
          }
          
          if (list_started == false)
          {
               result_list = createNode(num_to_add);
               result_ptr = result_list;
          }
          else
          {
               result_ptr = advanceList(result_ptr, num_to_add);
          }

          if (result_ptr == NULL)
          {
               destroyList(result_list);
               return NULL;
          }
     }
     while (list1 != NULL)
     {
         result_ptr = advanceList(result_ptr, list1->x);
          if (result_ptr == NULL)
          {
               destroyList(result_list);
               return NULL;
          }
     }
     while (list2 != NULL)
     {
          result_ptr = advanceList(result_ptr, list2->x);
          if (result_ptr == NULL)
          {
               destroyList(result_list);
               return NULL;
          }
     }
     return result_list;
}

Node mergeSortedLists(Node list1, Node list2, ErrorCode* error_code)
{
     if (listsValid(list1, list2, error_code) == false)
     {
          return NULL;
     }
     Node ptr1 = list1, ptr2 = list2;
     Node result_list = mergeLists(ptr1, ptr2);
     if (result_list == NULL)
     {
          error_code = MEMORY_ERROR;
          return NULL;
     }
     error_code = SUCCESS;
     return result_list;
}
