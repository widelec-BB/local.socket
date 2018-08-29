#include "library.h"

IPTR New(Class *cl, Object *obj, struct opSet *msg)
{
	IPTR newobj = NULL;

	obj = (Object*)DoSuperMethodA(cl, obj, (Msg)msg);

	if (obj)
	{
		struct ObjData *d = INST_DATA(cl, obj);

		// object initialization here, set 'newobj' to 'obj' if success

		newobj = (IPTR)obj;
	}
	else CoerceMethod(cl, obj, OM_DISPOSE);

	return newobj;
}
