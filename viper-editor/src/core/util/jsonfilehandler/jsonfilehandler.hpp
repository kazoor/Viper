#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace Viper::Util {
    class JSONFileHandler {
    public:
        JSONFileHandler() = default;

        nlohmann::json Read(const std::string &FilePath) const;

        void Write(const std::string &FilePath, const nlohmann::json &JSONObj);
    };
}