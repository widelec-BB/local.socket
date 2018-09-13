/* Process code */


#define __NOLIBBASE__
typedef char* STRPTR;
typedef const char* CONST_STRPTR;
#define STRPTR_TYPEDEF

#include <proto/exec.h>
#include <proto/intuition.h>

#include <dos/dosextens.h>
//#include <exec/memory.h>

#include <libvstring.h>

#include "process.h"


#define LOCAL_BASE(x) struct Library *x##Base = sd->x##Base


struct Event
{
	struct MsgPort  *Port;
	struct Process  *Process;
	ULONG            SigMask;
};


struct SubData
{
	struct Library  *SysBase;
	struct Event     TxEvent;
	struct Event     RxEvent;
	struct MsgPort  *CommPort;      // communication with creator application
	BOOL             Listen;
	STRPTR           LocalAddr;
	STRPTR           RemoteAddr;
};



static void Loop(struct SubData *sd)
{
	LOCAL_BASE(Sys);
	BOOL running = TRUE;
	struct PacketMsg *pm;

	while (running)
	{
		WaitPort(sd->CommPort);

		while (pm = (struct PacketMsg*)GetMsg(sd->CommPort))
		{
			if (pm->Flags & PKTFLAG_DIE) running = FALSE;
			ReplyMsg((struct Message*)pm);
		}
	}
}



static BOOL SetupAddresses(struct SubData *sd, struct StartupMsg *sm)
{
	sd->LocalAddr = NULL;
	sd->RemoteAddr = NULL;
	sd->Listen = sm->Listen;
	
	if (sd->Listen)
	{
		if (sd->LocalAddr = StrNew(sm->Address)) return TRUE;
	}
	else
	{
		if (sd->RemoteAddr = StrNew(sm->Address)) return TRUE;
	}
	
	return FALSE;
}



static BOOL ProcessSetup(struct SubData *sd, struct StartupMsg *sm)
{
	if (SetupAddresses(sd, sm)) return TRUE;
	return FALSE;
}



static void ProcessCleanup(struct SubData *sd)
{
	if (sd->LocalAddr) StrFree(sd->LocalAddr);
	if (sd->RemoteAddr) StrFree(sd->RemoteAddr);
}



LONG Worker(void)
{
	struct Library *SysBase = *(struct Library**)4L;
	struct SubData sd;
	struct StartupMsg *sm;
	struct Message *cm;

	sd.SysBase = SysBase;
	NewGetTaskAttrs(NULL, &sm, sizeof(struct StartupMsg*), TASKINFOTYPE_STARTUPMSG, TAG_END);
	NewGetTaskAttrs(NULL, &sd.CommPort, sizeof(APTR), TASKINFOTYPE_TASKMSGPORT, TAG_END);

	if (!sm || !sd.CommPort) return 0;   // general failure, can do nothing but exit

	cm = &sm->CnfMsg;
	cm->mn_Node.ln_Type = NT_MESSAGE;
	cm->mn_Length = sizeof(struct Message);
	cm->mn_ReplyPort = sd.CommPort;

	if (ProcessSetup(&sd, sm))
	{
		PutMsg(sm->SysMsg.mn_ReplyPort, cm);
		WaitPort(sd.CommPort);
		GetMsg(sd.CommPort);
		Loop(&sd);
	}

	ProcessCleanup(&sd);

	return 0;
}
