/*
    local.socket definitions

    Copyright © Grzegorz Kraszewski 2018. All rights reserved.
*/

#ifndef CLASSES_LOCAL_H
#define CLASSES_LOCAL_H


/* attributes */

#define SCKA_Blocking                           0x8EDA0001
#define SCKA_Listen                             0x8EDA0002
#define SCKA_RemoteAddr                         0x8EDA0003


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

#define SCKM_OnDataReceived                     0x8EDA0103

struct SCKP_OnDataReceived
{
	ULONG MethodID;
};

#define SCKM_OnDataSent                         0x8EDA0104

struct SCKP_OnDataSent
{
	ULONG MethodID;
};

#endif      /* CLASSES_LOCAL_H */
