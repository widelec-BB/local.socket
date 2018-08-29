#include "library.h"

IPTR Receive(Class *cl, Object *obj, struct SCKP_Receive *msg)
{
	struct ObjData *d = INST_DATA(cl, obj);

	return 0;
}
