#ifndef LOCAL_SOCKET_PROCESS_H
#define LOCAL_SOCKET_PROCESS_H

#include <sys/types.h>


struct StartupMsg
{
	struct Message     SysMsg;         // usual stuff
	struct Message     CnfMsg;         // this is sent from subtask after succesfull init
};


struct PacketMsg
{
	struct Message     SysMsg;
};


LONG Worker(void);

#endif      /* LOCAL_SOCKET_PROCESS_H */
