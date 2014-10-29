/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_

#include <stdio.h>
#include <sys/types.h>    
#include <sys/stat.h>   
#include <fcntl.h>

/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */

#ifdef WIN32
#define ENOENT           2      /* No such file or directory  just for linux*/
#define ngx_close_file fclose
#else
#define ngx_close_file           close
#endif

#define ngx_errno               errno
#define NGX_ENOENT        ENOENT 
#define  NGX_OK          0
#define NGX_FILE_ERROR           -1
#define  NGX_DECLINED   -5

#define NGX_MAX_ALLOC_FROM_POOL  (4096 - 1)
#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)
#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)
#define ngx_free          free
#define ngx_delete_file(name)    unlink((const char *) name)

#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

typedef void (*ngx_pool_cleanup_pt)(void *data);
typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;
typedef unsigned char u_char;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

typedef struct ngx_pool_s        ngx_pool_t;
typedef uintptr_t       ngx_uint_t;

typedef struct {
    unsigned char               *last;
    unsigned char                *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;

typedef struct ngx_chain_s       ngx_chain_t;
typedef struct ngx_log_s         ngx_log_t;

struct ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
    ngx_log_t            *log;
};

typedef int             ngx_fd_t;
typedef struct {
    ngx_fd_t              fd;
    unsigned char     *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
void ngx_destroy_pool(ngx_pool_t *pool);
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
int ngx_pfree(ngx_pool_t *pool, void *p);

ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);

#if (NGX_HAVE_POSIX_MEMALIGN || NGX_HAVE_MEMALIGN)
void *ngx_memalign(size_t alignment, size_t size, ngx_log_t *log);
#else
#define ngx_memalign(alignment, size, log)  ngx_alloc(size, log)

#endif

#endif /* _NGX_PALLOC_H_INCLUDED_ */
