/* Process code */

typedef char* STRPTR;
typedef const char* CONST_STRPTR;
#define STRPTR_TYPEDEF

#include <proto/exec.h>
#include <proto/intuition.h>

#include <dos/dosextens.h>
//#include <exec/memory.h>

#include <clib/debug_protos.h>
#include <libvstring.h>

#include "process.h"


#define LOCAL_BASE(x) struct Library *x##Base = sd->x##Base



struct SubData
{
	struct Library  *SysBase;
	struct MsgPort  *CommPort;      // communication with creator application
	struct MsgPort  *SockPort;      // communication with remote end
	struct MsgPort  *EventPort;     // event messages will go there
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


static void SendLifeSign(struct SubData *sd, struct StartupMsg *sm)
{
	struct Message *cm;
	
	cm = &sm->CnfMsg;
	cm->mn_Node.ln_Type = NT_MESSAGE;
	cm->mn_Length = sizeof(struct Message);
	cm->mn_ReplyPort = sd->CommPort;
	PutMsg(sm->SysMsg.mn_ReplyPort, cm);
	WaitPort(sd->CommPort);
	GetMsg(sd->CommPort);
}


static void ListeningPort(struct SubData *sd, struct StartupMsg *sm)
{
	KPrintF("creating listening port\n");
	SendLifeSign(sd, sm);
	Loop(sd);
}


static void ConnectingPort(struct SubData *sd, struct StartupMsg *sm)
{
	KPrintF("creating connecting port\n");
	SendLifeSign(sd, sm);
	Loop(sd);
}


static void LetsGo(struct SubData *sd, struct StartupMsg *sm)
{
	sd->LocalAddr = NULL;
	sd->RemoteAddr = NULL;
	sd->Listen = sm->Listen;
	sd->EventPort = sm->EventPort;

	if (sd->SockPort = CreateMsgPort())
	{
		if (sd->Listen) ListeningPort(sd, sm);
		else ConnectingPort(sd, sm);
		DeleteMsgPort(sd->SockPort);
	}
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

	LetsGo(&sd, sm);

	return 0;
}
