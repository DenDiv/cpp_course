#include <iostream>
#include "LinkedList.h"

LinkedList::LinkedList()
{
    head = NULL;
}
void LinkedList::print()
{
    list_p current_p = head;
    while (current_p != NULL)
    {
        int *pInt = static_cast<int *>(current_p->data);
        std::cout << *pInt << " ";
        current_p = current_p->next;
    }
}
void LinkedList::push(int value)
{
    list_p new_tag = new list_t();
    new_tag->data = new int;
    *((int *)new_tag->data) = value;
    new_tag->next = head;
    head = new_tag;
}
void LinkedList::reverse()
{
    list_p current_p = head;
    list_p prev_p = NULL, next_p = NULL;
    while (current_p != NULL)
    {
        next_p = current_p->next;
        current_p->next = prev_p;
        prev_p = current_p;
        current_p = next_p;
    }
    head = prev_p;
}
