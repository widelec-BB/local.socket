/* Process code */


#define __NOLIBBASE__
typedef char* STRPTR;
typedef const char* CONST_STRPTR;
#define STRPTR_TYPEDEF

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/multimedia.h>
#include <exec/memory.h>

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
	struct MsgPort  *CommPort;
};



static void DoEvents(struct SubData *sd, struct Event *ev)
{
	LOCAL_BASE(Sys);
	struct Message *m;

	if (ev->Process && ev->SigMask) Signal(&ev->Process->pr_Task, ev->SigMask);
	while (m = GetMsg(ev->Port)) ReplyMsg(m);
}



static void DoTxEvents(struct SubData *sd)
{
	DoEvents(sd, &sd->TxEvent);
}



static void DoRxEvents(struct SubData *sd)
{
	DoEvents(sd, &sd->RxEvent);
}



static BOOL SetupEvent(struct SubData *sd, struct Event *ev)
{
	LOCAL_BASE(Sys);

	ev->Process = NULL;
	ev->SigMask = 0;
	if (ev->Port = CreateMsgPort()) return TRUE;
	return FALSE;
}



static void CleanupEvent(struct SubData *sd, struct Event *ev)
{
	LOCAL_BASE(Sys);

	DeleteMsgPort(ev->Port);
}



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
			ReplyMsg((struct Message*)pm);
		}
	}
}



static BOOL ProcessSetup(struct SubData *sd)
{
	if (SetupEvent(sd, &sd->RxEvent))
	{
		if (SetupEvent(sd, &sd->TxEvent))
		{
			return TRUE;
		}
		
		CleanupEvent(sd, &sd->RxEvent);
	}
	
	return FALSE;
}



static void ProcessCleanup(struct SubData *sd)
{
	CleanupEvent(sd, &sd->RxEvent);
	CleanupEvent(sd, &sd->TxEvent);
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

	// Initialization of confirmation message sent back to the creator. This
	// message is already allocated inside the startup message. Thanks to this
	// I need not to allocate it here and take care of errors.

	cm = &sm->CnfMsg;
	cm->mn_Node.ln_Type = NT_MESSAGE;
	cm->mn_Length = sizeof(struct Message);
	cm->mn_ReplyPort = sd.CommPort;

	if (ProcessSetup(&sd))
	{
		// Send confirmation message and get the reply. Continue to the main
		// loop then.

		PutMsg(sm->SysMsg.mn_ReplyPort, cm);
		WaitPort(sd.CommPort);
		GetMsg(sd.CommPort);
		Loop(&sd);
	}
	
	ProcessCleanup(&sd);

	// After return startup message is replied automaticaly with ErrCode field
	// set.

	return 0;
}

///
