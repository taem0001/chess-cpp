#ifndef MAGIC_H
#define MAGIC_G
#include "utils.h"

u64 random_u64();
u64 random_u64_fewbits();
int count_1s(u64);
int pop_1st_bit(u64 *);
u64 index_to_u64(int, int, u64);
int transform(u64, u64, int);

#endif // !MAGIC_H