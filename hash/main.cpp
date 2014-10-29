#include "config.h"
#include "hlist.h"
#include "hash_fnv.h"
#include "hash.h"

#define HASH_SIZE 8

struct test_hlist_st
{
    hlist_node list;
    uint64_t data;
};

struct test_hash_data
{
    uint64_t data;
};

int test_visit()
{
    HLIST_HEAD(head);
    hlist_node* tpos, *pos;
    struct test_hlist_st* stpos; 

    for(int i =1;i<10;i++)
    {
        struct test_hlist_st* temp = (test_hlist_st*)malloc(sizeof(test_hlist_st));
        temp->data = sd_hash_64((uint64_t)i);
        hlist_add_head(&(temp->list), &head);
    }

	hlist_for_each(pos, &head)
    {
        struct test_hlist_st* temp = (struct test_hlist_st*)container_of(pos, struct test_hlist_st, list);
        printf("%ul\n", temp->data);
    }

    hlist_for_each_entry(stpos, pos, &head, list, struct test_hlist_st)
    {
    	 printf("hlist_for_each_entry: %ul\n", stpos->data);
    }

    hlist_for_each_safe(pos, tpos, &head)
    {
        struct test_hlist_st* temp = (struct test_hlist_st*)container_of(pos, struct test_hlist_st, list);
        printf("delete: %ul\n", temp->data);
        free(temp);
    }

    return 0;
}

void rb_delete(void* p)
{
    struct test_hash_data*  d = (struct test_hash_data* )p;
    free(p);
}

int main()
{
    struct hash_table_st* t = create_hash_table(HASH_SIZE);

    for(int i =1;i<256;i++)
    {
        struct test_hash_data* temp = (test_hash_data*)malloc(sizeof(test_hash_data));
        temp->data = i;

        hash_insert(temp, t, i);
    }

    struct test_hash_data*  d = (struct test_hash_data* )find_node(77,t);
    if(d != NULL)
    {
      printf("find:%d\n", d->data);
    }
    else
    {
        printf("not found\n");
    }

    delete_hash_table(t, rb_delete);
}