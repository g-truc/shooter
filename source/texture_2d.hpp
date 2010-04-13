#ifndef TEXTURE2D_INCLUDED
#define TEXTURE2D_INCLUDED

#include "texture.hpp"

class textureManager;

class texture2d : public texture
{
	friend class textureManager;

private:
	texture2d(std::string const & Filename);
	virtual ~texture2d();

	void load(std::string const & Filename);
};

#endif //TEXTURE2D_INCLUDED
