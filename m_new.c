#include <proto/exec.h>
#include <proto/dos.h>
#include <dos/dostags.h>

#include "library.h"
#include "process.h"


static struct Process* RunWorker(struct ObjData *d)
{
	struct Process *pr = NULL;
	struct Message *m;

	d->StartMsg.SysMsg.mn_Node.ln_Type = NT_MESSAGE;
	d->StartMsg.SysMsg.mn_ReplyPort = d->LocalPort;
	d->StartMsg.SysMsg.mn_Length = sizeof(struct StartupMsg);

	pr = CreateNewProcTags(
		NP_Entry, (ULONG)Worker,
		NP_CodeType, CODETYPE_PPC,
		NP_Priority, 0,
		NP_PPCStackSize, 32768,
		NP_Name, (ULONG)"local.socket worker",
		NP_TaskMsgPort, (ULONG)&d->WorkerPort,
		NP_StartupMsg, (ULONG)&d->StartMsg,
	TAG_END);

	// There are two alternatives now. I can get StartMsg back with error code, it means
	// player process failed. I can get a ConfirmMsg however, it means success.

	WaitPort(d->LocalPort);
	m = GetMsg(d->LocalPort);
	if (m == &d->StartMsg.SysMsg) pr = NULL;
	else ReplyMsg(m);

	return pr;
}




IPTR New(Class *cl, Object *obj, struct opSet *msg)
{
	IPTR newobj = NULL;

	obj = (Object*)DoSuperMethodA(cl, obj, (Msg)msg);

	if (obj)
	{
		struct ObjData *d = INST_DATA(cl, obj);

		if (d->LocalPort = CreateMsgPort())
		{
			if (d->Worker = RunWorker(d))
			{
				newobj = (IPTR)obj;
			}
		}
	}
	else CoerceMethod(cl, obj, OM_DISPOSE);

	return newobj;
}
