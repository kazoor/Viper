#include <sstream>
#include <spdlog/spdlog.h>
#include "filehandler.hpp"

namespace Viper::Util
{
    std::string FileHandler::ReadFile()
    {
        std::ifstream InputFile(FilePath);
        if (!InputFile.is_open()) {
            spdlog::error("Failed to open file: ", FilePath);
        }

        return std::string((std::istreambuf_iterator<char>(InputFile)), std::istreambuf_iterator<char>());
    }
}