#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int get_left_children_height(bsnode* node)
{
    if(node->left == NULL)
    {
        return 0;
    }
    return node->left->height + 1;
}

int get_right_children_height(bsnode* node)
{
    if(node->right == NULL)
    {
        return 0;
    }
    return node->right->height + 1;
}

void set_height(bsnode* node)
{
    int l_h = get_left_children_height(node);
    int r_h = get_right_children_height(node);
    node->height = l_h > r_h ? l_h : r_h ;
}

void set_height_parent(bsnode* node)
{
    node->height += 1;
    node = node->parent;

    while(node != NULL)
    {
        set_height(node);
        node = node->parent;
    }
}

void set_bf(bsnode* node)
{
    node->bf = get_left_children_height(node) - get_right_children_height(node);
}

void l_rotation(bsnode* node)
{
    bsnode* p = node->parent;
    bsnode* pp = NULL;
    if(p != NULL)
    {
        pp = p->parent;
    }

    bsnode* l = node->left;

    node->left = p;
    p->parent = node;
    node->left->right = l;

    if(pp== NULL)
    {
        node->parent = NULL;
    }
    else
    {
        if(pp->left == p)
        {
            pp->left = node;          
        }
        else
        {
            pp->right = node;
        }
        node->parent = pp;
    }

    set_height(node->left);
    set_bf(node->left);

    set_height(node);
    set_bf(node);
}

void r_rotation(bsnode* node)
{
    bsnode* pp = NULL;
    bsnode* p = node->parent;
    if(p != NULL)
    {
        pp = p->parent;
    }
    bsnode* r = node->right;

    node->right = p;
    p->parent = node;
    p->left = r;

    if(pp== NULL)
    {
        node->parent = NULL;
    }
    else
    {
        if(pp->left == p)
        {
            pp->left = node;          
        }
        else
        {
            pp->right = node;
        }
        node->parent = pp;
    }

    set_height(node->right);
    set_bf(node->right);

    set_height(node);
    set_bf(node);
}

bsnode*  adjust_bf(bsnode* node)
{
    while(1)
    {
        set_bf(node);
        if(node->bf == 2)
        {
            if(node->left->bf == -1)
            {
                l_rotation(node->left);
                r_rotation(node);
            }
            else if(node->left->bf == 1)
            {
                r_rotation(node->left);
            }
        }
        else if(node->bf == -2)
        {
            if(node->right->bf == 1)
            {
                r_rotation(node->right);
                l_rotation(node);
            }
            else if(node->right->bf == -1)
            {
                l_rotation(node->right);
            }
        }
        if(node->parent == NULL)
        {
            return node;
        }
        else
        {
            node = node->parent;
        }
    }
}

bsnode* insert(bsnode* root, bsnode* node)
{
    bsnode* new_root = NULL;

    if (node->data == root->data)
    {
        return NULL;
    }

    if(node->data > root->data)
    {
        if(root->right == NULL)
        {
            root->right = node;
            node->parent = root;
            node->height = 0;
            if(root->left == NULL)
            {
                set_height_parent(root);
                new_root = adjust_bf(node->parent);
            }
        }
        else
        {
            new_root = insert(root->right, node);
        }
    }
    else
    {
        if(root->left == NULL)
        {
            root->left = node;
            node->parent = root;
            node->height = 1;
            if(root->right == NULL)
            {
                set_height_parent(root);
                new_root = adjust_bf(node->parent);
            }
        }
        else
        {
            new_root = insert(root->left, node);
        }
    }
    return new_root;
}

bsnode* del(bsnode* root, bsnode* node)
{


    return NULL;
}

void pre_order(bsnode* root, cb_in_order cb)
{
    if(root)
    {        
        pre_order(root->left,cb);
        bsnode* r = root->right;
        cb(root);     
        pre_order(r,cb);
    }
}

void in_order(bsnode* root)
{
}

void post_order(bsnode* root)
{
}

bsnode* create_node(int data)
{
    bsnode*  pb = (bsnode*)malloc(sizeof(bsnode));
    pb->left=pb->parent=pb->right=NULL;
    pb->height = 0;
    pb->bf = 0;
    pb->data = data;
    return pb;
}