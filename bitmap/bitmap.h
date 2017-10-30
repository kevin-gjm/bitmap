#ifndef _BITMAP_H__
#define _BITMAP_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  the bit0 refer to number 0;
*/

typedef struct _bitmap 
{
  /* bitmap: byte array */
  unsigned char *bitmap; 
  /* bitmap bit size: bytes = (size >> 3) + ((size & 0x7) ? 1 : 1)*/                
  size_t size;    
} bitmap_t;


/* create size bits bitmap. have size+1 number. store [0,size] */
bitmap_t *bitmap_create(size_t size);

/* destroy the unused bitmap */
void bitmap_destroy(bitmap_t *bitmap);

/* set bitmap the index-th bit to 1 */
int bitmap_set(bitmap_t *bitmap, size_t index);

/* clear bitmap the index-th bit to 0 */
int bitmap_clear(bitmap_t *bitmap, size_t index);

/* test bitmap the index-th bit is 1 or 0 */
int  bitmap_test(bitmap_t *bitmap, size_t index);

/* set all bits of bitmap  to 1 */
void bitmap_set_all(bitmap_t *bitmap);

/* clear all bits of bitmap  to 0 */
void bitmap_clear_all(bitmap_t *bitmap);

/* set bits of [ begin,end ) to 1 */
void bitmap_set_range(bitmap_t* bitmap, size_t begin, size_t end);

/* set bits of [ begin,end ) to 0 */
void bitmap_clear_range(bitmap_t* bitmap, size_t begin, size_t end);

/* return a new bitmap of x1|x2 */
bitmap_t* bitmap_or(const bitmap_t* x1, const bitmap_t* x2);

/* return a new bitmap of x1&x2 */
bitmap_t* bitmap_and(const bitmap_t* x1, const bitmap_t* x2);

/* calculate the result of x1&x2, and store it to parameter result */
int bitmap_samesize_and(bitmap_t* result, const bitmap_t* x2);

/* return a new bitmap of x1^x2 */
bitmap_t* bitmap_xor(const bitmap_t* x1, const bitmap_t* x2);

/* return the count of nums in the bitmap*/
size_t bitmap_count(bitmap_t* bitmap);

/**
* return a new bitmap. x[offset,offset+count)-->newbitmap[0,count)
* param: offset must be an integer multiple of 8.
* param: count must be an integer multiple of 8.
**/
bitmap_t* bitmap_move_range_to_new(const bitmap_t* x,size_t offset,size_t count);
/**
* src[0,count)-->x[offset,offset+count)
* param: offset must be an integer multiple of 8.
* param: count must be an integer multiple of 8.
**/
int bitmap_move_range_to_old(bitmap_t* dst, const bitmap_t* src, size_t offset, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* endif of _BITMAP_H__ */
