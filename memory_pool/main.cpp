#include <stdio.h>
#include "memory_pool.h"

int main()
{
    ngx_pool_t   *pool = ngx_create_pool(NGX_DEFAULT_POOL_SIZE,  NULL);
    if (pool == NULL) {
        printf("alloc pool failed!\n");
        return NULL;
    }

    char* p1 = (char*)ngx_pcalloc(pool, 1024);
    char* p2 = (char*)ngx_pcalloc(pool, 1024);
    char* p3 = (char*)ngx_pcalloc(pool, 1000);
    char* p4 = (char*)ngx_pnalloc(pool, 1024);

    ngx_destroy_pool(pool);

    return 0;
}