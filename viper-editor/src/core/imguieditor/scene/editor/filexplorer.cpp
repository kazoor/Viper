#include "filexplorer.hpp"
#include <ImGui/imgui.h>
#include <imguieditor/fontawesome5.hpp>

#include <ghc/filesystem.hpp>

#include <util/globals/global.hpp>
#include <util/timer/timer.hpp>

#include <vector>

static const size_t hash_png = std::hash< std::string >( )(".png");
static const size_t hash_jpg = std::hash< std::string >( )(".jpg");
static const size_t hash_vert = std::hash< std::string >( )(".vert");
static const size_t hash_frag = std::hash< std::string >( )(".frag");
static const size_t hash_ini = std::hash< std::string >( )(".init");
static const size_t hash_font_ttf = std::hash< std::string >( )( ".ttf" );
static const size_t hash_font_otf = std::hash< std::string >( )( ".otf" );

struct FileManager_t {
    std::string filename;
    std::string fileext;
};

constexpr const char* s_Directory = "resources";

namespace Viper::Editor {
    void SceneFilexplorer::OnImGuiRender( Timestep::Timestep ts ) {
        if( ImGui::Begin("File Explorer")) {
            Timer::Timer ts;

            static ghc::filesystem::path m_CurrentDir = s_Directory;
            std::vector< FileManager_t > m_FilesWithinFolder;
            ImGui::Columns(2, "##FileExplorer");
            ImGui::SetColumnWidth(0, 150.0f);
            if( ghc::filesystem::exists(m_CurrentDir) ) {
                for(auto& p : ghc::filesystem::directory_iterator(m_CurrentDir)) {
                    std::string path = p.path().string();
                    auto filename = p.path().filename().string();
                    auto filename_ext = p.path().filename().extension().string();

                    if( p.is_directory()) {
                        if(ImGui::Button(
                            VIPER_FORMAT_STRING( ICON_FA_FOLDER " %s", filename.c_str( ) ).c_str( )
                            )) {
                            m_CurrentDir /= p.path().filename();
                        };
                    } else {
                        m_FilesWithinFolder.push_back( { filename, filename_ext } );
                    }
                };
            }
            ImGui::NextColumn();
            if( m_CurrentDir != s_Directory ) {
                if( !ghc::filesystem::exists(m_CurrentDir) )
                    m_CurrentDir = m_CurrentDir.parent_path( );

                if(ImGui::Button(ICON_FA_ARROW_CIRCLE_LEFT " Back")) {
                    m_CurrentDir = m_CurrentDir.parent_path();
                }
            };

            for( auto f : m_FilesWithinFolder ) {
                const size_t m_file_hash = std::hash< std::string >( )( f.fileext );

                std::string sz_StringSerialized;
                if( m_file_hash == hash_frag || m_file_hash == hash_vert )
                    sz_StringSerialized = VIPER_FORMAT_STRING(ICON_FA_WHEELCHAIR " %s :: %s", f.filename.c_str(), f.fileext.c_str());//ImGui::Text(ICON_FA_WHEELCHAIR " %s :: %s", f.filename.c_str(), f.fileext.c_str());
                else if( m_file_hash == hash_png || m_file_hash == hash_jpg )
                    sz_StringSerialized = VIPER_FORMAT_STRING( ICON_FA_FILE_IMAGE " %s :: %s", f.filename.c_str(), f.fileext.c_str() );//ImGui::Text(ICON_FA_FILE_IMAGE " %s :: %s", f.filename.c_str(), f.fileext.c_str());
                else if( m_file_hash == hash_font_ttf || m_file_hash == hash_font_otf )
                    sz_StringSerialized = VIPER_FORMAT_STRING( ICON_FA_FONT " %s :: %s", f.filename.c_str(), f.fileext.c_str() );//ImGui::Text(ICON_FA_FONT " %s :: %s", f.filename.c_str(), f.fileext.c_str());
                else
                    sz_StringSerialized = VIPER_FORMAT_STRING( ICON_FA_FILE " %s :: %s", f.filename.c_str(), f.fileext.c_str() );//ImGui::Text(ICON_FA_FILE " %s :: %s", f.filename.c_str(), f.fileext.c_str());
            
                ImGui::Text(sz_StringSerialized.c_str());
            };
            ImGui::Columns(1);
            
            ImGui::End();
        };

        OnImGuiConsole( ts ); // <!-- Creating a new file for this would be necessary. --!>
    };

    // <!-- Creating a new file for this would be necessary. --!>
    void SceneFilexplorer::OnImGuiConsole( Timestep::Timestep ts ) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 8.0f));
            if(ImGui::Begin(ICON_FA_TERMINAL "  Debug Console")) {
                for( auto info : Globals::ConsoleContext::GetLogs( ) ) {
                    auto color = Globals::ConsoleContext::GetConsoleColor( info.Flag );
                    ImGui::TextColored(ImVec4(color[0], color[1], color[2], 1.0f), info.StringType.c_str( ) );
                    ImGui::Text(info.ConsoleMessage.c_str( ) );
                    ImGui::Separator();
                }
                ImGui::End();
            };
            ImGui::PopStyleVar();
    }
};