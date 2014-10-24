tree
=======

  - rb_tree 移植自linux内核, windows平台可用
  - avl_tree 上学时的练习代码


函数
=======

1.	rb_set_parent	设置父节点的地址
2.	rb_set_color	设置节点颜色
3.	rb_init_node	初始化节点
4.	rb_insert_color	设置新插入节点的颜色
5.	rb_erase	删除一个节点
6.	rb_next	返回当前节点的下一个节点
7.	rb_prev	返回当前节点的上一个节点
8.	rb_first	返回第一个叶子节点（也就是最左边的叶子节点）
9.	rb_last	返回最后一个叶子节点（也就是最右边的叶子节点）
10.	rb_replace_node	替换rbtree中的一个node（只是简单的替换，没有管替换的颜色对不对，数据的顺序对不对）
 

宏
=======

1.	rb_parent	获取父节点的地址
2.	rb_color	节点的颜色
3.	rb_is_red	是否红节点
4.	rb_is_black	是否黑节点
5.	rb_set_red	设置节点为红色
6.	rb_set_black	设置节点为黑色
7.	RB_ROOT	初始化根节点
8.	rb_entry	获取包含rbtree node的struct
9.	RB_EMPTY_ROOT	判断是否只有根节点
10.	RB_EMPTY_NODE	判断节点是否刚初始化，还没有加到树中
11.	RB_CLEAR_NODE	设置节点的父节点也指向自己