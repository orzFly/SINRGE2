/*
** SINRGE2
** Copyright (C) 2013 Syalon, Shy07
** Gernischt@gmail.com
**
** Ruby Moudle Input
*/
#ifndef _MODULE_RB_INPUT_H_
#define _MODULE_RB_INPUT_H_

#include "RbExport.h"

#define VK_0               0x30
#define VK_1               0x31
#define VK_2               0x32
#define VK_3               0x33
#define VK_4               0x34
#define VK_5               0x35
#define VK_6               0x36
#define VK_7               0x37
#define VK_8               0x38
#define VK_9               0x39

#define VK_A               0x41
#define VK_B               0x42
#define VK_C               0x43
#define VK_D               0x44
#define VK_E               0x45
#define VK_F               0x46
#define VK_G               0x47
#define VK_H               0x48
#define VK_I               0x49
#define VK_J               0x4A
#define VK_K               0x4B
#define VK_L               0x4C
#define VK_M               0x4D
#define VK_N               0x4E
#define VK_O               0x4F
#define VK_P               0x50
#define VK_Q               0x51
#define VK_R               0x52
#define VK_S               0x53
#define VK_T               0x54
#define VK_U               0x55
#define VK_V               0x56
#define VK_W               0x57
#define VK_X               0x58
#define VK_Y               0x59
#define VK_Z               0x5A

struct MRbInput
{
	static void	InitLibrary();

	static bool OnFocus();
	static int MouseWheel();
	static int MouseDblClk(int iKey);
	//static int GetMouseMove();

	static VALUE on_focus();
	static VALUE mouse_wheel();
	static VALUE mouse_dblclk(int argc, VALUE key);
	static VALUE mouse_over();
	static VALUE get_mouse_pos();
	static VALUE update_input();
	static VALUE is_press(int argc, VALUE key);
	static VALUE is_trigger(int argc, VALUE key);
	static VALUE is_repeat(int argc, VALUE key);
	static VALUE is_click(int argc, VALUE key);
	static VALUE get_dir4();
	static VALUE get_dir8();
	
	static VALUE get_character();
};

#endif //_RB_INPUT_H_