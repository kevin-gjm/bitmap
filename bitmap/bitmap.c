#include "bitmap.h"

/* 1st: used the index to locate the 8 bits block( bytes index);
 *
 * 2nd: used the index to locate the offset in the 8 bits block;
 *
 * */

/* 1 byte = 8 bits = 2^3 bits */
#define BITS 8
#define SHIFT 3  
#define MASK 0x7


/**
 * N.B.: if bitmap.size == 2^power, the problem is :
 *  1.if bitmap allocate bytes = (bitmap.size >> 3) + ((bitmap.size & 0x7) ? 1 : 0);
 *    eg: bitmap.size = 8 bits, bitmap allocate bytes = (8 >> 3) + ((8 & 0x7) ? 1 : 0) = 1 + 0 = 1;
 *    only 1 bytes allocated, 
 *    namely 8bits: bit:0 bit:1 bit:2 bit:3 bit:4 bit:5 bit:6 bit:7, 
 *    not include bit:8, therefore, we can't do bitmap_set(bitmap, bitmap.size);
 *
 * 2. resolve the above problem (edge case):
 *  bitmap allocate bytes = (bitmap.size >> 3) + ((bitmap.size & 0x7) ? 1 : 1);
 *  when bitmap.size == 2^power, we also allocate another extra bytes too.
 *
 * */

bitmap_t *bitmap_create(size_t size) 
{
  if(size == 0) return NULL;

  bitmap_t *bitmap = (bitmap_t *) malloc(sizeof(bitmap_t));
  if (bitmap == NULL) 
  {
    fprintf(stderr, "Error: out of memory, memory not enough!\n");
    exit(0);
  }

  size_t bytes = (size >> SHIFT) + ((size & MASK) ? 1 : 1);
  void *map = malloc(bytes);
  if (map == NULL) 
  {
    free(bitmap);
    fprintf(stderr, "Error: out of memory, memory not enough!\n");
    exit(0);
  }
  memset(map, 0, bytes);

  bitmap->bitmap = map;
  bitmap->size   = size;

  return bitmap;
}

void bitmap_destroy(bitmap_t *bitmap)
{
  if (bitmap) 
  {
    if (bitmap->bitmap) 
	{
      free(bitmap->bitmap);
    }
    free(bitmap);
  }
}

void bitmap_set(bitmap_t *bitmap, size_t index)
{
	bitmap->bitmap[index >> SHIFT] |= 1 << (index & MASK);
}

void bitmap_clear(bitmap_t *bitmap, size_t index)
{
	bitmap->bitmap[index >> SHIFT] &= ~(1 << (index & MASK));
}

int bitmap_test(bitmap_t *bitmap, size_t index)
{
  return (bitmap->bitmap[index >> SHIFT] & (1 << (index & MASK))) >> (index & MASK);
}

void bitmap_set_all(bitmap_t *bitmap)
{
	memset(bitmap->bitmap, 0xff, (bitmap->size >> SHIFT) + ((bitmap->size & MASK) ? 1 : 1));
}

void bitmap_clear_all(bitmap_t *bitmap)
{
	memset(bitmap->bitmap, 0x00, (bitmap->size >> SHIFT) + ((bitmap->size & MASK) ? 1 : 1));
}

void bitmap_set_range(bitmap_t* bitmap, size_t begin, size_t end)
{
	if (begin == end) return;
	size_t start_pos = begin >> SHIFT;
	size_t end_pos = end >> SHIFT;
	if (start_pos == end_pos)
	{
		bitmap->bitmap[start_pos] |= ~(0xFF << (end % BITS))&(0xFF << (begin % BITS));
		return;
	}
	bitmap->bitmap[start_pos] |= 0xFF << (begin % BITS);
	memset(&bitmap->bitmap[start_pos + 1], 0xFF, end_pos - start_pos - 1);
	bitmap->bitmap[end_pos] |= ~(0xFF << (end % BITS));
}
void bitmap_clear_range(bitmap_t* bitmap, size_t begin, size_t end)
{
	if (begin == end) return;
	size_t start_pos = begin >> SHIFT;
	size_t end_pos = end >> SHIFT;
	if (start_pos == end_pos)
	{
		bitmap->bitmap[start_pos] &= ~( ~(0xFF << (end % BITS)) & (0xFF << (begin % BITS)));
		return;
	}
	bitmap->bitmap[start_pos] &= ~(0xFF << (begin % BITS));
	memset(&bitmap->bitmap[start_pos + 1], 0x00, end_pos - start_pos - 1);
	bitmap->bitmap[end_pos] &= 0xFF << (end % BITS);
}

