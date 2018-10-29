/*
   local.socket, library skeleton
   Generated with LibMaker 0.12.
*/

/****** local.socket/background *********************************************
*
* DESCRIPTION
*
* HISTORY
*
*****************************************************************************
*
*/

typedef char* STRPTR;
typedef const char* CONST_STRPTR;
#define STRPTR_TYPEDEF

#include <proto/exec.h>
#include <exec/resident.h>
#include <exec/libraries.h>
#include <exec/system.h>
#include <libraries/query.h>
#include <clib/alib_protos.h>
#include <proto/intuition.h>
#include <proto/utility.h>
#include <classes/local_socket.h>

#define UNUSED __attribute__((unused))

#include "lib_version.h"
#include "library.h"

const char LibName[] = LIBNAME;
extern const char VTag[];

struct Library *SysBase;
struct Library *IntuitionBase;
struct Library *UtilityBase;
struct Library *DOSBase;

struct Library *LibInit(struct Library *unused, APTR seglist, struct Library *sysb);
struct MyLibBase *lib_init(struct MyLibBase *base, APTR seglist, struct Library *SysBase);
APTR lib_expunge(struct MyLibBase *base);
struct Library *LibOpen(void);
ULONG LibClose(void);
APTR LibExpunge(void);
ULONG LibReserved(void);
BOOL InitResources(struct MyLibBase *base);
BOOL FreeResources(struct MyLibBase *base);
Class* InitClass(struct MyLibBase *base);
Class *GetClass(void);
static IPTR ClassDispatcher(void);




BOOL InitResources(struct MyLibBase *base)
{
	if (!(DOSBase = OpenLibrary("dos.library", 50))) return FALSE;
	if (!(IntuitionBase = OpenLibrary("intuition.library", 50))) return FALSE;
	if (!(UtilityBase = OpenLibrary("utility.library", 50))) return FALSE;

	if (InitClass(base))
	{
		return TRUE;
	}

	return FALSE;
}


BOOL FreeResources(struct MyLibBase *base)
{
	if (base->BoopsiClass)
	{
		if (!FreeClass(base->BoopsiClass))
		{
			AddClass(base->BoopsiClass);
			return FALSE;
		}
	}

	if (UtilityBase) CloseLibrary(UtilityBase);
	if (IntuitionBase) CloseLibrary(IntuitionBase);
	if (DOSBase) CloseLibrary(DOSBase);
	return TRUE;
}


const struct TagItem RTags[] =
{
	{ QUERYINFOATTR_NAME, (IPTR)LibName },
	{ QUERYINFOATTR_IDSTRING, (IPTR)&VTag[1] },
	{ QUERYINFOATTR_COPYRIGHT, (IPTR)"© Grzegorz Kraszewski 2018" },
	{ QUERYINFOATTR_DATE, (IPTR)DATE },
	{ QUERYINFOATTR_VERSION, VERSION },
	{ QUERYINFOATTR_REVISION, REVISION },
	{ QUERYINFOATTR_SUBTYPE, QUERYSUBTYPE_LIBRARY },
	{ TAG_END,  0 }
};

struct Resident ROMTag =
{
	RTC_MATCHWORD,
	&ROMTag,
	&ROMTag + 1,
	RTF_EXTENDED | RTF_PPC,
	VERSION,
	NT_LIBRARY,
	0,
	(char*)LibName,
	VSTRING,
	(APTR)LibInit,
	REVISION,
	(struct TagItem*)RTags
};

APTR JumpTable[] =
{
	(APTR)FUNCARRAY_BEGIN,
	(APTR)FUNCARRAY_32BIT_NATIVE,
	(APTR)LibOpen,
	(APTR)LibClose,
	(APTR)LibExpunge,
	(APTR)LibReserved,
	(APTR)GetClass,
	(APTR)0xFFFFFFFF,
	(APTR)FUNCARRAY_END
};


static const struct EmulLibEntry ClassDispatcher_gate =
{
	TRAP_LIB,
	0,
	(void(*)(void))ClassDispatcher
};


Class* InitClass(struct MyLibBase *base)
{
	Class *cl = NULL;

	if ((cl = MakeClass(LibName, "rootclass", NULL, sizeof(struct ObjData), 0L)))
	{
		cl->cl_Dispatcher.h_Entry = (HOOKFUNC)&ClassDispatcher_gate;
		cl->cl_UserData = (ULONG)base;
		AddClass(cl);
	}

	base->BoopsiClass = cl;
	return cl;
}



struct MyLibBase* lib_init(struct MyLibBase *base, APTR seglist, UNUSED struct Library *sysbase)
{
	InitSemaphore(&base->BaseLock);
	base->Seglist = seglist;
	return base;
}

