#ifndef WINDOW_H
#define WINDOW_H

#include "util.hpp"

class CWindow
{
protected:
	unsigned short m_unHeight;
	unsigned short m_unWidth;
	unsigned char m_ucColor;
	unsigned char m_ucFFSA;
	bool m_bFullscreen;
	bool m_bCreate;

	CWindow ();
	virtual ~CWindow ();

public:
	virtual void Swap () = 0;
	virtual bool Create (const char* szName, unsigned short unWidth, unsigned short unHeight, unsigned char ucColor, bool bFullscreen) = 0;

	bool IsCreate () const;
	unsigned short GetHeight () const;
	unsigned short GetWidth () const;
	unsigned char GetColor () const;
	unsigned char GetFFSA () const;
	bool GetFullscreen () const;
};

#endif //WINDOW_H
