#pragma once

typedef struct node{  
    int data;  
    int height;
    int bf;
    struct node *left,*right,*parent;  
}bsnode;  

bsnode* insert(bsnode* root, bsnode* node);
bsnode* del(bsnode* root, bsnode* node);

typedef void (*cb_in_order)(bsnode* );

void pre_order(bsnode* root, cb_in_order cb);
void in_order(bsnode* root, cb_in_order cb);
void post_order(bsnode* root, cb_in_order cb);

bsnode* create_node(int data);