typedef struct list_tag
{
    void *data;
    struct list_tag *next;
} list_t, *list_p;

struct LinkedList
{
    list_p head = nullptr;
    ~LinkedList();
    void print();
    void push(int value);
    void reverse();
};