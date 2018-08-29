/*
   local.socket internal library definitions.
   Generated with LibMaker 0.12.
*/

#ifndef LOCAL_LIBRARY_H
#define LOCAL_LIBRARY_H

#include <clib/alib_protos.h>
#include <proto/intuition.h>
#include <proto/utility.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <intuition/classes.h>
#include <classes/local_socket.h>

struct MyLibBase
{
	struct Library          LibNode;
	APTR                    Seglist;
	struct SignalSemaphore  BaseLock;
	BOOL                    InitFlag;
	Class*                  BoopsiClass;
};


struct StartupMsg
{
	struct Message     SysMsg;         // usual stuff
	struct Message     CnfMsg;         // this is sent from subtask after succesfull init
};


struct PacketMsg
{
	struct Message     SysMsg;
	UBYTE             *Payload;
	UWORD              Flags;
};


struct ObjData
{
	struct Process         *Worker;
	struct MsgPort         *WorkerPort;
	struct MsgPort         *LocalPort;
	struct StartupMsg       StartMsg;
	struct PacketMsg        Packet;
};


#endif      /* LOCAL_LIBRARY_H */
