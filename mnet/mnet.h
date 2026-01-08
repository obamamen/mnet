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
#define MNET_CONNECT(socket, addr) mnet_connect(socket, (void*)&addr, sizeof(addr))

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

// ----------------------------------------------------------------
// initialize an iovec buffer descriptor.
//
// iov: [out] iovec to initialize.
// base: pointer to buffer.
// len: length of buffer.
// ----------------------------------------------------------------
void mnet_iovec_init(mnet_iovec_t* iov, void* base, size_t len);

// ----------------------------------------------------------------
// send multiple buffers in a single call. (vectored I/O)
//
// iov: array of buffer descriptors.
// iovcnt: number of buffers in array.
// flags: flags to send with.
// ----------------------------------------------------------------
// returns: number of bytes sent, or -1 on error.
int mnet_sendv(
            mnet_socket_t sock,
            const mnet_iovec_t* iov,
            int iovcnt,
            mnet_msg_flags_t flags);

// receive into multiple buffers in a single call. (vectored I/O)
//
// iov: array of buffer descriptors.
// iovcnt: number of buffers in array.
// flags: flags to receive with.
// ----------------------------------------------------------------
// returns: number of bytes received, 0 if connection closed, -1 on error.
int mnet_recvv(
            mnet_socket_t sock,
            mnet_iovec_t* iov,
            int iovcnt,
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

// ----------------------------------------------------------------
// get the number of bytes available to read without blocking.
//
// bytes: [out] number of bytes available.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_available(mnet_socket_t sock, unsigned long* bytes);


// ================================================
//             I/O MULTIPLEXING (POLL)
// ================================================


// ----------------------------------------------------------------
// poll multiple sockets for events.
//
// fds: array of mnet_pollfd_t structures.
// nfds: number of elements in fds array.
// timeout: timeout in milliseconds.
//  (-1 = block forever, 0 = return immediately)
// ----------------------------------------------------------------
// returns: number of ready sockets, 0 on timeout, -1 on error.
// NOTE: indices are preserved. fds[X] input corresponds to fds[X] output.
int mnet_poll(mnet_pollfd_t* fds, int nfds, int timeout);

// ----------------------------------------------------------------
// check if a pollfd has a specific event set in revents.
//
// pfd: pollfd to check.
// event: event to check for (mnet_pollin, mnet_pollout, etc.)
// ----------------------------------------------------------------
// returns: 1 if event is set, 0 otherwise.
int mnet_pollfd_has_event(const mnet_pollfd_t* pfd, short event);


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
// resolve hostname to IP addresses. (DNS lookup)
//
// node: hostname or IP address string. (e.g., "google.com")
// service: service name or port string. (e.g., "http" or "80")
// hints: optional hints for resolution. (can be NULL)
// res: [out] result list. (must be freed with mnet_freeaddrinfo)
// ----------------------------------------------------------------
// returns: mnet_ok on success, error code on failure.
int mnet_getaddrinfo(
                const char* node,
                const char* service,
                const struct addrinfo* hints,
                struct addrinfo** res);

// ----------------------------------------------------------------
// free address info returned by mnet_getaddrinfo.
//
// res: address info to free.
// ----------------------------------------------------------------
void mnet_freeaddrinfo(struct addrinfo* res);

// ----------------------------------------------------------------
// get name information from a socket address. (reverse DNS)
//
// addr: socket address to convert.
// addrlen: size of address.
// host: [out] hostname buffer.     (can be NULL)
// hostlen: size of host buffer.
// serv: [out] service name buffer. (can be NULL)
// servlen: size of service buffer.
// flags: control flags.
// ----------------------------------------------------------------
// returns: mnet_ok on success, error code on failure.
int mnet_getnameinfo(
                const mnet_sockaddr_t* addr,
                socklen_t addrlen,
                char* host,
                socklen_t hostlen,
                char* serv,
                socklen_t servlen,
                int flags);

// ----------------------------------------------------------------
// convert ascii string IP to binary form.
//
// af: the address family.
// src: ascii form.
// dst: binary form.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_inet_pton(mnet_address_family_t af, const char* src, void* dst);

// ----------------------------------------------------------------
// convert binary IP to ascii string form.
//
// af: the address family.
// src: binary form.
// dst: ascii form buffer. (at least INET_ADDRSTRLEN or INET6_ADDRSTRLEN)
// size: size of dst buffer.
// ----------------------------------------------------------------
// returns: pointer to dst on success, NULL on error.
const char* mnet_inet_ntop(mnet_address_family_t af, const void* src, char* dst, socklen_t size);


// ================================================
//              ADDRESS UTILITIES
// ================================================


// ----------------------------------------------------------------
// get address family of a socket addres.
//
// addr: pointer to address.
// af: pointer to address family.
// ----------------------------------------------------------------
// returns: mnet_address_family_t.
mnet_result_t mnet_get_family(
    const mnet_sockaddr_t* addr,
    mnet_address_family_t* af);

// ----------------------------------------------------------------
// convert socket address IP to string representation.
//  (e.g. "192.168.1.1"/"2001:db8:85a3:0:0:8a2e:370:7334"
//
// addr: socket address (IPv4 or IPv6).
// ip_buf: [out] buffer for IP string.
// ip_buf_size: size of ip_buf.
// port: port number in host byte order. (can be NULL)
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_ip_to_string(
            const mnet_sockaddr_t* addr,
            char ip_buf[],
            size_t ip_buf_size);

// ----------------------------------------------------------------
// convert socket address to string representation.
//  (e.g. "192.168.1.1:80"/"2001:db8:85a3:0:0:8a2e:370:7334]:8001"
//
// addr: socket address (IPv4 or IPv6).
// ip_buf: [out] buffer for IP string.
// ip_buf_size: size of ip_buf.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_to_string(
            const mnet_sockaddr_t* addr,
            char ip_buf[],
            size_t ip_buf_size,
            uint16_t port);

