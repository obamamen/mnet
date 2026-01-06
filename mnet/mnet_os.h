/* ================================== *\
 @file     mnet_os.h
 @project  mnet
 @author   moosm
 @date     1/5/2026
*\ ================================== */

#ifndef MNET_MNET_OS_H
#define MNET_MNET_OS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#   define MNET_WINDOWS
#elif defined(__unix__) || defined(__unix) || defined(__linux__) || defined(__APPLE__)
#   define MNET_UNIX
#else
#   error PLATFORM_NOT_SUPPORTED
#endif

#ifdef MNET_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <winsock2.h>
#   include <ws2tcpip.h>

    typedef SOCKET mnet_socket_t;
#   define MNET_INVALID_SOCKET INVALID_SOCKET
#elif defined(MNET_UNIX)
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <netinet/tcp.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <unistd.h>
#   include <fcntl.h>
#   include <errno.h>

    typedef int mnet_socket_t;
#   define MNET_INVALID_SOCKET (-1)
#endif

#endif //MNET_MNET_OS_H