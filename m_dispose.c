#include <proto/exec.h>

#include "libvstring/libvstring.h"

#include "library.h"



static void StopWorker(struct ObjData *d)
{
	SendPacket(d, NULL, PKTFLAG_DIE);
	
	/* Waiting for startup message to be sent back. */
	
	WaitPort(d->LocalPort);
	GetMsg(d->LocalPort);
	
	/* Worker process is no longer running. */
}



IPTR Dispose(Class *cl, Object *obj, Msg msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	if (d->LocalPort)
	{
		if (d->Worker)
		{
			StopWorker(d);
		}
		
		DeleteMsgPort(d->LocalPort);
	}

	if (d->StartMsg.LocalAddr) StrFree(d->StartMsg.LocalAddr);
	if (d->StartMsg.RemoteAddr) StrFree(d->StartMsg.RemoteAddr);
	
	return DoSuperMethodA(cl, obj, msg);
}