// ----------------------------------------------------------------
// get port from socket address.
//
// addr: socket address.
// ----------------------------------------------------------------
// returns: port in host byte order, or 0 on error.
uint16_t mnet_addr_get_port(const mnet_sockaddr_t* addr);

// ----------------------------------------------------------------
// set port on socket address.
//
// addr: socket address.
// port: port in host byte order.
// ----------------------------------------------------------------
// returns: mnet_ok on success, mnet_error on failure.
int mnet_addr_set_port(mnet_sockaddr_t* addr, uint16_t port);

// ================================================
//              BYTE ORDER CONVERSION
// ================================================

// ----------------------------------------------------------------
// convert 16-bit value from host to network byte order.
// ----------------------------------------------------------------
uint16_t mnet_htons(uint16_t hostshort);

// ----------------------------------------------------------------
// convert 16-bit value from network to host byte order.
// ----------------------------------------------------------------
uint16_t mnet_ntohs(uint16_t netshort);

// ----------------------------------------------------------------
// convert 32-bit value from host to network byte order.
// ----------------------------------------------------------------
uint32_t mnet_htonl(uint32_t hostlong);

// ----------------------------------------------------------------
// convert 32-bit value from network to host byte order.
// ----------------------------------------------------------------
uint32_t mnet_ntohl(uint32_t netlong);

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

void mnet_iovec_init(mnet_iovec_t* iov, void* base, size_t len)
{
    if (!iov) return;

#ifdef MNET_WINDOWS
    iov->buf = (char*)base;
    iov->len = (ULONG)len;
#elif defined(MNET_UNIX)
    iov->iov_base = base;
    iov->iov_len = len;
#endif
}

int mnet_sendv(mnet_socket_t sock, const mnet_iovec_t* iov, int iovcnt, mnet_msg_flags_t flags)
{
    if (!iov || iovcnt <= 0) return -1;

#ifdef MNET_WINDOWS

    DWORD sent = 0;
    int result = WSASend(sock, (LPWSABUF)iov, iovcnt, &sent, (DWORD)flags, NULL, NULL);
    return result == 0 ? (int)sent : -1;

#elif defined(MNET_UNIX)

    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = (struct iovec*)iov;
    msg.msg_iovlen = iovcnt;

    return (int)sendmsg(sock, &msg, (int)flags);

#endif
}

int mnet_recvv(mnet_socket_t sock, mnet_iovec_t* iov, int iovcnt, mnet_msg_flags_t flags)
{
    if (!iov || iovcnt <= 0) return -1;

#ifdef MNET_WINDOWS

    DWORD received = 0;
    DWORD flags_dword = (DWORD)flags;
    int result = WSARecv(sock, (LPWSABUF)iov, iovcnt, &received, &flags_dword, NULL, NULL);
    return result == 0 ? (int)received : -1;

#elif defined(MNET_UNIX)

    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = (struct iovec*)iov;
    msg.msg_iovlen = iovcnt;

    return (int)recvmsg(sock, &msg, (int)flags);

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

    u_long mode = do_block ? 0 : 1;
    return ioctlsocket(sock, FIONBIO, &mode) == 0 ? mnet_ok : mnet_error;

#elif defined(MNET_UNIX)

    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) return mnet_error;

    if (do_block)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    return fcntl(sock, F_SETFL, flags) == 0 ? mnet_ok : mnet_error;

#endif
}

int mnet_available(mnet_socket_t sock, unsigned long* bytes)
{
    if (!bytes) return mnet_error;

#ifdef MNET_WINDOWS
    return ioctlsocket(sock, FIONREAD, bytes) == 0 ? mnet_ok : mnet_error;
#elif defined(MNET_UNIX)
    return ioctl(sock, FIONREAD, bytes) == 0 ? mnet_ok : mnet_error;
#endif
}

// ================================================
//             I/O MULTIPLEXING (POLL)
// ================================================

