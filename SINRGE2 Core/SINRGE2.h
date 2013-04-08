#ifndef __SINRGE2_H__
#define __SINRGE2_H__
#include <Windows.h>
#include "hge.h"
#include "sin_color.h"

typedef struct __SinFrameStruct
{
	bool			m_fullscreen_start;
	bool			m_forbid_fullscreen;
	bool			m_forbid_switch;

	unsigned long	m_screen_width;
	unsigned long	m_screen_height;

	HWND			m_hwnd;

	__SinFrameStruct()
		: m_fullscreen_start(false), m_forbid_fullscreen(false), m_forbid_switch(false),
			m_screen_width(0), m_screen_height(0), m_hwnd(NULL)
	{
	}

	void Default()
	{
		m_screen_width	= 800;
		m_screen_height = 600;
	}

} SinFrameStruct;

namespace Sin
{
	SinFrameStruct*	GetFrmStructPtr();
	HGE*			GetHgePtr();
	void			CreateHge();
	bool			RenderProc();

	HWND			GetHwnd();
	//char*			GetTitle();
	wchar_t*		GetTitleW();
	void			SINRGE2Initialize();
	int				SINRGE2Eval(const char* script);
	int				SINRGE2Entry(const char* rubyfile);
	void			SetRubyLibHandle(HINSTANCE hModule);
}

#endif	//	__SINRGE2_H__