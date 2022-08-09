#include "filexplorer.hpp"
#include "../fontawesome5.hpp"

#include <ImGui/imgui.h>
#include <viper/base.hpp>

#include <ghc/filesystem.hpp>

#include <util/globals/global.hpp>
#include <util/timer/timer.hpp>
#include <unordered_map>

constexpr const uint32_t fnv1_basis = 0x811C9DC5;
constexpr const uint32_t fnv1_prime = 0x1000193;

constexpr uint32_t hash32( const char* string, const uint32_t basis = fnv1_basis ) {
	return ( string[ 0 ] == '\0' ) ? basis : hash32( &string[ 1 ], ( basis ^ string[ 0 ] ) * fnv1_prime );
};

uint32_t hash32_runtime( const char* string ) { uint32_t m_hash = fnv1_basis;
	for ( std::size_t i = 0; i < strlen( string ); i++ ) {
		m_hash ^= string[ i ];
		m_hash *= fnv1_prime;
	};
	return m_hash;
};

constexpr const char* s_Directory = "resources";

namespace Viper {
    void SceneFilexplorer::OnImGuiRender( Timestep::Timestep ts ) {
        if( ImGui::Begin("File Explorer")) {
            static ghc::filesystem::path m_CurrentDir = s_Directory;
            std::vector< FileManager_t > m_FilesWithinFolder;
            ImGui::Columns(2, "##FileExplorer");
            ImGui::SetColumnWidth(0, 150.0f);
            if( ghc::filesystem::exists(m_CurrentDir) ) {
                for(auto& p : ghc::filesystem::directory_iterator(m_CurrentDir)) {
                    std::string path = p.path().string();
                    auto filename = p.path().filename().string();
                    auto filename_ext = p.path().filename().extension().string();
                    auto filename_rel = p.path().relative_path().string();

                    if( p.is_directory()) {
                        if(ImGui::Button(
                            VIPER_FORMAT_STRING( ICON_FA_FOLDER " %s", filename.c_str( ) ).c_str( )
                            )) {
                            m_CurrentDir /= p.path().filename();
                        };
                    } else {
                        m_FilesWithinFolder.push_back( { filename, filename_ext, filename_rel } );
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
            OnListFiles(m_FilesWithinFolder);
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
    };

    void SceneFilexplorer::OnListFiles( const std::vector< FileManager_t >& files_in_directory ) {
        for( const auto file : files_in_directory ) {
            const auto m_file_hash = hash32_runtime( file.fileext.c_str( ) );
            switch(m_file_hash) {
                case hash32(".ini"): break;

                case hash32(".png"):
                case hash32(".jpg"): ImGui::Text("%s", VIPER_FORMAT_STRING( ICON_FA_FILE_IMAGE " %s :: image", file.filename.c_str(), file.fileext.c_str() ).c_str()); break;

                case hash32(".glsl"):
                case hash32(".sprv"):
                case hash32(".frag"):
                case hash32(".vert"): ImGui::Text("%s", VIPER_FORMAT_STRING(ICON_FA_WHEELCHAIR " %s :: shader", file.filename.c_str(), file.fileext.c_str()).c_str()); break;

                case hash32(".ttf"):
                case hash32(".otf"): ImGui::Text("%s", VIPER_FORMAT_STRING( ICON_FA_FONT " %s :: font", file.filename.c_str(), file.fileext.c_str() ).c_str()); break;

                case hash32(".json"): ImGui::Text("%s", VIPER_FORMAT_STRING( ICON_FA_CUBE " %s :: scene", file.filename.c_str(), file.fileext.c_str() ).c_str()); break;

                default:
                    ImGui::Text("%s", VIPER_FORMAT_STRING( ICON_FA_FILE " %s :: %s", file.filename.c_str(), file.fileext.c_str() ).c_str());
                    break;
            };
        };

        /*
        ImGui::Text("%s -> rel: %s", sz_StringSerialized.c_str(), f.relative_path.c_str() );
        ImGui::PushID(imgui_next_id++);
        ImGui::Button("DragSource");
        if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
            const char* relative_path_location = f.relative_path.c_str( );
            ImGui::SetDragDropPayload("FILE_EXPLORER_DRAG_DROP", relative_path_location, ( strlen(relative_path_location) + 1 ) * sizeof( char* ) );
            ImGui::EndDragDropSource();
        };
        ImGui::PopID();*/
    };
};