#include "hash.h"
#include "hash_fnv.h"

static ngx_pool_t   *pool = ngx_create_pool(NGX_DEFAULT_POOL_SIZE,  NULL);

void* h_malloc(int size)
{
    return ngx_pcalloc(pool, size);
}

void h_free()
{
    ngx_destroy_pool(pool);
}

struct hash_table_st* create_hash_table(size_t size)
{
    struct hlist_head* h = (struct hlist_head*)h_malloc(sizeof(struct hlist_head) * size); 
    memset(h, 0, sizeof(hlist_head) * size);

    struct hash_table_st* t = (struct hash_table_st*)h_malloc(sizeof(struct hash_table_st)); 
    t->h = h;
    t->size = size;

    return t;
}

bool hash_insert(void* data, struct hash_table_st* t, uint64_t key)
{
    uint64_t index = sd_hash_64(key)%(t->size);
    struct hlist_head* t_array = t->h;

    if(t_array[index].first == NULL)
    {
        struct hlist_head* h = (hlist_head*) h_malloc(sizeof(hlist_head));
        h->first = NULL;
        t_array[index] = *h;
    } 

    struct hash_item_st* node = (hash_item_st*) h_malloc(sizeof(hash_item_st));
    node->key = key;
    node->data = data;
    INIT_HLIST_NODE(&node->list);

    hlist_add_head(&(node->list), &(t_array[index]));

    return true;
}

struct hlist_head* find_head(uint64_t key, struct hash_table_st* t)
{
    uint64_t index = sd_hash_64(key)%(t->size);
    struct hlist_head* t_array = t->h;

    return &(t_array[index]);
}

void* find_node(uint64_t key, struct hash_table_st* t)
{
    struct hlist_head*  h = find_head(key, t);
    hlist_node *pos;
    struct hash_item_st* stpos; 

    hlist_for_each_entry(stpos, pos, h, list, struct hash_item_st)
    {
        if(stpos->key == key)
        {
            return stpos->data;
        }
    }
    return NULL;
}

void delete_hash_table(struct hash_table_st* t, delete_hash_item rb_delete)
{
    hlist_node *pos, *n;
    struct hash_table_st* t_bak = t;

    for(int i = 0; i< t->size; i++)
    {
        hlist_for_each_safe(pos, n, (t->h)+i)
        {
            struct hash_item_st* temp = (struct hash_item_st*)container_of(pos, struct hash_item_st, list);
            rb_delete(temp->data); 
            //h_free(temp);
        }
    }
    //h_free(t_bak->h);
    //h_free(t_bak);
    h_free();
}