/**
 * @file  Main.cpp
 * @author Sherry Lynch (or Shy07) <Gernischt@gmail.com>
 * @date ‎2013‎/‎3‎/12‎ 11:58:09
 *
 *  Copyright  2013  Sherry Lynch (or Shy07) <Gernischt@gmail.com>
 *
 */
#include <Windows.h>
#include <stdio.h>

extern int SINRGE2Entry();
static const wchar_t * pDefaultLibrary	= L"SINRGE2.dll";
static const wchar_t * pDefaultTitle	= L"SINRGE2";

static void ShowErrorMsg(HWND hWnd, const wchar_t * pDefaultTitle, const wchar_t * szFormat, ...)
{
	static wchar_t szError[1024];

	va_list ap;
	va_start(ap, szFormat);
	vswprintf_s(szError, szFormat, ap);
	va_end(ap);

	MessageBoxW(hWnd, szError, pDefaultTitle, MB_ICONERROR);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wchar_t szAppPath[MAX_PATH], szIniPath[MAX_PATH], szConsole[MAX_PATH];//szLibrary[MAX_PATH], 
	//HMODULE hSINRGE2 = NULL;

	// app路径
	DWORD len = ::GetModuleFileNameW(hInstance, szAppPath, MAX_PATH);
	for (--len; len > 0; --len)
	{
		if (szAppPath[len] == L'\\' || szAppPath[len] == L'/')
		{
			szAppPath[len] = 0;
			break;
		}
	}
	::SetCurrentDirectoryW(szAppPath);
	
	// ini文件路径
	len = ::GetModuleFileNameW(hInstance, szIniPath, MAX_PATH);
	szIniPath[len - 1] = L'i';
	szIniPath[len - 2] = L'n';
	szIniPath[len - 3] = L'i';
	
	// ini文件存在
	if (GetFileAttributesW(szIniPath) != INVALID_FILE_ATTRIBUTES)
	{
		//GetPrivateProfileStringW(L"Setting", L"Library", pDefaultLibrary, szLibrary, MAX_PATH, szIniPath);
		GetPrivateProfileStringW(L"Setting", L"Console", L"", szConsole, MAX_PATH, szIniPath);
	}
	else
	{
		//wcscpy_s(szLibrary, pDefaultLibrary);
		wcscpy_s(szConsole, L"");
	}

	////	控制台
	if (szConsole[0] == '1')
	{
		if (AllocConsole())
		{
			SetConsoleTitle(L"SINRGE2 Console");
			FILE * frw = NULL;
			freopen_s(&frw, "conout$", "w", stdout);
		}
	}

	//// 加载SINRGE2核心库
	/*hSINRGE2 = ::LoadLibraryW(szLibrary);
	if (!hSINRGE2)
	{
		DWORD e = ::GetLastError();
		ShowErrorMsg(NULL, pDefaultTitle, L"加载 SINRGE2 核心库失败 %s。", szLibrary);
		return 0;
	}

	typedef int	(*SINRGE2Entry)();

	SINRGE2Entry pSINRGE2Entry = NULL;

	pSINRGE2Entry = (SINRGE2Entry)::GetProcAddress(hSINRGE2, "SINRGE2Entry");
	if (!pSINRGE2Entry)	
	{
		ShowErrorMsg(NULL, pDefaultTitle, L"获取 SINRGE2 核心库导出函数失败 %s。", "SINRGE2Entry");
		goto __exit;
	}
	*/
	//(void)pSINRGE2Entry();
	SINRGE2Entry();

__exit:
	/*{
		if (hSINRGE2 != NULL)
			::FreeLibrary(hSINRGE2);
	}*/

	return 0;

}