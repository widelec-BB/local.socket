#include "library.h"

#include <proto/dos.h>
#include <dos/dostags.h>

#if 0
static struct Process* RunWorker(struct ObjData *d)
{
	struct Process *pr = NULL;
	struct Message *m;

	d->StartMsg.SysMsg.mn_Node.ln_Type = NT_MESSAGE;
	d->StartMsg.SysMsg.mn_ReplyPort = d->LocalPort;
	d->StartMsg.SysMsg.mn_Length = sizeof(struct StartupMsg);

	pr = CreateNewProcTags(
		NP_Entry, (ULONG)Process,
		NP_CodeType, CODETYPE_PPC,
		NP_Priority, d->ProcessPri,
		NP_PPCStackSize, 32768,
		NP_Name, (ULONG)d->ProcessName,
		NP_TaskMsgPort, (ULONG)&d->PlayerPort,
		NP_StartupMsg, (ULONG)&d->StartMsg,
	TAG_END);

	// There are two alternatives now. I can get StartMsg back with error code, it means
	// player process failed. I can get a ConfirmMsg however, it means success.

	MLOGV(LOG_VERBOSE, "\"%s\" [$%08lX] is running, waiting for confirmation...", d->ProcessName, pr);

	WaitPort(d->LocalPort);
	m = GetMsg(d->LocalPort);

	if (m == &d->StartMsg.SysMsg)
	{
		pr = NULL;
		MLOGV(LOG_ERRORS, "\"%s\" failed with error code %ld.", d->ProcessName, d->StartMsg.Param.ErrCode);
	}
	else
	{
		ReplyMsg(m);
		MLOGV(LOG_INFO, "\"%s\" started with success.", d->ProcessName);
	}

	return pr;
}

#endif




IPTR New(Class *cl, Object *obj, struct opSet *msg)
{
	IPTR newobj = NULL;

	obj = (Object*)DoSuperMethodA(cl, obj, (Msg)msg);

	if (obj)
	{
		struct ObjData *d = INST_DATA(cl, obj);

		d->Worker = CreateNewProcTags(
			NP_Entry,
		);
		
		newobj = (IPTR)obj;
	}
	else CoerceMethod(cl, obj, OM_DISPOSE);

	return newobj;
}
