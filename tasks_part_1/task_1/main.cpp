#include <iostream>
#include "LinkedList.h"

int main()
{
    /* Reverse list task */
    LinkedList ll;
    ll.push(20);
    ll.push(4);
    ll.push(15);
    ll.push(85);
    ll.push(123);

    std::cout << "Given linked list" << std::endl;
    ll.print();

    ll.reverse();

    std::cout << std::endl << "Reversed linked list" << std::endl;
    ll.print();
    return 0;
}