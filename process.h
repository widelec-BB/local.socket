/* This source code is part of Reggae file.output class. */

#include <sys/types.h>

// This message is sent from OM_NEW of object and replied automatically when
// the subprocess finishes.


struct StartupMsg
{
	struct Message     SysMsg;         // usual stuff
	struct Message     CnfMsg;         // this is sent from subtask after succesfull init
};


struct PacketMsg
{
	struct Message     SysMsg;
};

//==============================================================================

LONG Worker(void);
