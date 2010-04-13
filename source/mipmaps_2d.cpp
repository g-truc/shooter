#include "mipmaps_2d.hpp"
#include "texture_manager.hpp"
#include "window_sdl.hpp"

mipmaps2d::mipmaps2d
(
	std::string const & Mipmap1,
	std::string const & Mipmap2,
	std::string const & Mipmap3,
	std::string const & Mipmap4,
	std::string const & Mipmap5
)
{
	this->load(Mipmap1, Mipmap2, Mipmap3, Mipmap4, Mipmap5);
}

mipmaps2d::~mipmaps2d()
{}

void mipmaps2d::load
(
	std::string const & Mipmap1,
	std::string const & Mipmap2,
	std::string const & Mipmap3,
	std::string const & Mipmap4,
	std::string const & Mipmap5
)
{
	this->Filename = Mipmap1;

	gli::image Image1 = gli::loadImage(Mipmap1);
	gli::image Image2 = gli::loadImage(Mipmap2);
	gli::image Image3 = gli::loadImage(Mipmap3);
	gli::image Image4 = gli::loadImage(Mipmap4);
	gli::image Image5 = gli::loadImage(Mipmap5);

	assert(Image1.levels());
	assert(Image2.levels());
	assert(Image3.levels());
	assert(Image4.levels());
	assert(Image5.levels());

	assert(Image1[0].value_size() == 3);

	this->bind();
	// 03/03/2004 - Indique le filtre qui sera appliqué à la texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 03/03/2004 - Indique comment la texture sera appliqué en decors des coordonnées de la texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

	// 14/03/2004 - Le second paramètre de la fonction glTexImage2D () indique le numero de mipmap. Plus le numéro est grand, plus la mipmap sera utilisé pour une distance lointaine par rapport au point de vue
	// 14/03/2004 - Usuellement, pour chaque incrément du numéro de mipmap, la texture est 2 fois plus petite. Personnaliser les mipmaps permet d'outrepasser cette règle.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image1[0].width(), Image1[0].height(), 0, GL_BGR, GL_UNSIGNED_BYTE, Image1[0].data());
	glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, Image2[0].width(), Image2[0].height(), 0, GL_BGR, GL_UNSIGNED_BYTE, Image2[0].data());
	glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, Image3[0].width(), Image3[0].height(), 0, GL_BGR, GL_UNSIGNED_BYTE, Image3[0].data());
	glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, Image4[0].width(), Image4[0].height(), 0, GL_BGR, GL_UNSIGNED_BYTE, Image4[0].data());
	glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, Image5[0].width(), Image5[0].height(), 0, GL_BGR, GL_UNSIGNED_BYTE, Image5[0].data());
}
