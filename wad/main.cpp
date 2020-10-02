#include "wad.h"

using namespace std;

int main(const int argc, const char* argv[])
{
    std::ios_base::sync_with_stdio(false);

    if (argc > 1) {
        try {
            const wad::Wad_file wad_file{std::string{argv[1]}};

            cout << "Loaded:\t" << wad_file.filename() << '\n';
            cout << wad_file;
        }
        catch (const std::exception& e) {
            cerr << e.what();
            return 1;
        }
    }

    return 0;
}
