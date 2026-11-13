#include"Texture.h"

#include <stdexcept>
#include <string>

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of the texture to the texture object
	type = texType;
	// Texture unit this texture will be sampled from
	unit = slot;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it as bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 4);
	if (!bytes)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + image +
			" (" + stbi_failure_reason() + ")");
	}

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);

	// Selects the texture unit and binds the new texture before setting parameters
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Assigns the image to the OpenGL texture object
	glTexImage2D(type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generates MipMaps
	glGenerateMipmap(type);

	// Deletes the image data as it is already in the OpenGL texture object
	stbi_image_free(bytes);

	// Unbinds the OpenGL texture object so that it can't accidentally be modified
	glBindTexture(type, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform to this texture's unit
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}
