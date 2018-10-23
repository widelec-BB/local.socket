#include "main.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>

#include <workbench/startup.h>
#include <workbench/workbench.h>
#include <intuition/classusr.h>

#include "../../os-include/classes/local_socket.h"

extern struct Library *SysBase;
extern struct Library *DOSBase;

struct Library *IntuitionBase;
struct Library *LocalSocketBase;


__attribute__((section(".text"))) const char VString[] = "$VER: " APP_NAME " " APP_VER " (" APP_DATE ") ? " APP_CYEARS " " APP_AUTHOR;


struct TestCase
{
	BOOL Listen;
	STRPTR RemoteAddr;
	STRPTR LocalAddr;
	BOOL ResultExpected;
};


struct TestCase Tests[8] = {
	{ FALSE, NULL, NULL, FALSE },			/* active mode requires both the addresses */
	{ FALSE, NULL, "Local1", FALSE },
	{ FALSE, "Remote1", NULL, FALSE },
	{ FALSE, "Remote1", "Local1", TRUE },
	{ TRUE, NULL, NULL, FALSE },            /* listen mode requires local, ignores remote */
	{ TRUE, NULL, "Local1", TRUE },
	{ TRUE, "Remote1", NULL, FALSE },
	{ TRUE, "Remote1", "Local1", TRUE }
};


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
		int test;
		
		Printf("LocalSocketBase = $%08lx.\n", (LONG)LocalSocketBase);
		
		for (test = 0; test < 8; test++)
		{
			Object *obj1;
			STRPTR local_addr = "NULL";
			STRPTR remote_addr = "NULL";			

			if (Tests[test].LocalAddr) local_addr = Tests[test].LocalAddr;
			if (Tests[test].RemoteAddr) remote_addr = Tests[test].RemoteAddr;
			
			Printf("Listen = %s.\nLocalAddr = \"%s\".\nRemoteAddr = \"%s\".\nExpected result: %s.\n",
				Tests[test].Listen ? (IPTR)"TRUE" : (IPTR)"FALSE", (IPTR)local_addr, (IPTR)remote_addr,
				Tests[test].ResultExpected ? (IPTR)"SUCCESS" : (IPTR)"FAIL");
			
			obj1 = NewObject(NULL, "local.socket",
				SCKA_Listen, Tests[test].Listen,
				SCKA_LocalAddr, (IPTR)Tests[test].LocalAddr,
				SCKA_RemoteAddr, (IPTR)Tests[test].RemoteAddr,
			TAG_END);
			
			if (obj1)
			{
				PutStr("Encountered: SUCCESS.\n");
				DisposeObject(obj1);
			}
			else PutStr("Encountered: FAIL.\n");
			
			PutStr("------\n");
		}
		
		result = RETURN_OK;
	}
	else PutStr("GetResources() failed.\n");

	FreeResources();
	return result;
}
