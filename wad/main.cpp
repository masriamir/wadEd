#include "wad.h"

int main(const int argc, const char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    if (argc > 1) {
        try {
            wad::Wad_file wad_file{std::string{argv[1]}};
        }
        catch (const std::exception& e) {
            std::cerr << e.what();
            return 1;
        }
    }

    return 0;
}
