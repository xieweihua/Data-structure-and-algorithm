// list.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "list_sort.h"

struct test_struct
{
    int data;
    list_head list;
};

int  cmp(void *priv, struct list_head *a, struct list_head *b)
{
	struct test_struct *ela, *elb;

	ela = container_of(a, struct test_struct, list);
	elb = container_of(b, struct test_struct, list);

	return ela->data - elb->data;
}

void print_list(struct list_head *h)
{
    struct test_struct *pos = NULL;
    
    list_for_each_entry(pos, h, list) {
        printf ("%d\n", pos->data);
    }
}

void delete_list(struct list_head *h)
{
    struct list_head *pos, *n ;

    list_for_each_safe(pos, n, h)
    {
        struct test_struct* temp  = container_of(pos, struct test_struct, list);
        printf("delete node: %d\n", temp->data);
        free(temp);
    }
}

int main()
{
    LIST_HEAD(head);
 
    for(int i =1;i<10;i++)
    {
        struct test_struct* temp = (test_struct*)malloc(sizeof(test_struct));
        temp->data = rand()%100;
        list_add_tail(&(temp->list), &head);
    }

    print_list(&head);
    list_sort(NULL, &head, cmp);
    printf("after sort:\n");
    print_list(&head);

    printf("delete list:\n");
    delete_list(&head);
	return 0;
}

