hash
=======

hlist: ��ֲ��linux�ں�, windowsƽ̨����(�ɵ���ʹ�ã���һ��Ӧ����hash��)
mempory pool : nginx �ڴ�ģ��(�ɵ���ʹ��)

����windowsû��typeof�� ��WIN32��:

```c
#define list_for_each_entry(pos, head, member, type)        		\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))
```	  

�������ƶδ�����vc���޷�����:

   - for ѭ����֧��: pos && ({ n = pos->next; 1; });

```c

#define hlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

```   
����WIN�����¶�����: 

  - hlist_for_each_safe
  - hlist_for_each_entry

�����漰�����δ��ֲ������hash�Ľӿڲ��漰WIN��֧�ֵĽӿڣ���hash��WIN��LINUX����ʹ��.

hlist: ��ר��Ϊhash����Ƶ�����hlist_nodeֻ��һ��firstָ�룬�ڽ���hash��ʱ��ʹ�õ��ڴ����٣��磺

100W���ڵ�(Ͱ)��hash��ʹ���ڴ�4M(1024*1024*4)


����
=======

  - struct hash_table_st* create_hash_table(size_t size);
  - bool hash_insert(void* data, struct hash_table_st* t, uint64_t key);
  - struct hlist_head* find_head(uint64_t key, struct hash_table_st* t);
  - void* find_node(uint64_t key, struct hash_table_st* t);
  - void delete_hash_table(struct hash_table_st* t, delete_hash_item rb_delete);
