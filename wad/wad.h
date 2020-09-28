#ifndef WADED_WAD_H
#define WADED_WAD_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WAD_ID_SZ 4 /* wad file id length */
#define LUMP_NAME_SZ 8 /* lump name length */

/* wad file header */
typedef struct wadinfo_t {
    char identification[WAD_ID_SZ];
    uint32_t numlumps;
    int32_t infotableofs;
} wadinfo_t;

/* lump info directory entry */
typedef struct filelump_t {
    int32_t filepos;
    int32_t size;
    char name[LUMP_NAME_SZ];
} filelump_t;

extern FILE *fp; /* wad file */
extern uint32_t numlumps; /* number of lumps in wad */
extern filelump_t *filelumps; /* lump info directory */

/* allocation and cleanup */
int load_wad(const char *);
void free_wad(void);

/* io */
int read_lump(void *, const uint32_t);
int write_lump(const uint32_t, FILE *);

/* utility */
int valid_wad(const char *);
int get_lump_num(const char *);
void list_lumps(void);

#endif
