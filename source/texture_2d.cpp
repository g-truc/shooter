#include "texture_2d.hpp"
#include "texture_manager.hpp"
#include "window_sdl.hpp"

texture2d::texture2d
(
	std::string const & Filename
)
{
	this->load(Filename);
}

texture2d::~texture2d()
{}

void texture2d::load
(
	std::string const & Filename
)
{
	this->Filename = Filename;

	gli::image Image = gli::loadImage(Filename);
	assert(Image.levels() > 0);

	this->bind();
	// 03/03/2004 - Indique le filtre qui sera appliqué à la texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 03/03/2004 - Indique comment la texture sera appliqué en decors des coordonnées de la texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

	// 03/03/2004 - Image.Width () : Retourne la largeur de l'image
	// 03/03/2004 - Image.Height () : Retourne la hauteur de l'image
	// 03/03/2004 - Image.Data () : Retourne un pointeur vers les données de l'image
	switch(Image[0].value_size())
	{
	default:
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Image[0].width(), Image[0].height(), 0, GL_RGB, GL_UNSIGNED_BYTE, Image[0].data());
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Image[0].width(), Image[0].height (), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image[0].data());
		break;
	}
}
