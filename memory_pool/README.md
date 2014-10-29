memory pool
=======

移植自nginx, windows平台可用

主要应用于小内存块管理

函数
=======

	- ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
	- void ngx_destroy_pool(ngx_pool_t *pool);
	- void ngx_reset_pool(ngx_pool_t *pool);
	- void *ngx_palloc(ngx_pool_t *pool, size_t size);
	- void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
	- void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
	- void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
	- int ngx_pfree(ngx_pool_t *pool, void *p);
	
	
以下接口未测试
=======

	- ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
	- void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
	- void ngx_pool_cleanup_file(void *data);
	- void ngx_pool_delete_file(void *data);