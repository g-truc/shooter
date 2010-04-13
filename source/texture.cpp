#include "texture.hpp"
#include "window_sdl.hpp"

texture::texture()
{
	glGenTextures(1, &this->Name);
}

texture::~texture()
{
	glDeleteTextures(1, &this->Name);
}

void texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->Name);
}

const char* texture::filename() const
{
	return this->Filename.c_str();
}

