#ifndef __CLASS_RGE_BITMAP_H__
#define __CLASS_RGE_BITMAP_H__

#ifdef WIN32
#pragma once
#endif

#include "RbClassBase.h"
#include "sin_bitmap.h"

using namespace Sin;

class RbRect;
class RbFont;

class RbBitmap : public RbClassBase
{
public:
	RbBitmap();
	virtual ~RbBitmap();

public:
	static void				InitLibrary();
	static bool				AdjustTexturesTone(const bitmap_p pBmp, DWORD dwTone);
	static void				ColorSpaceRGB2HSV(int R, int G, int B, float &H, float &S, float &V);
	static void				ColorSpaceHSV2RGB(float H, float S, float V, BYTE &R, BYTE &G, BYTE &B);
	static bool				GetTextRect(HFONT hFont, const wchar_t* pStr, s32 &cx, s32 &cy, HDC hDC);

public:
	u32						GetWidth()					const { return m_bmp.width; }
	u32						GetHeight()					const { return m_bmp.height; }
	u32						GetMemWidth()				const { return m_bmp.texw; }
	u32						GetMemHeight()				const { return m_bmp.texh; }
	bitmap_p				GetBitmapPtr()				{ return &m_bmp; }

	int						GetModifyCount()			const { return m_modify_count; }

	bool					IsDisposed()				const { return m_disposed; }

protected:
	virtual void			mark();
	virtual VALUE			initialize(int argc, VALUE *argv, VALUE obj);

protected:
	void					check_raise();

protected:
	bool					m_disposed;
	bitmap_t				m_bmp;
	int						m_modify_count;

	VALUE					m_filename;		// mark

	RbRect*					m_rect_ptr;
	RbFont*					m_font_ptr;

protected:
	dm_method(dispose)
	dm_method(is_disposed)
	dm_method_vargs(save_to_file)
	dm_method01(hue_change)
	dm_method01(brightness_change)
	dm_method_vargs(tone_change)

	dm_method_vargs(blt)
	dm_method_vargs(stretch_blt)
	dm_method_vargs(fill_rect)
	dm_method(clear)
	dm_method02(get_pixel)
	dm_method03(set_pixel)
	dm_method_vargs(draw_text)
	dm_method01(text_size)

	dm_method_vargs(gradient_fill_rect)
	dm_method_vargs(clear_rect)
	dm_method(blur)
	dm_method02(radial_blur)
	
	dm_method(render)
	dm_method(flip_h)
	dm_method(flip_v)

	attr_reader(rect)
	attr_reader(width)
	attr_reader(height)
	attr_reader(filename)
	attr_accessor(font)
};

#endif	//	__CLASS_RGE_BITMAP_H__