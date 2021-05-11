#include <stdio.h>
#include "node.h"


int main ()
{
    int a=1;
    int b=2;
    int c=3;
    printf("naama is married to dani\n");
    
    Node node1 = nodeCreate(&a);
    Node node2 = nodeCreate(&b);
    Node node3 = nodeCreate(&c);
    
    nodeLink(node1,node2);
    nodeLink(node2,node3); 
    int aids = nodeDestroy(node1);
    int sars = nodeDestroy(node2);
    int ebola = nodeDestroy(node3);
    printf ("%d\n" , aids);

    return 0;
}
