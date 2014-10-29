/* 64 bit FNV-1a non-zero initial basis */
#define FNV1A_64_INIT ((uint64_t) 0xcbf29ce484222325ULL)
#define FNV_64_PRIME ((uint64_t) 0x100000001b3ULL)
#define SD_NR_VDIS   (1U << 24)

#include "hash_fnv.h"

/* 64 bit Fowler/Noll/Vo FNV-1a hash code */
uint64_t fnv_64a_buf(const void *buf, size_t len, uint64_t hval)
{
	const unsigned char *p = (const unsigned char *) buf;
	size_t i;

	for (i = 0; i < len; i++) {
		hval ^= (uint64_t) p[i];
		hval *= FNV_64_PRIME;
	}

	return hval;
}

/*
 * The result is same as fnv_64a_buf(&oid, sizeof(oid), hval) but this function
 * is a bit faster.
 */
uint64_t fnv_64a_64(uint64_t oid, uint64_t hval)
{
	hval ^= oid & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 8 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 16 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 24 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 32 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 40 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 48 & 0xff;
	hval *= FNV_64_PRIME;
	hval ^= oid >> 56 & 0xff;
	hval *= FNV_64_PRIME;

	return hval;
}

uint64_t sd_hash(const void *buf, size_t len)
{
	uint64_t hval = fnv_64a_buf(buf, len, FNV1A_64_INIT);

	return fnv_64a_64(hval, hval);
}

uint64_t sd_hash_64(uint64_t oid)
{
	uint64_t hval = fnv_64a_64(oid, FNV1A_64_INIT);

	return fnv_64a_64(hval, hval);
}

