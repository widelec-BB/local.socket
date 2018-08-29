#include "library.h"

IPTR Dispose(Class *cl, Object *obj, Msg msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	// object destruction here

	return DoSuperMethodA(cl, obj, msg);
}
