#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include "shader.h"

class Texture
{
    public:
        GLuint ID;
        GLenum type;
        GLuint unit;

        // slot is the texture unit index (0 for GL_TEXTURE0, 1 for GL_TEXTURE1, ...)
        Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

        // Assigns this texture's unit to a sampler uniform
        void texUnit(Shader& shader, const char* uniform);
        // Binds a texture
        void Bind();
        // Unbinds a texture
        void Unbind();
        // Deletes a texture
        void Delete();
};

#endif // TEXTURE_H