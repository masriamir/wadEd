#include "pch.h"
#include "framework.h"

#include "memory.h"

int new_chunk(memory_t* mc, const uint32_t size)
{
    /* make sure size is valid */
    if (size < 1) {
        fprintf(stderr, "memory_t:new_memory_t - invalid size\n");
        return -1;
    }

    /* allocation memory for memory_t */
    if (!(mc = (memory_t*)malloc(sizeof(*mc)))) {
        fprintf(stderr,
            "memory_t:new_memory_t - memory_t allocation failed\n");
        return -1;
    }

    /* set size and position */
    mc->ptr = 0;
    mc->size = size;

    /* if we cannot allocate the data set it to NULL */
    if (!(mc->data = (uint8_t*)malloc(size))) {
        fprintf(stderr,
            "memory_t:new_memory_t - data allocation failed\n");
        mc->data = NULL;
        mc->size = 0;
        return 1;
    }

    return 0;
}

bool free_chunk_data(memory_t* const mc)
{
    /* check for valid data */
    if (mc->data) {
        free(mc->data);
        mc->data = NULL;
        return true;
    }

    return false;
}

bool free_chunk(memory_t* mc)
{
    /* check for valid memory_t */
    if (mc) {
        free_chunk_data(mc);

        free(mc);
        mc = NULL;

        return true;
    }

    return false;
}

bool resize_chunk(memory_t* mc, const uint32_t new_size)
{
    /* make sure size is valid */
    if (new_size < 1) {
        fprintf(stderr, "memory_t:resize_chunk - invalid size\n");
        return false;
    }

    /* temp pointer to new memory location for 'data' in case the following realloc fails */
    uint8_t* tmp_data = (uint8_t*)realloc(mc->data, new_size);

    if (tmp_data) {
        mc->data = tmp_data;
    }
    else {
        free(mc->data);
        fprintf(stderr,
            "memory_t:resize_chunk - allocation of %u bytes failed\n",
            new_size);
        return false;
    }

    mc->size = new_size;

    /* if we decreased the size set the postion to the end of the chunk */
    if (mc->ptr > mc->size) {
        mc->ptr = mc->size;
    }

    return true;
}

bool clear_chunk(memory_t* const mc)
{
    if (!is_chunk_empty(mc)) {
        mc->ptr = 0;
        mc->size = 0;
        free_chunk_data(mc);
        return true;
    }

    return false;
}

bool is_chunk_empty(const memory_t* mc)
{
    return mc->size == 0 && !mc->data;
}

void seek_chunk(memory_t* const mc,
    const uint32_t offset,
    const uint32_t start)
{
    if (start == SEEK_CUR) {
        /* offset based on current position */
        mc->ptr += offset;
        if (mc->ptr > mc->size) {
            mc->ptr = mc->size;
        }
    }
    else if (start == SEEK_SET) {
        /* offset based on beginning of file */
        mc->ptr = offset;
        if (mc->ptr > mc->size) {
            mc->ptr = mc->size;
        }
    }
    else if (start == SEEK_END) {
        /* offset based on end of file */
        if (offset > mc->size) {
            mc->ptr = 0;
        }
        else {
            mc->ptr = mc->size - offset;
        }
    }
}

bool read_chunk(void* const buf, memory_t* const mc, const uint32_t size)
{
    /* valid destination and source data */
    if (!mc->data || !buf) {
        return false;
    }

    /* make sure we are not reading past the end of the chunk */
    if (mc->ptr + size > mc->size) {
        return false;
    }

    /* copy the mem chunk to the given buffer and advance the position */
    memcpy(buf, mc->data + mc->ptr, size);
    mc->ptr += size;

    return true;
}

bool read_chunk_ofs(void* const buf,
    memory_t* const mc,
    const uint32_t size,
    const uint32_t offset)
{
    /* make sure we are not reading past end of chunk */
    if (offset + size > mc->size) {
        return false;
    }

    /* seek to offset from beginning of chunk and then read as normal */
    seek_chunk(mc, offset, SEEK_SET);
    return read_chunk(buf, mc, size);
}

bool write_chunk(memory_t* mc, const void* data, const uint32_t size)
{
    /* valid data to write */
    if (!data) {
        return false;
    }

    /* resize the memory_t if trying to write past the end of the chunk */
    if (mc->ptr + size > mc->size) {
        resize_chunk(mc, mc->size + size);
    }

    /* copy the data and set the new position */
    memcpy(mc->data + mc->ptr, data, size);
    mc->ptr += size;

    return true;
}

bool write_chunk_ofs(memory_t* mc,
    const void* data,
    const uint32_t size,
    const uint32_t offset)
{
    /* seek to offset from beginning of chunk and then write as normal */
    seek_chunk(mc, offset, SEEK_SET);
    return write_chunk(mc, data, size);
}
