/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Image Header
*/
#ifndef __SIN_IMAGE_H__
#define __SIN_IMAGE_H__

#ifdef WIN32
#pragma once
#endif

#include "hge.h"

typedef struct tagImage
{
	hgeQuad		quad;
	int			width;	/**< ͼƬ��ͼ�� */
	int			height;	/**< ͼƬ��ͼ�� */
	int			owidth;    /**< ͼƬԭ�� */
	int			oheight;   /**< ͼƬԭ�� */
	//float		rcentrex; /**< ͼ������x */
	//float		rcentrey; /**< ͼ������y */
} image_t, * image_p;

#endif //__SIN_IMAGE_H__