hash
=======

hlist: 移植自linux内核, windows平台可用(可单独使用，但一般应用与hash表)
mempory pool : nginx 内存模块(可单独使用)

由于windows没有typeof， 故WIN32下:

```c
#define list_for_each_entry(pos, head, member, type)        		\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))
```	  

由于类似段代码在vc下无法编译:

   - for 循环不支持: pos && ({ n = pos->next; 1; });

```c

#define hlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

```   
故在WIN下重新定义了: 

  - hlist_for_each_safe
  - hlist_for_each_entry

其余涉及宏或函数未移植，由于hash的接口不涉及WIN不支持的接口，故hash在WIN和LINUX均可使用.

hlist: 是专门为hash而设计的链表，hlist_node只有一个first指针，在建立hash表时，使用的内存最少，如：

100W个节点(桶)的hash表使用内存4M(1024*1024*4)


函数
=======

  - struct hash_table_st* create_hash_table(size_t size);
  - bool hash_insert(void* data, struct hash_table_st* t, uint64_t key);
  - struct hlist_head* find_head(uint64_t key, struct hash_table_st* t);
  - void* find_node(uint64_t key, struct hash_table_st* t);
  - void delete_hash_table(struct hash_table_st* t, delete_hash_item rb_delete);
