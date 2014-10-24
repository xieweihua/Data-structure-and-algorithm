list
=======

��ֲ��linux�ں�, windowsƽ̨����

����windowsû��typeof�� ��WIN32��:

```c
#define list_for_each_entry(pos, head, member, type)        		\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))
```	     

�����漰��ʹ��typeof�ؼ��ֵĺ������ߺ겢δ��ֲ


����
=======

1.	list_add	�� head ֮��׷��һ���ڵ�
2.	list_add_tail	�� head ֮ǰ׷��һ���ڵ�, Ҳ������ĩβ׷��һ���ڵ�
3.	list_del	ɾ��һ���ڵ�, ��������ڵ��next, prev ��Ϊ NULL
4.	list_del_init	ɾ��һ���ڵ㲢��ʼ��ɾ���Ľڵ�
5.	list_replace	�滻һ���ڵ�
6.	list_replace_init	�滻һ���ڵ�, ����ʼ�����滻�Ľڵ�
7.	list_move	�ƶ��ڵ㵽 head ֮��
8.	list_move_tail	�ƶ��ڵ㵽 head ֮ǰ
9.	list_is_last	�жϽڵ��Ƿ������������һ��
10.	list_empty	�ж������Ƿ�Ϊ�� (��, �Ƿ�ֻ�� head �ڵ�)
11.	list_is_singular	�ж��������Ƿ�ֻ��һ���ڵ� (���� head ֮��)
12.	list_cut_position	��1������ض�Ϊ2������
13.	list_splice	��2������ϲ�Ϊ1������, @list�е����нڵ�(������list)���뵽 head ֮��
14.	list_splice_tail	��2������ϲ�Ϊ1������, @list�е����нڵ�(������list)���뵽 head ֮ǰ
15.	list_splice_init	ͬ list_splice, �����ʼ�� @list
16.	list_splice_tail_init	ͬ list_splice_tail, �����ʼ�� @list
 

��
=======

1.	list_entry	��ȡ�����˽ڵ�� struct
2.	list_first_entry	��ȡ�����˽ڵ�� �׸� struct
3.	list_for_each	�� head�ڵ�֮��һ���ڵ㿪ʼ���ѭ��
4.	list_for_each_prev	�� head�ڵ�֮ǰһ���ڵ㿪ʼ��ǰѭ��
5.	list_for_each_safe	list_for_each �İ�ȫ�汾, ��, ѭ��ʱ��ʹ�������߳�ɾ���ڵ�Ҳ����������
6.	list_for_each_prev_safe	list_for_each_prev �İ�ȫ�汾
7.	list_for_each_entry	ͬ list_for_each, ֻ�ǲ�����ͬ
8.	list_for_each_entry_reverse	ͬ list_for_each_prev, ֻ�ǲ�����ͬ
9.	list_for_each_entry_continue	ͬ list_for_each_entry, �����Ǵ�ͷ(head)��ʼѭ����
10.	list_for_each_entry_continue_reverse	ͬ list_for_each_entry_reverse, �����Ǵ�ͷ(head)��ʼѭ����
11.	list_for_each_entry_from	��ָ��λ�ÿ�ʼ���ѭ��
12.	list_for_each_entry_safe	list_for_each_entry �İ�ȫ�汾
13.	list_for_each_entry_safe_continue	list_for_each_entry_continue �İ�ȫ�汾
14.	list_for_each_entry_safe_from	list_for_each_entry_from �İ�ȫ�汾
15.	list_for_each_entry_safe_reverse	list_for_each_entry_reverse �İ�ȫ�汾