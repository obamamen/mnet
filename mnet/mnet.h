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

#include <stdint.h>

#include "mnet_os.h"
#include "mnet_types.h"

int mnet_initialize(void);
void mnet_cleanup(void);

mnet_socket_t mnet_socket(int domain, int type, int protocol);
int mnet_close(mnet_socket_t sock);
int mnet_bind(mnet_socket_t sock, const struct sockaddr* addr, socklen_t addrlen);
int mnet_listen(mnet_socket_t sock, int backlog);
mnet_socket_t mnet_accept(mnet_socket_t sock, struct sockaddr* addr, socklen_t* addrlen);
int mnet_connect(mnet_socket_t sock, const struct sockaddr* addr, socklen_t addrlen);

int mnet_socket_is_valid(mnet_socket_t sock);

int mnet_get_error(void);
const char* mnet_error_string(int error_code);

int mnet_addr_ipv4(struct sockaddr_in* addr, const char* ip,    uint16_t port);
int mnet_addr_ipv6(struct sockaddr_in6* addr, const char* ip,   uint16_t port);
int mnet_addr_any_ipv4(struct sockaddr_in* addr,                uint16_t port);
int mnet_addr_any_ipv6(struct sockaddr_in6* addr,               uint16_t port);

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

mnet_socket_t mnet_socket(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

int mnet_close(mnet_socket_t sock)
{
#ifdef MNET_WINDOWS
    return closesocket(sock);
#elif defined(MNET_UNIX)
    return close(sock);
#endif
}

int mnet_bind(mnet_socket_t sock, const struct sockaddr *addr, socklen_t addrlen)
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

int mnet_socket_is_valid(mnet_socket_t sock)
{
    return sock != MNET_INVALID_SOCKET;
}

int mnet_get_error(void)
{
#ifdef MNET_WINDOWS
    return WSAGetLastError();
#elif defined(MNET_UNIX)
    return errno;
#endif
}

const char* mnet_error_string(int error_code)
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

int mnet_addr_ipv4(struct sockaddr_in *addr, const char *ip, uint16_t port)
{
    if (!addr) return -1;

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    if (!ip || strcmp(ip, "0.0.0.0") == 0) {
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

    if (!ip || strcmp(ip, "::") == 0) {
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