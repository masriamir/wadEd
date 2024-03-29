#ifndef CUMIN_WAD_H
#define CUMIN_WAD_H

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace wad
{
    constexpr std::size_t sz_wad_id{4}; /* wad file id length */
    constexpr std::size_t sz_lump_name{8}; /* lump name max-length */

    constexpr std::size_t wd_lump_name{16}; /* lump name column width */
    constexpr std::size_t wd_lump_size{8}; /* lump size column width */

    const std::string_view nm_iwad{"IWAD"};
    const std::string_view nm_pwad{"PWAD"};

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

        std::string_view lump_name() const;
    };

    class Wad_file
    {
    public:
        Wad_file(const std::string& filename);

        std::string_view filename() const;
        std::string_view id() const;
        const Wadinfo& wadinfo() const;
        const Wad_type& type() const;
        const std::vector<Filelump>& lumps() const;
    private:
        std::string filename_; /* full path to file */
        std::ifstream fp_; /* underlying file stream */
        Wadinfo wadinfo_;
        Wad_type type_;
        std::vector<Filelump> lump_directory_;
    };

    // Wad_file operators
    bool operator==(const Wad_file& lhs, const Wad_file& rhs);
    std::ostream& operator<<(std::ostream& os, const Wad_file& wad_file);

    // Filelump operators
    std::ostream& operator<<(std::ostream& os, const Filelump& lump);
}

#endif
