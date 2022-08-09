#pragma once
#include <util/timer/timestep.hpp>
#include <vector>
#include <string>

namespace Viper {
    class SceneFilexplorer {
    public:
        SceneFilexplorer() = default;

        void OnImGuiRender( Timestep::Timestep ts );
        void OnImGuiConsole( Timestep::Timestep ts );
    private:
        struct FileManager_t {
            std::string filename;
            std::string fileext;
            std::string relative_path;
        };
        void OnListFiles( const std::vector< FileManager_t >& files_in_directory );
    };
};