/* ================================== *\
 @file     mnet_types.h
 @project  mnet
 @author   moosm
 @date     6-1-2026
*\ ================================== */

#ifndef MNET_MNET_TYPES_H
#define MNET_MNET_TYPES_H

#include "mnet_os.h"

// return codes for mnet functions.
typedef enum
{
    mnet_ok                 = 0,
    mnet_error              = -1
}mnet_result_t;

typedef enum
{
    mnet_af_inet            = AF_INET, // ipv4
    mnet_af_inet6           = AF_INET6 // ipv6
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
    mnet_so_reuseaddr       = SO_REUSEADDR,
    // (int) 1=enable 0=disable.
    // allows binding to a port in TIME_WAIT state.

    mnet_so_keepalive       = SO_KEEPALIVE,
    // TCP ONLY
    // (int) 1=enable 0=disable.
    // sends keepalive probes to detect dead peers.

    mnet_so_broadcast       = SO_BROADCAST,
    // UDP ONLY
    // (int) 1=enable 0=disable.

    mnet_so_rcvbuf          = SO_RCVBUF,
    // (int) recv buffer resize. (not guaranteed)

    mnet_so_sndbuf          = SO_SNDBUF,
    // (int) send buffer resize. (not guaranteed)

    mnet_so_rcvtimeo        = SO_RCVTIMEO,
    // (struct timeval) timeout for receiving.

    mnet_so_sndtimeo        = SO_SNDTIMEO,
    // (struct timeval) timeout for sending.

    mnet_so_error           = SO_ERROR,
    // OUTPUT
    // (int) error code.

    mnet_so_type            = SO_TYPE,
    // OUTPUT
    // (int) get socket type.

    mnet_tcp_nodelay        = TCP_NODELAY
    // TCP ONLY , and on TCP level
    // (int) 1=disable nagle 0=enable nagle (default).

}mnet_sockopt_t;

typedef enum
{
    mnet_sol_socket         = SOL_SOCKET,
    mnet_ipproto_tcp_level  = IPPROTO_TCP,
    mnet_ipproto_ip_level   = IPPROTO_IP
}mnet_sockopt_level_t;


// check the WINDOWS version for documentation.
typedef enum
{
#ifdef MNET_WINDOWS

    mnet_ewouldblock        = WSAEWOULDBLOCK,
    // operation would block on a blocking socket.
    // if recv isn't ready this we probably be returned.

    mnet_einprogress        = WSAEINPROGRESS,
    // connect in progress.

    mnet_ealready           = WSAEALREADY,
    // operation already in progress.

    mnet_econnrefused       = WSAECONNREFUSED,
    // connection refused.

    mnet_econnreset         = WSAECONNRESET,
    // connection reset.

    mnet_etimedout          = WSAETIMEDOUT,
    // send/recv timeout.

    mnet_eaddrinuse         = WSAEADDRINUSE,
    // port already in use.

    mnet_eaddrnotavail      = WSAEADDRNOTAVAIL,
    // address not available.

    mnet_eisconn            = WSAEISCONN,
    // socket already connected.

    mnet_enotconn           = WSAENOTCONN
    // trying to use non-connected socket.

#else

    mnet_ewouldblock        = EWOULDBLOCK,
    mnet_einprogress        = EINPROGRESS,
    mnet_ealready           = EALREADY,
    mnet_econnrefused       = ECONNREFUSED,
    mnet_econnreset         = ECONNRESET,
    mnet_etimedout          = ETIMEDOUT,
    mnet_eaddrinuse         = EADDRINUSE,
    mnet_eaddrnotavail      = EADDRNOTAVAIL,
    mnet_eisconn            = EISCONN,
    mnet_enotconn           = ENOTCONN

#endif
}mnet_error_code_t;

// check the WINDOWS version for documentation.
typedef enum
{
#ifdef MNET_WINDOWS

    mnet_shut_rd            = SD_RECEIVE,
    // stop receiving.

    mnet_shut_wr            = SD_SEND,
    // stop writing.

    mnet_shut_rdwr          = SD_BOTH
    // stop both.

#else

    mnet_shut_rd            = SHUT_RD,
    mnet_shut_wr            = SHUT_WR,
    mnet_shut_rdwr          = SHUT_RDWR

#endif
}mnet_shutdown_t;

// used as flags for some functions.
typedef enum
{
    mnet_msg_none           = 0,
    // no extra instruction.

    mnet_msg_default        = mnet_msg_none,
    // same as msg_none.

    mnet_msg_peek           = MSG_PEEK,
    // peek at incoming data.

    mnet_msg_dontroute      = MSG_DONTROUTE,
    // don't try to route it.

    mnet_msg_waitall        = MSG_WAITALL
    // block until buffer is full.
}mnet_msg_flags_t;


#endif //MNET_MNET_TYPES_H