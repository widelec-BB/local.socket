/*
    local.socket definitions

    Copyright © Grzegorz Kraszewski 2018. All rights reserved.
*/

#ifndef CLASSES_LOCAL_SOCKET_H
#define CLASSES_LOCAL_SOCKET_H


/* attributes */

#define SCKA_Blocking                           0x8EDA0001  /* [I..] */
#define SCKA_Listen                             0x8EDA0002  /* [I..] */
#define SCKA_RemoteAddr                         0x8EDA0003  /* [I.G] */
#define SCKA_LocalAddr                          0x8EDA0004  /* [I.G] */

/* methods */

#define SCKM_Receive                            0x8EDA0101

struct SCKP_Receive
{
	ULONG MethodID;
};

#define SCKM_Send                               0x8EDA0102

struct SCKP_Send
{
	ULONG MethodID;
};

#endif      /* CLASSES_LOCAL_SOCKET_H */