bitmap_t* bitmap_or(const bitmap_t* x1, const bitmap_t* x2)
{
	size_t i;
	bitmap_t* ans = bitmap_create(x1->size > x2->size ? x1->size : x2->size);
	size_t x1_count = (x1->size >> SHIFT) + ((x1->size & MASK) ? 1 : 1);
	size_t x2_count = (x2->size >> SHIFT) + ((x2->size & MASK) ? 1 : 1);
	//extern part set to 0
	x1->bitmap[x1_count - 1] = x1->bitmap[x1_count - 1] & (~(0xFF << ((x1->size + 1) % BITS == 0 ? BITS : (x1->size + 1) % BITS) ));
	x2->bitmap[x2_count - 1] = x2->bitmap[x2_count - 1] & (~(0xFF << ((x2->size + 1) % BITS == 0 ? BITS : (x2->size + 1) % BITS) ));
	size_t min = x1_count <= x2_count ? x1_count : x2_count;
	size_t max = x1_count <= x2_count ? x2_count : x1_count;
	
		for (i = 0; i < min; i++)
		{
			ans->bitmap[i] = x2->bitmap[i] | x1->bitmap[i];
		}
		for (i = min; i < max; i++)
		{
			if (x1_count < x2_count)
			{
				ans->bitmap[i] = x2->bitmap[i];
			}
			else
			{
				ans->bitmap[i] = x1->bitmap[i];
			}
		}
	
	return ans;
}
bitmap_t* bitmap_and(const bitmap_t* x1, const bitmap_t* x2)
{
	size_t i;
	bitmap_t* ans = bitmap_create(x1->size > x2->size ? x1->size : x2->size);
	size_t x1_count = (x1->size >> SHIFT) + ((x1->size & MASK) ? 1 : 1);
	size_t x2_count = (x2->size >> SHIFT) + ((x2->size & MASK) ? 1 : 1);
	//extern part set to 1
	x1->bitmap[x1_count - 1] = x1->bitmap[x1_count - 1] | (0xFF << ((x1->size + 1) % BITS == 0 ? BITS : (x1->size + 1) % BITS));
	x2->bitmap[x2_count - 1] = x2->bitmap[x2_count - 1] | (0xFF << ((x2->size + 1) % BITS == 0 ? BITS : (x2->size + 1) % BITS));
	size_t min = x1_count <= x2_count ? x1_count : x2_count;
	size_t max = x1_count <= x2_count ? x2_count : x1_count;

	for (i = 0; i < min; i++)
	{
		ans->bitmap[i] = x2->bitmap[i] & x1->bitmap[i];
	}
	for (i = min; i < max; i++)
	{
		if (x1_count < x2_count)
		{
			ans->bitmap[i] = x2->bitmap[i];
		}
		else
		{
			ans->bitmap[i] = x1->bitmap[i];
		}
	}

	return ans;
}
bitmap_t* bitmap_xor(const bitmap_t* x1, const bitmap_t* x2)
{
	size_t i;
	bitmap_t* ans = bitmap_create(x1->size > x2->size ? x1->size : x2->size);
	size_t x1_count = (x1->size >> SHIFT) + ((x1->size & MASK) ? 1 : 1);
	size_t x2_count = (x2->size >> SHIFT) + ((x2->size & MASK) ? 1 : 1);
	//extern part set to 0
	x1->bitmap[x1_count - 1] = x1->bitmap[x1_count - 1] & (~(0xFF << ((x1->size + 1) % BITS == 0 ? BITS : (x1->size + 1) % BITS)));
	x2->bitmap[x2_count - 1] = x2->bitmap[x2_count - 1] & (~(0xFF << ((x2->size + 1) % BITS == 0 ? BITS : (x2->size + 1) % BITS)));
	size_t min = x1_count <= x2_count ? x1_count : x2_count;
	size_t max = x1_count <= x2_count ? x2_count : x1_count;

	for (i = 0; i < min; i++)
	{
		ans->bitmap[i] = x2->bitmap[i] ^ x1->bitmap[i];
	}
	for (i = min; i < max; i++)
	{
		if (x1_count < x2_count)
		{
			ans->bitmap[i] = x2->bitmap[i];
		}
		else
		{
			ans->bitmap[i] = x1->bitmap[i];
		}
	}

	return ans;
}