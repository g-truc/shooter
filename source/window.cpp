#include "window.hpp"

CWindow::CWindow ()
{

}

CWindow::~CWindow ()
{

}

bool CWindow::IsCreate () const
{
	return m_bCreate;
}

unsigned short CWindow::GetHeight () const
{
	return m_unHeight;
}

unsigned short CWindow::GetWidth () const
{
	return m_unWidth;
}

unsigned char CWindow::GetColor () const
{
	return m_ucColor;
}

unsigned char CWindow::GetFFSA () const
{
	return m_ucFFSA;
}

bool CWindow::GetFullscreen () const
{
	return m_bFullscreen;
}


