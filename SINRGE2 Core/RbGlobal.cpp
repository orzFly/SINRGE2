/**
 * @file  RbGlobal.cpp
 * @author Sherry Lynch (or Shy07) <Gernischt@gmail.com>
 * @date 2013‎/3‎/‎12‎ 12:40:48
 *
 *  Copyright  2013  Sherry Lynch (or Shy07) <Gernischt@gmail.com>
 *
 */
#include "RbExport.h"
#include "SINRGE2.h"

namespace Sin
{
	VALUE rb_mSin = Qnil;
	//VALUE rb_mNge = Qnil;
	
	VALUE rb_eSinError;

	void InitRbGlobal()
	{
		///<	定义Sin顶层模块
		rb_mSin = rb_define_module("SINRGE2");
		//rb_mNge = rb_define_module_under(rb_mSin, "NGE");
		
		rb_define_const(rb_mSin, "SINRGE2_COPYRIGHT",		rb_str_freeze(rb_str_new2(SIN_COPYRIGHT)));
		rb_define_const(rb_mSin, "SINRGE2_DESCRIPTION",		rb_str_freeze(rb_str_new2(SIN_DESCRIPTION)));
		rb_define_const(rb_mSin, "SINRGE2_ENGINE",			rb_str_freeze(rb_str_new2(SIN_ENGINE)));
		rb_define_const(rb_mSin, "SINRGE2_RELEASE_DATE",	rb_str_freeze(rb_str_new2(SIN_RELEASE_DATE)));
		rb_define_const(rb_mSin, "SINRGE2_THANKS",			rb_str_freeze(rb_str_new2(SIN_THANKS)));
		rb_define_const(rb_mSin, "SINRGE2_VERSION",			rb_str_freeze(rb_str_new2(SIN_VERSION)));
		
		rb_eSinError = rb_define_class_under(rb_mSin, "SINRGE2Error", rb_eStandardError);
		//bind_input();
	}
}