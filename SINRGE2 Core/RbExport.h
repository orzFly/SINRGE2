#ifndef __SIN_RB_EXPORT_H__
#define __SIN_RB_EXPORT_H__
extern "C"
{
#include "ruby.h"
#include "ruby/encoding.h"
}
#include "Kconv.h"
#include <string>

namespace Sin
{
	typedef VALUE(*RbFunc)(...);

	extern VALUE rb_mSin;
	
	void InitRbGlobal();
	void InitNGE();
	void InitSeal();

	extern "C"
	{

	void	Init_zlib();
	void	ruby_Init_Fiber_as_Coroutine(void);

	}

}

#endif	//	__SIN_RB_EXPORT_H__
