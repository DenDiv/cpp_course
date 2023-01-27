#include <iostream>
#include "LinkedList.h"

int main()
{
    /* Start with the empty list */
    LinkedList ll;
    ll.push(20);
    ll.push(4);
    ll.push(15);
    ll.push(85);
    ll.push(123);

    std::cout << "Given linked list\n";
    ll.print();

    ll.reverse();

    std::cout << "\nReversed linked list \n";
    ll.print();
    return 0;
}