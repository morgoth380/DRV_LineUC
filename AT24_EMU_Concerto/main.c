#define	DEFINE_MAIN_GLOBAL

#include "windows.h"


#include "GlobalVar.h"


GlobalVar GlobalM4;



//#include "stdafx.h"
//void __stdcall InitDllIFace(void);

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	return 1;
}
*/

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return 1;
}
