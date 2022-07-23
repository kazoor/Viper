#include <stb/stb_image.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include "../entity.hpp"
#include "../../graphics/shaders/shader/shader.hpp"

namespace Viper::Components {
    class SpriteRenderer2D : public Entity {
    public:
        SpriteRenderer2D(const std::string &SpritePath, glm::vec2 Position, glm::vec2 Size = glm::vec2(10.0f, 10.0f), float Rotate = 0.0f, glm::vec3 Color = glm::vec3(1.0f)) : Position(Position), Size(Size), Rotate(Rotate), Color(Color) {
            glGenTextures(1, &Texture);
            glBindTexture(GL_TEXTURE_2D, Texture);

            // Set texture wrapping to GL_REPEAT (default wrapping method).
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set texture filtering parameters.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Data = stbi_load(SpritePath.c_str(), &Width, &Height &NrChannels, NULL);
            Shader = new Graphics::Shader("resources/spriterenderer2d/spriterenderer2d.vert", "resources/spriterenderer2d/spriterenderer2d.frag");

            if(Data) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else {
                spdlog::error("Failed to load texture! Texture: {0}", SpritePath);
            }
            stbi_image_free(Data);

            Shader->Use();
            Shader->SetInt("Texture", 1);
        }

        ~SpriteRenderer2D() {
            delete Shader;
        }

        void OnUpdate() override {

        }

    private:
        unsigned char *Data;
        int Width, Height, NrChannels;

        Graphics::Shader *Shader;
        unsigned int QuadVAO, Texture;
        glm::vec2 Position;
        glm::vec2 Size;
        float Rotate;
        glm::vec3 Color;
    };
}