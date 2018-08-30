#include <proto/exec.h>

#include "library.h"



static void StopWorker(struct ObjData *d)
{
	// TODO: implement
	
	return;
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
	
	return DoSuperMethodA(cl, obj, msg);
}
