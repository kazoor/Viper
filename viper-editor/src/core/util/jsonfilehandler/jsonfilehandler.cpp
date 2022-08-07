#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "jsonfilehandler.hpp"

namespace Viper::Util {
    nlohmann::json JSONFileHandler::Read(const std::string &FilePath) {
        std::ifstream InStream(FilePath);

        // Use nlohmann's built in function to parse
        auto ret = nlohmann::json::parse(InStream);

        InStream.close();
        return ret;
    }

    void JSONFileHandler::Write(const std::string &FilePath, const nlohmann::json& JSONObj) {
        std::ofstream OutStream (FilePath);

        OutStream << JSONObj;

        OutStream.close();
    } 
}