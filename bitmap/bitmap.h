/*  
 * This file provides the api of bitmap.
 * bitmap: a form of hash, using one bit to present an int number.
 *
 *  FileName:    bitmap.h
 *  Description: bitmap related functions  api declarations.
 *  Author:      wzb (zhongbo.wzb@alibaba-inc.com)
 *  Date:        2013-11-25 16:12
 *  Version:     0.1
 *
 * */

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


/* create size bits bitmap */
bitmap_t *bitmap_create(size_t size);

/* destroy the unused bitmap */
void bitmap_destroy(bitmap_t *bitmap);

/* set bitmap the index-th bit to 1 */
void bitmap_set(bitmap_t *bitmap, size_t index);

/* clear bitmap the index-th bit to 0 */
void bitmap_clear(bitmap_t *bitmap, size_t index);

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

/* return a new bitmap of x1^x2 */
bitmap_t* bitmap_xor(const bitmap_t* x1, const bitmap_t* x2);


#ifdef __cplusplus
}
#endif

#endif /* endif of _BITMAP_H__ */