struct TagItem LibTags[] = {
	{ LIBTAG_FUNCTIONINIT, (IPTR)JumpTable },
	{ LIBTAG_LIBRARYINIT,  (IPTR)lib_init },
	{ LIBTAG_MACHINE,      MACHINE_PPC },
	{ LIBTAG_BASESIZE,     sizeof(struct MyLibBase) },
	{ LIBTAG_SEGLIST,      0 },
	{ LIBTAG_TYPE,         NT_LIBRARY },
	{ LIBTAG_NAME,         0 },
	{ LIBTAG_IDSTRING,     0 },
	{ LIBTAG_FLAGS,        LIBF_CHANGED | LIBF_SUMUSED },
	{ LIBTAG_VERSION,      VERSION },
	{ LIBTAG_REVISION,     REVISION },
	{ LIBTAG_PUBLIC,       TRUE },
	{ TAG_END,             0 }
};

struct Library* LibInit(UNUSED struct Library *unused, APTR seglist, struct Library *sysbase)
{
	SysBase = sysbase;

	LibTags[4].ti_Data = (IPTR)seglist;
	LibTags[6].ti_Data = (IPTR)ROMTag.rt_Name;
	LibTags[7].ti_Data = (IPTR)ROMTag.rt_IdString;

	return (NewCreateLibrary(LibTags));
}


struct Library* LibOpen(void)
{
	struct MyLibBase *base = (struct MyLibBase*)REG_A6;
	struct Library *lib = (struct Library*)base;

	ObtainSemaphore(&base->BaseLock);

	if (!base->InitFlag)
	{
		if (InitResources(base)) base->InitFlag = TRUE;
		else
		{
			FreeResources(base);
			lib = NULL;
		}
	}

	if (lib)
	{
		base->LibNode.lib_Flags &= ~LIBF_DELEXP;
		base->LibNode.lib_OpenCnt++;
	}

	ReleaseSemaphore(&base->BaseLock);
	if (!lib) lib_expunge(base);
	return lib;
}


ULONG LibClose(void)
{
	struct MyLibBase *base = (struct MyLibBase*)REG_A6;
	ULONG ret = 0;

	ObtainSemaphore(&base->BaseLock);

	if (--base->LibNode.lib_OpenCnt == 0)
	{
		if (base->LibNode.lib_Flags & LIBF_DELEXP) ret = (ULONG)lib_expunge(base);
	}

	if (ret == 0) ReleaseSemaphore(&base->BaseLock);
	return ret;
}


APTR LibExpunge(void)
{
	struct MyLibBase *base = (struct MyLibBase*)REG_A6;

	return(lib_expunge(base));
}


APTR lib_expunge(struct MyLibBase *base)
{
	APTR seglist = NULL;

	ObtainSemaphore(&base->BaseLock);

	if (base->LibNode.lib_OpenCnt == 0)
	{
		if (FreeResources(base))
		{
			Forbid();
			Remove((struct Node*)base);
			Permit();
			seglist = base->Seglist;
			FreeMem((UBYTE*)base - base->LibNode.lib_NegSize, base->LibNode.lib_NegSize + base->LibNode.lib_PosSize);
			base = NULL;    /* freed memory, no more valid */
		}
	}
	else base->LibNode.lib_Flags |= LIBF_DELEXP;

	if (base) ReleaseSemaphore(&base->BaseLock);
	return seglist;
}


ULONG LibReserved(void)
{
	return 0;
}


Class *GetClass(void)
{
	struct MyLibBase *base = (struct MyLibBase*)REG_A6;

	return base->BoopsiClass;
}


/* Prototypes of methods */

IPTR New(Class *cl, Object *obj, struct opSet *msg);
IPTR Get(Class *cl, Object *obj, struct opGet *msg);
IPTR Dispose(Class *cl, Object *obj, Msg msg);
IPTR Receive(Class *cl, Object *obj, struct SCKP_Receive *msg);
IPTR Send(Class *cl, Object *obj, struct SCKP_Send *msg);


static IPTR ClassDispatcher(void)
{
	Class *cl = (Class*)REG_A0;
	Object *obj = (Object*)REG_A2;
	Msg msg = (Msg)REG_A1;

	switch(msg->MethodID)
	{
		case OM_NEW: return New(cl, obj, (struct opSet*)msg);
		case OM_GET: return Get(cl, obj, (struct opGet*)msg);
		case OM_DISPOSE: return Dispose(cl, obj, msg);
		case SCKM_Receive: return Receive(cl, obj, (struct SCKP_Receive*)msg);
		case SCKM_Send: return Send(cl, obj, (struct SCKP_Send*)msg);
		default: return DoSuperMethodA(cl, obj, msg);
	}
}

//------------------------------------------------------------------------------

UWORD SendPacket(struct ObjData *d, UBYTE *payload, UWORD flags)
{
	d->Packet.SysMsg.mn_Node.ln_Type = NT_MESSAGE;
	d->Packet.SysMsg.mn_Length = sizeof(struct PacketMsg);
	d->Packet.SysMsg.mn_ReplyPort = d->LocalPort;
	d->Packet.Payload = payload;
	d->Packet.Flags = flags;
	PutMsg(d->WorkerPort, &d->Packet.SysMsg);
	WaitPort(d->LocalPort);
	GetMsg(d->LocalPort);
	return d->Packet.Flags;
}
