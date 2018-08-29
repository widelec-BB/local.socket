#include "library.h"

IPTR Send(Class *cl, Object *obj, struct SCKP_Send *msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	return 0;
}
