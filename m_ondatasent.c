#include "library.h"

IPTR OnDataSent(Class *cl, Object *obj, struct SCKP_OnDataSent *msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	return 0;
}
