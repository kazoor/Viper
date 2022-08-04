#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "jsonfilehandler.hpp"

namespace Viper::Util {
    nlohmann::json JSONFileHandler::Read(const std::string &FilePath) {
        std::ifstream InStream(FilePath);

        // Use nlohmann's built in function to 
        return nlohmann::json::parse(InStream);
    }

    void JSONFileHandler::Write(const std::string &FilePath, const nlohmann::json& JSONObj) {
        std::ofstream OutStream (FilePath);

        OutStream << JSONObj;

        OutStream.close();
    } 
}