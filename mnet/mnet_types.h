/* ================================== *\
 @file     mnet_types.h
 @project  mnet
 @author   moosm
 @date     6-1-2026
*\ ================================== */

#ifndef MNET_MNET_TYPES_H
#define MNET_MNET_TYPES_H

#include "mnet_os.h"

typedef enum
{
    mnet_ok = 0,
    mnet_error = -1
}mnet_result_t;

typedef enum
{
    mnet_af_inet            = AF_INET,
    mnet_af_inet6           = AF_INET6
}mnet_address_family_t;

typedef enum
{
    mnet_sock_stream        = SOCK_STREAM,
    mnet_sock_dgram         = SOCK_DGRAM,
    mnet_sock_raw           = SOCK_RAW
}mnet_socket_type_t;

typedef enum
{
    mnet_ipproto_default    = 0,
    mnet_ipproto_automatic  = 0,
    mnet_ipproto_ip         = IPPROTO_IP,
    mnet_ipproto_tcp        = IPPROTO_TCP,
    mnet_ipproto_udp        = IPPROTO_UDP,
    mnet_ipproto_icmp       = IPPROTO_ICMP
}mnet_protocol_t;

typedef enum
{
#ifdef MNET_WINDOWS
    mnet_ewouldblock = WSAEWOULDBLOCK,
    mnet_einprogress = WSAEINPROGRESS,
    mnet_ealready = WSAEALREADY,
    mnet_econnrefused = WSAECONNREFUSED,
    mnet_econnreset = WSAECONNRESET,
    mnet_etimedout = WSAETIMEDOUT,
    mnet_eaddrinuse = WSAEADDRINUSE,
    mnet_eaddrnotavail = WSAEADDRNOTAVAIL,
    mnet_eisconn = WSAEISCONN,
    mnet_enotconn = WSAENOTCONN
#else
    mnet_ewouldblock = EWOULDBLOCK,
    mnet_einprogress = EINPROGRESS,
    mnet_ealready = EALREADY,
    mnet_econnrefused = ECONNREFUSED,
    mnet_econnreset = ECONNRESET,
    mnet_etimedout = ETIMEDOUT,
    mnet_eaddrinuse = EADDRINUSE,
    mnet_eaddrnotavail = EADDRNOTAVAIL,
    mnet_eisconn = EISCONN,
    mnet_enotconn = ENOTCONN
#endif
}mnet_error_code_t;


#endif //MNET_MNET_TYPES_H