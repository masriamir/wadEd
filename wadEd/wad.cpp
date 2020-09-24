#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

#include "wad.h"

FILE *fp;
uint32_t numlumps;
filelump_t *filelumps;

int load_wad(const char *filename) {
    /* open wad file to read */
    fp = fopen(filename, "rb");
    if (!fp) {
        perror("unable to open file");
        return -1;
    }

    /* read the header bytes */
    wadinfo_t wad;
    if (fread(&wad, sizeof(wad), 1, fp) != 1) {
        perror("error reading wad header");
        return -1;
    }

    /* make sure file is a valid wad */
    if (valid_wad(wad.identification) != 0) {
        fprintf(stderr, "invalid wad file\n");
        return -1;
    }

    /* set number of lumps found in wad */
    numlumps = wad.numlumps;

    /* seek to beginning of lump info directory */
    if (fseek(fp, wad.infotableofs, SEEK_SET) != 0) {
        perror("unable to find start of info table");
        return -1;
    }

    /* allocate memory for lump info directory */
    if (!(filelumps = (filelump_t *) malloc(numlumps * sizeof(*filelumps)))) {
        fprintf(stderr, "unable to allocate memory for lump info directory\n");
        return -1;
    }

    /* read lump info directory table in wad */
    for (size_t i = 0; i < numlumps; i++) {
        /* check for valid read */
        if (fread(&filelumps[i], sizeof(*filelumps), 1, fp) != 1) {
            /* check errors */
            if (feof(fp)) {
                perror("premature end of file");
            } else if (ferror(fp)) {
                perror("error reading lump");
            }

            /* cleanup */
            free_wad();
            return -1;
        }
    }

    return 0;
}

void free_wad(void) {
    /* free lump info directory and close wad file */
    if (filelumps) {
        free(filelumps);
        filelumps = NULL;
    }
    fclose(fp);
    numlumps = 0;
}

int read_lump(void *dest, const uint32_t lump) {
    /* check for valid destination buffer */
    if (!dest) {
        fprintf(stderr, "invalid read destination\n");
        return -1;
    }

    /* check for valid lump number */
    if (lump >= numlumps) {
        fprintf(stderr, "invalid lump %u\n", lump);
        return -1;
    }

    /* get pointer to the lump we want */
    filelump_t *lump_p = filelumps + lump;

    /* seek to position of lump in file */
    if (fseek(fp, lump_p->filepos, SEEK_SET) != 0 && ferror(fp)) {
        perror("seek error while reading lump");
        return -1;
    }

    /* read lump data to destination buffer */
    if (fread(dest, lump_p->size, 1, fp) != 1) {
        /* check errors */
        if (feof(fp)) {
            perror("premature end of file");
        } else if (ferror(fp)) {
            perror("error reading lump data");
        }

        return -1;
    }

    return 0;
}

int write_lump(const uint32_t lump, FILE *file) {
    /* check for valid lump number */
    if (lump >= numlumps) {
        fprintf(stderr, "invalid lump %u\n", lump);
        return -1;
    }

    /* get pointer to the lump we want */
    filelump_t *lump_p = filelumps + lump;

    /* allocate buffer to hold lump data */
    void *buf = malloc(lump_p->size);
    if (!buf) {
        fprintf(stderr, "unable to allocate memory for lump\n");
        return -1;
    }

    /* read lump data into buffer */
    if (read_lump(buf, lump) == -1) {
        free(buf);
        buf = NULL;

        fprintf(stderr, "unable to load lump %u for writing\n", lump);
        return -1;
    }

    /* write buffer to file */
    if (fwrite(buf, lump_p->size, 1, file) != 1) {
        /* check errors */
        if (feof(file)) {
            perror("premature end of file");
        } else if (ferror(file)) {
            perror("error writing lump data");
        }

        /* cleanup */
        if (buf) {
            free(buf);
            buf = NULL;
        }

        return -1;
    }

    /* cleanup */
    if (buf) {
        free(buf);
        buf = NULL;
    }

    return 0;
}

int valid_wad(const char *id) {
    return strncmp(id, "IWAD", WAD_ID_SZ) && strncmp(id, "PWAD", WAD_ID_SZ);
}

int get_lump_num(const char *name) {
    /* linear search for given lump name */
    for (size_t i = 0; i < numlumps; i++) {
        if (strncmp(filelumps[i].name, name, strlen(name)) == 0) {
            return i;
        }
    }

    /* not found */
    return -1;
}

void list_lumps(void) {
    for (size_t i = 0; i < numlumps; i++) {
        printf("name: %.*s, size: %d, offset: %d\n", LUMP_NAME_SZ, filelumps[i].name, filelumps[i].size, filelumps[i].filepos);
    }
}