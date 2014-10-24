list
=======

移植自linux内核, windows平台可用

由于windows没有typeof， 故WIN32下:

```c
#define list_for_each_entry(pos, head, member, type)        		\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))
```	     

其余涉及到使用typeof关键字的函数或者宏并未移植


函数
=======

1.	list_add	在 head 之后追加一个节点
2.	list_add_tail	在 head 之前追加一个节点, 也就是在末尾追加一个节点
3.	list_del	删除一个节点, 并将这个节点的next, prev 置为 NULL
4.	list_del_init	删除一个节点并初始化删除的节点
5.	list_replace	替换一个节点
6.	list_replace_init	替换一个节点, 并初始化被替换的节点
7.	list_move	移动节点到 head 之后
8.	list_move_tail	移动节点到 head 之前
9.	list_is_last	判断节点是否是链表中最后一个
10.	list_empty	判断链表是否为空 (即, 是否只有 head 节点)
11.	list_is_singular	判断链表中是否只有一个节点 (除了 head 之外)
12.	list_cut_position	将1个链表截断为2个链表
13.	list_splice	将2个链表合并为1个链表, @list中的所有节点(不包括list)加入到 head 之后
14.	list_splice_tail	将2个链表合并为1个链表, @list中的所有节点(不包括list)加入到 head 之前
15.	list_splice_init	同 list_splice, 最后会初始化 @list
16.	list_splice_tail_init	同 list_splice_tail, 最后会初始化 @list
 

宏
=======

1.	list_entry	获取包含此节点的 struct
2.	list_first_entry	获取包含此节点的 首个 struct
3.	list_for_each	从 head节点之后一个节点开始向后循环
4.	list_for_each_prev	从 head节点之前一个节点开始向前循环
5.	list_for_each_safe	list_for_each 的安全版本, 即, 循环时即使有其它线程删除节点也可正常运行
6.	list_for_each_prev_safe	list_for_each_prev 的安全版本
7.	list_for_each_entry	同 list_for_each, 只是参数不同
8.	list_for_each_entry_reverse	同 list_for_each_prev, 只是参数不同
9.	list_for_each_entry_continue	同 list_for_each_entry, 但不是从头(head)开始循环的
10.	list_for_each_entry_continue_reverse	同 list_for_each_entry_reverse, 但不是从头(head)开始循环的
11.	list_for_each_entry_from	从指定位置开始向后循环
12.	list_for_each_entry_safe	list_for_each_entry 的安全版本
13.	list_for_each_entry_safe_continue	list_for_each_entry_continue 的安全版本
14.	list_for_each_entry_safe_from	list_for_each_entry_from 的安全版本
15.	list_for_each_entry_safe_reverse	list_for_each_entry_reverse 的安全版本