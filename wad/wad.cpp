#include "wad.h"

wad::Wad_file::Wad_file(const std::string& filename) : filename_{filename}, fp_{filename, std::ios::binary}, wadinfo_{},
                                                       type_{}
{
    if (!fp_) {
        throw std::runtime_error{"Could not open " + filename + "."};
    }

    fp_.read(reinterpret_cast<char*>(&wadinfo_), sizeof(Wadinfo));

    if (id() == nm_iwad) {
        type_ = Wad_type::iwad;
    }
    else if (id() == nm_pwad) {
        type_ = Wad_type::pwad;
    }
    else {
        throw std::runtime_error{"Not a valid WAD."};
    }
}

std::string_view wad::Wad_file::filename() const
{
    return filename_;
}

std::string_view wad::Wad_file::id() const
{
    return std::string_view{wadinfo_.identification.data(), sz_wad_id};
}

const wad::Wadinfo& wad::Wad_file::wadinfo() const
{
    return wadinfo_;
}

const wad::Wad_type& wad::Wad_file::type() const
{
    return type_;
}

bool wad::operator==(const Wad_file& lhs, const Wad_file& rhs)
{
    // for now just use the filename
    return lhs.filename() == rhs.filename();
}

std::ostream& wad::operator<<(std::ostream& os, const Wad_file& wad_file)
{
    os << "Type:\t";
    switch (wad_file.type()) {
    case wad::Wad_type::iwad:
        os << nm_iwad << '\n';
        break;
    case wad::Wad_type::pwad:
        os << nm_pwad << '\n';
        break;
    }

    return os << "Lumps:\t" << wad_file.wadinfo().num_lumps << '\n'
        << "Ofs:\t" << wad_file.wadinfo().info_table_offset << '\n';
}
