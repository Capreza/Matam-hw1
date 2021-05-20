#include "map.h"
#include "node.h"
#include <assert.h>


struct Map_t 
{
    Node head;
    int size;

    copyMapDataElements copy_data_function;
    copyMapKeyElements copy_key_function;
    freeMapDataElements free_data_function;
    freeMapKeyElements free_key_function;
    compareMapKeyElements compare_key_function;

    MapKeyElement iterator;
};

typedef struct map_entry
{
    MapDataElement data_element;
    MapKeyElement key_element;
}*MapEntry;



/** Creates a new map entry with copies of the given data. Returns NULL in case of memory error */
static MapEntry mapEntryCreate(Map map, MapKeyElement key_element, MapDataElement data_element)
{
    MapEntry new_entry = malloc(sizeof(*new_entry));
    if (new_entry == NULL)
    {
        return NULL;
    }
    new_entry->key_element = map->copy_key_function(key_element);
     if(new_entry->key_element == NULL)
    {
        free(new_entry);
        return NULL;
    }
    new_entry->data_element = map->copy_data_function(data_element);
    if(new_entry->data_element == NULL)
    {
        map->free_key_function(new_entry->key_element);
        free(new_entry);
        return NULL;
    }
    return new_entry;
}


/** Destroys a map entry and returns the appropriate enum result */
static MapResult mapEntryDestroy(Map map, MapEntry map_entry)               
{
    if (map_entry == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    assert(map_entry->data_element != NULL && map_entry->key_element != NULL);
    map->free_data_function(map_entry->data_element);   
    map->free_key_function(map_entry->key_element);
    free(map_entry);
    return MAP_SUCCESS;
}


/** Changes an existing map entry using copies of the given data and returns the appropriate enum result */
static MapResult mapEntrychange(Map map, MapEntry map_entry, MapDataElement data_element)
{
    if (map_entry == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    MapDataElement temp =map_entry->data_element; 
    map_entry->data_element = map->copy_data_function(data_element);
    free(temp);
    return MAP_SUCCESS;
}


/** Creates a new map entry and inserts it in the appropriate placement, returns enum result */
static MapResult mapKeyAndDataInsert(MapKeyElement key_element, MapDataElement data_element, Node node, Map map)
{
    MapEntry new_entry = mapEntryCreate(map, key_element, data_element);
    if (new_entry == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    Node new_node = nodeCreate(new_entry);
    if (new_node == NULL)
    {
        mapEntryDestroy(map, new_entry);
        return MAP_OUT_OF_MEMORY;
    }
    
    Node next_node = nodeGetNext(node);
    nodeLink(new_node, next_node);
    nodeLink(node, new_node);
    map->size++;
    return MAP_SUCCESS;
}


/** Returns the node before the node containing the given key, or NULL if the key does not exist in the map */
static Node mapFindKey(Map map, MapKeyElement key_element)
{
    Node curr_node = map->head;
    MapEntry curr_entry = NULL;
    while (nodeGetNext(curr_node) != NULL)  
    {
        curr_entry = nodeGetData(nodeGetNext(curr_node));
        if (map->compare_key_function(curr_entry->key_element, key_element) == 0)
        {
            return curr_node;
        }
        curr_node = nodeGetNext(curr_node);
    }
    return NULL;
}


static void listDestroy(Map map, Node head)  //MAYBE WILL CHANGE NAME OF FUNC
{
    Node curr_node=head, next_node = nodeGetNext(curr_node);
    while(curr_node!=NULL)
    {
        mapEntryDestroy(map, nodeGetData(curr_node));
        free(curr_node);
        curr_node = next_node;
        next_node = nodeGetNext(curr_node);
    }
}

void mapDestroy(Map map)
{
    if(map==NULL)
    {
        return;
    }
    listDestroy(map,map->head);
    free(map);
}


int mapGetSize(Map map)
{
    if (map == NULL)
    {
        return -1;
    }
    return map->size;
}

/** ,חסרה התייחסות לאיטרטור*/
MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if (map == NULL||keyElement==NULL||dataElement==NULL) 
    {
        return MAP_NULL_ARGUMENT;
    }
    MapKeyElement iterator =mapGetFirst(map);
    free(iterator);
    Node curr_node = map->head, next_node = nodeGetNext(curr_node);
    while (next_node != NULL)
    {
        MapKeyElement key_to_compare =((MapEntry) (nodeGetData(next_node)))->key_element;
        int compare_result = map->compare_key_function(keyElement, key_to_compare);
        if (compare_result < 0)
        {
            return mapKeyAndDataInsert(keyElement, dataElement, curr_node, map);
        }
        if (compare_result == 0)
        {
            return mapEntrychange(map, nodeGetData(next_node), dataElement);
        }
        curr_node = next_node;
        next_node = nodeGetNext(curr_node);
    }
    return mapKeyAndDataInsert(keyElement, dataElement, curr_node, map);
}


MapResult mapRemove(Map map, MapKeyElement keyElement) 
{
    if (map == NULL || keyElement==NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Node result = mapFindKey(map, keyElement);
    if (result == NULL)
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    Node node_to_free = nodeGetNext(result);
    mapEntryDestroy(map, nodeGetData(node_to_free));
    Node node_to_link = nodeGetNext(node_to_free);
    nodeLink(result, node_to_link);
    nodeDestroy(node_to_free);
    map->size--;
    MapKeyElement iterator = mapGetFirst(map);
    free(iterator);
    return MAP_SUCCESS;
}


Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
              
{
    if(copyDataElement==NULL||copyKeyElement==NULL||freeDataElement==NULL||freeKeyElement==NULL||compareKeyElements==NULL)
    {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if(map==NULL)
    {
        return NULL;
    }
    map->compare_key_function=compareKeyElements;
    map->copy_data_function=copyDataElement;
    map->free_data_function=freeDataElement;
    map->free_key_function=freeKeyElement;
    map->copy_key_function=copyKeyElement;

    Node head_node = nodeCreate(NULL);
    if(head_node==NULL)
    {
        free(map);
        return NULL;
    }
    map->head=head_node;
    map->size=0;
    map->iterator=NULL;
    return map;
}


Map mapCopy(Map map)
{
    if (map==NULL)
    {
        return NULL;
    }
    assert(map->compare_key_function!=NULL && map->copy_data_function!=NULL&& map->copy_key_function!=NULL
            && map->free_data_function!=NULL && map->free_key_function!=NULL);
    Map copy = mapCreate(map->copy_data_function, map->copy_key_function, 
                         map->free_data_function, map->free_key_function, map->compare_key_function);     
    Node node_to_be_copied = nodeGetNext(map->head);
    while(node_to_be_copied!=NULL)
    {
        mapPut(copy,((MapEntry) nodeGetData(node_to_be_copied))->key_element,((MapEntry) nodeGetData(node_to_be_copied))->data_element);
        node_to_be_copied =nodeGetNext(node_to_be_copied);
    }
    return copy;
}


bool mapContains(Map map, MapKeyElement element)
{
    if (map==NULL||element==NULL)
    {
        return false;
    }
    MapKeyElement iterator = mapGetFirst(map);
    free(iterator);
    Node result = mapFindKey(map,element);
    if(result==NULL)
    {
        return false;
    }
    return true;
}


MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if (map==NULL || keyElement == NULL)
    {
        return NULL;
    }
    Node result =nodeGetNext(mapFindKey(map,keyElement));
    if(result==NULL)
    {
        return NULL;
    }
    return ((MapEntry) nodeGetData(result))->data_element;
}


MapKeyElement mapGetFirst(Map map)  //CHANGES TO TARGIL
{
    if(map==NULL||map->size==0)
    {
        return NULL;
    }
    map->iterator =((MapEntry) nodeGetData(nodeGetNext(map->head)))->key_element;
    return  map->copy_key_function(map->iterator);
}


MapKeyElement mapGetNext(Map map)  //CHANGES TO TARGIL
{
    if (map == NULL)
    {
        return NULL;
    }
    Node result = mapFindKey(map, map->iterator);
    result = nodeGetNext(nodeGetNext(result));
    if(result==NULL)
    {
        return NULL;
    }
    else
    {
    map->iterator = ((MapEntry) nodeGetData(result))->key_element;
    }
    return map->copy_key_function(map->iterator);
}


MapResult mapClear(Map map)
{
    if(map==NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    listDestroy(map, nodeGetNext(map->head));
    nodeLink(map->head,NULL);
    map->size=0; 
    map->iterator=NULL;
    return MAP_SUCCESS;
}