/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */

#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "memory_pool.h"

ngx_uint_t  ngx_pagesize;
ngx_uint_t  ngx_pagesize_shift;
ngx_uint_t  ngx_cacheline_size;

#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)

void *
ngx_alloc(size_t size, ngx_log_t *log)
{
    void  *p;

    p = malloc(size);
    if (p == NULL)
    {
        //ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
        //              "malloc(%uz) failed", size);
    }

    //ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, log, 0, "malloc: %p:%uz", p, size);
    return p;
}


void *
ngx_calloc(size_t size, ngx_log_t *log)
{
    void  *p;

    p = ngx_alloc(size, log);

    if (p) 
    {
        ngx_memzero(p, size);
    }

    return p;
}

ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = (ngx_pool_t*)ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) 
    {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}

void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) 
    {
        if (c->handler) 
        {
            //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
            //               "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) 
    {
        //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);
        if (l->alloc) 
        {
            ngx_free(l->alloc);
        }
    }

#if (NGX_DEBUG)
    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        if (n == NULL) {
            break;
        }
    }
#endif

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        if (n == NULL) {
            break;
        }
    }
}

#if (NGX_HAVE_POSIX_MEMALIGN)

void *
ngx_memalign(size_t alignment, size_t size, ngx_log_t *log)
{
    void  *p;
    int    err;

    err = posix_memalign(&p, alignment, size);

    if (err) {
        ngx_log_error(NGX_LOG_EMERG, log, err,
                      "posix_memalign(%uz, %uz) failed", alignment, size);
        p = NULL;
    }

    ngx_log_debug3(NGX_LOG_DEBUG_ALLOC, log, 0,
                   "posix_memalign: %p:%uz @%uz", p, size, alignment);

    return p;
}

#elif (NGX_HAVE_MEMALIGN)

void *
ngx_memalign(size_t alignment, size_t size, ngx_log_t *log)
{
    void  *p;
#ifdef WIN32
    p = _aligned_malloc(size, alignment);
#else
    p = memalign(alignment, size);
#endif
    if (p == NULL)
    {
        //ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
        //              "memalign(%uz, %uz) failed", alignment, size);
    }

    //ngx_log_debug3(NGX_LOG_DEBUG_ALLOC, log, 0,
    //               "memalign: %p:%uz @%uz", p, size, alignment);

    return p;
}
#endif

static void *ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char     *m;
    size_t       psize;
    ngx_pool_t  *p, *new_pool, *current;

    psize = (size_t) (pool->d.end - (u_char *) pool);
    m = (u_char*)ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);
    if (m == NULL) 
    {
        return NULL;
    }

    new_pool = (ngx_pool_t *) m;

    new_pool->d.end = m + psize;
    new_pool->d.next = NULL;
    new_pool->d.failed = 0;

    m += sizeof(ngx_pool_data_t);
    m = ngx_align_ptr(m, NGX_ALIGNMENT);
    new_pool->d.last = m + size;

    current = pool->current;

    for (p = current; p->d.next; p = p->d.next) 
    {
        if (p->d.failed++ > 4)
        {
            current = p->d.next;
        }
    }

    p->d.next = new_pool;
    pool->current = current ? current : new_pool;
    return m;
}

static void *ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

    p = ngx_alloc(size, pool->log);
    if (p == NULL)
    {
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) 
    {
        if (large->alloc == NULL)
        {
            large->alloc = p;
            return p;
        }

        if (n++ > 3)
        {
            break;
        }
    }

    large = (ngx_pool_large_t*)ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) 
    {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

void * ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max)
    {
        p = pool->current;
        do
        {
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);
            if ((size_t) (p->d.end - m) >= size)
            {
                p->d.last = m + size;
                return m;
            }
            p = p->d.next;
        } while (p);
        return ngx_palloc_block(pool, size);
    }
    return ngx_palloc_large(pool, size);
}

int ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next)
    {
        if (p == l->alloc) 
        {
            //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
            //               "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;
            return NGX_OK;
        }
    }

    return NGX_DECLINED;
}

void *ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) 
    {
        p = pool->current;
        do
        {
            m = p->d.last;

            if ((size_t) (p->d.end - m) >= size) 
            {
                p->d.last = m + size;
                return m;
            }
            p = p->d.next;
        } while (p);
        return ngx_palloc_block(pool, size);
    }
    return ngx_palloc_large(pool, size);
}

ngx_pool_cleanup_t * ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = (ngx_pool_cleanup_t*)ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    if (c == NULL) 
    {
        return NULL;
    }

    if (size) 
    {
        c->data = ngx_palloc(p, size);
        if (c->data == NULL) 
        {
            return NULL;
        }
    } 
    else 
    {
        c->data = NULL;
    }
    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;
    //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);
    return c;
}

void ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) 
    {
        if (l->alloc) 
        {
            ngx_free(l->alloc);
        }
    }

    pool->large = NULL;
    for (p = pool; p; p = p->d.next) 
    {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    }
}

void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    for (c = p->cleanup; c; c = c->next) {
        if (c->handler == ngx_pool_cleanup_file) {
            cf = (ngx_pool_cleanup_file_t*) (c->data);
            if (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                return;
            }
        }
    }
}


void ngx_pool_cleanup_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = (ngx_pool_cleanup_file_t*) data;

    //ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
    //               c->fd);

#ifdef WIN32
    if (ngx_close_file((FILE*)(c->fd)) == NGX_FILE_ERROR) {
#else
    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
#endif
        //ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
        //              ngx_close_file_n " \"%s\" failed", c->name);
    }
}


void ngx_pool_delete_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = (ngx_pool_cleanup_file_t*)data;

    int  err;

    //ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
    //               c->fd, c->name);

    if (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        if (err != NGX_ENOENT) {
            //ngx_log_error(NGX_LOG_CRIT, c->log, err,
            //              ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

#ifdef WIN32
    if (ngx_close_file((FILE*)(c->fd)) == NGX_FILE_ERROR) {
#else
    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
#endif
        //ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
        //              ngx_close_file_n " \"%s\" failed", c->name);
    }
}

void *ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    if (p) 
    {
        ngx_memzero(p, size);
    }

    return p;
}