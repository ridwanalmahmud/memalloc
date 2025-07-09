#include "main.h"

// aligns the memory block size to system's word size
// (n + sizeof(word_t) - 1) adds enough padding to guarantee we cross the next alignment boundery
// ~(sizeof(word_t) - 1) creates a bitmask to strip of unaligned bits
// bitwise & clears the lower bits, rounding down to the nearest value
// 64 -> align(3)=8, align(12)=16
// 32 -> align(3)=4, align(6)=8
// 64 -> if n = 13
// (n + sizeof(word_t) -1) = 20 ->          0b00010100
// (sizeof(word_t) - 1) = 7 ->              0b00000111
// ~(sizeof(word_t) - 1) = ~7 ->            0b11111000
// bitwise &  (0b00010100 & 0b11111000) ->  0b00100000
static inline size_t align(size_t n) {
    return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}
