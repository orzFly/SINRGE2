#ifndef _MODULE_RB_INPUT_H_
#define _MODULE_RB_INPUT_H_

#include "RbClassBase.h"

struct MRbInput
{
	static void	InitLibrary();

	static bool OnFocus();
	static int MouseWheel();
	static int MouseDblClk(int iKey);
	static int GetMouseMove();
	static void HideMouse(bool hide);

	static VALUE show_mouse(int argc, VALUE show);
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
};

#endif //_RB_INPUT_H_