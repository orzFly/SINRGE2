/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Class Viewport
*/
#ifndef __C_RB_VIEWPORT_H__
#define __C_RB_VIEWPORT_H__

#include "CRbDisplayObject.h"

class CRbRect;
class CRbColor;
class CRbTone;

class CRbViewport : public DisplayObject
{
public:
	CRbViewport();
	virtual ~CRbViewport();

public:
	static void			InitLibrary();

public:
	RbRenderNodePtr *	GetHeadPtr() { return &m_head; }
	RbRenderNodePtr *	GetTailPtr() { return &m_tail; }
	const CRbRect *		GetRectPtr() const { return m_rect_ptr; }

protected:
	virtual void		mark();
	virtual VALUE		initialize(int argc, VALUE *argv, VALUE obj);

	virtual	void		render(u32 id);
	
	virtual VALUE		get_z();
	virtual VALUE		set_z(VALUE z);

protected:
	int					m_flash_duration;					//	�ӿ���˸֡��
	int					m_flash_reduce_count_per_frame;		//	�ӿ���˸ÿ֡���͵�͸����
	int					m_flash_hide_spr;					//	�Ƿ���ȥ�ӿڣ�������˸��ɫָ��Ϊnilʱ����ȥ�ӿڡ�
	u32					m_flash_color;						//	�ӿ���˸����ɫֵ��

	hgeQuad				m_quad;

	RbRenderNode *		m_node;

	RbRenderNode *		m_tail;
	RbRenderNode *		m_head;

	CRbRect *			m_rect_ptr;
	CRbColor *			m_color_ptr;
	CRbTone *			m_tone_ptr;
	
protected:
	void				check_raise();

protected:
	dm_method(dispose)
	dm_method(is_disposed)

	dm_method(update)
	dm_method02(flash)
	attr_accessor(rect)
	attr_accessor(color)
	attr_accessor(tone)
	
protected:
	static HGE *		s_pHge;
};

#endif