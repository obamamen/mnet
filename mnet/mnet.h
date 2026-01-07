/* ================================== *\
 @file     mnet.h
 @project  mnet
 @author   moosm
 @date     1/5/2026
*\ ================================== */

///////////////////////////////////
// ...
///////////////////////////////////

// ================================================
// ...
// ================================================

// ----------------------------------------------------------------
// ...
// ----------------------------------------------------------------


///////////////////////////////////////
//             INTERFACE
///////////////////////////////////////
#ifndef MNET_MNET_H
#define MNET_MNET_H

#include <stdint.h>

#include "mnet_os.h"
#include "mnet_types.h"


// ================================================
// INITIALIZATION & CLEANUP
// ================================================


// ----------------------------------------------------------------
// initialize the library.
//
// must be called before any mnet functions are called.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_initialize(void);

// ----------------------------------------------------------------
// cleanup networking resources.
//
// should be called when done using library,
//  or at end of program.
// ----------------------------------------------------------------
void mnet_cleanup(void);


// ================================================
//       CORE SOCKET CREATION AND MANAGEMENT
// ================================================


// ----------------------------------------------------------------
// create a new socket.
//
// domain: mnet_address_family_t
// type: mnet_socket_type_t
// protocol: mnet_protocol_t
// ----------------------------------------------------------------
// returns: valid socket handle, or MNET_INVALID_SOCKET on error.
mnet_socket_t mnet_socket(
                        mnet_address_family_t   domain,
                        mnet_socket_type_t      type,
                        mnet_protocol_t         protocol);

// ----------------------------------------------------------------
// close a socket.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_close(mnet_socket_t sock);

// ----------------------------------------------------------------
// check if a socket handle is valid.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_socket_is_valid(mnet_socket_t sock);


// ================================================
//         CONNECTION MANAGEMENT (TCP)
// ================================================


// ----------------------------------------------------------------
// bind a socket to a local address and port.
//
// addr: mnet_sockaddr_in_t or mnet_sockaddr_in6_t.
// addrlen: sizeof of addr structure.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_bind(
            mnet_socket_t sock,
            const mnet_sockaddr_t* addr,
            socklen_t addrlen);

// ----------------------------------------------------------------
// put a socket into listening  mode.
//
// backlog: maximum pending connection queue.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_listen(mnet_socket_t sock, int backlog);

// ----------------------------------------------------------------
// accept incoming connection. (blocks by default)
//
// addr: [out] client address.              (can be NULL)
// addrlen: [out] sizeof addr structure.    (can be NULL)
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
mnet_socket_t mnet_accept(
                        mnet_socket_t sock,
                        mnet_sockaddr_t* addr,
                        socklen_t* addrlen);

// ----------------------------------------------------------------
// connect to a server.
//
// addr: server address.
// addrlen: sizeof addr structure.
// ----------------------------------------------------------------
int mnet_connect(mnet_socket_t sock, const mnet_sockaddr_t* addr, socklen_t addrlen);

// ----------------------------------------------------------------
// shutdown part or all of a connection.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_shutdown(mnet_socket_t sock, mnet_shutdown_code_t how);


// ================================================
//               DATA TRANSFER (TCP)
// ================================================

// ----------------------------------------------------------------
// send data on a connected socket. (TCP)
//
// buf: data to send.
// len: number of bytes to send. (shouldn't be more than the buffers size)
// flags: flags to send with.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_send(
            mnet_socket_t sock,
            const void* buf,
            size_t len,
            mnet_msg_flags_t flags);

// ----------------------------------------------------------------
// receive data from the connected socket on the other end.
//
// buf: local buffer to move data into.
// len: maximum bytes to receive. (shouldn't be more than the buffers size)
// flags: flags to receive with.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_recv(
            mnet_socket_t sock,
            void* buf,
            size_t len,
            mnet_msg_flags_t flags);


// ================================================
//                DATA TRANSFER (UDP)
// ================================================


