#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "rbtree.h"
//#include <typeinfo>

void cb_tree(bsnode* node)
{
    printf("%d\n", node->data);
    free(node);
}

int test_avl_tree()
{
    int i = 1;
    bsnode* root = create_node(i++);

    for(; i<10;i++)
    {
        bsnode* node = create_node(rand()%100);
        bsnode* temp = insert(root, node);
        if(temp != NULL)
        {
            root = temp;
        }
    }

    pre_order(root, cb_tree);

    return 0;
}

struct mytype {  
    struct rb_node my_node;  
    int num;  
};  
  
struct mytype *my_search(struct rb_root *root, int num)  
{  
    struct rb_node *node = root->rb_node;  
  
    while (node) {  
    struct mytype *data = container_of(node, struct mytype, my_node);  
  
    if (num < data->num)  
        node = node->rb_left;  
    else if (num > data->num)  
        node = node->rb_right;  
    else  
        return data;  
    }  
      
    return NULL;  
}  
  
int my_insert(struct rb_root *root, struct mytype *data)  
{  
    struct rb_node **tmp = &(root->rb_node), *parent = NULL;  
  
    /* Figure out where to put new node */  
    while (*tmp) {  
    struct mytype *pst = container_of(*tmp, struct mytype, my_node);  
  
    parent = *tmp;  
    if (data->num < pst->num)  
        tmp = &((*tmp)->rb_left);  
    else if (data->num > pst->num)  
        tmp = &((*tmp)->rb_right);  
    else   
        return -1;  
    }  
      
    /* Add new node and rebalance tree. */  
    rb_link_node(&data->my_node, parent, tmp);  
    rb_insert_color(&data->my_node, root);  
      
    return 0;  
}  
  
void my_delete(struct rb_root *root, int num)  
{  
    struct mytype *data = my_search(root, num);  
    if (!data) {   
    fprintf(stderr, "Not found %d.\n", num);  
    return;  
    }  
      
    rb_erase(&data->my_node, root);  
    free(data);  
}  
  
void print_rbtree(struct rb_root *tree)  
{  
    struct rb_node *node;  
      
    for (node = rb_first(tree); node; node = rb_next(node))  
    {
        struct mytype *pt = rb_entry(node, struct mytype, my_node);  
        printf("%d ", pt->num);   
        printf("\n");  
    }
}  
  
int main(int argc, char *argv[])  
{  
    struct rb_root mytree =  RB_ROOT;  
    int i, ret, num;  
    struct mytype *tmp;  
  
    num = 10;  
  
    printf("Please enter %d integers:\n", num);  
    for (i = 0; i < num; i++) 
    {  
        tmp = (mytype *)malloc(sizeof(struct mytype));  
        tmp->num = rand()%100;
        if (!tmp)  
            perror("Allocate dynamic memory");  
      
        ret = my_insert(&mytree, tmp);  
        if (ret < 0)
        {  
            fprintf(stderr, "The %d already exists.\n", tmp->num);  
            free(tmp);  
        }  
    }  
  
    printf("\nthe first test\n");  
    print_rbtree(&mytree);  
  
    my_delete(&mytree, 24);  
  
    printf("\nthe second test\n");  
    print_rbtree(&mytree);  
  
    return 0;  
}  