#ifndef _HASH_INTERFACE_H
#define _HASH_INTERFACE_H

#include <stdint.h>
#include "hlist.h"
#include "memory_pool.h"

struct hash_item_st
{
    hlist_node list;
    void* data;
    uint64_t key;
};

struct hash_table_st
{
    hlist_head* h;
    uint64_t size;
};

typedef void delete_hash_item(void* data);

struct hash_table_st* create_hash_table(size_t size);
bool hash_insert(void* data, struct hash_table_st* t, uint64_t key);
struct hlist_head* find_head(uint64_t key, struct hash_table_st* t);
void* find_node(uint64_t key, struct hash_table_st* t);
void delete_hash_table(struct hash_table_st* t, delete_hash_item rb_delete);

#endif