// ----------------------------------------------------------------
// send data to a specific address. (UDP)
//
// buf: data buffer to send. (shouldn't be more than the buffers size)
// len: number of bytes to send.
// flags: flags to send with.
// dest_addr: destination address to send to.
// addrlen: sizeof of dest_addr structure.
// ----------------------------------------------------------------
// returns: number of bytes sent, or -1 on error.
int mnet_sendto(
            mnet_socket_t sock,
            const void* buf,
            size_t len,
            mnet_msg_flags_t flags,
            const mnet_sockaddr_t* dest_addr,
            socklen_t addrlen);

// ----------------------------------------------------------------
// receive data and get the sender's address. (UDP)
//
// buf: buffer to receive into.
// len: maximum bytes to receive. (shouldn't be more than the buffers size)
// flags: flags to receive with.
// src_addr: [out] senders address.             (can be NULL)
// addrlen: [out] sizeof src_addr structure.    (can be NULL)
// ----------------------------------------------------------------
// returns: number of bytes received, or -1 on error.
int mnet_recvfrom(
                mnet_socket_t sock,
                void* buf,
                size_t len,
                mnet_msg_flags_t flags,
                mnet_sockaddr_t* src_addr,
                socklen_t* addrlen);


// ================================================
//                  SOCKET OPTIONS
// ================================================


// ----------------------------------------------------------------
// set a socket option value.
//
// level: level.
// optname: option to set. (from mnet_sockopt_t)
// optval: pointer to option value. (see mnet_sockopt_t)
// optlen: value length in bytes.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_setsockopt(
                mnet_socket_t sock,
                mnet_sockopt_level_t level,
                mnet_sockopt_t optname,
                const void* optval,
                socklen_t optlen);

// ----------------------------------------------------------------
// get a socket option value.
//
// level: level.
// optname: option to get. (from mnet_sockopt_t)
// optval: buffer to receive value into.
// optlen: pointer to size of buffer. (updated with actual size)
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_getsockopt(
                mnet_socket_t sock,
                mnet_sockopt_level_t level,
                mnet_sockopt_t optname,
                void* optval,
                socklen_t* optlen);


// ================================================
//              EXTRA SOCKET CONTROL
// ================================================


// ----------------------------------------------------------------
//  set socket blocking mode. (blocking is default)
//
// do_block: 1 to block, 0 to not block.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
mnet_result_t mnet_set_blocking(mnet_socket_t sock, int do_block);


// ================================================
//           ADDRESS AND NAME RESOLUTION
// ================================================

// ----------------------------------------------------------------
// get the local address its bound to.
//
// addr: output buffer for the address.
// addrlen: pointer to buffer size. (updated with actual size)
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_getsockname(
            mnet_socket_t sock,
            mnet_sockaddr_t* addr,
            socklen_t* addrlen);

// ----------------------------------------------------------------
// get the remote address it's connected to.
//
// addr: output buffer for the address.
// addrlen: pointer to buffer size. (updated with actual size)
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_getpeername(
                mnet_socket_t sock,
                mnet_sockaddr_t* addr,
                socklen_t* addrlen);

// ----------------------------------------------------------------
// convert ascii string IP to binary form.
//
// af: the address family.
// src: ascii form.
// dst: binary form.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_inet_pton(mnet_address_family_t af, const char* src, void* dst);


// ================================================
//                  ERROR HANDLING
// ================================================

// ----------------------------------------------------------------
// get the last error for this thread.
// ----------------------------------------------------------------
mnet_error_code_t mnet_get_error(void);

// ----------------------------------------------------------------
// human readable ascii string representing the error code.
// ----------------------------------------------------------------
// returns: the string , DO NOT FREE.
const char* mnet_error_string(mnet_error_code_t error_code);


// ================================================
//              ADDRESS CREATORS
// ================================================


// ----------------------------------------------------------------
// create an IPv4 address structure.
//
// addr: [out] address structure.
// ip: IP address string.
//  (e.g. "129.168.1.1", or NULL/"0.0.0.0" for INADDR_ANY)
// port: port, host byte order.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_ipv4(     mnet_sockaddr_in_t *    addr, const char* ip,   uint16_t port);

