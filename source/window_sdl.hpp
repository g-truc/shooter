#ifndef WINDOW_SDL_H
#define WINDOW_SDL_H

#ifdef WIN32
#include <windows.h>
#endif //WIN32

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL.h>

#include "window.hpp"

class CWindowSDL : public CWindow
{
	static CWindowSDL* m_pInstance;

private:
	SDL_Surface* m_pSurface;
	float m_fTime;

	void _Resize (unsigned short unWidth, unsigned short unHeight);

	CWindowSDL ();
	~CWindowSDL ();

public:
	static CWindowSDL* Instance ();
	static void Kill ();
	static bool IsCreate ();

	virtual void Swap ();
	virtual bool Create (const char* szName, unsigned short unWidth, unsigned short unHeight, unsigned char ucColor, bool bFullscreen);

	float GetTime() const{return m_fTime;}
};

#endif //WINDOW_SDL_H
