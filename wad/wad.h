#ifndef CUMIN_WAD_H
#define CUMIN_WAD_H

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace wad
{
    constexpr std::size_t sz_wad_id{4}; /* wad file id length */
    constexpr std::size_t sz_lump_name{8}; /* lump name max-length */

    enum class Wad_type { iwad, pwad };

    // wad file header
    struct Wadinfo
    {
        std::array<char, sz_wad_id> identification; /* "PWAD" or "IWAD" */
        int num_lumps; /* number of lumps in WAD */
        int info_table_offset; /* offset to lump info directory in file */
    };

    // lump info directory entry
    struct Filelump
    {
        int offset; /* offset to lump data in file */
        int size; /* bytes */
        std::array<char, sz_lump_name> name; /* not ALWAYS null-terminated */
    };

    class Wad_file
    {
    public:
        Wad_file(const std::string& filename);
        //TODO: void print_lump_directory();
    private:
        //TODO: bool valid_type();

        std::string filename_; /* full path to file */
        std::ifstream fp_; /* underlying file stream */
        //Wadinfo wadinfo;
        //std::vector<Filelump> lump_directory;
        //Wad_type type;
    };
}

#endif