// ----------------------------------------------------------------
// create an IPv6 address structure.
//
// addr: [out] address structure.
// ip: IP address string.
//  (e.g. "::1" or NULL/"::" for in6addr_any)
// port: port, host byte order.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_ipv6(     mnet_sockaddr_in6_t*    addr, const char* ip,   uint16_t port);

// ----------------------------------------------------------------
// create an IPv4 address structure. (INADDR_ANY)
//
// addr: [out] address structure.
// port: port, host byte order.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_any_ipv4( mnet_sockaddr_in_t *    addr,                   uint16_t port);

// ----------------------------------------------------------------
// create an IPv6 address structure. (in6addr_any)
//
// addr: [out] address structure.
// port: port, host byte order.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_any_ipv6( mnet_sockaddr_in6_t*    addr,                   uint16_t port);

#endif//MNET_MNET_H

///////////////////////////////////////
//              SOURCE
///////////////////////////////////////
#ifdef MNET_SOURCE


// ================================================
// INITIALIZATION & CLEANUP
// ================================================


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


// ================================================
//       CORE SOCKET CREATION AND MANAGEMENT
// ================================================


mnet_socket_t mnet_socket(
    const mnet_address_family_t domain,
    const mnet_socket_type_t    type,
    const mnet_protocol_t       protocol)
{
    return socket((int)domain, (int)type, (int)protocol);
}

int mnet_close(mnet_socket_t sock)
{
#ifdef MNET_WINDOWS
    return closesocket(sock);
#elif defined(MNET_UNIX)
    return close(sock);
#endif
}

int mnet_socket_is_valid(mnet_socket_t sock)
{
    return sock != MNET_INVALID_SOCKET;
}

// ================================================
//         CONNECTION MANAGEMENT (TCP)
// ================================================


int mnet_bind(mnet_socket_t sock, const mnet_sockaddr_t* addr, socklen_t addrlen)
{
    return bind(sock, addr, addrlen);
}

int mnet_listen(mnet_socket_t sock, int backlog)
{
    return listen(sock, backlog);
}

mnet_socket_t mnet_accept(mnet_socket_t sock, struct sockaddr *addr, socklen_t *addrlen)
{
    return accept(sock, addr, addrlen);
}

int mnet_connect(mnet_socket_t sock, const struct sockaddr *addr, socklen_t addrlen)
{
    return connect(sock, addr, addrlen);
}

int mnet_shutdown(mnet_socket_t sock, mnet_shutdown_code_t how)
{
    return shutdown(sock, (int)how);
}


// ================================================
//               DATA TRANSFER (TCP)
// ================================================


int mnet_send(mnet_socket_t sock, const void* buf, size_t len, mnet_msg_flags_t flags)
{
#ifdef MNET_WINDOWS
    return send(sock, (const char*)buf, (int)len, (int)flags);
#elif defined(MNET_UNIX)
    return (int)send(sock, buf, len, (int)flags);
#endif
}

int mnet_recv(mnet_socket_t sock, void* buf, size_t len, mnet_msg_flags_t flags)
{
#ifdef MNET_WINDOWS
    return recv(sock, (char*)buf, (int)len, (int)flags);
#elif defined(MNET_UNIX)
    return (int)recv(sock, buf, len, (int)flags);
#endif
}


// ================================================
//                DATA TRANSFER (UDP)
// ================================================


int mnet_sendto(mnet_socket_t sock, const void* buf, size_t len, mnet_msg_flags_t flags,
                const mnet_sockaddr_t* dest_addr, socklen_t addrlen)
{
#ifdef MNET_WINDOWS
    return sendto(sock, (const char*)buf, (int)len, (int)flags, dest_addr, addrlen);
#elif defined(MNET_UNIX)
    return (int)sendto(sock, buf, len, (int)flags, dest_addr, addrlen);
#endif
}

int mnet_recvfrom(mnet_socket_t sock, void* buf, size_t len, mnet_msg_flags_t flags,
                  mnet_sockaddr_t* src_addr, socklen_t* addrlen)
{
#ifdef MNET_WINDOWS
    return recvfrom(sock, (char*)buf, (int)len, (int)flags, src_addr, addrlen);
#elif defined(MNET_UNIX)
    return (int)recvfrom(sock, buf, len, (int)flags, src_addr, addrlen);
#endif
}


