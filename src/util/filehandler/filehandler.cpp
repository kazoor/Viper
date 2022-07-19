#include <sstream>
#include "filehandler.hpp"

namespace Viper::Util
{
    std::string FileHandler::ReadFile()
    {
        std::ifstream InputFile(FilePath);
        if (!InputFile.is_open()) {
            std::cerr << "Could not open the file - '"
                 << FilePath << "'" << std::endl;
        }

        return std::string((std::istreambuf_iterator<char>(InputFile)), std::istreambuf_iterator<char>());
    }
}