#include "main.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/locale.h>

#include <workbench/startup.h>
#include <workbench/workbench.h>


extern struct Library *SysBase;
extern struct Library *DOSBase;

struct Library *IntuitionBase;
struct Library *LocalSocketBase;


__attribute__((section(".text"))) const char VString[] = "$VER: " APP_NAME " " APP_VER " (" APP_DATE ") ? " APP_CYEARS " " APP_AUTHOR;



int GetResources(void)
{
	if (!(IntuitionBase = OpenLibrary("intuition.library", 50))) return FALSE;
	if (!(LocalSocketBase = OpenLibrary("local.socket", 0))) return FALSE;
	return TRUE;
}



void FreeResources(void)
{
	if (LocalSocketBase) CloseLibrary(LocalSocketBase);
	if (IntuitionBase) CloseLibrary(IntuitionBase);
}



int Main(UNUSED struct WBStartup *wbmessage)
{
	ULONG result = RETURN_FAIL;

	if (GetResources())
	{
		Printf("LocalSocketBase = $%08lx.\n", (LONG)LocalSocketBase);
		result = RETURN_OK;
	}
	else PutStr("GetResources() failed.\n");

	FreeResources();
	return result;
}