#include "map.h"
#include "node.h"

struct Map_t 
{
    Node head;
    int size;

    copyMapDataElements copy_data_function;
    copyMapKeyElements copy_key_function;
    freeMapDataElements free_data_function;
    freeMapKeyElements free_key_function;
    compareMapKeyElements compare_key_function;

    Node iterator;
};



typedef struct map_entry
{
    MapDataElement data_element;
    MapKeyElement key_element;
}*Map_entry;

static int key_compare (MapKeyElement key, Map map)
{

}