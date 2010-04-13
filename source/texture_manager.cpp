#include "texture_manager.hpp"

textureManager * textureManager::Instance = 0;

textureManager::textureManager()
{}

textureManager::~textureManager()
{
	for(objects::iterator it = this->Objects.begin (); it != this->Objects.end (); ++it)
		delete it->second;
}

textureManager * textureManager::instance()
{
	if(Instance == 0)
		Instance = new textureManager;
	return Instance;
}

void textureManager::destroy()
{
	if(Instance != 0)
	{
		delete Instance;
		Instance = 0;
	}
}

texture2d * textureManager::createTexture2D
(
	std::string const & Filename
)
{
	texture2d * Texture = 0;

	objects::iterator ItTexture = this->Objects.find(Filename);
	if(ItTexture != this->Objects.end())
	{
		Texture = static_cast<texture2d *>(ItTexture->second);
		counts::iterator ItNumber = this->Counts.find(Filename);
		ItNumber->second++;
	}
	else
	{
		Texture = new texture2d(DIRECTORY + Filename);
		this->Objects.insert(std::make_pair(Filename, Texture));
		this->Counts.insert(std::make_pair(Filename, 1));
	}

	return Texture;
}

mipmaps2d * textureManager::createMipmaps2D
(
	std::string const & Mipmap1, 
	std::string const & Mipmap2, 
	std::string const & Mipmap3, 
	std::string const & Mipmap4, 
	std::string const & Mipmap5
)
{
	mipmaps2d * Texture = 0;

	objects::iterator ItTexture = this->Objects.find(Mipmap1);
	if(ItTexture != this->Objects.end())
	{
		counts::iterator ItNumber = this->Counts.find(Mipmap1);
		Texture = static_cast<mipmaps2d*>(ItTexture->second);
		ItNumber->second++;
	}
	else
	{
		Texture = new mipmaps2d(
			DIRECTORY + Mipmap1, 
			DIRECTORY + Mipmap2, 
			DIRECTORY + Mipmap3, 
			DIRECTORY + Mipmap4, 
			DIRECTORY + Mipmap5);

		this->Objects.insert(std::make_pair(Mipmap1, Texture));
		this->Counts.insert(std::make_pair(Mipmap1, 1));
	}

	return Texture;
}

bool textureManager::release(texture * Texture)
{
	if(!Texture)
		return false;

	objects::iterator ItTexture = this->Objects.find(Texture->filename());
	if(ItTexture == this->Objects.end())
		return false;

	counts::iterator ItNumber = this->Counts.find(Texture->filename());
	if(ItNumber == this->Counts.end())
		return false;

	ItNumber->second--;
	if(ItNumber->second <= 0)
	{
		delete ItTexture->second;
		this->Objects.erase(ItTexture);
		this->Counts.erase(ItNumber);
	}

	return true;
}
