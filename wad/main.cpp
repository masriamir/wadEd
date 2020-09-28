#include <stdlib.h>
#include <stdio.h>

#include "wad.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        /* load wad file into memory */
        if (load_wad(argv[1]) == -1) {
            fprintf(stderr, "unable to read wad\n");
            exit(EXIT_FAILURE);
        }

        /* list file lump info table */
        list_lumps();
        printf("found %u lumps\n", numlumps);

        /* cleanup */
        free_wad();
    }

    return 0;
}