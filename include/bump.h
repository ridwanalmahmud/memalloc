#ifndef __MEMALLOC_H__
#define __MEMALLOC_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Memory layout of a block
//
// +----------------------------+  <-------+
// | block size                 |          |
// +----------------------------+          | Block header
// | is used (1 byte)           |          |
// +----------------------------+          |
// | pointer to next block      |   <------+
// +----------------------------+
// |     Payload pointer        |   <------+
// |            ...             |          |
// |            ...             |          | Addressable content
// |            ...             |          |
// |            ...             |   <------+
// +----------------------------+

// Macine word size. Defending on the architecture
// Can be 4 or 8 bytes
typedef intptr_t word_t;

typedef struct Block {
    size_t size;        // size of the block
    bool is_used;       // block used status (malloc->true, free->false)
    struct Block *next; // pointer to the next block in the list
    word_t data[1];     // payload pointer, points to the first word of user data
} Block;

//  blocks in memory
//                    ↓----------------------↓
// +-----------------------------+ +----------------------------+
// | size | used | next * | data | | size | used | next * | data|
// +-----------------------------+ +----------------------------+

// since this is a linked list we will keep track of the head and tail of the heap
static Block *head = NULL, *tail = NULL;

// for faster access a memory block should be aligned
// usually by machine word size
// aligned block with an object header
// call -> memalloc(5) (bytes)
// +------------------------------------+-----------+
// | object header (+n) | user data (5) | align (3) |
// +------------------------------------+-----------+

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
size_t align(size_t n);

// reserves allocation size of memory block
// total allocation = (block header) + (first data word) - (word_t data[1])
// since the `word_t data[1]` already allocates one word inside the Block
// structure, we decrease it from the size request: if a user allocates
// only one word, it's fully in the Block struct.
size_t alloc_size(size_t size);

// request memory from os
// the heap grows upwards and the end is denoted by brk pointer
// brk pointer points to the unmapped are between stack and heap
// we will use sbrk system call to map memory at this space
// sbrk requests the os to increase heap size by n bytes
Block *request_from_os(size_t size);

// --- Core API ---
// allocates a block of memory of at least size bytes
word_t *memalloc(size_t size);

// --- Helper functions ---
// returns the object header
Block *get_header(word_t *data);

#endif // ! __MEMALLOC_H__
