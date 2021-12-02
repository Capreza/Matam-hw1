#include <iostream>
#include "avltree.h"
#include "node.h"




void testAVL();


int main() {
    testAVL();
    return 0;
}

void testAVL()
{
    AVLTree<int> tree1;
    AVLTree<int> tree2;
    AVLTree<int> tree3;

    int test1[]={82,11,37,80,27,91,79,3,19,7};
    int test2[]={3,7,11,19,27,37,79,80,82,91};
    int test3[]={91,82,80,79,37,27,19,11,7,3};
    for(int i=0;i<10;i++)
    {
        tree1.append(test1[i]);
    }

    for(int i=0;i<10;i++)
    {
        tree2.append(test2[i]);
    }

    for(int i=0;i<10;i++)
    {
        tree3.append(test3[i]);
    }

/*
    tree1.inOrder();
    printf("finished in order1\n");
    tree1.inOrderRev();
    printf("finished in orderrev1\n");

    tree2.inOrder();
    printf("finished in order2\n");
    tree2.inOrderRev();
    printf("finished in orderrev2\n");

    tree3.inOrder();
    printf("finished in order3\n");
    tree3.inOrderRev();
    printf("finished in orderrev3\n");
  */

//     for(int i=0;i<10;i++)
//     {
//         tree1.remove(test1[i]);
//     }
//
//
//     for(int i=0;i<10;i++)
//     {
//         tree2.remove(test2[i]);
//     }
//
//     for(int i=0;i<10;i++)
//     {
//         tree3.remove(test3[i]);
//     }

    /*
    int first_int = 27;
    bool first = (tree1.get(first_int) == nullptr);

    int second_int = 80;
    bool second = (tree1.get(second_int) != nullptr);

    int third_int = 37;
    bool third = (tree1.get(third_int) == nullptr);
*/
    /*
    tree1.inOrder();
    printf("finished in order1\n");
    tree1.inOrderRev();
    printf("finished in orderrev1\n");

    tree2.inOrder();
    printf("finished in order2\n");
    tree2.inOrderRev();
    printf("finished in orderrev2\n");

    tree3.inOrder();
    printf("finished in order3\n");
    tree3.inOrderRev();
    printf("finished in orderrev3\n");
*/
    //printf("1- %d, 2- %d, 3- %d \n", first,second,third);

 }
