#include "library.h"

IPTR Get(Class *cl, Object *obj, struct opGet *msg)
{
	struct ObjData *d = INST_DATA(cl, obj);
	IPTR result = TRUE;

	switch (msg->opg_AttrID)
	{
		case SCKA_Listen:
			*msg->opg_Storage = d->StartMsg.Listen;			
		break;

		case SCKA_RemoteAddr:
			if (d->StartMsg.Listen) *(STRPTR*)msg->opg_Storage = NULL;
			else *(STRPTR*)msg->opg_Storage = d->StartMsg.RemoteAddr;
		break;

		case SCKA_EventPort:
			*(struct MsgPort**)msg->opg_Storage = d->StartMsg.EventPort;
		break;

		case SCKA_LocalAddr:
			*(STRPTR*)msg->opg_Storage = d->StartMsg.LocalAddr;			
		break;

		default:
			result = DoSuperMethodA(cl, obj, (Msg)msg);
		break;
	}

	return result;
}