int mnet_poll(mnet_pollfd_t* fds, int nfds, int timeout)
{
    if (!fds) return mnet_error;

#ifdef MNET_WINDOWS
    return WSAPoll((WSAPOLLFD*)fds, nfds, timeout);
#elif defined(MNET_UNIX)
    return poll((struct pollfd*)fds, nfds, timeout);
#endif
}

int mnet_pollfd_has_event(const mnet_pollfd_t* pfd, short event)
{
    if (!pfd) return 0;
    return (pfd->revents & event) != 0;
}


// ================================================
//           ADDRESS AND NAME RESOLUTION
// ================================================


int mnet_getsockname(mnet_socket_t sock, mnet_sockaddr_t* addr, socklen_t* addrlen)
{
    return getsockname(sock, addr, addrlen);
}

int mnet_getpeername(mnet_socket_t sock, mnet_sockaddr_t* addr, socklen_t* addrlen)
{
    return getpeername(sock, addr, addrlen);
}

int mnet_getaddrinfo(const char* node, const char* service,
                     const struct addrinfo* hints, struct addrinfo** res)
{
    return getaddrinfo(node, service, hints, res);
}

void mnet_freeaddrinfo(struct addrinfo* res)
{
    freeaddrinfo(res);
}

int mnet_getnameinfo(const mnet_sockaddr_t* addr, socklen_t addrlen,
                     char* host, socklen_t hostlen,
                     char* serv, socklen_t servlen, int flags)
{
    return getnameinfo(addr, addrlen, host, hostlen, serv, servlen, flags);
}

int mnet_inet_pton(mnet_address_family_t af, const char* src, void* dst)
{
#ifdef MNET_WINDOWS
    return InetPtonA((int)af, src, dst);
#elif defined(MNET_UNIX)
    return inet_pton((int)af, src, dst);
#endif
}

const char* mnet_inet_ntop(mnet_address_family_t af, const void* src, char* dst, socklen_t size)
{
#ifdef MNET_WINDOWS
    return InetNtopA((int)af, (void*)src, dst, size);
#elif defined(MNET_UNIX)
    return inet_ntop((int)af, src, dst, size);
#endif
}


// ================================================
//              ADDRESS UTILITIES
// ================================================


mnet_result_t mnet_get_family(const mnet_sockaddr_t* addr, mnet_address_family_t* out_family)
{
    if (!addr || !out_family) return mnet_error;
    *out_family = (mnet_address_family_t)addr->sa_family;
    return mnet_ok;
}

int mnet_addr_ip_to_string(
    const mnet_sockaddr_t* addr,
    char ip_buf[],
    size_t ip_buf_size)
{
    if (!addr || !ip_buf) return mnet_error;

    if (addr->sa_family == AF_INET)
    {
        mnet_sockaddr_in_t* addr_in = (mnet_sockaddr_in_t*)addr;
        if (mnet_inet_ntop(mnet_af_inet, &addr_in->sin_addr, ip_buf, ip_buf_size) == NULL)
            return mnet_error;

        return mnet_ok;
    }
    else if (addr->sa_family == AF_INET6)
    {
        mnet_sockaddr_in6_t* addr_in6 = (mnet_sockaddr_in6_t*)addr;
        if (mnet_inet_ntop(mnet_af_inet6, &addr_in6->sin6_addr, ip_buf, ip_buf_size) == NULL)
            return mnet_error;

        return mnet_ok;
    }
}

// mnet_addr_to_string.

uint16_t mnet_addr_get_port(const mnet_sockaddr_t* addr)
{
    if (!addr) return 0;

    if (addr->sa_family == AF_INET)
    {
        mnet_sockaddr_in_t* addr_in = (mnet_sockaddr_in_t*)addr;
        return mnet_ntohs(addr_in->sin_port);
    }
    else if (addr->sa_family == AF_INET6)
    {
        const mnet_sockaddr_in6_t* addr_in6 = (mnet_sockaddr_in6_t*)addr;
        return mnet_ntohs(addr_in6->sin6_port);
    }

    return 0;
}

int mnet_addr_set_port(mnet_sockaddr_t* addr, uint16_t port)
{
    if (!addr) return mnet_error;

    if (addr->sa_family == AF_INET)
    {
        mnet_sockaddr_in_t* addr_in = (mnet_sockaddr_in_t*)addr;
        addr_in->sin_port = mnet_htons(port);
        return mnet_ok;
    }
    else if (addr->sa_family == AF_INET6)
    {
        mnet_sockaddr_in6_t* addr_in6 = (mnet_sockaddr_in6_t*)addr;
        addr_in6->sin6_port = mnet_htons(port);
        return mnet_ok;
    }

    return mnet_error;
}


// ================================================
//              BYTE ORDER CONVERSION
// ================================================

uint16_t mnet_htons(uint16_t hostshort)
{
    return htons(hostshort);
}

uint16_t mnet_ntohs(uint16_t netshort)
{
    return ntohs(netshort);
}

uint32_t mnet_htonl(uint32_t hostlong)
{
    return htonl(hostlong);
}

uint32_t mnet_ntohl(uint32_t netlong)
{
    return ntohl(netlong);
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