/*
* 由来：FNV哈希算法全名为Fowler-Noll-Vo算法，是以三位发明人Glenn Fowler，Landon Curt Noll，Phong Vo
*           的名字来命名的，最早在1991年提出。
*
* 特点和用途：FNV能快速hash大量数据并保持较小的冲突率，它的高度分散使它适用于hash一些非常相近的字符串，
*                     比如URL，hostname，文件名，text，IP地址等。
* 算法版本：FNV算法有三个版本：FNV-0（已废弃）、FNV-1和FNV-1a
*                 FNV-1和FNV-1a算法对于最终生成的哈希值（hash）有一定限制
* 
* 　　1，hash是无符号整型
* 　　2，hash的位数（bits），应该是2的n次方（32，64，128，256，512，1024），一般32位的就够用了。
*/
#ifndef _LINUX_HASH_FNV_H
#define _LINUX_HASH_FNV_H

#include "config.h"
#include <stdint.h>

uint64_t sd_hash_64(uint64_t oid);
/* 64 bit Fowler/Noll/Vo FNV-1a hash code */
uint64_t sd_hash(const void *buf, size_t len);

#endif