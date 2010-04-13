#ifndef MIPMAPS2D_INCLUDED
#define MIPMAPS2D_INCLUDED

#include "texture.hpp"

class textureManager;

class mipmaps2d : public texture
{
	friend class textureManager;

private:
	void load(
		std::string const & szMipmap1, 
		std::string const & szMipmap2, 
		std::string const & szMipmap3, 
		std::string const & szMipmap4, 
		std::string const & szMipmap5);

	mipmaps2d(
		std::string const & szMipmap1, 
		std::string const & szMipmap2, 
		std::string const & szMipmap3, 
		std::string const & szMipmap4, 
		std::string const & szMipmap5);

	virtual ~mipmaps2d();
};

#endif //MIPMAPS2D_INCLUDED
