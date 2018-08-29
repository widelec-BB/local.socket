#include "library.h"

IPTR OnDataReceived(Class *cl, Object *obj, struct SCKP_OnDataReceived *msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	return 0;
}
