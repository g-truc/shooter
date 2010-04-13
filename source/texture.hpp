#ifndef TEXTURE_H
#define TEXTURE_H

#include "util.hpp"

class textureManager;

class texture
{
	friend class textureManager;

public:
    virtual void bind() const;

	char const * filename() const;

protected:
	texture();
	virtual ~texture();

	glm::uint Name;
	std::string Filename;
};

#endif //TEXTURE_H
