#ifndef TEXTURE_MANAGER_INCLUDED
#define TEXTURE_MANAGER_INCLUDED

#include "window_sdl.hpp"
#include "texture_2d.hpp"
#include "mipmaps_2d.hpp"

class textureManager
{
private:
	static textureManager * Instance;

	typedef std::map<std::string, texture *> objects;
	typedef std::map<std::string, unsigned int> counts;

	objects Objects;
	counts Counts;

	textureManager();
	~textureManager();

public:
	typedef objects::iterator iterator;
	typedef objects::const_iterator const_iterator;

	static textureManager * instance();
	static void destroy();

	texture2d * createTexture2D(
		std::string const & Filename);

	mipmaps2d * createMipmaps2D(
		std::string const & Mipmap1, 
		std::string const & Mipmap2, 
		std::string const & Mipmap3, 
		std::string const & Mipmap4, 
		std::string const & Mipmap5);

	bool release(texture * Texture);
};

#endif //TEXTURE_MANAGER_INCLUDED
