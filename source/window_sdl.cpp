#include "window_sdl.hpp"
#include "util.hpp"
#include <cstdio>

CWindowSDL* CWindowSDL::m_pInstance = 0;

CWindowSDL::CWindowSDL () :
	m_pSurface(0),
	m_fTime(0)
{}

CWindowSDL::~CWindowSDL ()
{
	if (m_pSurface)
		SDL_FreeSurface (m_pSurface);
}

CWindowSDL* CWindowSDL::Instance ()
{
	if (m_pInstance == 0)
		m_pInstance = new CWindowSDL;
	return m_pInstance;
}

void CWindowSDL::Kill ()
{
	if (m_pInstance != 0)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

bool CWindowSDL::IsCreate ()
{
	return m_pInstance != 0;
}

void CWindowSDL::Swap ()
{
	m_fTime = SDL_GetTicks () / 1000.f;

	SDL_GL_SwapBuffers ();
}

bool CWindowSDL::Create (const char* szName, unsigned short unWidth, unsigned short unHeight, unsigned char ucColor, bool bFullscreen)
{
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
        printf ("Impossible d'initializing SDL Video : %s", SDL_GetError ());
        return false;
    }

	// Utilise OpenGL avec SDL
	// Utilise le double buffering
	// Pas d'approximations pour les couleurs en mode 8 bits
	// Accepte le redimensionnement de la fenêtre
	// SDL_NOFRAME : Pas de cadre à la fenetre
	// SDL_RESIZABLE : Taille de la fenetre modifiable
	unsigned int uiVideoFlags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWPALETTE;

	if (bFullscreen)
		uiVideoFlags |= SDL_FULLSCREEN;

	const SDL_VideoInfo *pVideoInfo = SDL_GetVideoInfo ();

	// Allocation de la surface en mémoire video
	if (pVideoInfo->hw_available)
		uiVideoFlags |= SDL_HWSURFACE;
	else
		uiVideoFlags |= SDL_SWSURFACE;

	if (pVideoInfo->blit_hw)
		uiVideoFlags |= SDL_HWACCEL;

	// initialisation du double buffering pour OpenGL
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute (SDL_GL_ACCUM_RED_SIZE, 0);
    SDL_GL_SetAttribute (SDL_GL_ACCUM_GREEN_SIZE, 0);
    SDL_GL_SetAttribute (SDL_GL_ACCUM_BLUE_SIZE, 0);
    SDL_GL_SetAttribute (SDL_GL_ACCUM_ALPHA_SIZE, 0);

	if ((m_pSurface = SDL_SetVideoMode(unWidth, unHeight, ucColor, uiVideoFlags)) == 0)
	{
		return false;
	}

	glewInit();

    SDL_WM_SetCaption(szName, szName);
	_Resize(unWidth, unHeight);
    SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void CWindowSDL::_Resize(unsigned short unWidth, unsigned short unHeight)
{
	if (unHeight == 0)
		unHeight = 1;
	glm::mat4 Perspective = glm::perspective(45.f, (float)unWidth / (float)unHeight, 0.1f, 512.f);

    glViewport (0, 0, unWidth, unHeight);
    glMatrixMode (GL_PROJECTION);
    glLoadMatrixf (&Perspective[0][0]);
	
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

