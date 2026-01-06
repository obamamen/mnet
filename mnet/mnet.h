/* ================================== *\
 @file     mnet.h
 @project  mnet
 @author   moosm
 @date     1/5/2026
*\ ================================== */


///
/// INTERFACE
///
#ifndef MNET_MNET_H
#define MNET_MNET_H

#include "mnet_os.h"

int mnet_initialize(void);
void mnet_cleanup(void);

#endif//MNET_MNET_H

///
/// SOURCE
///
#ifdef MNET_SOURCE

int mnet_initialize(void)
{
#ifdef MNET_WINDOWS

    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data);

#elif MNET_UNIX

    return 0

#endif
}

void mnet_cleanup(void)
{
#ifdef MNET_WINDOWS
    WSACleanup();
#endif
}

#endif