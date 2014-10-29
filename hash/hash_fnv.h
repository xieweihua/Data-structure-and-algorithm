/*
* ������FNV��ϣ�㷨ȫ��ΪFowler-Noll-Vo�㷨��������λ������Glenn Fowler��Landon Curt Noll��Phong Vo
*           �������������ģ�������1991�������
*
* �ص����;��FNV�ܿ���hash�������ݲ����ֽ�С�ĳ�ͻ�ʣ����ĸ߶ȷ�ɢʹ��������hashһЩ�ǳ�������ַ�����
*                     ����URL��hostname���ļ�����text��IP��ַ�ȡ�
* �㷨�汾��FNV�㷨�������汾��FNV-0���ѷ�������FNV-1��FNV-1a
*                 FNV-1��FNV-1a�㷨�����������ɵĹ�ϣֵ��hash����һ������
* 
* ����1��hash���޷�������
* ����2��hash��λ����bits����Ӧ����2��n�η���32��64��128��256��512��1024����һ��32λ�ľ͹����ˡ�
*/
#ifndef _LINUX_HASH_FNV_H
#define _LINUX_HASH_FNV_H

#include "config.h"
#include <stdint.h>

uint64_t sd_hash_64(uint64_t oid);
/* 64 bit Fowler/Noll/Vo FNV-1a hash code */
uint64_t sd_hash(const void *buf, size_t len);

#endif