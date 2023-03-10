struct list_t
{
    void *data;
    struct list_t *next;
};
using list_p = list_t *;

struct LinkedList
{
    list_p head = nullptr;
    ~LinkedList();
    void print();
    void push(int value);
    void reverse();
};