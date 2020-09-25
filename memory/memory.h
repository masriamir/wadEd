#ifndef __WADED_MEMORY_H__
#define __WADED_MEMORY_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memory_t {
    uint32_t ptr;
    uint32_t size;
    uint8_t* data;
} memory_t;

int new_chunk(memory_t*, const uint32_t);
bool free_chunk_data(memory_t* const);
bool free_chunk(memory_t*);

bool resize_chunk(memory_t* const, const uint32_t);
bool clear_chunk(memory_t* const);
bool is_chunk_empty(const memory_t*);

void seek_chunk(memory_t* const, const uint32_t, const uint32_t);

bool read_chunk(void* const, memory_t* const, const uint32_t);
bool read_chunk_ofs(void* const,
    memory_t* const,
    const uint32_t,
    const uint32_t);

bool write_chunk(memory_t*, const void*, const uint32_t);
bool write_chunk_ofs(memory_t*, const void*, const uint32_t, const uint32_t);

#endif