// ================================================
//                  SOCKET OPTIONS
// ================================================


int mnet_setsockopt(mnet_socket_t sock, mnet_sockopt_level_t level, mnet_sockopt_t optname,
                    const void* optval, socklen_t optlen)
{
#ifdef MNET_WINDOWS
    return setsockopt(sock, (int)level, (int)optname, (const char*)optval, optlen);
#elif defined(MNET_UNIX)
    return setsockopt(sock, (int)level, (int)optname, optval, optlen);
#endif
}

int mnet_getsockopt(mnet_socket_t sock, mnet_sockopt_level_t level, mnet_sockopt_t optname,
                    void* optval, socklen_t* optlen)
{
#ifdef MNET_WINDOWS
    return getsockopt(sock, (int)level, (int)optname, (char*)optval, optlen);
#elif defined(MNET_UNIX)
    return getsockopt(sock, (int)level, (int)optname, optval, optlen);
#endif
}


// ================================================
//              EXTRA SOCKET CONTROL
// ================================================

mnet_result_t mnet_set_blocking(mnet_socket_t sock, const int do_block)
{
#ifdef MNET_WINDOWS

    u_long mode = (u_long)do_block;
    return ioctlsocket(sock, FIONBIO, &mode);

#elif defined(MNET_UNIX)

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return mnet_error;

    if (do_block == 1) return fcntl(sock, F_SETFL, flags | ~O_NONBLOCK);
    if (do_block == 0) return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    return mnet_error;

#endif
}


// ================================================
//           ADDRESS AND NAME RESOLUTION
// ================================================


int mnet_getsockname(mnet_socket_t sock, struct sockaddr* addr, socklen_t* addrlen)
{
    return getsockname(sock, addr, addrlen);
}

int mnet_getpeername(mnet_socket_t sock, struct sockaddr* addr, socklen_t* addrlen)
{
    return getpeername(sock, addr, addrlen);
}

int mnet_inet_pton(mnet_address_family_t af, const char* src, void* dst)
{
#ifdef MNET_WINDOWS
    return InetPtonA(af, src, dst);
#elif defined(MNET_UNIX)
    return inet_pton(af, src, dst);
#endif
}


// ================================================
//                  ERROR HANDLING
// ================================================


mnet_error_code_t mnet_get_error(void)
{
#ifdef MNET_WINDOWS
    return WSAGetLastError();
#elif defined(MNET_UNIX)
    return errno;
#endif
}

const char* mnet_error_string(mnet_error_code_t error_code)
{
#ifdef MNET_WINDOWS
    static char buf[256];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   buf, sizeof(buf), NULL);
    return buf;
#elif defined(MNET_UNIX)
    return strerror(error_code);
#endif
}


// ================================================
//              ADDRESS CREATORS
// ================================================


int mnet_addr_ipv4(struct sockaddr_in *addr, const char *ip, uint16_t port)
{
    if (!addr) return -1;

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    if (!ip || strcmp(ip, "0.0.0.0") == 0)
    {
        addr->sin_addr.s_addr = INADDR_ANY;
        return 0;
    }

    return inet_pton(AF_INET, ip, &addr->sin_addr) == 1 ? 0 : -1;
}

int mnet_addr_ipv6(struct sockaddr_in6 *addr, const char *ip, uint16_t port)
{
    if (!addr) return -1;

    memset(addr, 0, sizeof(*addr));
    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(port);

    if (!ip || strcmp(ip, "::") == 0)
    {
        addr->sin6_addr = in6addr_any;
        return 0;
    }

    return inet_pton(AF_INET6, ip, &addr->sin6_addr) == 1 ? 0 : -1;
}

int mnet_addr_any_ipv4(struct sockaddr_in *addr, uint16_t port)
{
    return mnet_addr_ipv4(addr, NULL, port);
}

int mnet_addr_any_ipv6(struct sockaddr_in6 *addr, uint16_t port)
{
    return mnet_addr_ipv6(addr, NULL, port);
}

#endif