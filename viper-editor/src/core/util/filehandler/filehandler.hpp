#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace Viper::Util
{
    class FileHandler
    {
    public:
        FileHandler(const std::string &FilePath) : FilePath(FilePath){};
        ~FileHandler(){};

        std::string ReadFile();

    private:
        std::string FilePath;
    };
}